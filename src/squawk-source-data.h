#ifndef SQUAWK_SOURCE_DATA_H
#define SQUAWK_SOURCE_DATA_H

#include <memory>
#include <obs.h>

#include "sherpa-tts/sherpa-tts.h"
#include "audio-thread.h"
#include "input-thread.h"

struct squawk_source_data {
	obs_source_t *context;
	std::unique_ptr<AudioThread> audioThread;
	sherpa_tts_context tts_context;
	std::unique_ptr<InputThread> inputThread;

	int speaker_id;
	bool phonetic_transcription;

	squawk_source_data() { context = nullptr; }
};

#endif // SQUAWK_SOURCE_DATA_H
