
#include "sherpa-tts.h"
#include "plugin-support.h"
#include "model-utils/model-find-utils.h"
#include "model-utils/model-downloader.h"

#include <obs-module.h>

void generate_audio_from_text(sherpa_tts_context &ctx, const char *text, int speaker_id)
{
	if (ctx.tts == nullptr) {
		return;
	}

	const SherpaOnnxGeneratedAudio *audio =
		SherpaOnnxOfflineTtsGenerate(ctx.tts, text, speaker_id, 1.0);

	// Call the audio callback function with the generated audio samples
	ctx.audio_callback(ctx.callback_data, audio->samples, audio->n, audio->sample_rate);

	SherpaOnnxDestroyOfflineTtsGeneratedAudio(audio);
}

void init_sherpa_tts_context(sherpa_tts_context &context,
			     void (*audio_callback)(void *data, const float *samples,
						    int num_samples, int sample_rate),
			     void *data)
{
	SherpaOnnxOfflineTtsConfig config;
	memset(&config, 0, sizeof(config));

	auto model_info = find_model_info_by_name(context.model_name);
	std::string model_folder = find_model_folder(model_info);

	if (model_folder.empty()) {
		obs_log(LOG_ERROR, "Model folder not found for model: %s", context.model_name);
		context.tts = nullptr;
		return;
	}

	// find the .onnx file in the model folder from context
	std::string onnx_model_path = find_file_in_folder_by_extension(model_folder, ".onnx");
	// find the "tokens.txt" file in the model folder from context
	std::string tokens_path = find_file_in_folder_by_name(model_folder, "tokens.txt");
	// find the "lexicon.txt" file in the model folder from context
	std::string lexicon_path = find_file_in_folder_by_name(model_folder, "lexicon.txt");
	// if there's no lexicon file, find the "espeak-ng-data" folder in the model folder from context
	std::string espeak_data_path = find_file_in_folder_by_name(model_folder, "espeak-ng-data");

	config.model.vits.model = onnx_model_path.c_str();
	if (lexicon_path.empty()) {
		config.model.vits.data_dir = espeak_data_path.c_str();
	} else {
		config.model.vits.lexicon = lexicon_path.c_str();
	}
	config.model.vits.tokens = tokens_path.c_str();
	config.model.vits.noise_scale = 0.667f;
	config.model.vits.noise_scale_w = 0.8f;
	config.model.vits.length_scale = 1.0f;
	config.model.num_threads = 1;
	config.model.provider = "cpu";
	config.model.debug = 1;
	config.max_num_sentences = 2;

	context.tts = SherpaOnnxCreateOfflineTts(&config);
	context.audio_callback = audio_callback;
	context.callback_data = data;
}

void destroy_sherpa_tts_context(sherpa_tts_context &context)
{
	if (context.tts == nullptr) {
		return;
	}
	SherpaOnnxDestroyOfflineTts(context.tts);
	context.tts = nullptr;
}
