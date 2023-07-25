#include "World.h"

#include <iostream>

#include "Updatable.h"

// temporary helper function, creates a 1x1x1 cube centered at offset with an index buffer
std::shared_ptr<vc::Model> createCubeModel(vc::VisualContext& vc) {
  std::vector<vc::Vertex>vertices = {
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
  std::vector<uint32_t> indices = { 0,  1,  2,  0,  3,  1,  4,  5,  6,  4,  7,  5,  8,  9,  10, 8,  11, 9, 12, 13, 14, 12, 15, 13, 16, 17, 18, 16, 19, 17, 20, 21, 22, 20, 23, 21 };
  auto res = std::make_shared<vc::Model>(vertices, indices);
	vc.loadModel(*res);
  return res;
}

void World::loadObject(Object& obj){
  auto o =&objects.emplace_back(std::move(obj));
}

void World::loadWorld() {//load objects
  auto model = createCubeModel(vc);
	const float VOXELS = 64.f;
  const float SIZE = 0.5f / VOXELS;
  for (int x = 0; x < VOXELS; x++) {
    for (int y = 0; y < VOXELS; y++) {
      for (int z = 0; z < VOXELS; z++) {
        Transform transform = {
          .position = {(SIZE + 0.001f) * x , 0.2f+(SIZE + 0.001f) * -(y) ,(SIZE + 0.001f) * z},
          .scale = glm::vec3{SIZE}
        };
        Object cube = Object(model, transform);
        loadObject(cube);
      }
    }
  }
  Transform transform = {
		.position = {-1.f,0,0},
		.scale = glm::vec3{0.5f}
  };
  Object cube = Object(model, transform);
  loadObject(cube);
}

void World::setup(){
  loadWorld();
  configureControl();
}

void World::configureControl(){
  CameraObject* camera = &cameras.emplace_back();
  camera->setPosition({ 0.f,-0.5f,0.f });
  vc.setCamera(camera->getCamera());
  camController = ic::FPMovementController(camera);

  ic::InputModule::addMouseListener(&camController);
  ic::InputModule::addKeyListener(GLFW_KEY_W, &camController);
  ic::InputModule::addKeyListener(GLFW_KEY_A, &camController);
  ic::InputModule::addKeyListener(GLFW_KEY_S, &camController);
  ic::InputModule::addKeyListener(GLFW_KEY_D, &camController);

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
    vc.renderFrame(objects);
  }
};