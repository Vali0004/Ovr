#pragma once
#include "includes.h"
#include "util/vehicle.h"

namespace tabs::vehicle {
	inline void tab() {
		elements::tabBar("Vehicle", [] {
			elements::tabItem("Spawner", [] {
				for (i8 i{}; i != 23; ++i) {
					elements::section(util::vehicle::g_modelClasses[i], [i] {
						auto vehicles{ util::vehicle::modelDataSection(i) };
						for (auto veh : vehicles) {
							elements::selectable(veh.m_finalLabel, false, [veh] {
								commands::g_engine.primitiveExecute("spawnVehicle {}", veh.m_modelInfo->m_model);
							}, true);
						}
					});
				}
			});
		});
	}
}