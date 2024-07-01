
#include "sherpa-tts.h"
#include "plugin-support.h"
#include "model-utils/model-find-utils.h"
#include "model-utils/model-downloader.h"

#include <obs-module.h>

void generate_audio_from_text(sherpa_tts_context &ctx, const std::string &text, uint32_t speaker_id,
			      float speed)
{
	if (ctx.tts == nullptr || !ctx.initialized || text.empty() ||
	    ctx.audio_callback == nullptr || speed <= 0.0f) {
		return;
	}

	if (ctx.num_speakers == 0) {
		obs_log(LOG_WARNING, "No speakers found in the model. Assuming speaker id 0.");
		speaker_id = 0;
	} else if (speaker_id >= ctx.num_speakers) {
		obs_log(LOG_WARNING, "Speaker id %d is out of range (0 -> %d), using speaker id 0",
			speaker_id, ctx.num_speakers - 1);
		speaker_id = 0;
	}

	try {
		obs_log(LOG_DEBUG, "Generating audio from text: %s, speaker_id: %d, speed: %f",
			text.c_str(), speaker_id, speed);
		const SherpaOnnxGeneratedAudio *audio =
			SherpaOnnxOfflineTtsGenerate(ctx.tts, text.c_str(), speaker_id, speed);

		// Call the audio callback function with the generated audio samples
		ctx.audio_callback(ctx.callback_data, audio->samples, audio->n, audio->sample_rate);

		SherpaOnnxDestroyOfflineTtsGeneratedAudio(audio);
	} catch (const std::exception &e) {
		obs_log(LOG_ERROR, "Error generating audio from text: %s", e.what());
	}
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
		obs_log(LOG_ERROR, "Model folder not found for model: %s",
			context.model_name.c_str());
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
	context.num_speakers = std::max(1, SherpaOnnxOfflineTtsNumSpeakers(context.tts));
	context.initialized = true;
}

void destroy_sherpa_tts_context(sherpa_tts_context &context)
{
	if (context.tts == nullptr) {
		return;
	}
	SherpaOnnxDestroyOfflineTts(context.tts);
	context.tts = nullptr;
}
