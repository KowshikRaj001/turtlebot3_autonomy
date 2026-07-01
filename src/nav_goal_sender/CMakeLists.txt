cmake_minimum_required(VERSION 3.8)
project(nav_goal_sender)

# ── Compiler warnings (good practice for ROS2 packages) ──────────────────────
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# ── Find dependencies ─────────────────────────────────────────────────────────
find_package(ament_cmake REQUIRED)       # ROS2 build system
find_package(rclcpp REQUIRED)            # C++ client library
find_package(rclcpp_action REQUIRED)     # Action client/server API
find_package(nav2_msgs REQUIRED)         # NavigateToPose action type
find_package(geometry_msgs REQUIRED)     # PoseStamped
find_package(tf2 REQUIRED)               # Quaternion helper

# ── Build the executable ──────────────────────────────────────────────────────
add_executable(nav_goal_sender src/nav_goal_sender.cpp)

target_include_directories(nav_goal_sender PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

target_compile_features(nav_goal_sender PUBLIC cxx_std_17)

ament_target_dependencies(nav_goal_sender
  rclcpp
  rclcpp_action
  nav2_msgs
  geometry_msgs
  tf2)

# ── Install ───────────────────────────────────────────────────────────────────
install(TARGETS nav_goal_sender
  DESTINATION lib/${PROJECT_NAME})

ament_package()