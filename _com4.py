import rclpy
from rclpy.node import Node

from std_msgs.msg import String
from geometry_msgs.msg import Point
from interfaces.msg import Communicate
import math
import time

class Robot4(Node):

    def __init__(self):
        super().__init__('Robot4')
        self.publisher_ = self.create_publisher(Communicate, 'edge43', 10)
        self.subscription=self.create_subscription(Communicate, 'edge34',self.listener_callback, 10)

        self.my_pos_x=float(input("Enter current x position: "))
        self.my_pos_y=float(input("Enter current y position: "))

    def listener_callback(self, info):
        dist=self.distance_to_target(info.x, info.y)
        if dist<info.dist:
            info.dist=dist
            info.robot=4
            self.get_logger().info(f'(x={info.x}, y={info.y}, {info.dist} ,{info.robot})')
        else:
            self.get_logger().info(f'(x={info.x}, y={info.y}, {info.dist} ,{info.robot})')
        time.sleep(1)  
        self.publisher_.publish(info)

    def distance_to_target(self, x,y):
        return math.sqrt((self.my_pos_x-x)**2+(self.my_pos_y-y)**2)
    
def main(args=None):
    rclpy.init(args=args)

    minimal_publisher = Robot4()

    rclpy.spin(minimal_publisher)

    # Destroy the node explicitly
    # (optional - otherwise it will be done automatically
    # when the garbage collector destroys the node object)
    minimal_publisher.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()