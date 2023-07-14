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
			j[transaction->m_category] = {
				{ "ID", transaction->m_transaction_id },
				{ "Category", transaction->m_category },
				{ "Action", transaction->m_action },
				{ "Target", transaction->m_target },
				{ "Count", transaction->m_transaction_count },
				{ "Transactions", {} }
			};
			for (i32 i{}; i != transaction->m_transaction_count; ++i) {
				auto& item{ transaction->m_transactions[i] };
				j[transaction->m_category]["Transactions"][item.m_id] = {
					{ "ID", item.m_id },
					{ "Variation", item.m_variation },
					{ "Price", item.m_price },
					{ "Multiplier", item.m_multiplier },
					{ "Value", item.m_value },
				};
			}
			LOG(Debug, j.dump(4));
			if (badTransaction(j[transaction->m_category], "SERVICE_BONUS"_joaat, "Transaction report hash")) {
				return false;
			}
			if (badAction(j[transaction->m_category], "CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, "NET_SHOP_ACTION_BONUS"_joaat, "Category Service (Threshold)")) {
				return false;
			}
		}
	}
	RET_CALL(addItemToBasket, pTransactionMgr, Items);
}