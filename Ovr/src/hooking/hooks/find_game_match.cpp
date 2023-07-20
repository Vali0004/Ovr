#include "hooking/hooking.h"
#include "fiber/pool.h"
#include "commands/features.h"

bool hooks::findGameMatch(i32 ProfileIndex, i32 AvailableSlots, NetworkGameFilterMatchmakingComponent* pFilter, u32 Count, rage::rlSessionInfo* pSessions, i32* OutputSize, rage::rlTaskStatus* pStatus) {
	u32 discriminator{ pFilter->m_param_values[0] };
	if (pFilter->m_filter_type == 1) {
		pStatus->m_state = 1;
		g_pool.add([&] {
			bool result{ commands::features::network::session::browser::backend::g_browser.matchmake(discriminator) };
			if (result) {
				for (i32 i{}; i != commands::features::network::session::browser::backend::g_browser.count(); ++i) {
					auto& s{ commands::features::network::session::browser::backend::g_browser.sessions()[i] };
					if (s) {
						pSessions[*OutputSize] = s.info;
						(*OutputSize)++;
						if (Count <= *OutputSize) {
							break;
						}
					}
				}
				pStatus->m_state = 3;
			}
			else {
				pStatus->m_state = 2;
			}
		});
	}
	RET_CALL(findGameMatch, ProfileIndex, AvailableSlots, pFilter, Count, pSessions, OutputSize, pStatus);
}