#pragma once
#include "script/tabs/includes.h"

namespace tabs::weapon {
	inline void loadout() {
		elements::tabItem("Loadout", [] {
			elements::section("Modify", [] {
				elements::section("Add", [] {
					elements::button("All", [] {
						
					});
				});

			});
		});
	}
}