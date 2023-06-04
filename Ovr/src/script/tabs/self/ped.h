#pragma once
#include "script/tabs/includes.h"

namespace tabs::self {
	inline void ped() {
		elements::tabItem("Ped", [] {
			elements::section("Proofs", [] {
				feature_checkbox("bulletProof"_TF);
				feature_checkbox("fireProof"_TF);
				feature_checkbox("collisionProof"_TF);
				feature_checkbox("meleeProof"_TF);
				feature_checkbox("godProof"_TF);
				feature_checkbox("explosionProof"_TF);
				feature_checkbox("steamProof"_TF);
				feature_checkbox("drownProof"_TF);
				feature_checkbox("waterProof"_TF);
			});
			elements::section("Health", [] {
				feature_button("maxHealth"_AF);
			});
			elements::section("Armor", [] {
				feature_button("maxArmor"_AF);
			});
			feature_intSlider("alpha"_IF, 0, 255);
			feature_checkbox("noRagdoll"_TF);
			feature_checkbox("invisibility"_TF);
			feature_button("suicide"_AF);
		});
	}
}