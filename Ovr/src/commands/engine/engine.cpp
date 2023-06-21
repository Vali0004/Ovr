#include "engine.h"
#include "util/util.h"

namespace commands {
	player engine::getPlayerForCommandArgument(std::string arg) {
		player p{};
		if (isNumber(arg)) {
			u64 indexFromArg{ stoull(arg) };
			if (indexFromArg > 30) {
				LOG(Commands, "The index '{}' provided is out of range! Please provide a name or valid index.", indexFromArg);
				return {};
			}
			util::network::iteratePlayers([&](u16 index, CNetGamePlayer* player) {
				if (index == indexFromArg) {
					p.m_index = index;
					p.m_name = player->GetName();
					p.m_gamePlayer = player;
					p.m_netPlayer = player;
					p.m_gamerInfo = player->GetGamerInfo();
					return true;
				}
			});
		}
		else {
			std::string name{ arg };
			util::network::iteratePlayers([&](u16 index, CNetGamePlayer* player) {
				if (player->GetName() == name) {
					p.m_index = index;
					p.m_name = player->GetName();
					p.m_gamePlayer = player;
					p.m_netPlayer = player;
					p.m_gamerInfo = player->GetGamerInfo();
					return true;
				}
				else if (std::string(player->GetName()).find(name) != -1) {
					LOG(Commands, "The name '{}' isn't unique enough! Please try again", name);
					return true;
				}
				//A context box would be a pretty good idea. TODO ig
				// This box would contain the players below instead of commands.
				// Maybe write a wrapper for the command box GUI
				//g_box.set_context_item?
			});
		}
	}
	void engine::executeWithCommand(abstractCommand*& command, std::string context) {
		std::vector<std::string> arguments{ g_splitStr(context, ' ') };
		size_t trueArgCount{ arguments.size() - 1 };
		if (command->m_type != eCommandType::ActionCommand && command->m_type != eCommandType::ToggleCommand && command->m_type != eCommandType::VariadicCommand) {
			if (command->m_type != eCommandType::ToggleIntCommand && command->m_type != eCommandType::ToggleFloatCommand) {
				if (trueArgCount != 1) {
					LOG(Commands, "You provided {} arguments for a command that requires one argument.", trueArgCount);
					return;
				}
			}
			else {
				if (trueArgCount != 2) {
					LOG(Commands, "You provided {} arguments for a command that requires 2 arguments.", arguments.size());
					return;
				}
			}
		}
		//Handle argument parsing
		switch (command->m_type) {
		case eCommandType::ToggleCommand: {
			command->v().toggle = convertData<bool>(arguments[1]);
		} break;
		case eCommandType::IntCommand: {
			command->v().i32 = convertData<int>(arguments[1]);
		} break;
		case eCommandType::FloatCommand: {
			command->v().floating_point = convertData<float>(arguments[1]);
		} break;
		case eCommandType::ToggleIntCommand: {
			command->get(0).toggle = convertData<bool>(arguments[1]);
			command->get(1).i32 = convertData<int>(arguments[1]);
		} break;
		case eCommandType::ToggleFloatCommand: {
			command->get(0).toggle = convertData<bool>(arguments[1]);
			command->get(1).floating_point = convertData<float>(arguments[1]);
		} break;
		case eCommandType::ActionCommand: {
			//We do handle it below, but we can't be sure
			// Just incase we provide a unneeded space, we won't kill everything. An example would be "suicide "
			command->run();
		} break;
		case eCommandType::ProtectionCommand: {
			command->get(0).string = arguments[1].c_str();
			static_cast<protectionCommand*>(command)->setState();
		} break;
		case eCommandType::VariadicCommand: {
			if (command->has_value()) {
				if (command->get_value(0)->m_type != eValueType::String) {
					if (command->value_count() != trueArgCount) {
						LOG(Commands, "You provided {} arguments for a command that requires {} arguments.", trueArgCount, command->value_count());
						return;
					}
					for (size_t i{ 1 }; i != arguments.size(); ++i) {
						switch (command->get_value(i)->m_type) {
						case eValueType::String: {
							command->get(i).string = arguments[i].c_str();
						} break;
						case eValueType::Boolean: {
							command->get(i).toggle = convertData<bool>(arguments[i]);
						} break;
						case eValueType::Int8: {
							command->get(i).i8 = convertData<int16_t>(arguments[i]);
						} break;
						case eValueType::UInt8: {
							command->get(i).u8 = convertData<uint16_t>(arguments[i]);
						} break;
						case eValueType::Int16: {
							command->get(i).i16 = convertData<int16_t>(arguments[i]);
						} break;
						case eValueType::UInt16: {
							command->get(i).u16 = convertData<uint16_t>(arguments[i]);
						} break;
						case eValueType::Int32: {
							command->get(i).i32 = convertData<int32_t>(arguments[i]);
						} break;
						case eValueType::UInt32: {
							command->get(i).u32 = convertData<uint32_t>(arguments[i]);
						} break;
						case eValueType::Int64: {
							command->get(i).i64 = convertData<int64_t>(arguments[i]);
						} break;
						case eValueType::UInt64: {
							command->get(i).u64 = convertData<uint64_t>(arguments[i]);
						} break;
						case eValueType::GamePlayer: {
							command->get(i).game_player = getPlayerForCommandArgument(arguments[i]).m_gamePlayer;
						} break;
						case eValueType::NetPlayer: {
							command->get(i).net_player = getPlayerForCommandArgument(arguments[i]).m_netPlayer;
						} break;
						case eValueType::GamerInfo: {
							command->get(i).gamer_info = getPlayerForCommandArgument(arguments[i]).m_gamerInfo;
						} break;
						}
					}
				}
				else {
					command->m_buffer.clear();
					if (command->value_count() == 1) {
						size_t index{ context.find(arguments[1]) };
						command->m_buffer.push_back(context.substr(index));
						command->get(0).string = command->m_buffer[0].c_str();
					}
					else {
						command->m_buffer.push_back("Reversed");
						for (size_t i{ 1 }; i != command->value_count(); ++i) {
							command->m_buffer.push_back(arguments[i]);
							command->get(i).string = command->m_buffer[i].c_str();
						}
					}
				}
			}
			if (command->m_type == eCommandType::VariadicCommand) {
				if (!dynamic_cast<variadicCommand*>(command)->looped())
					command->run();
			}
			if (!command->m_looped)
				command->run();
		} break;
		default: {

		} break;
		}
	}
	bool engine::execute(std::string& string) {
		if (!string.size()) {
			LOG(Commands, "Empty command string!");
			return false;
		}
		std::vector<std::string> words{ g_splitStr(string, ' ') };
		if (words.empty()) {
			LOG(Commands, "No command!");
			return false;
		}
		if (isNumber(words[0])) {
			LOG(Commands, "Provide a command!");
			return false;
		}
		abstractCommand* command{ getCommand(words[0]) };
		if (!command)
			return false;
		if (words.size() == 1) {
			if (command->m_type == eCommandType::ToggleCommand) {
				command->get(0).toggle ^= true;
				replaceCommand(command);
			}
			else if (command->m_type != eCommandType::ActionCommand) {
				LOG(Commands, "You provided no arguments for a command that requires arguments!");
				return false;
			}
			else {
				command->run();
			}
			return true;
		}
		else if (words.size() > 1) {
			executeWithCommand(command, string.substr(string.find(words[0])));
			string = m_autoComplete ? command->m_id : string;
			replaceCommand(command);
			return true;
		}
		return false;
	}
	void engine::replaceCommand(abstractCommand* command) {
		for (auto& f : g_manager.getCommands()) {
			if (f->m_lookupId == command->m_lookupId) {
				f = command;
				break;
			}
		}
	}
	std::vector<abstractCommand*> engine::findMatches(std::string command) {
		std::string lower{ command };
		lower[0] = tolower(lower[0]);
		std::vector<abstractCommand*> matches{};
		for (auto& f : g_manager.getCommands()) {
			if (f->m_id.find(command) != std::string::npos) {
				matches.push_back(f);
			}
			else if (f->m_id.find(lower) != std::string::npos) {
				matches.push_back(f);
			}
		}
		return matches;
	}
	abstractCommand* engine::getCommand(std::string search) {
		auto matches{ findMatches(search) };
		if (matches.empty()) {
			LOG(Commands, "'{}' isn't a valid command.", search);
			return nullptr;
		}
		else {
			if (m_useDirectMatchResults) {
				std::string lower{ search };
				lower[0] = tolower(lower[0]);
				for (auto& m : matches) {
					if (m->m_id == search || m->m_id == lower) {
						LOG(Commands, "{} found.", m->m_id);
						return m;
					}
				}
			}
			if (matches.size() > 1) {
				if (m_useFirstResultOnTooManyResults) {
					LOG(Commands, "'{}' isn't unique enough. Using {} instead.", search, matches[0]->m_id);
					return matches[0];
				}
				LOG(Commands, "'{}' isn't unique enough. Maybe you meant {}?", search, matches[0]->m_id);
			}
			LOG(Commands, "{} found.", matches[0]->m_id);
			return matches[0];
		}
		return nullptr;
	}
	template <typename t>
	t engine::convertData(std::string str) {
		if (isNumber(str)) {
			return (t)stod(str);
		}
		else if (containsANumber(str)) {
			if (str.find('.') == std::string::npos) {
				if (str.find("0x") != std::string::npos || str.find("0X") != std::string::npos) {
					return (t)stoull(str);
				}
			}
			else {
				return (t)stod(str);
			}
		}
		else {
			if (str == "on") {
				return (t)true;
			}
			else if (str == "off") {
				return (t)false;
			}
			else if (str == "true") {
				return (t)true;
			}
			else if (str == "false") {
				return (t)false;
			}
			else if (str == "t") {
				return (t)true;
			}
			else if (str == "f") {
				return (t)false;
			}
		}
		return (t)NULL;
	}
}