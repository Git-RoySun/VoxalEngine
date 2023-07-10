#include "World.h"

#include <iostream>

#include "Updatable.h"

// temporary helper function, creates a 1x1x1 cube centered at offset with an index buffer
std::unique_ptr<vc::Model> createCubeModel(vc::Device& device, glm::vec3 offset) {
  vc::Model::Builder modelBuilder{};
  modelBuilder.vertices = {
    // left face (white)
    {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
    {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
    {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
    {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},

    // right face (yellow)
    {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
    {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
    {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
    {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},

    // top face (orange, remember y axis points down)
    {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
    {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
    {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
    {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},

    // bottom face (red)
    {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
    {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
    {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
    {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},

    // nose face (blue)
    {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
    {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
    {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
    {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},

    // tail face (green)
    {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
    {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
    {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
    {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
  };
  for (auto& v : modelBuilder.vertices) {
    v.position += offset;
  }

  modelBuilder.indices = { 0,  1,  2,  0,  3,  1,  4,  5,  6,  4,  7,  5,  8,  9,  10, 8,  11, 9, 12, 13, 14, 12, 15, 13, 16, 17, 18, 16, 19, 17, 20, 21, 22, 20, 23, 21 };

  return std::make_unique<vc::Model>(device, modelBuilder);
}

void World::loadWorld() {//load objects
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> dis(-10.0f, 10.0f);

	for (int i = 0;i<50;i++){
    Transform transform = {
      .position = {dis(gen),0.f,dis(gen)},
      .scale = {dis(gen)/10+1,dis(gen)/10+1,dis(gen)/10+1},
      .rotation = {dis(gen) / 8,dis(gen) / 8,dis(gen) / 8}
    };
    Object cube = Object(createCubeModel(vc.device, { 0.0f,0.0f,0.f }), transform);
    objects.push_back(std::move(cube));
	}
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