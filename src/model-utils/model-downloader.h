#ifndef MODEL_DOWNLOADER_H
#define MODEL_DOWNLOADER_H

#include <string>

#include "model-downloader-types.h"

std::string find_model_folder(const ModelInfo &model_info);
std::string find_model_bin_file(const ModelInfo &model_info);

void unpack_model(const ModelInfo &model_info, const std::string &model_local_folder_path);

// Start the model downloader UI dialog with a callback for when the download is finished
void download_model_with_ui_dialog(const ModelInfo &model_info,
				   download_finished_callback_t download_finished_callback);

void delete_cached_models();

#endif // MODEL_DOWNLOADER_H
