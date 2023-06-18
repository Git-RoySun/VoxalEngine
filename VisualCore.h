#pragma once
#include "Window.h"
#include "Pipeline.h"
#include "SwapChain.h"
#include <memory>
#include <vector>

namespace vc {
	class VisualCore {
		Window window{WIDTH,HEIGHT, "Window" };
		Device device{ window };
		SwapChain swapChain{ device, window.getExtent() };
		std::vector<std::unique_ptr<Pipeline>> Pipelines;
		std::vector<VkCommandBuffer> commandBuffers;
		VkPipelineLayout pipelineLayout;

		void initPipelineLayout();
		void initPipeline();
		void initCommandBuffer();
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

