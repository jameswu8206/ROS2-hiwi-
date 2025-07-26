import rclpy
from rclpy.node import Node

from std_msgs.msg import String
from geometry_msgs.msg import Point
from interfaces.msg import Consensus
import math
import time

class Robot2(Node):

    def __init__(self):
        super().__init__('Robot2')
        self.publisher_1 = self.create_publisher(Consensus, 'edge21', 10)
        self.subscription1=self.create_subscription(Consensus, 'edge12',self.listener_callback, 10)

        self.publisher_3 = self.create_publisher(Consensus, 'edge23', 10)
        self.subscription3=self.create_subscription(Consensus, 'edge32',self.listener_callback, 10)

        self.publisher_4 = self.create_publisher(Consensus, 'edge24', 10)
        self.subscription4=self.create_subscription(Consensus, 'edge42',self.listener_callback, 10)

        self.received_1=0
        self.received_3=0
        self.received_4=0

        self.my_pos=float(input("Enter distance from target :")) 
        self.dist_list=[self.my_pos,]
        self.get_logger().info(f'current_min:{min(self.dist_list)}')
        self.iteration=0
        self.pub_info()
    def listener_callback(self, msg):
        if msg.from_robot==1:
            self.dist_list.append(msg.dist)
            self.received_1=1
        elif msg.from_robot==3:
            self.dist_list.append(msg.dist)
            self.received_3=1
        else:
            self.dist_list.append(msg.dist)
            self.received_4=1
        if self.received_1==1 & self.received_3==1 and self.received_4==1:
            self.get_logger().info(f'current_min:{min(self.dist_list)}')
            self.iteration+=1
            self.pub_info()
            self.received_1=0
            self.received_3=0
            self.received_4=0

        

        
        
    
    def pub_info(self):
        info=Consensus()
        info.dist=min(self.dist_list)
        info.from_robot = 2
        self.publisher_1.publish(info)
        self.publisher_3.publish(info)
        self.publisher_4.publish(info)
        self.dist_list=[info.dist,]
        time.sleep(1)


def main(args=None):
    rclpy.init(args=args)

    robot2 = Robot2()
    
    rclpy.spin(robot2)
    robot2.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()