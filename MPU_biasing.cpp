// Basic demo for accelerometer readings from Adafruit MPU6050s
#include <Arduino.h>
#include "MPU_biasing.hpp"

//================Operator Overloading================//
vec2d vec2d::operator+(const vec2d &obj)
  {return vec2d(this->x + obj.x,this->y + obj.y);}

vec2d vec2d::operator+(const float &obj)
  {return vec2d(this->x + obj, this->y + obj);}

// Operator * for multiplication
vec2d vec2d::operator*(const vec2d &obj)
  {return vec2d(this->x * obj.x, this->y * obj.y);}

vec2d vec2d::operator*(const float &obj)
  {return vec2d(this->x * obj, this->y * obj);}

void vec2d::print()
{//For Debugging Purposes.prints [x,y]
  Serial.print('[');
  Serial.print(this->x);
  Serial.print(',');
  Serial.print(this->y);
  Serial.print(']');
}

static vec2d zer0;



void Position::update(float readTime, float accX, float accY, float gyroZ)
  {//Using Velocity Verlet Algorithm, found on Wikipedia
  // Basically It's the same Newton equations used in Mechanics-2 and3
 //  Courses to calculate distance, as it's more accurate for 3D simulations.
//   Where acceleration is calculated from forces affecting the object.
    vec2d a2(accX,accY);
    dt = (readTime-prevTime)/1000;
    v2 = v1 + (a1+a2)*dt*0.5;
    p2 = p1 + (v1 +a1*dt*0.5)*dt;
    // Assigning old values to the new ones.
    a1=a2;
    v1=v2;
    p1=p2;
    prevTime=readTime;

    //Yaw Calculation (rotation around Z-axis)
    angZ=angZ_prev + (dt*gyroZ)*57.29577951; //output.
    angZ_prev = angZ;

    //Output of the object.
    x=p2.x;
    y=p2.y;
    vx=v2.x;
    vy=v2.y;
  }

  void Position::clearP()   {p1=zer0;p2=zer0;}//Zeros the drift Data.
  void Position::clearV()   {v1=zer0;v2=zer0;}//Zeros the velocity data.
  void Position::clearAng() {angZ=0; angZ_prev=0;}
  void Position::clearAll() {clearP();clearV();clearAng();}

