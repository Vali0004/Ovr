#include "hooking/hooking.h"
#include <json/json.h>

nlohmann::json j{};
bool badTransaction(nlohmann::json& data, u32 hash, std::string msg) {
	if (data["Category"].get<u32>() == hash) {
		return true;
	}
	return false;
}
bool badAction(nlohmann::json& data, u32 hash, u32 actionHash, std::string msg) {
	if (data["Category"].get<u32>() == hash && data["Action"].get<u32>() == actionHash) {
		return true;
	}
	return false;
}
bool hooks::addItemToBasket(CNetworkShoppingMgr* pTransactionMgr, i32* Items) {
	if (pTransactionMgr) {
		auto nodes{ pTransactionMgr->m_transaction_nodes };
		for (auto node{ nodes.m_head }; node; node->m_next) {
			auto transaction{ node->m_data };
			if (transaction->m_category == "SERVICE_BONUS"_joaat) {
				return false;
			}
			if (transaction->m_category == "CATEGORY_SERVICE_WITH_THRESHOLD"_joaat && transaction->m_action == "NET_SHOP_ACTION_BONUS"_joaat) {
				return false;
			}
		}
	}
	RET_CALL(addItemToBasket, pTransactionMgr, Items);
}