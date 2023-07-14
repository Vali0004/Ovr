#pragma once
#include "script/tabs/includes.h"

namespace tabs::self {
	inline void ped() {
		elements::menu("Ped", [] {
			elements::menu("Proofs", [] {
				cbutton("setAllProofs"_AC);
				ccheckbox("bulletProof"_TC);
				ccheckbox("fireProof"_TC);
				ccheckbox("collisionProof"_TC);
				ccheckbox("meleeProof"_TC);
				ccheckbox("godProof"_TC);
				ccheckbox("explosionProof"_TC);
				ccheckbox("steamProof"_TC);
				ccheckbox("drownProof"_TC);
				ccheckbox("waterProof"_TC);
			});
			elements::menu("Health", [] {
				cbutton("maxHealth"_AC);
			});
			elements::menu("Armor", [] {
				cbutton("maxArmor"_AC);
			});
			cintSlider("alpha"_IC, 0, 255);
			ccheckbox("noCollision"_TC);
			ccheckbox("noRagdoll"_TC);
			ccheckbox("invisibility"_TC);
			ccheckbox("autoHeal"_TC);
			ccheckbox("instantRespawn"_TC);
			ccheckbox("keepLastCoordinatesOnDeath"_TC);
			cbutton("clearTasks"_AC);
			cbutton("clone"_AC);
			cbutton("suicide"_AC);
		});
	}
}