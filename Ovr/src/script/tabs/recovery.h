#pragma once
#include "includes.h"
#include "util/transaction.h"

namespace tabs::recovery {
	inline void tab() {
		elements::menu("Recovery", [] {
			elements::button("Impulse detecc", [] {
				util::transactions::basic("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 0x129A5B6E, 0x562592BB, 50000, 2);
			});
			elements::button("Test", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_JOB_BONUS"_joaat),
						1, 
						15000000,
						0,
						1
					}
				});
			});
		});
	}
}