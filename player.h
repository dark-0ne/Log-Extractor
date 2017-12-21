#ifndef PLAYER_H
#define PLAYER_H

#include "vector_2d.h"
#include "movable.h"

class player : public movable {
  public:
    player();

    double getStamina() const;
    void setStamina(double value);

    double getBody_angle() const;
    void setBody_angle(double value);

    double getHead_angle() const;
    void setHead_angle(double value);

    int getUnum() const;
    void setUnum(int value);

  private:
    double stamina;
    double body_angle;
    double head_angle;
    int unum;
};

#endif // PLAYER_H
