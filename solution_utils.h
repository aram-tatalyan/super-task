#pragma once

// required by std::cout
#include <iostream>

// required by BridgeId
#include "common_defs.h"

// required by World
#include "world.h"

// required by SolutionResult
#include "solution_strategy.h"


void print_solution_result(const World & w, const SolutionResult & r) {
	BridgeId bridge_id = r.get_first_bridge_id();

	while (bridge_id != static_cast<BridgeId>(-1)) {
		const Bridge & bridge = *w.get_bridge_ptr(bridge_id);

		std::cout << "Bridge id=" << bridge_id
			<< " length=" << bridge.get_length()
			<< " cross time=" << r.get_bridge_cross_time(bridge_id)
			<< std::endl;

		bridge_id = r.get_next_bridge_id(bridge_id);
	}

	std::cout << "Total cross time=" << r.get_total_cross_time() << std::endl;
}
