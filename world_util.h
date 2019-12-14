#pragma once

#include <iostream>

#include "world.h"


void print_world(const World & world) {
	BridgeId first_b_id = world.get_first_bridge_id();
	if (first_b_id == static_cast<decltype(first_b_id)>(-1)) {
		return;
	}
	for (BridgeId b_id = first_b_id;
		b_id != static_cast<decltype(b_id)>(-1);
		b_id = world.get_next_bridge_id(b_id))
	{
		const Bridge & bridge = *world.get_bridge_ptr(b_id);
		auto hikers_ids = bridge.get_hikers();
		std::cout << "Bridge #" << b_id << " length="
			<< bridge.get_length() << std::endl;
		for (auto hiker_id : hikers_ids) {
			auto hiker = world.get_hiker(hiker_id);
			std::cout << "Hiker " << hiker.get_name() << " speed="
				<< hiker.get_speed() << std::endl;
		}
	}
}


double get_crossing_time(DistanceType distance, DistanceType speed) {
	return distance / speed;
}

MemberId get_fastest_teammate_id(const World & w) {
	auto team_members_id = w.get_team();

	DistanceType max_speed = 0;

	MemberId fastest_guy_id = -1;
	for (MemberId team_member_id : team_members_id) {
		const Hiker & hiker = w.get_hiker(team_member_id);
		if(hiker.get_speed() > max_speed) {
			max_speed = hiker.get_speed();
			fastest_guy_id = team_member_id;
		}
	}
	return fastest_guy_id;
}

