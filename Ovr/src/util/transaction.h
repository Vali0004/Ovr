#pragma once
#include "util.h"

namespace util::transactions {
	class queue {
	public:
		void push(std::function<void()> function) {
			std::lock_guard lock(m_mutex);
			m_stack.push(std::move(function));
		}
		void tick() {
			static GtaThread* thread{ util::classes::getGtaThread("shop_controller"_joaat) };
			std::unique_lock lock(m_mutex);
			if (!m_stack.empty()) {
				auto fn{ std::move(m_stack.top()) };
				m_stack.pop();
				lock.unlock();
				if (fn) {
					util::classes::runUnderThread(thread, fn);
				}
			}
		}
	private:
		std::recursive_mutex m_mutex{};
		std::stack<std::function<void()>> m_stack{};
	};
	inline queue g_queue{};
	inline void tickQueue() {
		while (true) {
			g_queue.tick();
			fiber::current()->sleep();
		}
	}
	inline void basket(u32 category, u32 action, u32 target, std::vector<std::array<i32, 5>> items) {
		g_queue.push([&] {
			const char* thrName{ SCRIPT::GET_THIS_SCRIPT_NAME() };
			printf("current thread from native: %s\n", thrName);
			if (NETSHOPPING::NET_GAMESERVER_BASKET_IS_ACTIVE()) {
				NETSHOPPING::NET_GAMESERVER_BASKET_END();
			}
			printf("basket pushed, ended current basket if active\n");
			int transactionId{ -1 };
			CNetworkShoppingMgr* pShoppingMgr{ *pointers::g_networkShoppingMgr };
			printf("deref ptr\n");
			if (pointers::g_constructBasket(pShoppingMgr, &transactionId, category, action, target)) {
				printf("construct basket\n");
				for (std::array<i32, 5>& item : items) {
					printf("add item\n");
					pointers::g_addItemToBasket(pShoppingMgr, item.data());
				}
				printf("checkout start\n");
				NETSHOPPING::NET_GAMESERVER_CHECKOUT_START(transactionId);
			}
		});
	}
	inline void basic(u32 category, u32 item, u32 action, i32 value, u32 target) {
		g_queue.push([&] {
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