// required by std::cout, std::cerr
#include <iostream>

// required by EXIT_SUCCESS, EXIT_FAILURE
#include <cstdlib>

// required by YAML::* tools
#include <yaml-cpp/yaml.h>

// required by EXCEPTION macro
#include "exception.h"

// required by World
#include "world.h"

// to include World implementation into the module
#include "world_impl.h"

// required by StrategySequential
#include "strategy_sequential.h"

// required by print_world
#include "world_util.h"

// required by print_solution_result
#include "solution_utils.h"

// required by EXCEPTION(msg) macro
using crow_treasure_chest::create_detailed_exception;

using crow_treasure_chest::Exception;


static void add_lone_hikers_on_bridge(World & world, const YAML::Node & hikers, BridgeId bridge_id) {
	if (hikers.Type() != YAML::NodeType::Sequence) {
		throw EXCEPTION("\"hikers\" should be a sequence in YAML file or null");
	}
	for (unsigned i=0; i<hikers.size(); ++i) {
		YAML::Node hiker = hikers[i];
		std::string name = hiker["name"].as<std::string>();
		float speed = hiker["speed"].as<float>();
		auto hiker_id = world.add_lone_hiker(name, speed, bridge_id);
		std::cout << "Adding lone hiker " << hiker_id << " name=" << name
			<< " speed=" << speed << " on bridge " << bridge_id << std::endl;
	}
}

static void load_bridges(World & world, const YAML::Node & bridges) {
	for (unsigned i=0; i<bridges.size(); ++i) {
		YAML::Node bridge = bridges[i];
		float bridge_length = bridge["length"].as<float>();
		auto bridge_id = world.add_bridge(bridge_length);
		std::cout << "Adding bridge " << bridge_id << " length="
			<< bridge_length << std::endl;
		YAML::Node hikers = bridge["hikers"];
		if (hikers.Type() != YAML::NodeType::Null) {
			add_lone_hikers_on_bridge(world, hikers, bridge_id);
		}
	}
}

static void load_team_members(World & world, const YAML::Node & team_members) {
	for (unsigned i=0; i<team_members.size(); ++i) {
		YAML::Node team_member = team_members[i];
		std::string tm_name = team_member["name"].as<std::string>();
		float tm_speed = team_member["speed"].as<float>();
		std::cout << "Adding team member " << tm_name << " " << tm_speed << std::endl;
		world.add_team_member(tm_name, tm_speed);
	}
}

static World yaml_to_world(const char * yaml_file_path) {
	YAML::Node world_config = YAML::LoadFile(yaml_file_path);
	YAML::Node bridges = world_config["bridges"];
	YAML::Node team_members = world_config["team_members"];

	if (bridges.Type() != YAML::NodeType::Sequence) {
		throw EXCEPTION("\"bridges\" should be sequence in YAML file");
	}
	if (team_members.Type() != YAML::NodeType::Sequence) {
		throw EXCEPTION("\"team_members\" should be a sequence in YAML file");
	}

	World world;
	 
	load_bridges(world, bridges);
	load_team_members(world, team_members);

	return world;
}

static const char usage[] = "<app> <path to yaml file>";

static void process_solutions(const World & world) {
	std::cout << "\n\nPrinting World\n" << std::endl;
	print_world(world);
	{
		auto mode = StrategySequential::Mode::mode_all_together;
		StrategySequential s(mode);
		World w = world;
		SolutionResult r = s.resolve(w);
		std::cout << "\nPrinting results for \"All together\" strategy"
			<< std::endl;
		print_solution_result(w, r);
	}
	{
		auto mode = StrategySequential::Mode::mode_help_in_place;
		StrategySequential s(mode);
		World w = world;
		SolutionResult r = s.resolve(w);
		std::cout << "\nPrinting results for \"Help in place\" strategy"
			<< std::endl;
		print_solution_result(w, r);
	}
}

int main(int argc, char * argv[]) {
	if (argc == 1) {
		std::cout << usage << std::endl;
		return EXIT_SUCCESS;
	}
	if (argc != 2) {
		std::cerr << "Invalid arguments" << std::endl;
		std::cerr << usage << std::endl;
		return EXIT_FAILURE;
	}

	const char * yaml_file_path = argv[1];
	try {
		World world = yaml_to_world(yaml_file_path);
		process_solutions(world);
	}
	catch (const Exception & err) {
		std::cerr << err.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
