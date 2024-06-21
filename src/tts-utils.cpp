#include "tts-utils.h"

#include <regex>

std::string phonetic_equivalent(std::string letter)
{
	// return the phonetic equivalent of the letter
	// e.g. "a" -> "aei", "b" -> "bee", "c" -> "see", etc.
	letter = tolower(letter[0]);
	if (letter == "a") {
		return "aei";
	} else if (letter == "b") {
		return "bee";
	} else if (letter == "c") {
		return "see";
	} else if (letter == "d") {
		return "dee";
	} else if (letter == "e") {
		return "ee";
	} else if (letter == "f") {
		return "eff";
	} else if (letter == "g") {
		return "gee";
	} else if (letter == "h") {
		return "aitch";
	} else if (letter == "i") {
		return "eye";
	} else if (letter == "j") {
		return "jay";
	} else if (letter == "k") {
		return "kay";
	} else if (letter == "l") {
		return "ell";
	} else if (letter == "m") {
		return "em";
	} else if (letter == "n") {
		return "en";
	} else if (letter == "o") {
		return "oh";
	} else if (letter == "p") {
		return "pee";
	} else if (letter == "q") {
		return "cue";
	} else if (letter == "r") {
		return "ar";
	} else if (letter == "s") {
		return "ess";
	} else if (letter == "t") {
		return "tee";
	} else if (letter == "u") {
		return "you";
	} else if (letter == "v") {
		return "vee";
	} else if (letter == "w") {
		return "double you";
	} else if (letter == "x") {
		return "ex";
	} else if (letter == "y") {
		return "why";
	} else if (letter == "z") {
		return "zee";
	} else {
		return "";
	}
}

std::string phonetic_transcription(const std::string &text)
{
	// apply phonetic transcription to the text
	// any standalone letter should be converted to its phonetic equivalent
	// e.g. "a" -> "aei", "b" -> "bee", "c" -> "see", etc.

	// find standalone letters in the text, use a regular expression
	// to find all standalone letters in the text
	std::regex standalone_letter("\b[a-zA-Z]\b");
	std::smatch match;
	std::string result = text;

	while (std::regex_search(result, match, standalone_letter)) {
		// get the first match
		std::string letter = match.str();
		// replace the letter with its phonetic equivalent
		result = std::regex_replace(result, standalone_letter, phonetic_equivalent(letter));
	}

	return result;
}
