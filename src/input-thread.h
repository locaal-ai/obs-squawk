#ifndef INPUT_THREAD_H
#define INPUT_THREAD_H

#include <thread>
#include <atomic>
#include <vector>
#include <string>
#include <functional>

enum class ReadingMode { Whole, LineByLine };
enum class DebouceMode { Debounced, Immediate };

class InputThread {
public:
	InputThread();

	void start()
	{
		running = true;
		if (thread.joinable()) {
			return;
		}
		thread = std::thread(&InputThread::run, this);
	}

	void stop()
	{
		running = false;
		if (thread.joinable()) {
			thread.join();
		}
	}

	void setFile(const std::string &filePath) { file = filePath; }
	void setReadingMode(ReadingMode mode) { readingMode = mode; }
	void setInterval(uint32_t milliseconds) { interval = milliseconds; }
	void setDebounceMode(DebouceMode mode) { debounceMode = mode; }

	void setOBSTextSource(const std::string &sourceName) { obsTextSource = sourceName; }

	void setSpeechGenerationCallback(std::function<void(const std::string &)> callback)
	{
		speechGenerationCallback = callback;
	}

private:
	std::atomic<bool> running;
	std::thread thread;
	std::string file;
	std::string obsTextSource;
	std::function<void(const std::string &)> speechGenerationCallback;
	uint32_t interval = 1000;
	std::string lastFileValue;
	std::string lastOBSTextSourceValue;
	ReadingMode readingMode = ReadingMode::Whole;
	DebouceMode debounceMode = DebouceMode::Debounced;
	uint64_t lastChangeTimeFile = 0;
	uint64_t lastChangeTimeSource = 0;
	bool debounceGenerated = false;

	void run();
};

#endif // INPUT_THREAD_H
