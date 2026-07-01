cmake_minimum_required(VERSION 3.8)
project(autonomous_mobile_robot)

# ==========================================================
# C++ Standard
# ==========================================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ==========================================================
# Compiler Warnings
# ==========================================================
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
    )
endif()

# ==========================================================
# Dependencies
# ==========================================================
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)

# ==========================================================
# Executable
# ==========================================================
add_executable(robot_node
    src/main.cpp
    src/robot_node.cpp
)

# ==========================================================
# Include Directories
# ==========================================================
target_include_directories(robot_node
PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# ==========================================================
# ROS Dependencies
# ==========================================================
ament_target_dependencies(robot_node
    rclcpp
)

# ==========================================================
# Install
# ==========================================================
install(TARGETS
    robot_node
    DESTINATION lib/${PROJECT_NAME}
)

install(
    DIRECTORY include/
    DESTINATION include
)

install(
    DIRECTORY launch config
    DESTINATION share/${PROJECT_NAME}
)

# ==========================================================
# Package
# ==========================================================
ament_package()