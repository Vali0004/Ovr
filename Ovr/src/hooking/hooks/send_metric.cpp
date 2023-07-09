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
	std::string name{ pMetric->get_name() };
	if (!name.compare("SPAWN")) {
		std::string s{ json.str() };
		size_t start{ s.find(R"("c":[)") + 5 };
		size_t end{ s.find(R"(],"d")") - 5 };
		std::string str{ s.substr(start, end) };
		auto words{ splitString(str, ',') };
		Vector3 coords{ stof(words[0]), stof(words[1]), stof(words[2]) };
		LOG(Info, "Spawning at {}, {}, {}", coords.x, coords.y, coords.z);
	}
	LOG(Debug, "[Metric{}][SendMetric{}]: {}", pMetric->get_name(), key, json.str());
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