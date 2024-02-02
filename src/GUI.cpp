#include "GUI.h"
#include "Descriptor.h"
#include "SwapChain.h"
#include "Graphic.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"
#include "Window.h"
#include "Widget.h"

namespace gm {
  Gui::Gui() {
    auto instance = Module::getInstance().getVkInstance();

    ImGui_ImplVulkan_LoadFunctions(
      [](const char* function_name, void* vulkan_instance) {
        return vkGetInstanceProcAddr(*(static_cast<VkInstance*>(vulkan_instance)), function_name);
      }, &instance
    );

    descriptorPool = DescriptorPool::Builder(DEVICE)
      .setMaxSets(3)
      .addPoolSize(VK_DESCRIPTOR_TYPE_SAMPLER, 2)
      .addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 2)
      .addPoolSize(VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 2)
      .addPoolSize(VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 2)
      .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 2)
      .addPoolSize(VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 2)
      .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 2)
      .addPoolSize(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 2)
      .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 2)
      .addPoolSize(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 2)
      .addPoolSize(VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 2)
      .build();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForVulkan(WINDOW.getGlWindow(), false);
    ImGui_ImplVulkan_InitInfo initInfo = {};
    initInfo.Instance                  = instance;
    initInfo.PhysicalDevice            = DEVICE.getPhysicalDevice();
    initInfo.Device                    = DEVICE.getVkDevice();
    initInfo.QueueFamily               = DEVICE.getQueueFamilies().graphicsFamily;
    initInfo.Queue                     = DEVICE.getQueueFamilies().graphicsQueue;
    initInfo.DescriptorPool            = descriptorPool->getVkDescriptorPool();
    initInfo.MinImageCount             = 2;
    initInfo.ImageCount                = 2;
    initInfo.MSAASamples               = DEVICE.getMsaaSample();
    ImGui_ImplVulkan_Init(&initInfo, WINDOW.getSwapChain().getRenderPass());
    ImGui_ImplGlfw_InstallCallbacks(WINDOW.getGlWindow());

    VkCommandBuffer command_buffer = DEVICE.beginInstantCommands();
    ImGui_ImplVulkan_CreateFontsTexture(command_buffer);
    DEVICE.endInstantCommands(command_buffer);
  }

  Gui::~Gui() {
    vkDeviceWaitIdle(DEVICE.getVkDevice());
    for(const auto& w: widgets) { delete w; }
    widgets.clear();
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
  }

  void Gui::render(VkCommandBuffer commandBuffer) {
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    for(const auto& w: widgets) { w->render(); }
    ImGui::ShowDemoWindow();
    ImGui::Render();
    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);
  }
}
