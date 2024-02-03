// This File is not meant to be a part of the engine, but is used for testing and can serve as an example usage
#include "Container.h"
#include "PlayerController.h"
#include "World.h"
#include "Window.h"
#include "Graphic.h"
#include "Player.h"
#include "Rasterizer.h"
#include "VectorText.h"
#include "GUI.h"
#include "VectorInput.h"

class Client {
  World            world{}; //TODO could be fetched from server for multiplayer or from local file system or generated on the spot
  Player           player{{}, &world};
  PlayerController playerController{&player};

  gm::Gui        gui{};
  gm::Rasterizer rasterizer{};

  void render() {
    auto& window = gm::Module::getInstance().getWindow();
    if(const auto commandBuffer = window.startFrame()) {
      window.startRenderPass(commandBuffer);
      rasterizer.render({commandBuffer, &player, 0, 0});
      gui.render(commandBuffer);
      window.endRenderPass(commandBuffer);
      window.endFrame();
    }
  }

public:
  Client() {
    gui.addWidget(new gm::Container("Debug Window", {
      new gm::VectorText(&player.getPosition()),
      new gm::VectorText(&player.getRotation()),
      new gm::VectorInput("Light", &rasterizer.light.x)
    }));
  }

  void run() {
    while(!glfwWindowShouldClose(gm::Module::getInstance().getWindow().getGlWindow())) {
      glfwPollEvents();
      world.update();
      render();
    }
  }
};

int main() {
  Client app{};
  app.run();
  gm::Rasterizer::cleanup();
}
