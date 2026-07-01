#include "autonomous_mobile_robot/robot_node.hpp"

#include <chrono>

using namespace std::chrono_literals;

namespace autonomous_mobile_robot
{

RobotNode::RobotNode()
: Node("autonomous_mobile_robot")
{
    RCLCPP_INFO(get_logger(), "=================================");
    RCLCPP_INFO(get_logger(), " Autonomous Mobile Robot Started ");
    RCLCPP_INFO(get_logger(), " ROS2 Jazzy");
    RCLCPP_INFO(get_logger(), "=================================");

    timer_ = create_wall_timer(
        1s,
        std::bind(&RobotNode::timerCallback, this));
}

void RobotNode::timerCallback()
{
    RCLCPP_INFO(get_logger(), "Robot node running...");
}

} // namespace autonomous_mobile_robot