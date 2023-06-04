#include "hooking/hooking.h"
#define S_TO_MS(s) 1000 * s

bool hooks::hasIntervalElapsed(u32 Timestamp, u32 Interval) {
	switch (Interval) {
	case S_TO_MS(3): {
		return false;
	} break;
	case S_TO_MS(30): {
		return false;
	} break;
	}
	RET_CALL(hasIntervalElapsed, Timestamp, Interval)
}