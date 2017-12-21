#ifndef MOVABLE_H
#define MOVABLE_H

#include "vector_2d.h"

using namespace rcsc;

class movable {
 public:
  movable();

  Vector2D getPos() const;
  void setPos(const Vector2D &value);

  Vector2D getVel() const;
  void setVel(const Vector2D &value);

 private:
  Vector2D pos;
  Vector2D vel;
};

#endif // MOVABLE_H
