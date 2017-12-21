#include "movable.h"

movable::movable() {

}

Vector2D movable::getPos() const {
  return pos;
}

void movable::setPos(const Vector2D &value) {
  pos = value;
}

Vector2D movable::getVel() const {
  return vel;
}

void movable::setVel(const Vector2D &value) {
  vel = value;
}
