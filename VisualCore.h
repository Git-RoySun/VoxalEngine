#pragma once
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "Window.h"
#include "Pipeline.h"
#include "SwapChain.h"
#include "Model.h"


namespace vc {

	struct PushConstantData {
		glm::vec3 offset;
		alignas(16) glm::vec3 color;
	};

	class VisualCore {
		Window window{WIDTH,HEIGHT, "Window" };
		Device device{ window };
		VkPipelineLayout pipelineLayout;

		std::unique_ptr<SwapChain> swapChain;
		std::unique_ptr<Pipeline> pipeline;
		std::unique_ptr<Model> model;
		std::vector<VkCommandBuffer> commandBuffers;

		void loadModels();
		void initPipelineLayout();
		void initPipeline();
		void initSwapChain();

		void recordCommandBuffer(int imageIndex);
		void initCommandBuffer();
		void freeCommandBuffers();
		void drawFrame();
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;
		void start();

		VisualCore();
		~VisualCore();

		VisualCore(const VisualCore&) = delete;
		VisualCore& operator=(const VisualCore&) = delete;
	};
}

