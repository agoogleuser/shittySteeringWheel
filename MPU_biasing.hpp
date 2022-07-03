#ifndef _MPU_BIASING
#define _MPU_BIASING

struct vec2d
{
  float x,y;
  vec2d()                    {x=0;y=0;}
  vec2d(float _x, float _y)  {x = _x; y=_y;}

  vec2d operator+(const vec2d &obj);
  vec2d operator+(const float &obj);
  vec2d operator*(const vec2d &obj);
  vec2d operator*(const float &obj);
  void print();
};

class Position
{
  //Object mis-named that calculates veclocity and drift
//This object should be a singleton, but I'm too lazy to do it.
private:
  vec2d p1,p2,v1,v2,a1;
  float dt, prevTime, angZ_prev;
public:
  float x,y, angZ;
  float vx,vy;
    Position()
  {//Constructor that resets the time
    prevTime =0;
    angZ_prev=0;
    x   =0;y =0;
    vx  =0;vy=0;
    angZ=0;
    angZ_prev=0;
  }
  void update(float readTime, float accX, float accY, float gyroZ);
  void clearP();
  void clearV();
  void clearAng();
  void clearAll();

};


#endif