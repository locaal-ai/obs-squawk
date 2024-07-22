#ifndef INPUT_THREAD_H
#define INPUT_THREAD_H

#include <thread>
#include <atomic>
#include <vector>
#include <string>
#include <functional>

enum class ReadingMode { Whole, LineByLine };

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

	void run();
};

#endif // INPUT_THREAD_H
