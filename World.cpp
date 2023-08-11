#include "World.h"
#include "Updatable.h"

void World::loadVoxel(Voxel::Instance instance){
  auto o = &objects.emplace_back(std::move(Voxel(instance)));
  vc.addInstance(instance);
}

void World::loadWorld() {//load objects
	const float VOXELS = 64.f;
  const float SIZE = 0.5f / VOXELS;
  for (int x = 0; x < VOXELS; x++) {
    for (int y = 0; y < VOXELS; y++) {
      for (int z = 0; z < VOXELS; z++) {
        loadVoxel(Voxel::Instance{
          .position = { (SIZE + 0.01f) * x , 0.2f + (SIZE + 0.01f) * -(y) ,(SIZE + 0.01f) * z },
        	.scale = glm::vec3{ SIZE }
        });
      }
    }
  }

    loadVoxel(Voxel::Instance{
      .position = {  -2,0, 0},
      .scale = glm::vec3{0.5f},
    	.colour = { 0.f,0.f,0.f }});
}

void World::setup(){
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
    vc.renderFrame();
  }
};