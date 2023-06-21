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
#include "Object.h"


namespace vc {

	struct PushConstantData {
		glm::mat2 transform{1.0f};
		alignas(16) glm::vec2 offset;
	};

	class VisualCore {
		Window window{WIDTH,HEIGHT, "Window" };
		Device device{ window };
		VkPipelineLayout pipelineLayout;

		std::unique_ptr<SwapChain> swapChain;
		std::unique_ptr<Pipeline> pipeline;
		std::vector<Object> objects;
		std::vector<VkCommandBuffer> commandBuffers;

		void loadObjects();
		void renderObjects(VkCommandBuffer commandBuffer);
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

