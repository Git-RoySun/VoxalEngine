#include "World.h"

#include <iostream>

#include "Updatable.h"

std::unique_ptr<vc::Model> createCubeModel(vc::Device& device, glm::vec3 offset) {
  std::vector<vc::Vertex> vertices{
    // left face (white)
    {{-.5f, -.5f, -.5f}, { .9f, .9f, .9f }},
    { {-.5f, .5f, .5f}, {.9f, .9f, .9f} },
    { {-.5f, -.5f, .5f}, {.9f, .9f, .9f} },
    { {-.5f, -.5f, -.5f}, {.9f, .9f, .9f} },
    { {-.5f, .5f, -.5f}, {.9f, .9f, .9f} },
    { {-.5f, .5f, .5f}, {.9f, .9f, .9f} },

      // right face (yellow)
    { {.5f, -.5f, -.5f}, {.8f, .8f, .1f} },
    { {.5f, .5f, .5f}, {.8f, .8f, .1f} },
    { {.5f, -.5f, .5f}, {.8f, .8f, .1f} },
    { {.5f, -.5f, -.5f}, {.8f, .8f, .1f} },
    { {.5f, .5f, -.5f}, {.8f, .8f, .1f} },
    { {.5f, .5f, .5f}, {.8f, .8f, .1f} },

      // top face (orange, remember y axis points down)
    { {-.5f, -.5f, -.5f}, {.9f, .6f, .1f} },
    { {.5f, -.5f, .5f}, {.9f, .6f, .1f} },
    { {-.5f, -.5f, .5f}, {.9f, .6f, .1f} },
    { {-.5f, -.5f, -.5f}, {.9f, .6f, .1f} },
    { {.5f, -.5f, -.5f}, {.9f, .6f, .1f} },
    { {.5f, -.5f, .5f}, {.9f, .6f, .1f} },

      // bottom face (red)
    { {-.5f, .5f, -.5f}, {.8f, .1f, .1f} },
    { {.5f, .5f, .5f}, {.8f, .1f, .1f} },
    { {-.5f, .5f, .5f}, {.8f, .1f, .1f} },
    { {-.5f, .5f, -.5f}, {.8f, .1f, .1f} },
    { {.5f, .5f, -.5f}, {.8f, .1f, .1f} },
    { {.5f, .5f, .5f}, {.8f, .1f, .1f} },

      // nose face (blue)
    { {-.5f, -.5f, 0.5f}, {.1f, .1f, .8f} },
    { {.5f, .5f, 0.5f}, {.1f, .1f, .8f} },
    { {-.5f, .5f, 0.5f}, {.1f, .1f, .8f} },
    { {-.5f, -.5f, 0.5f}, {.1f, .1f, .8f} },
    { {.5f, -.5f, 0.5f}, {.1f, .1f, .8f} },
    { {.5f, .5f, 0.5f}, {.1f, .1f, .8f} },

      // tail face (green)
    { {-.5f, -.5f, -0.5f}, {.1f, .8f, .1f} },
    { {.5f, .5f, -0.5f}, {.1f, .8f, .1f} },
    { {-.5f, .5f, -0.5f}, {.1f, .8f, .1f} },
    { {-.5f, -.5f, -0.5f}, {.1f, .8f, .1f} },
    { {.5f, -.5f, -0.5f}, {.1f, .8f, .1f} },
    { {.5f, .5f, -0.5f}, {.1f, .8f, .1f} },

  };
  for (auto& v : vertices) {
    v.position += offset;
  }
  return std::make_unique<vc::Model>(device, vertices);
}

void World::loadWorld() {//load objects
  Transform transform{
    .position = { 0.f,0.f,5.f }
  };
  Object cube = Object(createCubeModel(vc.device, { 0.0f,0.0f,0.f }), transform);
  objects.push_back(std::move(cube));
}

void World::init(){
  setup();
  start();
}

void World::setup(){
  loadWorld();
  //temporary camera setup
  float aspect = vc.renderer.getAspectRatio();
  cameras.emplace_back();
  cameras[0].getCamera().setPerspectiveProjection(glm::radians(50.f), aspect, .1f, 15.f);
  camController = ic::FPMovementController(&cameras[0]);
  ic::InputModule::addMouseListener(&camController);
  ic::InputModule::addKeyListener(GLFW_KEY_W, &camController);
  ic::InputModule::addKeyListener(GLFW_KEY_A, &camController);
  ic::InputModule::addKeyListener(GLFW_KEY_S, &camController);
  ic::InputModule::addKeyListener(GLFW_KEY_D, &camController);

  ic::InputModule::setDirection(GLFW_KEY_W, FRONT);
  ic::InputModule::setDirection(GLFW_KEY_A, LEFT);
  ic::InputModule::setDirection(GLFW_KEY_S, BACK);
  ic::InputModule::setDirection(GLFW_KEY_D, RIGHT);

  glfwSetCursorPos(vc.window.getGlWindow(), 0, 0);
  glfwSetInputMode(vc.window.getGlWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetKeyCallback(vc.window.getGlWindow(), ic::InputModule::sendKeyEvent);
  glfwSetCursorPosCallback(vc.window.getGlWindow(), ic::InputModule::sendMouseEvent);

}

void World::start() {
  while (!vc.window.shouldClose()) {
    glfwPollEvents();

    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<float> delta = now - last;
    last = now;

    Updatable::updateAll(delta.count());
    if (auto commandBuffer = vc.renderer.startFrame()) {
      vc.renderer.startRenderPass(commandBuffer);
      vc.renderSystem.renderObjects(commandBuffer, objects, cameras[0].getCamera());
      vc.renderer.endRenderPass(commandBuffer);
      vc.renderer.endFrame();
    }
  }
  vkDeviceWaitIdle(vc.device.getVkDevice());
};