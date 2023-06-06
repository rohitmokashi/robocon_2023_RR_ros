#!/usr/bin/env python3

import rospy
from geometry_msgs.msg import Twist
from std_msgs.msg import Float32

from nav_msgs.msg import Odometry

global odo, enc, lastEncValue
odo = [0, 0]
enc = [0, 0, 0, 0]
global pub
r = 0.005

def call1(data):
    enc[0] = data.data

def call2(data):
    enc[1] = data.data

def call3(data):
    enc[2] = data.data

def call4(data):
    enc[3] = data.data

def time_call(data):
    global pub
 
    odomm = Odometry()
    dt = data.data

    odomm.twist.twist.linear.x = (enc[0] + enc[1] + enc[2] + enc[3])*(r/4)
    odomm.twist.twist.linear.y = ((-1 * enc[0]) + enc[1] + enc[2] - enc[3])*(r/4)
    odomm.twist.twist.angular.z = ((-1 * enc[0]) + enc[1] - enc[2] + enc[3])*(r/4(0.250+0.250))

    odomm.pose.pose.position.x = odomm.twist.twist.linear.x * dt
    odomm.pose.pose.position.y = odomm.twist.twist.linear.y * dt
    odomm.pose.pose.orientation.z = odomm.twist.twist.angular.z * dt

    print(enc)

    pub.publish(odomm)

def main():
    print("started\n")
    global pub
    rospy.init_node('odom_node', anonymous=True)
    
    pub = rospy.Publisher("odom", Odometry, queue_size=10)
    rospy.Subscriber("enc1", Float32, call1)
    rospy.Subscriber("enc2", Float32, call2)
    rospy.Subscriber("enc3", Float32, call3)
    rospy.Subscriber("enc4", Float32, call4)
    rospy.Subscriber("delta_t", Int8, time_call)


    rospy.spin()

if __name__ == '__main__':
    main()