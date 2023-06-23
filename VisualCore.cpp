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


	void VisualCore::loadObjects() {
		std::vector<Vertex> vertices {
			{ { 0.0f, -0.5f,0.0f}, Vertex::RED },
			{ { 0.5f, 0.5f,0.0f} , Vertex::BLUE },
			{ { -0.5f, 0.5f,0.0f}, Vertex::GREEN },
		};

		Object triangle = Object(std::make_unique<Model>(device, vertices));
		triangle.transform.translation.x = -0.5f;
		triangle.transform.scale = glm::vec3{ 1.0f,0.5f ,1.0f};
		objects.push_back(std::move(triangle));
	}

}
