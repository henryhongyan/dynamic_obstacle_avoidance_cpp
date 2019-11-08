/**
 * @class Aggregate
 * @brief Class to define an aggregation of obstacles
 * @author Baptiste Busch
 * @date 2019/05/21
 */

#ifndef DYNAMIC_OBSTACLE_AVOIDANCE_OBSTACLE_AGGREGATE_H_
#define DYNAMIC_OBSTACLE_AVOIDANCE_OBSTACLE_AGGREGATE_H_

#include <eigen3/Eigen/Core>
#include <memory>
#include <deque>
#include <limits>
#include "DynamicObstacleAvoidance/Obstacle/Obstacle.hpp"
#include "DynamicObstacleAvoidance/Obstacle/StarShapeHull.hpp"
#include <iostream>

namespace DynamicObstacleAvoidance
{
	class Aggregate: public Obstacle 
	{
	private:
		unsigned int nb_samples;
		double intersec_factor;
		double inside_factor;
		double distance_factor;
		double area_factor;
		StarShapeHull hull;
		std::deque<std::unique_ptr<Obstacle> > primitives;

		void update_positions();

		Aggregate* implicit_clone() const override;

		void update_hull();

	public:
		explicit Aggregate();

		explicit Aggregate(const std::deque<std::unique_ptr<Obstacle> >& primitives);

		const auto& get_primitives() const;

		void add_primitive(const std::unique_ptr<Obstacle>& primitive);

		void add_primitive(const Obstacle& primitive);

		const Obstacle& get_active_obstacle(const Agent& agent) const;

		Eigen::Vector3d compute_normal_to_agent(const Agent& agent) const;
		
		double compute_distance_to_agent(const Agent& agent) const;

		void draw(const std::string& color="k") const;

		std::ostream& print(std::ostream& os) const override;
	};

	inline const auto& Aggregate::get_primitives() const
	{
		return this->primitives;
	}

	inline std::ostream& Aggregate::print(std::ostream& os) const
	{ 
		os << static_cast<Obstacle>(*this) << std::endl;
		os << "Composed of:" << std::endl;
		for(auto& p:this->primitives)
		{
			os << "---" << std::endl;
			os << *p << std::endl;
		}
  		return os;
	}
}
#endif