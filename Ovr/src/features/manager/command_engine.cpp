#include "command_engine.h"
#include "util/util.h"

namespace features::cmd {
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
				// This box would contain the players below instead of features.
				// Maybe write a wrapper for the command box GUI
				//g_box.set_context_item?
			});
		}
	}
	void engine::executeWithFeature(abstractFeature*& feature, std::string context) {
		std::vector<std::string> arguments{ g_splitStr(context, ' ') };
		size_t trueArgCount{ arguments.size() - 1 };
		if (feature->m_type != eFeatureType::ActionFeature && feature->m_type != eFeatureType::ToggleFeature && feature->m_type != eFeatureType::VariadicFeature) {
			if (feature->m_type != eFeatureType::ToggleIntFeature && feature->m_type != eFeatureType::ToggleFloatFeature) {
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
		switch (feature->m_type) {
		case eFeatureType::ToggleFeature: {
			feature->v().toggle = convertData<bool>(arguments[1]);
		} break;
		case eFeatureType::IntFeature: {
			feature->v().i32 = convertData<int>(arguments[1]);
		} break;
		case eFeatureType::FloatFeature: {
			feature->v().floating_point = convertData<float>(arguments[1]);
		} break;
		case eFeatureType::ToggleIntFeature: {
			feature->get(0).toggle = convertData<bool>(arguments[1]);
			feature->get(1).i32 = convertData<int>(arguments[1]);
		} break;
		case eFeatureType::ToggleFloatFeature: {
			feature->get(0).toggle = convertData<bool>(arguments[1]);
			feature->get(1).floating_point = convertData<float>(arguments[1]);
		} break;
		case eFeatureType::ActionFeature: {
			//We do handle it below, but we can't be sure
			// Just incase we provide a unneeded space, we won't kill everything. An example would be "suicide "
			feature->run();
		} break;
		case eFeatureType::ProtectionFeature: {
			feature->get(0).string = arguments[1].c_str();
		} break;
		case eFeatureType::VariadicFeature: {
			if (feature->has_value()) {
				if (feature->get_value(0)->m_type != eValueType::String) {
					if (feature->value_count() != trueArgCount) {
						LOG(Commands, "You provided {} arguments for a command that requires {} arguments.", trueArgCount, feature->value_count());
						return;
					}
					for (size_t i{ 1 }; i != arguments.size(); ++i) {
						switch (feature->get_value(i)->m_type) {
						case eValueType::String: {
							feature->get(i).string = arguments[i].c_str();
						} break;
						case eValueType::Boolean: {
							feature->get(i).toggle = convertData<bool>(arguments[i]);
						} break;
						case eValueType::Int8: {
							feature->get(i).i8 = convertData<int16_t>(arguments[i]);
						} break;
						case eValueType::UInt8: {
							feature->get(i).u8 = convertData<uint16_t>(arguments[i]);
						} break;
						case eValueType::Int16: {
							feature->get(i).i16 = convertData<int16_t>(arguments[i]);
						} break;
						case eValueType::UInt16: {
							feature->get(i).u16 = convertData<uint16_t>(arguments[i]);
						} break;
						case eValueType::Int32: {
							feature->get(i).i32 = convertData<int32_t>(arguments[i]);
						} break;
						case eValueType::UInt32: {
							feature->get(i).u32 = convertData<uint32_t>(arguments[i]);
						} break;
						case eValueType::Int64: {
							feature->get(i).i64 = convertData<int64_t>(arguments[i]);
						} break;
						case eValueType::UInt64: {
							feature->get(i).u64 = convertData<uint64_t>(arguments[i]);
						} break;
						case eValueType::GamePlayer: {
							feature->get(i).game_player = getPlayerForCommandArgument(arguments[i]).m_gamePlayer;
						} break;
						case eValueType::NetPlayer: {
							feature->get(i).net_player = getPlayerForCommandArgument(arguments[i]).m_netPlayer;
						} break;
						case eValueType::GamerInfo: {
							feature->get(i).gamer_info = getPlayerForCommandArgument(arguments[i]).m_gamerInfo;
						} break;
						}
					}
				}
				else {
					feature->m_buffer.clear();
					if (feature->value_count() == 1) {
						size_t index{ context.find(arguments[1]) };
						feature->m_buffer.push_back(context.substr(index));
						feature->get(0).string = feature->m_buffer[0].c_str();
					}
					else {
						feature->m_buffer.push_back("Reversed");
						for (size_t i{ 1 }; i != feature->value_count(); ++i) {
							feature->m_buffer.push_back(arguments[i]);
							feature->get(i).string = feature->m_buffer[i].c_str();
						}
					}
				}
			}
			if (feature->m_type == eFeatureType::VariadicFeature) {
				if (!dynamic_cast<variadicFeature*>(feature)->looped())
					feature->run();
			}
			if (!feature->m_looped)
				feature->run();
		} break;
		default: {

		} break;
		}
	}
	 void engine::execute(std::string& string) {
		if (!string.size()) {
			LOG(Commands, "Empty command string!");
			return;
		}
		std::vector<std::string> words{ g_splitStr(string, ' ') };
		if (words.empty()) {
			LOG(Commands, "No command!");
			return;
		}
		if (isNumber(words.at(0))) {
			LOG(Commands, "Provide a command!");
			return;
		}
		abstractFeature* feature{ getFeature(words[0]) };
		if (!feature)
			return;
		if (words.size() == 1) {
			if (feature->m_type == eFeatureType::ToggleFeature) {
				feature->get(0).toggle ^= true;
				replaceFeature(feature);
				return;
			}
			if (feature->m_type != eFeatureType::ActionFeature) {
				LOG(Commands, "You provided no arguments for a command that requires arguments!");
				return;
			}
			else {
				feature->run();
				return;
			}
		}
		else if (words.size() > 1) {
			executeWithFeature(feature, string.substr(string.find(words[0])));
			string = m_autoComplete ? feature->m_id : string;
			replaceFeature(feature);
		}
	}
	void engine::replaceFeature(abstractFeature* feature) {
		for (auto& f : g_manager.getFeatures()) {
			if (f->m_lookupId == feature->m_lookupId) {
				f = feature;
				break;
			}
		}
	}
	std::vector<abstractFeature*> engine::findMatches(std::string feature) {
		std::string lower{ feature };
		lower[0] = tolower(lower[0]);
		std::vector<abstractFeature*> matches{};
		for (auto& f : g_manager.getFeatures()) {
			if (f->m_id.find(feature) != std::string::npos) {
				matches.push_back(f);
			}
			else if (f->m_id.find(lower) != std::string::npos) {
				matches.push_back(f);
			}
		}
		return matches;
	}
	abstractFeature* engine::getFeature(std::string search) {
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
					LOG(Commands, "'{}' isn't unique enough. Using {} instead.", search, matches.at(0)->m_id);
					return matches.at(0);
				}
				LOG(Commands, "'{}' isn't unique enough. Maybe you meant {}?", search, matches.at(0)->m_id);
			}
			LOG(Commands, "{} found.", matches.at(0)->m_id);
			return matches.at(0);
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
	}
}