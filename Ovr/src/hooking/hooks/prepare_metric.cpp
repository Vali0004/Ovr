#include "hooking/hooking.h"

//Use this ONLY for logging. Do not block ANY metrics here.
bool hooks::prepareMetric(rage::datBitBuffer* pBitBuffer, i32 Unk, i32 Time, rage::rlMetric* pMetric) {
	#ifdef DEBUG
	ccp key{};
	if (pMetric->using_a()) {
		key = "A";
	}
	else if (pMetric->using_b()) {
		key = "B";
	}
	else if (pMetric->using_c()) {
		key = "C";
	}
	rage::JSONSerialiser json{ 256 };
	pMetric->to_json(&json);
	LOG(Info, "[Metric{}::Prepare][SendMetric{}]: {}", pMetric->get_name(), key, json.str());
	#endif
	RET_CALL(prepareMetric, pBitBuffer, Unk, Time, pMetric);
}