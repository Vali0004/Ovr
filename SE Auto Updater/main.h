#pragma once
#include "util.h"

extern void typeLogic();
extern void run();
class scriptEvent {
public:
	std::string m_name{};
	std::string m_oldNameHash{};
	std::string m_nameHash{};
	int32_t m_oldHash{};
	int32_t m_hash{};
};
class scriptEvents {
public:
	void write(int iteration, bool snake, bool camel, bool upper, bool toFile, bool print);
	void read();
private:
	void readContents();
	void getMatches();
public:
	void formatMatches(int enumIteration);
private:
	void writeEnum(bool snake, bool camel, bool upper);
	void writeAllOccurrences();
	void writeHashCheck();
private:
	std::stringstream m_stream{};
	std::string m_contents{};
	std::vector<std::smatch> m_matches{};
	std::vector<scriptEvent> m_events{};
};
inline scriptEvents g_scriptEvents{};