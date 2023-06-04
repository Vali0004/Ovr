#pragma once
#include "includes.h"
#include "network/session.h"
#include "network/friends.h"
#include "network/socialclub.h"
#include "network/tunables.h"

namespace tabs::network {
	inline void tab() {
		elements::tabBar("Network", [] {
			network::session::tab();
			network::friends();
			network::socialclub();
			network::tunables();
		});
	}
}