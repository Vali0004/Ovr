#include "hooking/hooking.h"

u64 hooks::writePlayerGameStateDataNode(rage::netObject* pObject, CPlayerGameStateDataNode* pNode) {
	u64 ret{ CALL(writePlayerGameStateDataNode, pObject, pNode) };
	pNode->m_is_invincible = false;
	pNode->m_bullet_proof = false;
	pNode->m_melee_proof = false;
	pNode->m_steam_proof = false;
	pNode->m_explosion_proof = false;
	pNode->m_water_proof = false;
	pNode->m_fire_proof = false;
	pNode->m_collision_proof = false;

	pNode->m_super_jump = false;

	pNode->m_is_spectating = false;
	pNode->m_spectating_net_id = NULL;
	return ret;
}