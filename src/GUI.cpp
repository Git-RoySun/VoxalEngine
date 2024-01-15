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
    auto& module   = Module::getInstance();
    auto  instance = module.getVkInstance();
    auto& window   = module.getWindow();
    auto& device   = module.getDevice();

    ImGui_ImplVulkan_LoadFunctions(
      [](const char* function_name, void* vulkan_instance) {
        return vkGetInstanceProcAddr(*(static_cast<VkInstance*>(vulkan_instance)), function_name);
      }, &instance
    );

    descriptorPool = DescriptorPool::Builder(Module::getInstance().getDevice())
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
    ImGui_ImplGlfw_InitForVulkan(window.getGlWindow(), false);
    ImGui_ImplVulkan_InitInfo initInfo = {};
    initInfo.Instance                  = module.getVkInstance();
    initInfo.PhysicalDevice            = device.getPhysicalDevice();
    initInfo.Device                    = device.getVkDevice();
    initInfo.QueueFamily               = device.getQueueFamilies().graphicsFamily;
    initInfo.Queue                     = device.getQueueFamilies().graphicsQueue;
    initInfo.DescriptorPool            = descriptorPool->getVkDescriptorPool();
    initInfo.MinImageCount             = 2;
    initInfo.ImageCount                = 2;
    initInfo.MSAASamples               = device.getMsaaSample();
    ImGui_ImplVulkan_Init(&initInfo, window.getSwapChain().getRenderPass());
    ImGui_ImplGlfw_InstallCallbacks(window.getGlWindow());

    VkCommandBuffer command_buffer = device.beginInstantCommands();
    ImGui_ImplVulkan_CreateFontsTexture(command_buffer);
    device.endInstantCommands(command_buffer);
  }

  Gui::~Gui() {
    vkDeviceWaitIdle(Module::getInstance().getDevice().getVkDevice());
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
