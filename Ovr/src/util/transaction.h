#pragma once
#include "util.h"
#include <util/game_vm_guard.h>

namespace util::transactions {
	inline void basket(u32 category, u32 action, u32 target, std::vector<std::array<i32, 5>> items) {
		g_GlobalGameVMGuard.GetGuard("shop_controller"_joaat)->setCallbackOnThread([&] {
			const char* thrName{ SCRIPT::GET_THIS_SCRIPT_NAME() };
			printf("current thread from native: %s\n", thrName);
			printf("current thread from ctx: %s\n", rage::tlsContext::get()->m_script_thread->m_name);
			if (NETSHOPPING::NET_GAMESERVER_BASKET_IS_ACTIVE()) {
				NETSHOPPING::NET_GAMESERVER_BASKET_END();
			}
			printf("basket pushed, ended current basket if active\n");
			int transactionId{ 2147483647 };
			CNetworkShoppingMgr* pShoppingMgr{ *pointers::g_networkShoppingMgr };
			if (pointers::g_constructBasket(pShoppingMgr, &transactionId, category, action, target)) {
				printf("construct basket\n");
				for (std::array<i32, 5>& item : items) {
					printf("add item\n");
					pointers::g_addItemToBasket(pShoppingMgr, item.data());
				}
				printf("checkout start (id: %i)\n", transactionId);
				NETSHOPPING::NET_GAMESERVER_CHECKOUT_START(transactionId);
			}
		});
	}
	inline void basic(u32 category, u32 item, u32 action, i32 value, u32 target) {
		g_GlobalGameVMGuard.GetGuard("shop_controller"_joaat)->setCallbackOnThread([&] {
			if (NETSHOPPING::NET_GAMESERVER_BASKET_IS_ACTIVE()) {
				NETSHOPPING::NET_GAMESERVER_BASKET_END();
			}
			int transactionId{ -1 };
			CNetworkShoppingMgr* pShoppingMgr{ *pointers::g_networkShoppingMgr };
			if (pointers::g_beginService(pShoppingMgr, &transactionId, "NET_SHOP_TTYPE_SERVICE"_joaat, category, item, action, value, target)) {
				NETSHOPPING::NET_GAMESERVER_CHECKOUT_START(transactionId);
			}
		});
	}
}