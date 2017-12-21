#include "player.h"

player::player() {

}

double player::getStamina() const {
    return stamina;
}

void player::setStamina(double value) {
    stamina = value;
}

double player::getBody_angle() const {
    return body_angle;
}

void player::setBody_angle(double value) {
    body_angle = value;
}

double player::getHead_angle() const {
    return head_angle;
}

void player::setHead_angle(double value) {
    head_angle = value;
}

int player::getUnum() const {
    return unum;
}

void player::setUnum(int value) {
    unum = value;
}
