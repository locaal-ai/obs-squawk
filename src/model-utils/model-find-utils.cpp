#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <regex>

#include <obs-module.h>

#include "model-find-utils.h"
#include "plugin-support.h"

std::string find_file_in_folder_by_name(const std::string &folder_path,
					const std::string &file_name)
{
	for (const auto &entry : std::filesystem::directory_iterator(folder_path)) {
		if (entry.path().filename() == file_name) {
			return entry.path().string();
		}
	}
	return "";
}

// Find a file in a folder by expression
std::string find_file_in_folder_by_regex_expression(const std::string &folder_path,
						    const std::string &file_name_regex)
{
	for (const auto &entry : std::filesystem::directory_iterator(folder_path)) {
		if (std::regex_match(entry.path().filename().string(),
				     std::regex(file_name_regex))) {
			return entry.path().string();
		}
	}
	return "";
}

std::string find_file_in_folder_by_extension(const std::string &model_local_folder_path,
					     const std::string &extension)
{
	// find .bin file in folder
	for (const auto &entry : std::filesystem::directory_iterator(model_local_folder_path)) {
		if (entry.path().extension() == extension) {
			const std::string file_path = entry.path().string();
			obs_log(LOG_INFO, "File found in folder: %s", file_path.c_str());
			return file_path;
		}
	}
	obs_log(LOG_ERROR, "File not found in folder: %s", model_local_folder_path.c_str());
	return "";
}

std::string find_bin_file_in_folder(const std::string &model_local_folder_path)
{
	// find .bin file in folder
	return find_file_in_folder_by_extension(model_local_folder_path, ".bin");
}

ModelInfo find_model_info_by_name(const std::string &model_name)
{
	for (auto model_info : model_infos) {
		if (model_info.local_folder_name == model_name) {
			return model_info;
		}
	}
	return ModelInfo();
}
