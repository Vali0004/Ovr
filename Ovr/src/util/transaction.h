#pragma once
#include "util.h"
#include <util/game_vm_guard.h>

namespace util::transactions {
	inline void basket(u32 category, u32 action, u32 flags, std::vector<CNetShopTransaction> items) {
		g_GlobalGameVMGuard.GetGuard("shop_controller"_joaat)->setCallbackOnThread([&] {
			cc* thrName{ SCRIPT::GET_THIS_SCRIPT_NAME() };
			printf("current thread from native: %s\n", thrName);
			printf("current thread from ctx: %s\n", rage::tlsContext::get()->m_script_thread->m_name);
			if (NETSHOPPING::NET_GAMESERVER_BASKET_IS_ACTIVE()) {
				NETSHOPPING::NET_GAMESERVER_BASKET_END();
			}
			printf("basket pushed, ended current basket if active\n");
			i32 transactionId{ -1 };
			CNetworkShoppingMgr* pShoppingMgr{ *pointers::g_networkShoppingMgr };
			if (pointers::g_constructBasket(pShoppingMgr, &transactionId, category, action, flags)) {
				printf("construct basket\n");
				for (auto& item : items) {
					printf("add item\n");
					pointers::g_addItemToBasket(pShoppingMgr, (i32*)&item);
				}
				printf("checkout start (id: %i)\n", transactionId);
				NETSHOPPING::NET_GAMESERVER_CHECKOUT_START(transactionId);
			}
		});
	}
	inline void basic(u32 category, u32 item, u32 action, i32 value, i32 flags) {
		g_GlobalGameVMGuard.GetGuard("shop_controller"_joaat)->setCallbackOnThread([&] {
			if (NETSHOPPING::NET_GAMESERVER_BASKET_IS_ACTIVE()) {
				NETSHOPPING::NET_GAMESERVER_BASKET_END();
			}
			i32 transactionId{};
			CNetworkShoppingMgr* pShoppingMgr{ *pointers::g_networkShoppingMgr };
			if (pointers::g_beginService(pShoppingMgr, &transactionId, "NET_SHOP_TTYPE_SERVICE"_joaat, category, item, action, value, flags)) {
				NETSHOPPING::NET_GAMESERVER_CHECKOUT_START(transactionId);
			}
		});
	}
}