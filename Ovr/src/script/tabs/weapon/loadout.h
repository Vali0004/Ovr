#pragma once
#include "script/tabs/includes.h"

namespace tabs::weapon {
	inline void loadout() {
		elements::menu("Loadout", [] {
			elements::menu("Modify", [] {
				elements::menu("Add", [] {
					elements::button("All", [] {
						
					});
				});

			});
		});
	}
}