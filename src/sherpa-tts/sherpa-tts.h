#ifndef SHERPA_TTS_H
#define SHERPA_TTS_H

#include "sherpa-onnx/c-api/c-api.h"

#include <string>

struct sherpa_tts_context {
	SherpaOnnxOfflineTts *tts = nullptr;
	std::string model_name;
	void *callback_data = nullptr;
	void (*audio_callback)(void *data, const float *samples, int num_samples, int sample_rate);
	uint32_t num_speakers = 0;
	bool initialized = false;
};

void init_sherpa_tts_context(sherpa_tts_context &context,
			     void (*audio_callback)(void *data, const float *samples,
						    int num_samples, int sample_rate),
			     void *data);
void destroy_sherpa_tts_context(sherpa_tts_context &context);

void generate_audio_from_text(sherpa_tts_context &ctx, const std::string &text, uint32_t speaker_id,
			      float speed);

#endif
