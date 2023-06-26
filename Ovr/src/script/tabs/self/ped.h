#pragma once
#include "script/tabs/includes.h"

namespace tabs::self {
	inline void ped() {
		elements::tabItem("Ped", [] {
			elements::section("Proofs", [] {
				feature_checkbox("bulletProof"_TC);
				feature_checkbox("fireProof"_TC);
				feature_checkbox("collisionProof"_TC);
				feature_checkbox("meleeProof"_TC);
				feature_checkbox("godProof"_TC);
				feature_checkbox("explosionProof"_TC);
				feature_checkbox("steamProof"_TC);
				feature_checkbox("drownProof"_TC);
				feature_checkbox("waterProof"_TC);
			});
			elements::section("Health", [] {
				feature_button("maxHealth"_AC);
			});
			elements::section("Armor", [] {
				feature_button("maxArmor"_AC);
			});
			feature_intSlider("alpha"_IC, 0, 255);
			feature_checkbox("noRagdoll"_TC);
			feature_checkbox("invisibility"_TC);
			feature_button("suicide"_AC);
		});
	}
}