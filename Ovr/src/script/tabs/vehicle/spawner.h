#pragma once
#include "script/tabs/includes.h"
#include "util/vehicle.h"

namespace tabs::vehicle {
	inline char g_dummy[20]{};
	inline void spawner() {
		elements::menu("Spawner", [] {
			elements::menu("Options", [] {
				ccheckbox("spawnInsideVehicle"_TC);
				ccheckbox("spawnVehicleMaxed"_TC);
				ccheckbox("deleteLastSpawnedVehicle"_TC);
			});
			elements::menu("Custom", [] {
				elements::inputText("Input a name or hash", g_dummy);
				elements::button("Spawn", [] {
					commands::g_engine.primitiveExecute("spawnVehicle {}", g_dummy);
				});
			});
			elements::menu("Classes", [] {
				for (u8 i{}; i != 23; ++i) {
					elements::menu(util::vehicle::g_modelClasses[i], [i] {
						auto vehicles{ util::vehicle::g_models.get(i) };
						for (auto veh : vehicles) {
							elements::selectable(veh.m_finalLabel, false, [veh] {
								commands::g_engine.primitiveExecute("spawnVehicle {}", veh.m_modelInfo->m_name);
							});
						}
					});
				}
			});
		});
	}
}