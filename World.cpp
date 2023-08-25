#include "World.h"
#include "Updatable.h"

void World::loadWorld() {//load objects
  loader.loadAround(0, 0);
}

void World::setup(){
  srand(seed);
  loadWorld();
  configureControl();
}

void World::configureControl(){
  CameraObject* camera = &cameras.emplace_back();
  camera->setPosition({ 0.f,-0.5f,0.f });
  vc.setCamera(camera->getCamera());
  camController = ic::FPMovementController(camera,camera);

  ic::InputModule::addMouseListener(&camController);
  ic::InputModule::addKeyListener(GLFW_KEY_W, &camController);
  ic::InputModule::addKeyListener(GLFW_KEY_A, &camController);
  ic::InputModule::addKeyListener(GLFW_KEY_S, &camController);
  ic::InputModule::addKeyListener(GLFW_KEY_D, &camController);

  ic::InputModule::addKeyListener(GLFW_KEY_F, &camController);
  ic::InputModule::addKeyListener(GLFW_KEY_SPACE, &camController);
  ic::InputModule::setDirection(GLFW_KEY_SPACE, UP);
  ic::InputModule::setDirection(GLFW_KEY_F, DOWN);

  ic::InputModule::setDirection(GLFW_KEY_W, FRONT);
  ic::InputModule::setDirection(GLFW_KEY_A, LEFT);
  ic::InputModule::setDirection(GLFW_KEY_S, BACK);
  ic::InputModule::setDirection(GLFW_KEY_D, RIGHT);

  //this could be done in the visual context class instead
  auto window = vc.getWindow().getGlWindow();
  glfwSetCursorPos(window, 0, 0);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetKeyCallback(window, ic::InputModule::sendKeyEvent);
  glfwSetCursorPosCallback(window, ic::InputModule::sendMouseEvent);
}


void World::run() {
  while (alive) {
    glfwPollEvents();
    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<float> delta = now - last;
    last = now;
    
    Updatable::updateAll(delta.count());
    loader.loadAround(cameras[0].getPosition().x, cameras[0].getPosition().z);
    vc.renderFrame();
  }
};