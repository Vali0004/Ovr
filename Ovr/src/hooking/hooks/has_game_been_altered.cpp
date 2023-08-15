#include "hooking/hooking.h"

bool hooks::hasGameBeenAltered() {
	bool res{ CALL(hasGameBeenAltered) };
	if (res) {
		res = false;
	}
	return res;
}