#include "PlayerController.h"

#include "Config.h"
#include "Graphic.h"
#include "Input.h"
#include "Player.h"
#include "Window.h"

PlayerController::PlayerController(Player* target): target{target} {
  im::Module&                           inputs = im::Module::getInstance();
  inputs.addMouseListener([this](double x, double y) { this->target->setRotation({-y / sensibility, x / sensibility, 0.f}); });
  inputs.mapKey(Config::leftKey, GLFW_PRESS, 0, [this]() { this->addDirection(LEFT); });
  inputs.mapKey(Config::rightKey, GLFW_PRESS, 0, [this]() { this->addDirection(RIGHT); });
  inputs.mapKey(Config::frontKey, GLFW_PRESS, 0, [this]() { this->addDirection(FRONT); });
  inputs.mapKey(Config::backKey, GLFW_PRESS, 0, [this]() { this->addDirection(BACK); });
  inputs.mapKey(Config::upKey, GLFW_PRESS, 0, [this]() { this->addDirection(UP); });
  inputs.mapKey(Config::downKey, GLFW_PRESS, 0, [this]() { this->addDirection(DOWN); });

  inputs.mapKey(Config::leftKey, GLFW_RELEASE, 0, [this]() { this->removeDirection(LEFT); });
  inputs.mapKey(Config::rightKey, GLFW_RELEASE, 0, [this]() { this->removeDirection(RIGHT); });
  inputs.mapKey(Config::frontKey, GLFW_RELEASE, 0, [this]() { this->removeDirection(FRONT); });
  inputs.mapKey(Config::backKey, GLFW_RELEASE, 0, [this]() { this->removeDirection(BACK); });
  inputs.mapKey(Config::upKey, GLFW_RELEASE, 0, [this]() { this->removeDirection(UP); });
  inputs.mapKey(Config::downKey, GLFW_RELEASE, 0, [this]() { this->removeDirection(DOWN); });

  inputs.mapKey(Config::cursorToggleKey, GLFW_PRESS, GLFW_MOD_CONTROL, []() { gm::Module::getInstance().getWindow().toggleCursor(); });
}

void PlayerController::addDirection(DIRECTION_TITLE dir) {
  auto axis = dir / DIRECTION_PER_AXIS;
  auto vec  = DIRECTIONS[dir];
  directionQueues[axis].emplace_back(std::move(vec));
  queueUpdate();
}

void PlayerController::removeDirection(DIRECTION_TITLE dir) {
  auto axis = dir / DIRECTION_PER_AXIS;
  auto vec  = DIRECTIONS[dir];

  directionQueues[axis].erase(std::remove(directionQueues[axis].begin(), directionQueues[axis].end(), vec), directionQueues[axis].end());

  if(directionQueues[0].size() + directionQueues[1].size() + directionQueues[2].size() == 0) unqueuUpdate();
}

void PlayerController::update(float delta) {
  glm::vec3 step = ((directionQueues[0].empty()) ? glm::vec3{0.f} : directionQueues[0].back()) + ((directionQueues[1].empty()) ? glm::vec3{0.f} : directionQueues[1].back()) + ((directionQueues[2].empty()) ? glm::vec3{0.f} : directionQueues[2].back());
  target->move(glm::normalize(step) * delta);
}
