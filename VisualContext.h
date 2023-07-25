#pragma once
#include <chrono>

#include "Descriptor.h"
#include "MeshGroup.h"
#include "ObjectRenderer.h"
#include "Renderer.h"

class World;

namespace vc {
	struct UniformBuffer {
		glm::mat4 projectionView{1.f};
		glm::vec3 lightDirection = glm::normalize(glm::vec3{-1.f, 3.f, -1.f});
	};

	class VisualContext {
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		Window window{ WIDTH,HEIGHT, "Window" };
		Device device{ window };
		Renderer renderer{ window,device };
		ObjectRenderer renderSystem{ device };

		Buffer ubo;
		std::unique_ptr<DescriptorPool> descriptorPool{};
		std::unique_ptr<DescriptorSetLayout> setLayout{};
		std::vector<VkDescriptorSet> descriptorSets{SwapChain::MAX_FRAMES_IN_FLIGHT};

		MeshGroup meshes{device};
		Camera* camera = nullptr;
		std::chrono::steady_clock::time_point last;
	public:
		VisualContext();
		~VisualContext();
		Window& getWindow() { return window; }
		void setCamera(Camera* cam);
		void loadModel(Model& model);
		void renderFrame(std::vector<Object>& objects);
	};
}

