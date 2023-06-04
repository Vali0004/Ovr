#pragma once
#include "pch/pch.h"

namespace tables {
	enum class eSessionType : s32 {
		Public,
		NewPublic,
		ClosedCrew,
		Crew,
		ClosedFriend = 6,
		FindFriend = 9,
		Solo,
		InviteOnly,
		JoinCrew,
		SCTV,
		Leave = -1
	};
	struct SessionType {
		eSessionType type{};
		const char id[22]{};
		const char name[22]{};
	};
	inline constexpr SessionType g_sessions[]{
		{ eSessionType::Public, "public", "Public" },
		{ eSessionType::NewPublic, "new public", "New Public" },
		{ eSessionType::ClosedCrew, "closed crew", "Closed Crew" },
		{ eSessionType::Crew, "crew", "Crew" },
		{ eSessionType::ClosedFriend, "closed friend", "Closed Friend" },
		{ eSessionType::FindFriend, "friends", "Find Friends" },
		{ eSessionType::Solo, "solo", "Solo" },
		{ eSessionType::InviteOnly, "invite", "Invite-Only" },
		{ eSessionType::JoinCrew, "join crew", "Join Crewmates" },
		{ eSessionType::SCTV, "admin", "SCTV" },
		{ eSessionType::Leave, "leave", "Leave" },
	};
}