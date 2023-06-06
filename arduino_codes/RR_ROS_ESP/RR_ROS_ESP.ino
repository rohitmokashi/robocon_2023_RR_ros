#include <ros.h>
#include <analogWrite.h>
#include <math.h>
#include <geometry_msgs/Twist.h>

#define M1_cw 33  //r_en_1 = m1_cw
#define M2_cw 27
#define M3_cw 17
#define M4_cw 23

#define M1_ccw 32  // l_en_1 = m1_ccw
#define M2_ccw 26
#define M3_ccw 16
#define M4_ccw 18

#define rpwm1 25 
#define rpwm2 12
#define rpwm3 4
#define rpwm4 19

#define lpwm4 15
#define lpwm2 5
#define lpwm3 13
#define lpwm1 14 

//13 12 14 32
//15 18 22 23
#define MAX_PWM 80
#define base_pwm 100
#define base_length 0.5
#define wbc 0.4
#define wheel_radius 0.0635
#define Mega_reset 12

float angular_z = 0;
long int global_time;
long publisher_timer;

ros::NodeHandle  nh;
float rate = 0.05; // seconds
float x, y ,z;
int u1=0u, u2=0, u3=0, u4=0;

/*
0 Touchpad        - t kill
1 PSButton        - p automatic feeding
2 L1              - l finger open
3 R1              - r finger close
4 Down            - d stack down
5 Up              - u stack up
6 Options         - o angle up
7 Share           - s angle down
8 L3              - y pneumatic ring throw
9 Left            - b feeder backward
10 Right          - f feeder forward

//9 R3              - z roller on / off
//10 Left           - x roller up
//11 Right          - w roller down

NULL        - N

*/


void velocityCb(const geometry_msgs::Twist& msg){

    x = msg.linear.x;
   y = msg.linear.y;
   z = msg.angular.z;
   
//      u1 = (- x + y + (base_length * angular_z) / wheel_radius);
//      u2 = (+ x + y + (base_length * angular_z) / wheel_radius);
//      u3 = (+ x - y + (base_length * angular_z) / wheel_radius);
//      u4 = (- x - y + (base_length * angular_z) / wheel_radius);
//      
   u1 = ( - 0.7071 * x + 0.7071 * y + 1.414 * wbc * -angular_z) / wheel_radius;
   u2 = ( + 0.7071 * x + 0.7071 * y + 1.414 * wbc * angular_z) / wheel_radius;
   u3 = ( + 0.7071 * x - 0.7071 * y + 1.414 * wbc * -angular_z) / wheel_radius;
   u4 = ( - 0.7071 * x - 0.7071 * y + 1.414 * wbc * angular_z) / wheel_radius;
  
  int u1_map = map(u1,-44,44,-MAX_PWM,MAX_PWM);
  int u2_map = map(u2,-44,44,-MAX_PWM,MAX_PWM);
  int u3_map = map(u3,-44,44,-MAX_PWM,MAX_PWM);
  int u4_map = map(u4,-44,44,-MAX_PWM,MAX_PWM);
    
  if(u1_map > 0){
    analogWrite(M1_cw, 255);
    analogWrite(M1_ccw, abs(u1_map));
    
    analogWrite(rpwm1,0);
    analogWrite(lpwm1,255);
    
//    Serial.print("nice");
  }
  else if(u1_map < 0){
    analogWrite(M1_cw, abs(u1_map));
    analogWrite(M1_ccw, 255);
    
    analogWrite(rpwm1,255);
    analogWrite(lpwm1,0);
    
//    Serial.print("VEYYYY nice");
  }
  else{
    
    analogWrite(M1_cw,255);
    analogWrite(M1_ccw,255);
    analogWrite(rpwm1,255);
    analogWrite(lpwm1,255);
  }
  
  if(u2_map > 0){
    analogWrite(M2_cw, 255);
    analogWrite(M2_ccw, abs(u2_map));
    
    analogWrite(rpwm2,0);
    analogWrite(lpwm2,255);
    
   }
   else if(u2_map < 0){
    analogWrite(M2_cw, abs(u2_map));
    analogWrite(M2_ccw, 255);
    
    analogWrite(rpwm2,255);
    analogWrite(lpwm2,0);
  }
  else{
    analogWrite(M2_cw,255);
    analogWrite(M2_ccw,255);
    analogWrite(rpwm2,255);
    analogWrite(lpwm2,255);
  }
  if(u3_map > 0){
    analogWrite(M3_cw, abs(u3_map));
    analogWrite(M3_ccw, 255);
    
    analogWrite(rpwm3,255);
    analogWrite(lpwm3,0);
    
  }else if(u3_map < 0){
    analogWrite(M3_cw, 255);
    analogWrite(M3_ccw, abs(u3_map));

   analogWrite(rpwm3,0);
   analogWrite(lpwm3,255);
    
  }
  else{
    analogWrite(M3_cw, 255);
    analogWrite(M3_ccw, 255);
  
    analogWrite(rpwm3,255);
    analogWrite(lpwm3,255);
  }
  
  if(u4_map > 0){
    analogWrite(M4_cw, abs(u4_map));
    analogWrite(M4_ccw, 255);
    
    analogWrite(rpwm4,255);
    analogWrite(lpwm4,0);
    
  }else if(u4_map < 0){
    analogWrite(M4_cw, 255);
    analogWrite(M4_ccw, abs(u4_map));
    
    analogWrite(rpwm4,0);
    analogWrite(lpwm4,255);
  }
  else{
    analogWrite(M4_cw, 255);
    analogWrite(M4_ccw, 255);
   
    analogWrite(rpwm4,255);
    analogWrite(lpwm4,255);
  }
  u1=0;
  u2=0;
  u3=0;
  u4=0;
}

ros::Subscriber<geometry_msgs::Twist> sub("cmd_vel", velocityCb);


void setup() {
  
  nh.initNode();
  nh.subscribe(sub);

  pinMode(M1_ccw, OUTPUT);
  pinMode(M2_ccw, OUTPUT);
  pinMode(M3_ccw, OUTPUT);
  pinMode(M4_ccw, OUTPUT);

  pinMode(M1_cw, OUTPUT);
  pinMode(M2_cw, OUTPUT);
  pinMode(M3_cw, OUTPUT);
  pinMode(M4_cw, OUTPUT);
  
  pinMode(rpwm1, OUTPUT); 
  pinMode(rpwm2, OUTPUT); 
  pinMode(rpwm3, OUTPUT); 
  pinMode(rpwm4, OUTPUT);

  pinMode(lpwm1, OUTPUT); 
  pinMode(lpwm2, OUTPUT); 
  pinMode(lpwm3, OUTPUT); 
  pinMode(lpwm4, OUTPUT);

  analogWriteFrequency(10000);
  
}

void loop() {
  global_time = millis();

  publisher_timer = millis() + 1000;  
  nh.spinOnce();
  delay(rate);
}
