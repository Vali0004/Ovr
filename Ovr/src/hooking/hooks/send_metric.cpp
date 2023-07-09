#include "hooking/hooking.h"

const u32 g_blockedMetrics[]{
	"DIG"_joaat,
	"XP_LOSS"_joaat,
	"AWARD_XP"_joaat,
	"CF"_joaat,
	"CC"_joaat,
	"CNR"_joaat,
	"SCRIPT"_joaat,
	"CHEAT"_joaat,
	"AUX_DEUX"_joaat,
	"HARDWARE_OS"_joaat,
	"HARDWARE_GPU"_joaat,
	"HARDWARE_MOBO"_joaat,
	"HARDWARE_MEM"_joaat,
	"HARDWARE_CPU"_joaat,
	"PCSETTINGS"_joaat,
	"CASH_CREATED"_joaat,
	"DS_PS"_joaat,
	"UVC"_joaat,
	"W_L"_joaat,
	"ESVCS"_joaat,
	"IDLEKICK"_joaat,
	"GSCB"_joaat,
	"GSINV"_joaat,
	"GSCW"_joaat,
	"GSINT"_joaat,
	"EARN"_joaat,
	"GARAGE_TAMPER"_joaat
};
bool hooks::sendMetric(rage::rlMetric* pMetric, bool Unk) {
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
	if (pMetric->get_name() == "SPAWN" && json.str().find("-") != std::string::npos) {
		LOG(Debug, "Oh no bro.");
	}
	LOG(Info, "[Metric{}][SendMetric{}]: {}", pMetric->get_name(), key, json.str());
	#endif
	if (pMetric->using_c()) {
		if (pMetric->crc_flag()) {
			return false;
		}
	}
	for (i8 i{}; i != COUNT(g_blockedMetrics); ++i) {
		if (pMetric->get_name_hash() == g_blockedMetrics[i]) {
			return false;
		}
	}
	RET_CALL(sendMetric, pMetric, Unk);
}