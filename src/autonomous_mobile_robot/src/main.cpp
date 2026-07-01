#include "autonomous_mobile_robot/robot_node.hpp"

#include <memory>

#include <rclcpp/rclcpp.hpp>

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    auto node = std::make_shared<autonomous_mobile_robot::RobotNode>();

    rclcpp::spin(node);

    rclcpp::shutdown();

    return 0;
}