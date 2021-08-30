#include "player.h"

Player::Player() : Ship() {
    keyRight = false;
    keyUp = false;
    keyLeft = false;
    keyDown = false;
}

void Player::move() {
    const float rotAmount = 0.002f;
    const float moveAmount = 0.0005f;

    if (keyUp) {
        vel += moveAmount * mat2(Rotation) * vec2(1.0f, 0.0f);
    }
    if (keyDown) {
        vel -= moveAmount * mat2(Rotation) * vec2(1.0f, 0.0f);
    }
    if (keyLeft) spin += rotAmount;
    if (keyRight) spin -= rotAmount;
    spin *= 0.99f;
    Ship::move();
}