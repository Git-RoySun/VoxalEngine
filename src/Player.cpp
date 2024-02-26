#include "Player.h"
#include "Graphic.h"
#include "Window.h"
#include "glm/gtx/string_cast.hpp"

Player::Player(obj::Transform transform, World& world) : Base{transform}, Observer{world, transform.position} {
  auto extent = WINDOW.getExtent();
  getCamera().setPerspectiveProjection(glm::radians(50.f), (float)extent.width / extent.height,
    .1f, 35.f);
  setPosition(transform.position);
  setRotation(transform.rotation);
}

void Player::move(glm::vec3 direction) {
  float yaw = transform.rotation.y;

  glm::vec3 front{sin(yaw), 0.f, cos(yaw)};
  glm::vec3 right{front.z, 0.f, -front.x};
  glm::vec3 step = direction.x * right + direction.y * glm::vec3(0, 1, 0) + direction.z * front;

  setPosition(transform.position + step * speed);
};

void Player::setPosition(glm::vec3 pos) {
  Observer::move(pos);
  getCamera().setRotation(transform.position = pos, transform.rotation);
};

void Player::rotate(glm::vec3 rotation) {
  setRotation(transform.rotation + rotation);
};

void Player::setRotation(glm::vec3 rotation) {
  getCamera().setRotation(transform.position, transform.rotation = rotation);
};
