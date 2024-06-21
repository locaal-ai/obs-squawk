#ifndef SHERPA_TTS_H
#define SHERPA_TTS_H

#include "sherpa-onnx/c-api/c-api.h"

#include <string>

struct sherpa_tts_context {
	SherpaOnnxOfflineTts *tts = nullptr;
	std::string model_name;
	void *callback_data = nullptr;
	void (*audio_callback)(void *data, const float *samples, int num_samples, int sample_rate);
};

void init_sherpa_tts_context(sherpa_tts_context &context,
			     void (*audio_callback)(void *data, const float *samples,
						    int num_samples, int sample_rate),
			     void *data);
void destroy_sherpa_tts_context(sherpa_tts_context &context);

void generate_audio_from_text(sherpa_tts_context &ctx, const char *text, int speaker_id);

#endif
