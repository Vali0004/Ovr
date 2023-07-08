#pragma once
#include "pch/pch.h"
#include "memory/pointers.h"
#include "rage/classes.h"
#include "rage/enums.h"
#include "rage/commands/list.h"

namespace util::vehicle {
	class vehicleData {
	public:
		vehicleData(const CVehicleModelInfo* mi, const std::string ml, const std::string nl) : m_modelInfo(mi), m_manufacturerLabel(ml), m_nameLabel(nl), m_finalLabel(getLabelAsGXT()) {}
	public:
		const CVehicleModelInfo* m_modelInfo{};
		const std::string m_manufacturerLabel{}, m_nameLabel{}, m_finalLabel{};
	public:
		std::string getLabelAsGXT() {
			std::string modelName = m_nameLabel;
			std::string displayName = VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(m_modelInfo->m_model);
			std::string label = HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(displayName.c_str());
			auto lower = [](std::string data) {
				std::transform(data.begin(), data.end(), data.begin(), [](u8 c) { return std::tolower(c); });
				return data;
			};
			return strcmp(label.c_str(), "NULL") == 0 ? lower(strcmp(displayName.c_str(), "NULL") == 0 ? modelName : displayName) : label;
		}
	};
	struct models {
		std::vector<vehicleData> m_compacts{};
		std::vector<vehicleData> m_sedans{};
		std::vector<vehicleData> m_suvs{};
		std::vector<vehicleData> m_coupes{};
		std::vector<vehicleData> m_muscle{};
		std::vector<vehicleData> m_sportsClassics{};
		std::vector<vehicleData> m_sports{};
		std::vector<vehicleData> m_super{};
		std::vector<vehicleData> m_motorcycles{};
		std::vector<vehicleData> m_offRoad{};
		std::vector<vehicleData> m_industrial{};
		std::vector<vehicleData> m_utility{};
		std::vector<vehicleData> m_vans{};
		std::vector<vehicleData> m_cycles{};
		std::vector<vehicleData> m_boats{};
		std::vector<vehicleData> m_helicopters{};
		std::vector<vehicleData> m_planes{};
		std::vector<vehicleData> m_service{};
		std::vector<vehicleData> m_emergency{};
		std::vector<vehicleData> m_military{};
		std::vector<vehicleData> m_commercial{};
		std::vector<vehicleData> m_trains{};
		std::vector<vehicleData> m_openWheels{};
		std::vector<vehicleData> m_dummy{};
		std::vector<vehicleData>& get(i8 idx) {
			switch (idx) {
			case 0: { return m_compacts; } break;
			case 1: { return m_sedans; } break;
			case 2: { return m_suvs; } break;
			case 3: { return m_coupes; } break;
			case 4: { return m_muscle; } break;
			case 5: { return m_sportsClassics; } break;
			case 6: { return m_sports; } break;
			case 7: { return m_super; } break;
			case 8: { return m_motorcycles; } break;
			case 9: { return m_offRoad; } break;
			case 10: { return m_industrial; } break;
			case 11: { return m_utility; } break;
			case 12: { return m_vans; } break;
			case 13: { return m_cycles; } break;
			case 14: { return m_boats; } break;
			case 15: { return m_helicopters; } break;
			case 16: { return m_planes; } break;
			case 17: { return m_service; } break;
			case 18: { return m_emergency; } break;
			case 19: { return m_military; } break;
			case 20: { return m_commercial; } break;
			case 21: { return m_trains; } break;
			case 22: { return m_openWheels; } break;
			}
			return m_dummy;
		}
	};
	inline models g_models{};
	inline std::vector<std::string> g_modelClasses{};
	inline std::vector<vehicleData> modelDataSection(i8 modelClass) {
		std::vector<vehicleData> vehData{};
		for (i32 i{}; i != pointers::g_hashTable->m_size; ++i) {
			for (auto node = pointers::g_hashTable->m_lookup_table[i]; node; node = node->m_next) {
				if (const auto tableIdx = node->m_idx; tableIdx != pointers::g_hashTable->m_size) {
					if (auto model{ reinterpret_cast<CVehicleModelInfo*>(pointers::g_hashTable->m_data[tableIdx]) }; model && (static_cast<uint8_t>(model->m_model_type) & 0xFF) == (uint8_t)eModelType::Vehicle && (model->m_model_class & 0x1F) == modelClass) {
						std::string manufacturerLbl = HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(model->m_manufacturer);
						std::string nameLbl = HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(model->m_name);
						vehData.push_back({ model, manufacturerLbl, model->m_name });
					}
				}
			}
		}
		return vehData;
	}
	inline std::string getVehicleClassName(i8 vehClass) {
		std::stringstream ss; ss << "VEH_CLASS_" << vehClass;
		return std::string(HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(ss.str().c_str())).compare("NULL") ? HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(ss.str().c_str()) : "Unknown Class";
	}
	inline void cacheModelTable() {
		for (i8 i{}; i != 23; ++i) {
			g_modelClasses.push_back(getVehicleClassName(i));
			g_models.get(i) = modelDataSection(i);
		}
	}
}