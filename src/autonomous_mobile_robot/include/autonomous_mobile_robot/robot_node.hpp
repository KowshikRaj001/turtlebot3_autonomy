#pragma once

#include <rclcpp/rclcpp.hpp>

namespace autonomous_mobile_robot
{

class RobotNode : public rclcpp::Node
{
public:
    RobotNode();

private:
    void timerCallback();

    rclcpp::TimerBase::SharedPtr timer_;
};

} // namespace autonomous_mobile_robot