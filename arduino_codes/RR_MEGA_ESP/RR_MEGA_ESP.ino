#include <ros.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Int8.h>

const int ENCA[] = {18, 19, 2, 21};
const int ENCB[] = {32, 30, 36, 38};
volatile int32_t enc[] = {0, 0, 0, 0};
volatile int32_t last_enc[] = {0, 0, 0, 0};
volatile int curr, last_time;

ros::NodeHandle  nh;

std_msgs::Float32 enc1;
std_msgs::Float32 enc2;
std_msgs::Float32 enc3;
std_msgs::Float32 enc4;
std_msgs::Int8 delta_t;

ros::Publisher pub1("enc1", &enc1);
ros::Publisher pub2("enc2", &enc2);
ros::Publisher pub3("enc3", &enc3);
ros::Publisher pub4("enc4", &enc4);
ros::Publisher t_change("delta_t", &delta_t);


void setup()
{
  nh.initNode();
  nh.advertise(pub1);
  nh.advertise(pub2);
  nh.advertise(pub3);
  nh.advertise(pub4);
  nh.advertise(t_change);

  curr = millis();
  last_time = curr;

  for(int i=0; i<4; i++){
    pinMode(ENCA[i],INPUT_PULLUP);
    pinMode(ENCB[i],INPUT_PULLUP);
  }

  attachInterrupt(digitalPinToInterrupt(ENCA[0]),readEncoder<0>,RISING);
  attachInterrupt(digitalPinToInterrupt(ENCA[1]),readEncoder<1>,RISING);
  attachInterrupt(digitalPinToInterrupt(ENCA[2]),readEncoder<2>,RISING);
  attachInterrupt(digitalPinToInterrupt(ENCA[3]),readEncoder<3>,RISING);
}

void loop()
{
  curr = millis();
  int dt = curr - last_time;

  if(dt>100){
    enc1.data = ((enc[0]-last_enc[0])*1000)/(dt*300);
    enc2.data = ((enc[1]-last_enc[1])*1000)/(dt*300);
    enc3.data = ((enc[2]-last_enc[2])*1000)/(dt*300);
    enc4.data = ((enc[3]-last_enc[3])*1000)/(dt*300);
    delta_t.data = dt;
    last_time = curr;
    for(int i=0; i<4; i++){
      last_enc[i] = enc[i];
    }
  }
  
  pub1.publish(&enc1);
  pub2.publish(&enc2);
  pub3.publish(&enc3);
  pub4.publish(&enc4);
  t_change.publish(&delta_t);
  nh.spinOnce();
  delay(10);
}

template <int j>
void readEncoder(){
  int b = digitalRead(ENCB[j]);
  if(b > 0){
    enc[j]++;
  }
  else{
    enc[j]--;
  }
}
