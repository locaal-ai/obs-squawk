#include "model-downloader.h"
#include "plugin-support.h"
#include "model-downloader-ui.h"
#include "model-find-utils.h"

#include <filesystem>

#include <archive.h>
#include <archive_entry.h>
#include <bzlib.h>

#include <obs-module.h>
#include <obs-frontend-api.h>

std::string find_model_folder(const ModelInfo &model_info)
{
	char *data_folder_models = obs_module_file("models");
	if (data_folder_models != nullptr) {
		const std::filesystem::path module_data_models_folder =
			std::filesystem::absolute(data_folder_models);
		bfree(data_folder_models);

		const std::string model_local_data_path =
			(module_data_models_folder / model_info.local_folder_name).string();

		obs_log(LOG_INFO, "Checking if model '%s' exists in data...",
			model_info.friendly_name.c_str());

		if (!std::filesystem::exists(model_local_data_path)) {
			obs_log(LOG_INFO, "Model not found in data: %s",
				model_local_data_path.c_str());
		} else {
			obs_log(LOG_INFO, "Model folder found in data: %s",
				model_local_data_path.c_str());
			return model_local_data_path;
		}
	}

	// Check if model exists in the config folder
	char *config_folder = obs_module_config_path("models");
	const std::filesystem::path module_config_models_folder =
		std::filesystem::absolute(config_folder);
	bfree(config_folder);

	obs_log(LOG_INFO, "Checking if model '%s' exists in config...",
		model_info.friendly_name.c_str());

	const std::string model_local_config_path =
		(module_config_models_folder / model_info.local_folder_name).string();

	obs_log(LOG_INFO, "Model path in config: %s", model_local_config_path.c_str());
	if (std::filesystem::exists(model_local_config_path)) {
		obs_log(LOG_INFO, "Model exists in config folder: %s",
			model_local_config_path.c_str());
		return model_local_config_path;
	}

	obs_log(LOG_INFO, "Model '%s' not found.", model_info.friendly_name.c_str());
	return "";
}

std::string find_model_bin_file(const ModelInfo &model_info)
{
	const std::string model_local_folder_path = find_model_folder(model_info);
	if (model_local_folder_path.empty()) {
		return "";
	}

	return find_bin_file_in_folder(model_local_folder_path);
}

void download_model_with_ui_dialog(const ModelInfo &model_info,
				   download_finished_callback_t download_finished_callback)
{
	// Start the model downloader UI
	ModelDownloader *model_downloader = new ModelDownloader(
		model_info, download_finished_callback, (QWidget *)obs_frontend_get_main_window());
	model_downloader->show();
}

static int copy_data(struct archive *ar, struct archive *aw)
{
	int r;
	const void *buff;
	size_t size;
	la_int64_t offset;

	for (;;) {
		r = archive_read_data_block(ar, &buff, &size, &offset);
		if (r == ARCHIVE_EOF)
			return (ARCHIVE_OK);
		if (r < ARCHIVE_OK)
			return (r);
#ifdef WIN32
		r = archive_write_data_block(aw, buff, size, offset);
#else
		r = (int)archive_write_data_block(aw, buff, (int)size, offset);
#endif
		if (r < ARCHIVE_OK) {
			obs_log(LOG_ERROR, "Error writing data: %s", archive_error_string(aw));
			return (r);
		}
	}
}

void decompress_bz2(const char *input_file, const char *output_file)
{
	const int BUFFER_SIZE = 4096;
	FILE *input = fopen(input_file, "rb");
	if (!input) {
		throw std::runtime_error("Could not open input file.");
	}

	FILE *output = fopen(output_file, "wb");
	if (!output) {
		fclose(input);
		throw std::runtime_error("Could not open output file.");
	}

	BZFILE *bz_file = BZ2_bzReadOpen(nullptr, input, 0, 0, nullptr, 0);
	if (!bz_file) {
		fclose(input);
		fclose(output);
		throw std::runtime_error("Could not open bz2 file.");
	}

	std::vector<char> buffer(BUFFER_SIZE);
	int bz_error;

	while (true) {
		int bytes_read = BZ2_bzRead(&bz_error, bz_file, buffer.data(), (int)buffer.size());
		if (bz_error == BZ_OK || bz_error == BZ_STREAM_END) {
			fwrite(buffer.data(), 1, bytes_read, output);
			if (bz_error == BZ_STREAM_END) {
				break;
			}
		} else {
			BZ2_bzReadClose(&bz_error, bz_file);
			fclose(input);
			fclose(output);
			throw std::runtime_error("Error during decompression.");
		}
	}

	BZ2_bzReadClose(&bz_error, bz_file);
	fclose(input);
	fclose(output);
}

void extract(const char *filename, const char *dirname)
{
	struct archive *a;
	struct archive *ext;
	struct archive_entry *entry;
	int r;

	// Create a new archive object for reading
	a = archive_read_new();
	// Support tar format
	archive_read_support_format_tar(a);
	// Create a new archive object for writing
	ext = archive_write_disk_new();
	archive_write_disk_set_options(ext, ARCHIVE_EXTRACT_TIME);
	archive_write_disk_set_standard_lookup(ext);

	// Open the input archive file
	if ((r = archive_read_open_filename(a, filename, 10240))) {
		throw std::runtime_error("Could not open archive");
	}

	// Read and extract each entry in the archive
	while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {

		std::string current_file = archive_entry_pathname(entry);
		std::string full_path = std::string(dirname) + "/" + current_file;
		obs_log(LOG_DEBUG, "Extracting %s", full_path.c_str());

		archive_entry_set_pathname(entry, full_path.c_str());

		r = archive_write_header(ext, entry);
		if (r != ARCHIVE_OK) {
			obs_log(LOG_ERROR, "archive_write_header() failed: %s",
				archive_error_string(ext));
		} else {
			const void *buff;
			size_t size;
			la_int64_t offset;

			// Read data blocks from the input archive and write to the output
			while (true) {
				r = archive_read_data_block(a, &buff, &size, &offset);
				if (r == ARCHIVE_EOF)
					break;
				if (r != ARCHIVE_OK)
					throw std::runtime_error(archive_error_string(a));

#ifdef WIN32
				r = archive_write_data_block(ext, buff, size, offset);
#else
				r = (int)archive_write_data_block(ext, buff, (int)size, offset);
#endif
				if (r != ARCHIVE_OK)
					throw std::runtime_error(archive_error_string(ext));
			}
		}
		// Finish writing the current entry
		archive_write_finish_entry(ext);
	}

	// Close and free the archive objects
	archive_read_close(a);
	archive_read_free(a);
	archive_write_close(ext);
	archive_write_free(ext);
}

void unpack_model(const ModelInfo &model_info, const std::string &model_local_folder_path)
{
	const std::filesystem::path local_folder_path =
		std::filesystem::absolute(model_local_folder_path);

	// Unpack the model
	const std::string model_zip_file =
		(local_folder_path / (model_info.local_folder_name + ".tar.bz2")).string();

	// Check if the model zip file exists
	if (!std::filesystem::exists(model_zip_file)) {
		obs_log(LOG_ERROR, "Model bz2 file not found: %s", model_zip_file.c_str());
		return;
	}

	// decompress the .bz2 file
	const std::string model_tar_file =
		(local_folder_path / (model_info.local_folder_name + ".tar")).string();
	decompress_bz2(model_zip_file.c_str(), model_tar_file.c_str());

	// delete the .bz2 file
	std::filesystem::remove(model_zip_file);

	// get the models folder
	char *config_folder = obs_module_get_config_path(obs_current_module(), "models");
	const std::filesystem::path module_config_models_folder =
		std::filesystem::absolute(config_folder);
	bfree(config_folder);

	// Unpack the tar to the models folder
	obs_log(LOG_INFO, "Unpacking tar: %s", model_tar_file.c_str());
	try {
		extract(model_tar_file.c_str(), module_config_models_folder.string().c_str());
		// delete the .tar file
		std::filesystem::remove(model_tar_file);
	} catch (const std::exception &e) {
		obs_log(LOG_ERROR, "Error unpacking tar %s: %s", model_tar_file.c_str(), e.what());
		return;
	}

	obs_log(LOG_INFO, "Model unpacked: %s", model_local_folder_path.c_str());
}

void delete_cached_models()
{
	// get the models folder
	char *config_folder = obs_module_get_config_path(obs_current_module(), "models");
	const std::filesystem::path module_config_models_folder =
		std::filesystem::absolute(config_folder);
	bfree(config_folder);

	// delete all files in the models folder
	for (const auto &entry : std::filesystem::directory_iterator(module_config_models_folder)) {
		std::filesystem::remove_all(entry.path());
	}
}
