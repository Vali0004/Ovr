#include "hooking/hooking.h"

bool hooks::presencePostRawMessage(int localGamerindex, const rage::rlGamerHandle* recipients, unsigned int numRecipients, const char* message, unsigned int ttlSeconds) {
	bool res = CALL(presencePostRawMessage, localGamerindex, recipients, numRecipients, message, ttlSeconds);
	LOG_DEBUG("Message: {}", message);
	if (res) {
		LOG_DEBUG("Message post passed");
	}
	return res;
}