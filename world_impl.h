#include "world.h"

// required by EXCEPTION macro
#include "exception.h"

// required by EXCEPTION(msg) macro
using crow_treasure_chest::create_detailed_exception;


inline MemberId World::add_lone_hiker(
	const std::string & name, 
	DistanceType speed, 
	BridgeId bridge_id)
{
	if (bridge_id >= m_bridges.size()) {
		throw EXCEPTION("Invalid bridge_id");
	}
	Bridge & bridge = m_bridges[bridge_id];
	MemberId hiker_id = generate_new_member_id();
	auto insert_result = m_hikers.emplace(
		std::make_pair(hiker_id, Hiker(name, speed, bridge_id)));
	if (!insert_result.second) {
		throw EXCEPTION("this should not happen");
	}
	//Hiker & hiker_ref = insert_result.first->second;
	bridge.add_hiker(hiker_id);
	return hiker_id;
}

inline MemberId World::add_team_member(
	const std::string & name,
	DistanceType speed)
{
	BridgeId team_bridge_id = 0;
	MemberId member_id = this->add_lone_hiker(name, speed, team_bridge_id);
	m_team_members.insert(member_id);
	return member_id;
}

inline void World::move_hiker(MemberId member_id, BridgeId target_bridge_id) {
	auto hiker_it = m_hikers.find(member_id);
	if (hiker_it == m_hikers.end()) {
		throw EXCEPTION("this should not happen");
	}
	Hiker & hiker = hiker_it->second;
	Bridge & bridge = m_bridges[hiker.get_bridge_id()];
	bridge.remove_hiker(member_id);
	hiker.set_bridge_id(target_bridge_id);
	Bridge & new_bridge = m_bridges[target_bridge_id];
	new_bridge.add_hiker(member_id);
}

inline const Bridge * World::get_bridge_ptr(BridgeId bridge_id) const {
	if (bridge_id >= m_bridges.size()) {
		throw EXCEPTION("Invalid bridge_id");
	}
	return &(m_bridges[bridge_id]);
}

inline Bridge * World::get_bridge_ptr(BridgeId bridge_id) {
	if (bridge_id >= m_bridges.size()) {
		throw EXCEPTION("Invalid bridge_id");
	}
	return &(m_bridges[bridge_id]);
}

BridgeId World::get_first_bridge_id() const {
	if (m_bridges.empty()) {
		return -1;
	}
	return 0;
}

BridgeId World::get_next_bridge_id(BridgeId prev_bridge_id) const {
	if (prev_bridge_id >= m_bridges.size()-1) {
		return -1;
	}
	return ++prev_bridge_id;
}

const Hiker & World::get_hiker(MemberId id) const {
	auto hiker_it = m_hikers.find(id);
	if (hiker_it == m_hikers.end()) {
		throw EXCEPTION("this should not happen");
	}
	return hiker_it->second;
}

bool World::belongs_to_team(MemberId hiker_id) const {
	auto it = m_team_members.find(hiker_id);
	if (it == m_team_members.end()) {
		return false;
	}
	return true;
}
