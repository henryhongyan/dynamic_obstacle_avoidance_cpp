#include "DynamicObstacleAvoidance/Obstacle/Ellipsoid2D.hpp"
#include <gtest/gtest.h>
#include <eigen3/Eigen/Core>

TEST(ComputeDistanceToExternalPointNonNullPosition, PositiveNos)
{
	Eigen::Vector3f position;
	position << 2, 1, 0;

	Eigen::Vector4f orientation;
	orientation << 0, 0, 0, 1;

	Ellipsoid2D e(position, orientation);

	Eigen::Vector3f agent_position;
	agent_position << 1, 0, 0;

	std::cerr << "Computing ditance" << std::endl;
	double distance = e.compute_distance_to_external_point(agent_position);
	double truth = 2.0;
	std::cerr << "distance = " << distance << ", truth = " << truth << std::endl;

	ASSERT_NEAR(distance, truth, 0.01);
}

TEST(ComputeDistanceToExternalPointNullPosition, PositiveNos)
{
	Eigen::Vector3f position;
	position << 0, 0, 0;

	Eigen::Vector4f orientation;
	orientation << 0, 0, 0, 1;

	Ellipsoid2D e(position, orientation);

	Eigen::Vector3f agent_position;
	agent_position << 1, 0, 0;

	std::cerr << "Computing ditance" << std::endl;
	double distance = e.compute_distance_to_external_point(agent_position);
	double truth = 1.0;
	std::cerr << "distance = " << distance << ", truth = " << truth << std::endl;

	ASSERT_NEAR(distance, truth, 0.01);
}

TEST(ComputeDistanceToExternalPointCloseToObstacle, PositiveNos)
{
	Eigen::Vector3f position;
	position << 0.9, 0, 0;

	Eigen::Vector4f orientation;
	orientation << 0, 0, 0, 1;

	Ellipsoid2D e(position, orientation);

	Eigen::Vector3f agent_position;
	agent_position << 1, 0, 0;

	std::cerr << "Computing ditance" << std::endl;
	double distance = e.compute_distance_to_external_point(agent_position);
	double truth = 0.01;
	std::cerr << "distance = " << distance << ", truth = " << truth << std::endl;

	ASSERT_NEAR(distance, truth, 0.01);
}

TEST(ComputeNormalToExternalPointNonNullPosition, PositiveNos)
{
	Eigen::Vector3f position;
	position << 2, 1, 0;

	Eigen::Vector4f orientation;
	orientation << 0, 0, 0, 1;

	Ellipsoid2D e(position, orientation);

	Eigen::Vector3f agent_position;
	agent_position << 1, 0, 0;

	std::cerr << "Computing normal" << std::endl;
	Eigen::Vector3f normal = e.compute_normal_to_external_point(agent_position);
	Eigen::Vector3f truth;
	truth << -0.70710678, -0.70710678, 0.0;
	std::cerr << "normal = " << normal << ", truth = " << truth << std::endl;

	for(int i=0; i<normal.size(); ++i) ASSERT_NEAR(normal(i), truth(i), 0.01);
}

TEST(ComputeNormalToExternalPointNullPosition, PositiveNos)
{
	Eigen::Vector3f position;
	position << 0, 0, 0;

	Eigen::Vector4f orientation;
	orientation << 0, 0, 0, 1;

	Ellipsoid2D e(position, orientation);

	Eigen::Vector3f agent_position;
	agent_position << 1, 0, 0;

	std::cerr << "Computing normal" << std::endl;
	Eigen::Vector3f normal = e.compute_normal_to_external_point(agent_position);
	Eigen::Vector3f truth;
	truth << 1.0, 0.0, 0.0;
	std::cerr << "normal = " << normal << ", truth = " << truth << std::endl;

	for(int i=0; i<normal.size(); ++i) ASSERT_NEAR(normal(i), truth(i), 0.01);
}

TEST(ComputeNormalToExternalPointCloseToObstacle, PositiveNos)
{
	Eigen::Vector3f position;
	position << 0.9, 0, 0;

	Eigen::Vector4f orientation;
	orientation << 0, 0, 0, 1;

	Ellipsoid2D e(position, orientation);

	Eigen::Vector3f agent_position;
	agent_position << 1, 0, 0;

	std::cerr << "Computing normal" << std::endl;
	Eigen::Vector3f normal = e.compute_normal_to_external_point(agent_position);
	Eigen::Vector3f truth;
	truth << 1.0, 0.0, 0.0;
	std::cerr << "normal = " << normal << ", truth = " << truth << std::endl;

	for(int i=0; i<normal.size(); ++i) ASSERT_NEAR(normal(i), truth(i), 0.01);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}