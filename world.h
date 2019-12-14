#pragma once

// required by std::vector
#include <vector>

// required by std::map
#include <map>

// required by MemberId, BridgeId, DistanceType
#include "common_defs.h"

// required by Hiker
#include "hiker.h"

// required by Bridge
#include "bridge.h"


class World {
private:
	unsigned m_last_hiker_id = 1;
	std::vector<Bridge> m_bridges;
	std::map<MemberId, Hiker> m_hikers;
	std::set<MemberId> m_team_members;

	MemberId generate_new_member_id() {
		return ++m_last_hiker_id;
	}

public:
	World() :
		m_last_hiker_id(1),
		m_bridges(),
		m_hikers(),
		m_team_members() {
	}

	BridgeId add_bridge(DistanceType length) {
		m_bridges.push_back(Bridge(length));
		return m_bridges.size() - 1;
	}

	MemberId add_team_member(
		const std::string & name,
		DistanceType speed);

	MemberId add_lone_hiker(
		const std::string & name,
		DistanceType speed,
		BridgeId bridge_id);

	void move_hiker(MemberId hiker_id, BridgeId target_bridge_id);

	Bridge * get_bridge_ptr(BridgeId bridge_id);
	const Bridge * get_bridge_ptr(BridgeId bridge_id) const;

	BridgeId get_first_bridge_id() const;
	BridgeId get_next_bridge_id(BridgeId prev_bridge_id) const;
	const Hiker & get_hiker(MemberId id) const;

	const decltype(m_team_members) & get_team() const {
		return m_team_members;
	}

	bool belongs_to_team(MemberId hiker_id) const;
};

