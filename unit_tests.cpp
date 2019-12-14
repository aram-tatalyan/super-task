// required by std::cout, std::cerr
#include <iostream>

// required by EXIT_SUCCESS, EXIT_FAILURE
#include <cstdlib>

// required by BridgeId
#include "common_defs.h"

// required by World
#include "world.h"

// required to compile world.h implementation
#include "world_impl.h"

// required by print_world
#include "world_util.h"

// required by SolutionResult
#include "solution_strategy.h"

// required by StrategySequential
#include "strategy_sequential.h"

// required by print_solution_result
#include "solution_utils.h"


bool unit_test_1() {
	World world;

	BridgeId b1_id = world.add_bridge(100);
	BridgeId b2_id = world.add_bridge(250);
	BridgeId b3_id = world.add_bridge(150);

	world.add_team_member("A", 100.0);
	world.add_team_member("B", 50.0);
	world.add_team_member("C", 20.0);
	world.add_team_member("D", 10.0);

	world.add_lone_hiker("E", 2.5, b2_id);
	world.add_lone_hiker("F", 25.0, b3_id);
	world.add_lone_hiker("G", 15.0, b3_id);

	std::cout << "World is" << std::endl;
	print_world(world);
	std::cout << std::endl;

	{
		// For Strategy "All Together"
		SolutionResult expected_result;
		expected_result.set_bridge_cross_time(b1_id, 19.0);
		expected_result.set_bridge_cross_time(b2_id, 150.0);
		expected_result.set_bridge_cross_time(b3_id, 109.0);

		auto mode = StrategySequential::Mode::mode_all_together;
		StrategySequential s(mode);

		World w = world;
		SolutionResult r = s.resolve(w);
		std::cout << "Solution Results for \"All together\" mode" << std::endl;
		print_solution_result(w, r);
		if (expected_result != r) {
			std::cout << "Expected results are" << std::endl;
			print_solution_result(w, expected_result);
			return false;
		}
		else {
			std::cout << "Mode test PASS. Expected results are the same"
				<< std::endl;
		}
	}
	{
		// For Strategy "Help in place"
		SolutionResult expected_result;
		expected_result.set_bridge_cross_time(b1_id, 19.0);
		expected_result.set_bridge_cross_time(b2_id, 150.0);
		expected_result.set_bridge_cross_time(b3_id, 47.5);

		auto mode = StrategySequential::Mode::mode_help_in_place;
		StrategySequential s(mode);

		World w = world;
		SolutionResult r = s.resolve(w);
		std::cout << "Solution Results for \"Help in place\" mode" << std::endl;
		print_solution_result(w, r);
		if (expected_result != r) {
			std::cout << "Expected results are" << std::endl;
			print_solution_result(w, expected_result);
			return false;
		}
		else {
			std::cout << "Mode test PASS. Expected results are the same"
				<< std::endl;
		}
	}
	return true;
}

int main() {
	if (!unit_test_1()) {
		std::cerr << "\nUNIT TEST 1 FAILED" << std::endl;
		return EXIT_FAILURE;
	}
	std::cout << "\nALL TESTS PASS" << std::endl;
	return EXIT_SUCCESS;
}
