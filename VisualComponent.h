#pragma once
#include <vector>

#include "Window.h"
#include "Renderer.h"
#include "Object.h"
#include "Camera.h"
#include "CameraObject.h"


namespace vc {
	class VisualComponent {
		Window window{WIDTH,HEIGHT, "Window" };
		Device device{ window };
		Renderer renderer{ window,device };
		CameraObject& camera;
		std::vector<Object> objects;

		void loadObjects();
		void initPipelineLayout();
		void initPipeline();

	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;
		void start();

		VisualComponent(CameraObject& camera);
		~VisualComponent();

		VisualComponent(const VisualComponent&) = delete;
		VisualComponent& operator=(const VisualComponent&) = delete;

		void setCamera(CameraObject& camera) { camera = camera; };
		Window& getWindow() { return window; };
	};
}

