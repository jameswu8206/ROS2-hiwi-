import rclpy
from rclpy.node import Node

from std_msgs.msg import String
from geometry_msgs.msg import Point
import math
from interfaces.msg import Communicate
import time
class Robot1(Node):

    def __init__(self):
        super().__init__('Robot1')
        self.publisher_ = self.create_publisher(Communicate, 'edge13', 10)
        self.subscription=self.create_subscription(Communicate, 'edge31',self.listener_callback, 10)

        self.publisher_user= self.create_publisher(Communicate, 'to_user', 10)
        self.subscription_user=self.create_subscription(Point, 'from_user',self.pub_to_others ,10)

        self.my_pos_x=float(input("Enter current x position: "))
        self.my_pos_y=float(input("Enter current y position: "))

    def listener_callback(self, info):
        self.get_logger().info(f'(x={info.x}, y={info.y}, {info.dist} ,{info.robot})')
        time.sleep(1)
        self.publisher_user.publish(info)
        

    def pub_to_others(self, msg):

        info=Communicate()
        info.x=msg.x
        info.y=msg.y
        info.dist=self.distance_to_target(info.x, info.y)
        info.robot=1
        self.get_logger().info(f'(x={info.x}, y={info.y}, {info.dist} ,{info.robot})')
        time.sleep(1)
        self.publisher_.publish(info)
    
    def distance_to_target(self, x,y):
        return math.sqrt((self.my_pos_x-x)**2+(self.my_pos_y-y)**2)

def main(args=None):
    rclpy.init(args=args)

    robot1 = Robot1()

    rclpy.spin(robot1)

    # Destroy the node explicitly
    # (optional - otherwise it will be done automatically
    # when the garbage collector destroys the node object)
    robot1.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()