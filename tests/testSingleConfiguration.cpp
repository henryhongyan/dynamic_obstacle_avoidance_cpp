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
	bool aggregated = true;

	unsigned int nb_obstacles = 3;

	unsigned int seed = 5821;
	srand(seed);

	// generate the list of obstacles
	std::deque<std::unique_ptr<Obstacle> > obstacle_list;
	for(unsigned int i=0; i<nb_obstacles; ++i)
	{
		Eigen::Vector3d pos(MathTools::rand_float(3,-3), MathTools::rand_float(3,-3), 0);
		Eigen::Quaterniond rot(Eigen::AngleAxisd(MathTools::rand_float(2)*M_PI, Eigen::Vector3d::UnitZ()));
		auto ptrE = std::make_unique<Ellipsoid>(State(pos, rot));
		ptrE->set_axis_lengths(Eigen::Array3d(MathTools::rand_float(3,0.2), MathTools::rand_float(3,0.2), 0));
		obstacle_list.push_back(std::move(ptrE));
	}

	// aggregate the obstacles if necessary
	std::deque<std::unique_ptr<Obstacle> > aggregated_obstacle_list = Aggregation::aggregate_obstacles(obstacle_list);

	// create the agent
	Eigen::Vector3d agent_position(MathTools::rand_float(5, -5), 8, 0);
	State agent_state(agent_position);
	Agent agent(agent_state, 0.1);

	// create the target
	Eigen::Vector3d target_position(MathTools::rand_float(5, -5), -8, 0);
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
	Eigen::Vector3d previous_vel;
	for(int i=0; i<nb_steps; ++i)
	{
		Eigen::Vector3d current_position = agent.get_position();
		position_history.push_back(current_position);

		Eigen::Vector3d desired_velocity = -Kp * (current_position - target_position);
		agent.set_linear_velocity(0.8 * desired_velocity + 0.2 * previous_vel);

		Eigen::Vector3d modulated_velocity = Modulation::modulate_velocity(agent, (aggregated ? aggregated_obstacle_list : obstacle_list));
		Eigen::Vector3d modulated_position = current_position + dt * modulated_velocity;
		agent.set_position(modulated_position);

		if(plot_steps)
		{	
			PlottingTools::plot_configuration(agent, (aggregated ? aggregated_obstacle_list : obstacle_list), target_position, position_history, "test_seed_" + std::to_string(seed) + "_step_" + std::to_string(i), is_show);
		}

		previous_vel = desired_velocity;
	}

	PlottingTools::plot_configuration(agent, (aggregated ? aggregated_obstacle_list : obstacle_list), target_position, position_history, "test_seed_" + std::to_string(seed), is_show);
	//PlottingTools::plot_configuration((aggregated ? aggregated_obstacle_list : obstacle_list), "test_seed_" + std::to_string(seed), is_show);
}