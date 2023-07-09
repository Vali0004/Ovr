#pragma once
#include "script/tabs/includes.h"
#include "util/vehicle.h"

namespace tabs::vehicle {
	inline void spawner() {
		elements::tabItem("Spawner", [] {
			for (u8 i{}; i != 23; ++i) {
				elements::section(util::vehicle::g_modelClasses[i], [i] {
					auto vehicles{ util::vehicle::g_models.get(i) };
					for (auto veh : vehicles) {
						elements::selectable(veh.m_finalLabel, false, [veh] {
							commands::g_engine.primitiveExecute("spawnVehicle {}", veh.m_modelInfo->m_name);
						});
					}
				});
			}
		});
	}
}