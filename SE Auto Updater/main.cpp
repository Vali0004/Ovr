#include "main.h"

void scriptEvents::write(int iteration, bool snake, bool camel, bool upper, bool toFile, bool print) {
	m_stream << std::endl;
	writeEnum(snake, camel, upper);
	m_stream << " | Iteration: " << iteration;
	writeHashCheck();
	std::string str{ m_stream.str() };
	if (toFile) {
		std::ofstream file{ "ScriptEvents.hpp" };
		file << str << std::endl;
	}
	if (print) {
		std::cout <<  str << std::endl;
	}
}
void scriptEvents::read() {
	readContents();
	getMatches();
}
void scriptEvents::readContents() {
	std::ifstream file{ "MP_Event_Enums.sch" };
	m_contents = { (std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>() };
}
void scriptEvents::getMatches() {
	std::regex expression{ R"_(\w+\s*=\s*HASH\(\"(.*)\"\))_" };
	std::sregex_iterator iter{ m_contents.begin(), m_contents.end(), expression };
	std::sregex_iterator end{};
	while (iter != end) {
		m_matches.push_back(*iter);
		++iter;
	}
}
void scriptEvents::formatMatches(int enumIteration) {
	std::string strToReplace{ "Event_Enums12" }; //12 is from 1.58, 14 is from 1.62-1.64, 15 is from1.65-1.66
	for (auto& match : m_matches) {
		scriptEvent se{};
		std::string str{ match.str(1) };
		if (size_t pos{ str.find(strToReplace) }; pos != std::string::npos) {
			std::string strBefore{ str.substr(0, pos) };
			std::string seperatedName{ str.substr(pos + strToReplace.size() + 1) }; //+1 for the period
			std::string nameHash{ strBefore + "Event_Enums" + std::to_string(enumIteration) + "." + seperatedName };
			std::string oldNameHash{ strBefore + "Event_Enums" + std::to_string(enumIteration - 1) + "." + seperatedName };
			std::string name{ seperatedName };
			if (size_t schPos{ name.find("sch.") }; schPos != std::string::npos) {
				name = seperatedName.substr(schPos + sizeof("sch.") - 1); //-1 for null terminator
			}
			se.m_name = name;
			se.m_nameHash = nameHash;
			se.m_oldNameHash = oldNameHash;
			se.m_oldHash = util::joaat(se.m_oldNameHash);
			se.m_hash = util::joaat(se.m_nameHash);
			m_events.push_back(se);
		}
	}
}
void scriptEvents::writeEnum(bool snake, bool camel, bool upper) {
	m_stream << "enum class eScriptEvents {" << '\n';
	for (int i{}; i != m_events.size() - 2; ++i) {
		auto& se{ m_events[i] };
		m_stream << util::enumify(se.m_name, se.m_hash, snake, camel, upper) << '\n';
	}
	auto& se{ m_events[m_events.size() - 1] };
	m_stream << util::enumify(se.m_name, se.m_hash, snake, camel, upper, true) << '\n';
	m_stream << "}; //Number of events: " << m_events.size();
}
void scriptEvents::writeAllOccurrences() {
	for (int i{}; i != 20; ++i) {
		formatMatches(i);
	}
	for (int i{}; i != m_events.size(); ++i) {
		auto& se{ m_events[i] };
		if (se.m_nameHash.find("TICKER_EVENT_SWAP_TEAM") != -1) {
			m_stream << se.m_nameHash << " = " << se.m_hash << '\n';
		}
	}
}
void scriptEvents::writeHashCheck() {
	m_events.clear();
	formatMatches(7);
	for (int i{}; i != m_events.size(); ++i) {
		auto& se{ m_events[i] };
		if (se.m_nameHash.find("TICKER_EVENT_SWAP_TEAM") != -1) {
			if (se.m_hash != -2070646108) {
				m_stream << " | Hashes broke at some point. Please do not use these hashes until fixed";
			}
			else {
				m_stream << " | Hashes are functioning properly";
			}
		}
	}
	m_events.clear();
}

void typeLogic() {
	vars::g_iteration = stoi(util::getLine("Iteration"));
	vars::g_snakeCase = util::conditionMet("Use snake case");
	vars::g_camelCase = util::conditionMet("Use camel case");
	vars::g_uppercase = util::conditionMet("Uppercase");
	vars::g_toFile = util::conditionMet("Write to file");
	vars::g_print = util::conditionMet("Print contents");
}
void run() {
	g_scriptEvents.read();
	typeLogic();
	g_scriptEvents.formatMatches(vars::g_iteration);
	g_scriptEvents.write(vars::g_iteration, vars::g_snakeCase, vars::g_camelCase, vars::g_uppercase, vars::g_toFile, vars::g_print);
	if (vars::g_print) {
		util::wait();
	}
}
int main() {
	run();
	return 0;
}