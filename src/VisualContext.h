#pragma once
#include <chrono>

#include "Descriptor.h"
#include "OutlineRenderer.h"
#include "Renderer.h"
#include "VoxelRenderer.h"

namespace vc {
	struct UniformBuffer {
		glm::mat4 projectionView{1.f};
		glm::vec3 lightDirection = glm::normalize(glm::vec3{-1.f, 3.f, -1.f});
	};

	class VisualContext {
		static constexpr int WIDTH = 854;
		static constexpr int HEIGHT = 480;
		static constexpr uint32_t INSTANCEMAX = 1000000;

		Window window{ WIDTH,HEIGHT, "Window" };
		Device device{ window };
		Renderer renderer{ window,device };
		VoxelRenderer voxelStage{ device };
		OutlineRenderer outlineStage{ device };
		//outlineRenderer

		//data section (should probably be a separate class)
		Buffer instanceBuffer;
		int instanceCount = 0;

		Buffer ubo;

		std::unique_ptr<DescriptorPool> descriptorPool{};
		std::unique_ptr<DescriptorSetLayout> setLayout{};
		std::vector<VkDescriptorSet> descriptorSets{SwapChain::MAX_FRAMES_IN_FLIGHT};
		
		Camera* camera = nullptr;
		std::chrono::steady_clock::time_point last;
	public:
		VisualContext();
		~VisualContext();

		Window& getWindow() { return window; }
		void setCamera(Camera* cam);
		void renderFrame();

		bool addInstance(Voxel::Instance instance);
		void clearInstances() { instanceCount = 0; }
	};
}

