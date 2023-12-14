// This File is not meant to be a part of the engine, but is used for testing and can serve as an example usage
#include <glm/gtx/string_cast.hpp>

#include "CursorController.h"
#include "PlayerController.h"
#include "World.h"
#include "Window.h"
#include "Graphic.h"
#include "imgui.h"
#include "Player.h"
#include "Input.h"
#include "Rasterizer.h"
#include "UI.h"

class Client{
	gm::Module& graphics = gm::Module::getInstance();
	im::Module& inputs = im::Module::getInstance();

	World world{}; //TODO could be fetched from server for multiplayer or from local file system or generated on the spot
	Player player{ {} , &world };
	im::PlayerController playerController{ &player };
	im::CursorToggleController cursorToggleController{ graphics.getWindow().getGlWindow() };

	gm::Gui gui{};
	gm::Rasterizer rasterizer{};

	void render(){
		auto& window = graphics.getWindow();
		if (const auto commandBuffer = window.startFrame()) {
			window.startRenderPass(commandBuffer);
			rasterizer.render({commandBuffer, &player, 0, 0});
			gui.render(commandBuffer);
			window.endRenderPass(commandBuffer);
			window.endFrame();
		}
	}
public:
	Client(){
		auto debugWindow = new gm::Container{ "Debug Window", {} };
		gui.addWidget(debugWindow);
		debugWindow->addWidget([this]() {ImGui::Text(glm::to_string(player.getPosition()).c_str()); });
		debugWindow->addWidget([this]() {ImGui::Text(glm::to_string(player.getRotation()).c_str()); });
	}

	void run(){
		while (!glfwWindowShouldClose(graphics.getWindow().getGlWindow())){
			glfwPollEvents();
			world.update();
			render();
		}
	}
};

int main(){
	Client app{};
	app.run();
	gm::Rasterizer::cleanup();
}