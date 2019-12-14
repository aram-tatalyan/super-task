#pragma once

// required by std::min
#include <algorithm>

// required by SolutionStrategy, SolutionResult
#include "solution_strategy.h"

// required by get_fastest_teammate_id and get_crossing_time
#include "world_util.h"



class StrategySequential : public SolutionStrategy{
public:
	enum Mode {
		mode_all_together = 1,
		mode_help_in_place = 2
	};

	explicit StrategySequential(Mode mode) : m_mode(mode) {
	}

private:
	virtual SolutionResult resolve_impl(World & w) override;

	void process_bridge(World & w, MemberId torch_holder_id,
		BridgeId bridge_id, SolutionResult * result_ptr);

	void process_hiker_position(World & w, MemberId hiker_id,
			BridgeId next_bridge_id, Bridge & bridge);

	Mode m_mode;
};


// consider to move hiker_id position to another bridge
// depending on circumastances
inline void StrategySequential::process_hiker_position(
	World & w,
	MemberId hiker_id,
	BridgeId next_bridge_id,
	Bridge & bridge)
{
	// if hiker is on the last bridge
	if (next_bridge_id == static_cast<BridgeId>(-1)) {
		// remove hiker record from the bridge
		bridge.remove_hiker(hiker_id);
		return;
	}
	switch (m_mode) {
		case Mode::mode_all_together:
			// continue the journey with current hiker no matter if he/she is
			// from team or not
			w.move_hiker(hiker_id, next_bridge_id);
			break;
		case Mode::mode_help_in_place:
			// if hiker is teammate
			if (w.belongs_to_team(hiker_id)) {
				// move hiker record to the next bridge to continue the journey
				// together on the next step
				w.move_hiker(hiker_id, next_bridge_id);
			}
			else {
				// Hiker is stranger that is why we will not continue the
				// journey on the next step in this mode
				bridge.remove_hiker(hiker_id);
			}
			break;
		default:
			throw EXCEPTION("invalid mode");
	}
}


inline void StrategySequential::process_bridge(
	World & w,
	MemberId torch_holder_id,
	BridgeId bridge_id,
	SolutionResult * result_ptr)
{
	Bridge & bridge = *(w.get_bridge_ptr(bridge_id));

	const Hiker & torch_holder = w.get_hiker(torch_holder_id);

	auto hikers_ids = bridge.get_hikers();

	// excluding torch holder
	unsigned peers_count = hikers_ids.size() - 1;

	BridgeId next_bridge_id = w.get_next_bridge_id(bridge_id);

	// if current bridge is not the last one
	if (next_bridge_id != static_cast<BridgeId>(-1)) {
		// move torch holder record to the next bridge
		w.move_hiker(torch_holder_id, next_bridge_id);
	}
	else {
		// remove torch holder record from current bridge
		bridge.remove_hiker(torch_holder_id);
	}

	// if there is no peer then torch holder should cross the bridge by himself
	// only
	if (peers_count == 0) {
		double bridge_crossing_time =
			get_crossing_time(bridge.get_length(), torch_holder.get_speed());
		result_ptr->set_bridge_cross_time(bridge_id, bridge_crossing_time);
		return;
	}

	double bridge_crossing_time = 0.0;

	// if there is more than one peer then torch holder should cross back the
	// bridge to pick another hiker, we need to consider how many times torch
	// holder should cross back
	if (peers_count > 1) {
		double leader_cross_back_time =
			(peers_count - 1) * bridge.get_length() / torch_holder.get_speed();
		bridge_crossing_time += leader_cross_back_time;
	}

	// for each hiker at the bridge
	for (MemberId hiker_id : hikers_ids) {
		// except torch holder
		if (hiker_id == torch_holder_id) {
			continue;
		}
		const Hiker & hiker = w.get_hiker(hiker_id);
		// consider the lowest speed of hiker and torch holder
		DistanceType pair_crossing_speed =
			std::min(hiker.get_speed(), torch_holder.get_speed());
		
		double pair_crossing_time = bridge.get_length() / pair_crossing_speed;
		bridge_crossing_time += pair_crossing_time;

		// consider to move hiker_id position to another bridge
		// depending on circumastances
		this->process_hiker_position(w, hiker_id,
			next_bridge_id, bridge);
	}
	result_ptr->set_bridge_cross_time(bridge_id, bridge_crossing_time);
}


inline SolutionResult StrategySequential::resolve_impl(World & w) {
	SolutionResult result;

	MemberId torch_holder_id = get_fastest_teammate_id(w);

	BridgeId bridge_id = w.get_first_bridge_id();

	// NOTE: we assume that team is located at the first bridge

	while (bridge_id != static_cast<BridgeId>(-1)) {

		this->process_bridge(w, torch_holder_id, bridge_id, &result);

		bridge_id = w.get_next_bridge_id(bridge_id);
	}

	return result;
}
