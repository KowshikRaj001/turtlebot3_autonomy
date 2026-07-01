#!/usr/bin/env python3

from shutil import move

import rclpy
from rclpy.node import Node

from sensor_msgs.msg import LaserScan
from geometry_msgs.msg import TwistStamped


class ObstacleAvoidance(Node):

    def __init__(self):
        super().__init__('obstacle_avoidance')

        # Subscribe to LiDAR
        self.subscription = self.create_subscription(
            LaserScan,
            '/scan',
            self.scan_callback,
            10)

        # Publish robot velocity
        self.publisher = self.create_publisher(
            TwistStamped,
            '/cmd_vel',
            10)

        self.get_logger().info("Obstacle Avoidance Node Started")

    def scan_callback(self, msg):

        move = TwistStamped()

        # Clean ranges
        ranges = [r if r > 0.0 else 999.0 for r in msg.ranges]

        # Regions
        front = min(min(ranges[0:20]), min(ranges[-20:]))
        left = min(ranges[60:120])
        right = min(ranges[240:300])

        self.get_logger().info(
            f"Front: {front:.2f} Left: {left:.2f} Right: {right:.2f}"
        )

    # Obstacle ahead
        if front < 0.7:

            self.get_logger().info("Obstacle ahead!")

            move.twist.linear.x = 0.0

        # Turn toward more open space
            if left > right:
                self.get_logger().info("Turning left")
                move.twist.angular.z = 0.8
            else:
                self.get_logger().info("Turning right")
                move.twist.angular.z = -0.8

        else:

            self.get_logger().info("Moving forward")

            move.twist.linear.x = 0.2
            move.twist.angular.z = 0.0

        move.header.stamp = self.get_clock().now().to_msg()

        self.publisher.publish(move)


def main(args=None):

    rclpy.init(args=args)

    node = ObstacleAvoidance()

    rclpy.spin(node)

    node.destroy_node()

    rclpy.shutdown()


if __name__ == '__main__':
    main()