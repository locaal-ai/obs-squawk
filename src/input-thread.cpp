#include "input-thread.h"

#include <chrono>
#include <thread>
#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>

#include <obs.h>

#include "plugin-support.h"

InputThread::InputThread() : running(false), interval(1000) {}

void InputThread::run()
{
	while (running) {
		obs_log(LOG_DEBUG, "Input thread checking for changes");

		std::string new_content_for_generation;

		// Monitor files for changes
		if (!file.empty()) {
			// Check if file has changed
			// get file contents from file
			std::string fileContents;
			// read file
			std::filesystem::path filePath(file);
			if (std::filesystem::exists(filePath)) {
				std::ifstream fileStream(filePath);
				if (fileStream.is_open()) {
					std::string line;
					while (std::getline(fileStream, line)) {
						fileContents += line;
					}
					fileStream.close();
				}
			}
			if (fileContents != lastFileValue) {
				// Invoke speech generation if it has changed
				new_content_for_generation = fileContents;
				lastFileValue = fileContents;
			}
		}

		// Monitor OBS text sources for changes
		if (!obsTextSource.empty()) {
			obs_log(LOG_DEBUG, "Checking OBS text source: %s", obsTextSource.c_str());
			// Check if OBS text source has changed
			// get OBS text source value
			obs_source_t *source = obs_get_source_by_name(obsTextSource.c_str());
			if (source) {
				obs_data_t *sourceSettings = obs_source_get_settings(source);
				if (sourceSettings) {
					const char *text =
						obs_data_get_string(sourceSettings, "text");
					obs_data_release(sourceSettings);
					if (text && lastOBSTextSourceValue != text) {
						// Invoke speech generation if it has changed
						new_content_for_generation = text;
						lastOBSTextSourceValue = text;
					}
				}
				obs_source_release(source);
			}
		}

		if (!new_content_for_generation.empty() && speechGenerationCallback) {
			std::thread generationThread([this, new_content_for_generation]() {
				obs_log(LOG_DEBUG, "Generating speech from input: %s",
					new_content_for_generation.c_str());
				speechGenerationCallback(new_content_for_generation);
			});
			generationThread.detach();
		}

		// Sleep for a certain interval before checking again
		std::this_thread::sleep_for(std::chrono::milliseconds(interval));
	}
}
