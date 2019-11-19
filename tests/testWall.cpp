#include "DynamicObstacleAvoidance/Modulation.hpp"
#include "DynamicObstacleAvoidance/Obstacle/Ellipsoid.hpp"
#include "DynamicObstacleAvoidance/Obstacle/Aggregate.hpp"
#include "DynamicObstacleAvoidance/Utils/Plotting/PlottingTools.hpp"
#include "DynamicObstacleAvoidance/Utils/ObstacleGeneration/Aggregation.hpp"
#include "DynamicObstacleAvoidance/Utils/MathTools.hpp"
#include "DynamicObstacleAvoidance/Agent.hpp"
#include <eigen3/Eigen/Core>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>

using namespace DynamicObstacleAvoidance;

int main(int, char*[])
{
	srand(time(0));

	double Kp = 1;
	double dt = 0.01;

	int nb_steps = 1000;
	bool is_show = true;
	bool debug = false;
	bool plot_steps = false;

	unsigned int seed = 679;
	srand(seed);

	// generate the list of obstacles
	Eigen::Vector3d position_o1(-5, 4, 0);
	Eigen::Vector3d position_o2(-5, -4, 0);
	Eigen::Vector3d position_o3(-2, 0, 0);
	Eigen::Vector3d position_o4(-8, 0, 0);

	auto ptrE1 = std::make_unique<Ellipsoid>(State(position_o1), 0.1, "e1");
	auto ptrE2 = std::make_unique<Ellipsoid>(State(position_o2), 0.1, "e2");
	auto ptrE3 = std::make_unique<Ellipsoid>(State(position_o3), 0.1, "e3");
	auto ptrE4 = std::make_unique<Ellipsoid>(State(position_o4), 0.1, "e4");

	ptrE1->set_axis_lengths(Eigen::Array3d(3.5, 1, 0));
	ptrE2->set_axis_lengths(Eigen::Array3d(3.5, 1, 0));
	ptrE3->set_axis_lengths(Eigen::Array3d(1, 3.5, 0));
	ptrE4->set_axis_lengths(Eigen::Array3d(1, 3.5, 0));

	std::deque<std::unique_ptr<Obstacle> > obstacle_list;
	obstacle_list.push_back(std::move(ptrE1));
	obstacle_list.push_back(std::move(ptrE2));
	obstacle_list.push_back(std::move(ptrE3));
	obstacle_list.push_back(std::move(ptrE4));

	// aggregate the obstacles if necessary
	std::deque<std::unique_ptr<Obstacle> > aggregated_obstacle_list = Aggregation::aggregate_obstacles(obstacle_list);

	// create the agent
	Eigen::Vector3d agent_position(-4, 0, 0);
	State agent_state(agent_position);
	Agent agent(agent_state, 0.1);

	// create the target
	Eigen::Vector3d target_position(5, MathTools::rand_float(8, -8), 0);
	std::deque<Eigen::Vector3d> position_history;

	if(debug) 
	{
		for(auto& o:obstacle_list) std::cerr << *o << std::endl;
		std::cerr << std::endl;
		std::cerr << agent << std::endl;
		std::cerr << "target: (" << target_position(0) << ", ";
		std::cerr << target_position(1) << ", ";
		std::cerr << target_position(2) << ")" << std::endl;
	}

	// control loop
	for(int i=0; i<nb_steps; ++i)
	{
		Eigen::Vector3d current_position = agent.get_position();

		if(agent.exist_path(target_position, aggregated_obstacle_list))
		{
			position_history.push_back(current_position);

			Eigen::Vector3d desired_velocity = -Kp * (current_position - target_position);
			agent.set_linear_velocity(desired_velocity);

			Eigen::Vector3d modulated_velocity = Modulation::modulate_velocity(agent, aggregated_obstacle_list);
			Eigen::Vector3d modulated_position = current_position + dt * modulated_velocity;
			agent.set_position(modulated_position);
		}
		else
		{
			std::cout << "No path to target!" << std::endl;
		}

		if(plot_steps)
		{	
			PlottingTools::plot_configuration(agent, obstacle_list, target_position, position_history, "test_seed_" + std::to_string(seed) + "_step_" + std::to_string(i), is_show);
		}
	}

	PlottingTools::plot_configuration(agent, aggregated_obstacle_list, target_position, position_history, "test_seed_" + std::to_string(seed), is_show);
}