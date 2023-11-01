#include "World.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "Material.h"
#include "UIModule.h"
#include "Updatable.h"

void World::loadWorld() {//load objects
  //loader.loadAround(0, 0);
  vc.addInstance({ .position = {0.f,0.f,8.f}, .materialID = vc::Material::GREEN.getId() });
  //vc.addInstance({ .position = {2.f,0.f,0.f}, .materialID = vc::Material::GREEN.getId() });
  //vc.addInstance({ .position = {5.f,0.f,0.f} });
}

void World::setup(){
  srand(seed);
  loadWorld();
  configureControl();
  UIModule::add([this]() {
    auto pos = this->cameras[0].getPosition();
    ImGui::Text("%f %f %f",pos.x,pos.y,pos.z);
    
  });
}

void World::configureControl(){
  obj::Camera* camera = &cameras.emplace_back();
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
  ic::InputModule::addKeyListener(GLFW_KEY_LEFT_CONTROL, &cursorController);
  ic::InputModule::setDirection(GLFW_KEY_SPACE, UP);
  ic::InputModule::setDirection(GLFW_KEY_F, DOWN);

  ic::InputModule::setDirection(GLFW_KEY_W, FRONT);
  ic::InputModule::setDirection(GLFW_KEY_A, LEFT);
  ic::InputModule::setDirection(GLFW_KEY_S, BACK);
  ic::InputModule::setDirection(GLFW_KEY_D, RIGHT);

  auto window = vc.getWindow().getGlWindow();

  glfwSetKeyCallback(window, ic::InputModule::sendKeyEvent);
  glfwSetCursorPosCallback(window, ic::InputModule::sendMouseEvent);
  ImGui_ImplGlfw_InstallCallbacks(window);

}


void World::run() {
  while (!vc.getWindow().shouldClose()) {
    glfwPollEvents();
    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<float> delta = now - last;
    last = now;
    
    Updatable::updateAll(delta.count());
    //loader.loadAround(cameras[0].getPosition().x, cameras[0].getPosition().z);
    vc.renderFrame();
  }
};