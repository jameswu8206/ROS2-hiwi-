import rclpy
from rclpy.node import Node

from std_msgs.msg import String
from geometry_msgs.msg import Point
from interfaces.msg import Consensus
import math
import time

class Robot4(Node):

    def __init__(self):
        super().__init__('Robot4')

        self.publisher_2 = self.create_publisher(Consensus, 'edge42', 10)
        self.subscription2=self.create_subscription(Consensus, 'edge24',self.listener_callback, 10)

        self.publisher_3 = self.create_publisher(Consensus, 'edge43', 10)
        self.subscription3=self.create_subscription(Consensus, 'edge34',self.listener_callback, 10)


        self.received_2=0
        self.received_3=0

        self.my_pos=float(input("Enter distance from target :")) 
        self.dist_list=[self.my_pos,]
        self.get_logger().info(f'current_min:{min(self.dist_list)}')
        self.iteration=0
        self.pub_info()
    def listener_callback(self, msg):
        if msg.from_robot==2:
            self.dist_list.append(msg.dist)
            self.received_2=1
        else:
            self.dist_list.append(msg.dist)
            self.received_3=1
        if self.received_2==1 and self.received_3==1:
            self.get_logger().info(f'current_min:{min(self.dist_list)}')
            self.iteration+=1
            self.pub_info()
            self.received_2=0
            self.received_3=0

        

        
        
    
    def pub_info(self):
        info=Consensus()
        info.dist=min(self.dist_list)
        info.from_robot = 4
        self.publisher_2.publish(info)
        self.publisher_3.publish(info)
        self.dist_list=[info.dist,]
        time.sleep(1)


def main(args=None):
    rclpy.init(args=args)

    robot4 = Robot4()

    rclpy.spin(robot4)
    robot4.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()