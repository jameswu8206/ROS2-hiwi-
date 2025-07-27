import rclpy
from rclpy.node import Node

from std_msgs.msg import String
from geometry_msgs.msg import Point
from interfaces.msg import Consensus
import math
import time

class Robot1(Node):

    def __init__(self):
        super().__init__('Robot1')
        self.publisher_ = self.create_publisher(Consensus, 'edge12', 10)
        self.subscription=self.create_subscription(Consensus, 'edge21',self.listener_callback, 10)
        self.my_pos=float(input("Enter distance from target :")) 
        self.dist_list=[self.my_pos,]
        self.get_logger().info(f'current_min:{min(self.dist_list)}')
        self.iteration=0
        self.pub_info()
        
    def listener_callback(self, msg):
        self.dist_list.append(msg.dist)  
        self.iteration+=1
        current_min=min(self.dist_list)
        self.get_logger().info(f'current_min:{current_min}')
        self.pub_info()
        
    
    def pub_info(self):
        info=Consensus()
        info.dist=min(self.dist_list)
        info.from_robot = 1
        self.publisher_.publish(info)
        time.sleep(1)

def main(args=None):
    rclpy.init(args=args)

    robot1 = Robot1()

    rclpy.spin(robot1)
    robot1.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()