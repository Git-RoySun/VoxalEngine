#include "World.h"
#include "Updatable.h"

// temporary helper function, creates a 1x1x1 cube centered at offset with an index buffer
std::unique_ptr<vc::Model> createCubeModel(vc::Device& device) {
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
  modelBuilder.indices = { 0,  1,  2,  0,  3,  1,  4,  5,  6,  4,  7,  5,  8,  9,  10, 8,  11, 9, 12, 13, 14, 12, 15, 13, 16, 17, 18, 16, 19, 17, 20, 21, 22, 20, 23, 21 };

  return std::make_unique<vc::Model>(device, modelBuilder);
}

void World::loadWorld() {//load objects
	/*std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> dis(-10.0f, 10.0f);
  */
  const float VOXELS = 5.f;
  const float SIZE = 0.5f / VOXELS;
  for (int x = 0; x < VOXELS; x++) {
    for (int y = 0; y < VOXELS; y++) {
      for (int z = 0; z < VOXELS; z++) {
        Transform transform = {
          .position = {(SIZE + 0.01f) * x , 0.2f+(SIZE + 0.01f) * -(y) ,(SIZE + 0.01f) * z},
          .scale = glm::vec3{SIZE}
        };

        Object cube = Object(createCubeModel(vc.device), transform);
        objects.push_back(std::move(cube));
      }
    }
  }

  Transform transform = {
  	.position = {-1.f,0,0},
    .scale = glm::vec3{0.5f}
  };
  Object cube = Object(createCubeModel(vc.device), transform);
  objects.push_back(std::move(cube));
}

void World::setup(){
  descriptorPool = vc::DescriptorPool::Builder(vc.device)
    .setMaxSets(vc::SwapChain::MAX_FRAMES_IN_FLIGHT)
    .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, vc::SwapChain::MAX_FRAMES_IN_FLIGHT)
    .build();
  loadWorld();
  configureControl();
}

void World::configureControl(){
  auto aspect = vc.renderer.getAspectRatio();
  CameraObject* camera = &cameras.emplace_back();
  camera->setPosition({ 0.f,-0.5f,0.f });
  camera->getCamera().setPerspectiveProjection(glm::radians(50.f), aspect, .1f, 15.f);
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

  auto window = vc.window.getGlWindow();
  glfwSetCursorPos(window, 0, 0);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetKeyCallback(window, ic::InputModule::sendKeyEvent);
  glfwSetCursorPosCallback(window, ic::InputModule::sendMouseEvent);
}


void World::run() {
  vc::Buffer ubo {
    vc.device,
    sizeof(vc::UniformBuffer),
    vc::SwapChain::MAX_FRAMES_IN_FLIGHT,
    VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
  	vc.device.properties.limits.minUniformBufferOffsetAlignment
  };
  ubo.map();

  auto setLayout = vc::DescriptorSetLayout::Builder(vc.device)
	.addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
	.build();

  std::vector<VkDescriptorSet> descriptorSets(vc::SwapChain::MAX_FRAMES_IN_FLIGHT);
  for (int i = 0; i < descriptorSets.size();++i){
    auto bufferInfo = ubo.descriptorInfo();
    vc::DescriptorWriter(*setLayout, *descriptorPool)
      .writeBuffer(0, &bufferInfo)
      .build(descriptorSets[i]);
  }

  vc.renderSystem.init(setLayout->getDescriptorSetLayout(), vc.renderer.getRenderPass());

  while (!vc.window.shouldClose()) {
    glfwPollEvents();

    if (auto commandBuffer = vc.renderer.startFrame()) {
      auto now = std::chrono::steady_clock::now();
      std::chrono::duration<float> delta = now - last;
      last = now;
      Updatable::updateAll(delta.count());

      int frameIndex = vc.renderer.getFrameIndex();
      vc::UniformBuffer data;
      data.projectionView = cameras[0].getCamera().getProjection() * cameras[0].getCamera().getView();
      ubo.writeToIndex(&data, frameIndex);
      ubo.flushIndex(frameIndex);
      
      //rendering stage
      vc::FrameInfo frameInfo{
        .frameIndex = frameIndex,
          .frameTime = delta.count(),
          .commandBuffer = commandBuffer,
          .camera = cameras[0].getCamera(),
          .descriptorSet = descriptorSets[frameIndex]
      };

      vc.renderer.startRenderPass(commandBuffer);
      vc.renderSystem.renderObjects(frameInfo, objects);
      vc.renderer.endRenderPass(commandBuffer);
      vc.renderer.endFrame();
    }
  }
  vkDeviceWaitIdle(vc.device.getVkDevice());
};