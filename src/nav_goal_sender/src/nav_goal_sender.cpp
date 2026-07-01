#include <rclcpp/rclcpp.hpp>
#include <rclcpp_action/rclcpp_action.hpp>
#include <nav2_msgs/action/navigate_to_pose.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <tf2/LinearMath/Quaternion.h>

using NavToPose = nav2_msgs::action::NavigateToPose;

using GoalHandleNav = rclcpp_action::ClientGoalHandle<NavToPose>;

class NavGoalSender : public rclcpp::Node
{
public:

  NavGoalSender() : Node("nav_goal_sender")
  {
    
    client_ = rclcpp_action::create_client<NavToPose>(
      this, "navigate_to_pose");

    RCLCPP_INFO(get_logger(), "NavGoalSender node created. Waiting for Nav2...");

    timer_ = create_wall_timer(
      std::chrono::seconds(1),
      std::bind(&NavGoalSender::send_goal, this));
  }

private:

  rclcpp_action::Client<NavToPose>::SharedPtr client_;
  rclcpp::TimerBase::SharedPtr timer_;

  void send_goal()
  {
    timer_->cancel();
    if (!client_->wait_for_action_server(std::chrono::seconds(10))) {
      RCLCPP_ERROR(get_logger(),
        "Nav2 action server not available after 10 s. Is Nav2 running?");
      rclcpp::shutdown();  
      return;
    }

    RCLCPP_INFO(get_logger(), "Nav2 server found. Sending goal...");

    NavToPose::Goal goal_msg;

    goal_msg.pose.header.frame_id = "map";

    goal_msg.pose.header.stamp = now();

    goal_msg.pose.pose.position.x = 2.0;
    goal_msg.pose.pose.position.y = 1.0;
    goal_msg.pose.pose.position.z = 0.0;  

    tf2::Quaternion q;
    q.setRPY(0.0, 0.0, 0.0);   // roll=0, pitch=0, yaw=0
    goal_msg.pose.pose.orientation.x = q.x();
    goal_msg.pose.pose.orientation.y = q.y();
    goal_msg.pose.pose.orientation.z = q.z();
    goal_msg.pose.pose.orientation.w = q.w();

    goal_msg.behavior_tree = "";

    auto opts = rclcpp_action::Client<NavToPose>::SendGoalOptions();

    opts.goal_response_callback =
      [this](const GoalHandleNav::SharedPtr & handle)
      {
        if (!handle) {
          RCLCPP_ERROR(get_logger(), "Goal was REJECTED by the server.");
          rclcpp::shutdown();
        } else {
          RCLCPP_INFO(get_logger(), "Goal ACCEPTED. Executing...");
        }
      };

    opts.feedback_callback =
      [this](GoalHandleNav::SharedPtr /*handle*/,
             const std::shared_ptr<const NavToPose::Feedback> fb)
      {
        // [27] Print distance_remaining (float32, metres).
        //      The cast to double prevents printf width-mismatch warnings.
        RCLCPP_INFO(get_logger(),
          "Distance remaining: %.2f m  |  Recoveries: %d",
          static_cast<double>(fb->distance_remaining),
          fb->number_of_recoveries);
      };


    opts.result_callback =
      [this](const GoalHandleNav::WrappedResult & wr)
      {
        switch (wr.code)
        {
          case rclcpp_action::ResultCode::SUCCEEDED:
            RCLCPP_INFO(get_logger(), "✓ Goal SUCCEEDED! Robot reached (2.0, 1.0).");
            break;

          case rclcpp_action::ResultCode::ABORTED:
            // ABORTED means Nav2 gave up after exhausting recovery attempts.
            RCLCPP_ERROR(get_logger(), "✗ Goal ABORTED by Nav2 (path blocked or recovery failed).");
            break;

          case rclcpp_action::ResultCode::CANCELED:
            RCLCPP_WARN(get_logger(), "⚠ Goal CANCELED.");
            break;

          default:
            RCLCPP_ERROR(get_logger(), "Unknown result code: %d",
              static_cast<int>(wr.code));
            break;
        }

        rclcpp::shutdown();
      };

    client_->async_send_goal(goal_msg, opts);

    RCLCPP_INFO(get_logger(),
      "Goal sent: x=2.0  y=1.0  yaw=0.0 rad  frame=map");
  }
};

// ─────────────────────────────────────────────────────────────────────────────
// [32] main() – ROS2 entry point.
int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);

  auto node = std::make_shared<NavGoalSender>();

  rclcpp::spin(node);

  rclcpp::shutdown();

  return 0;
}