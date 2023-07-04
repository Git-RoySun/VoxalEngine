#include "World.h"

#include <iostream>

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
    .translation = { 0.f,0.f,7.f }
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

  ic.setKeyInput(GLFW_KEY_A, ic::KeyInput{
    [this]() {cameras[0].moveLeft.activate(); },
    []() {},
    [this]() {cameras[0].moveLeft.deactivate(); },
    });
  ic.setKeyInput(GLFW_KEY_D, ic::KeyInput{
  [this]() {cameras[0].moveRight.activate(); },
  []() {},
  [this]() {cameras[0].moveRight.deactivate(); },
    });
  ic.setKeyInput(GLFW_KEY_W, ic::KeyInput{
  [this]() {cameras[0].moveFront.activate(); },
  []() {},
  [this]() {cameras[0].moveFront.deactivate(); },
    });
  ic.setKeyInput(GLFW_KEY_S, ic::KeyInput{
  [this]() {cameras[0].moveBack.activate(); },
  []() {},
  [this]() {cameras[0].moveBack.deactivate(); },
    });
  glfwSetKeyCallback(vc.window.getGlWindow(), ic::InputModule::callMappedKey);

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