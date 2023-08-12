#include "hooking/hooking.h"
#include "commands/features.h"

bool hooks::sendMetric(rage::rlMetric* pMetric, bool Unk) {
	ccp key{};
	if (pMetric->using_a()) {
		key = "A";
	}
	else if (pMetric->using_b()) {
		key = "B";
	}
	else if (pMetric->using_c()) {
		key = "C";
	}
	rage::rlJSON json{ 256 };
	pMetric->to_json(&json);
	std::string name{ pMetric->get_name() };
	if (!name.compare("SPAWN")) {
		std::string s{ json.str() };
		size_t start{ s.find(R"("c":[)") + 5 };
		size_t end{ s.find(R"(],"d")") - 5 };
		std::string str{ s.substr(start, end) };
		auto words{ splitString(str, ',') };
		Vector3 newCoords{ stof(words[0]), stof(words[1]), stof(words[2]) };
		//I am so fucking glad I kept this debug shit in here
		if ("keepLastCoordinatesOnDeath"_TC->get(0).toggle) {
			g_pool.add([&] {
				Ped ped{ PLAYER::PLAYER_PED_ID() };
				while (ENTITY::IS_ENTITY_DEAD(ped, FALSE)) {
					fiber::current()->sleep(100ms);
				}
				fiber::current()->sleep(100ms);
				Vector3 coords{ commands::features::self::ped::g_coordsAtDeath };
				if (!coords.empty() && coords.valid()) {
					commands::g_engine.primitiveExecute("teleportToCoords {} {} {}", coords.x, coords.y, coords.z);
					LOG(Info, "Spawning at {}, {}, {}", coords.x, coords.y, coords.z);
				}
			});
		}
		else {
			LOG(Info, "Spawning at {}, {}, {}", newCoords.x, newCoords.y, newCoords.z);
		}
	}
	LOG_DEBUG("[Metric{}][SendMetric{}]: {}", pMetric->get_name(), key, json.str());
	return false;
	//RET_CALL(sendMetric, pMetric, Unk);
}