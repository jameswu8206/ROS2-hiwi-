import rclpy
from rclpy.node import Node

from std_msgs.msg import String
from geometry_msgs.msg import Point
from interfaces.msg import Communicate
import math
import time
class Robot3(Node):

    def __init__(self):
        super().__init__('Robot3')
        self.publisher_a = self.create_publisher(Communicate, 'edge31', 10)
        self.publisher_b = self.create_publisher(Communicate, 'edge32', 10)
        self.publisher_c = self.create_publisher(Communicate, 'edge34', 10)
        
        self.subscription_a=self.create_subscription(Communicate, 'edge13',self.listener_callback_a, 10)
        self.subscription_b=self.create_subscription(Communicate, 'edge23',self.listener_callback_b, 10)
        self.subscription_c=self.create_subscription(Communicate, 'edge43',self.listener_callback_c, 10)

        self.my_pos_x=float(input("Enter current x position: "))
        self.my_pos_y=float(input("Enter current y position: "))
        self.info_from_2=[0,0]
        self.info_from_4=[0,0]
    def listener_callback_a(self, info):
        dist=self.distance_to_target(info.x, info.y)
        if dist<info.dist:
            info.dist=dist
            info.robot=3
            self.get_logger().info(f'(x={info.x}, y={info.y}, {info.dist} ,{info.robot})')
            time.sleep(1)
        else:
            self.get_logger().info(f'(x={info.x}, y={info.y}, {info.dist} ,{info.robot})')
            time.sleep(1)

        self.publisher_b.publish(info)
        self.publisher_c.publish(info)

    def listener_callback_b(self, info):
        if self.info_from_2[1]==0 | (info.dist<self.info_from_2[0]):
            self.info_from_2[0]=info.dist
            self.info_from_2[1]= info.robot
            

        if self.info_from_4[1]!=0:
            if self.info_from_2[0]< self.info_from_4[0]:
                info.dist=self.info_from_2[0]
                info.robot=self.info_from_2[1]
                self.get_logger().info(f'(x={info.x}, y={info.y}, {info.dist} ,{info.robot})')
                time.sleep(1)
                self.publisher_a.publish(info)
                self.info_from_2=[0,0]
                self.info_from_4=[0,0]
            else:
                info.dist=self.info_from_4[0]
                info.robot=self.info_from_4[1]
                self.get_logger().info(f'(x={info.x}, y={info.y}, {info.dist} ,{info.robot})')
                time.sleep(1)
                self.publisher_a.publish(info)
                self.info_from_2=[0,0]
                self.info_from_4=[0,0]
        else:
            self.publisher_c.publish(info)
         
        
        
        
        self.get_logger().info(f'(x={info.x}, y={info.y}, {info.dist} ,{info.robot})')
        time.sleep(1)
        
    def listener_callback_c(self, info):
        if self.info_from_4[1]==0 | (info.dist<self.info_from_4[0]):
            self.info_from_4[0]=info.dist
            self.info_from_4[1]= info.robot

        if self.info_from_2[1]!=0:
            if self.info_from_2[0]< self.info_from_4[0]:
                info.dist=self.info_from_2[0]
                info.robot=self.info_from_2[1]
                self.publisher_a.publish(info)
            else:
                info.dist=self.info_from_4[0]
                info.robot=self.info_from_4[1]
                self.publisher_a.publish(info)
        else:
            self.publisher_b.publish(info)
        self.get_logger().info(f'(x={info.x}, y={info.y}, {info.dist} ,{info.robot})')
        
    def distance_to_target(self, x,y):
        return math.sqrt((self.my_pos_x-x)**2+(self.my_pos_y-y)**2)
    
def main(args=None):
    rclpy.init(args=args)

    minimal_publisher = Robot3()

    rclpy.spin(minimal_publisher)

    # Destroy the node explicitly
    # (optional - otherwise it will be done automatically
    # when the garbage collector destroys the node object)
    minimal_publisher.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()