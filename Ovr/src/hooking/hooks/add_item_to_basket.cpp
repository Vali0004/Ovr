#include "hooking/hooking.h"
#include <json/json.h>

nlohmann::json j{};
bool badTransaction(nlohmann::json& data, u32 hash, std::string msg) {
	if (data["Category"].get<u32>() == hash) {
		u32 category{ data["Category"].get<u32>() };
		u32 id{ data["ID"].get<u32>() };
		u32 action{ data["Action"].get<u32>() };
		u32 target{ data["Target"].get<u32>() };
		u32 count{ data["Count"].get<u32>() };
		LOG(Info, "[rage::CNetShopTransactionMgr::Push]: {} pushed.", msg);
		LOG(Info, "Data: 0x{:X}[{}], {} executing {} for {} {} times", category, id, action, target, count);
		return true;
	}
	return false;
}
bool hooks::addItemToBasket(CNetShopTransactionMgr* pTransactionMgr, i32* Items) {
	if (pTransactionMgr) {
		for (auto node{ pTransactionMgr->m_first }; node; node = node->m_next) {
			auto transaction{ node->m_transaction_basket };
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
			if (badTransaction(j[transaction->m_category], 0xAE04310C, "Transaction report hash")) {
				return false;
			}
			LOG(Info, j.dump(4));
		}
	}
	RET_CALL(addItemToBasket, pTransactionMgr, Items);
}