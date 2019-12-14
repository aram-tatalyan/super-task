#pragma once

// required by std::map
#include <map>

// required by std::fabs
#include <cmath>

// required by DistanceType, BridgeId, MemberId
#include "common_defs.h"

// required by World
#include "world.h"

// required by EXCEPTION macro
#include "exception.h"

// required by EXCEPTION(msg) macro
using crow_treasure_chest::create_detailed_exception;


class SolutionResult {
public:
	float get_total_cross_time() const {
		unsigned time = 0;
		for (auto item : m_time_to_cross_bridge) {
			time += item.second;
		}
		return time;
	}

	float get_bridge_cross_time(BridgeId id) const {
		auto it = m_time_to_cross_bridge.find(id);

		if (it == m_time_to_cross_bridge.end()) {
			throw EXCEPTION("invalid bridge id");
		}

		return it->second;
	}

	void set_bridge_cross_time(BridgeId id, float time) {
		m_time_to_cross_bridge[id] = time;
	}

	// TODO: bad performance, redesign
	BridgeId get_first_bridge_id() const {
		auto it = m_time_to_cross_bridge.begin();
		if (it == m_time_to_cross_bridge.end()) {
			return -1;
		}
		return it->first;
	}

	// TODO: bad performance, redesign
	BridgeId get_next_bridge_id(BridgeId prev_bridge_id) const {
		auto it = m_time_to_cross_bridge.find(prev_bridge_id);
		if (it == m_time_to_cross_bridge.end()) {
			return -1;
		}
		++it;
		if (it == m_time_to_cross_bridge.end()) {
			return -1;
		}
		return it->first;
	}

	bool operator == (const SolutionResult & other_result) const {
		if (m_time_to_cross_bridge.size() !=
			other_result.m_time_to_cross_bridge.size()) {
			return false;
		}
		for (auto pair : m_time_to_cross_bridge) {
			BridgeId b_id = pair.first;
			float b_crossing_time = pair.second;

			auto other_result_item_it = other_result.m_time_to_cross_bridge.find(b_id);
			if (other_result_item_it == other_result.m_time_to_cross_bridge.end()) {
				return false;
			}

			float other_bridge_crossing_time = other_result_item_it->second;

			if (std::fabs(b_crossing_time - other_bridge_crossing_time) > 0.000001) {
				return false;
			}
		}
		return true;
	}

	bool operator != (const SolutionResult & other_result) const {
		return !(this->operator == (other_result));
	}

private:
	std::map<BridgeId, float> m_time_to_cross_bridge;
};


class SolutionStrategy {
public:
	SolutionResult resolve(World & w) {
		return resolve_impl(w);
	}

private:
	virtual SolutionResult resolve_impl(World & w) = 0;
};
