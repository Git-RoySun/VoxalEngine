#include "VisualCore.h"
#include "RenderSystem.h"
#include <stdexcept>
#include <array>
#include <vector>
#include <iostream>
namespace vc {
	VisualCore::VisualCore() {
		loadObjects();
	};

	VisualCore::~VisualCore() {};

	void VisualCore::start() {
		RenderSystem renderSystem {device, renderer.getRenderPass() };
		while (!window.shouldClose()) {
			glfwPollEvents();

			if (auto commandBuffer = renderer.startFrame()) {
				renderer.startRenderPass(commandBuffer);
				renderSystem.renderObjects(commandBuffer, objects);
				renderer.endRenderPass(commandBuffer);
				renderer.endFrame();
			}
		}
		vkDeviceWaitIdle(device.getVkDevice());
	};

  std::unique_ptr<Model> createCubeModel(Device& device, glm::vec3 offset) {
    std::vector<Vertex> vertices{
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
    return std::make_unique<Model>(device, vertices);
  }


	void VisualCore::loadObjects() {
  Object cube = createCubeModel(device, { 0.0f,0.0f,0.0f });
    cube.transform.scale = { 0.5f,0.5f,0.5f };
    cube.transform.translation = { 0.0f,0.0f,0.5f };
		objects.push_back(std::move(cube));
	}

}
