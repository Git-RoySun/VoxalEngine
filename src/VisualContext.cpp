#include "VisualContext.h"

#include "Voxel.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"
#include "Material.h"
#include "UIModule.h"

namespace vc {
	VisualContext::VisualContext(){
		device.init();
		renderer.init();
		instanceBuffer = std::make_unique<Buffer>(
			device,
			sizeof(obj::Voxel::Instance),
			INSTANCEMAX,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			1
		);

		stagingBuffer = std::make_unique<Buffer>(
			device,
			sizeof(obj::Voxel::Instance),
			INSTANCEMAX,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			1
		);
		materialBuffer = std::make_unique<Buffer>(
			device,
			sizeof(Material::Data),
			static_cast<uint32_t>(Material::MATERIALS.size()),
			VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			1
		);

		ubo = std::make_unique<Buffer>(
			device,
			sizeof(UniformBuffer),
			SwapChain::MAX_FRAMES_IN_FLIGHT,
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
			device.properties.limits.minUniformBufferOffsetAlignment
		);
		Buffer msBuffer{
		device,
			sizeof(Material),
			static_cast<uint32_t>(Material::MATERIALS.size()),
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			1
		};
		std::vector<Material::Data> mats{};
		for(auto i:Material::MATERIALS){
			mats.emplace_back(i.getData());
		}
		msBuffer.map();
		msBuffer.writeToBuffer((void*)mats.data());
		device.copyBuffer(msBuffer.getVkBuffer(), materialBuffer->getVkBuffer(), sizeof(mats[0]) * mats.size());

		descriptorPool = DescriptorPool::Builder(device)
			.setMaxSets(3)
			.addPoolSize(VK_DESCRIPTOR_TYPE_SAMPLER, 20)
			.addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 20)
			.addPoolSize(VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 20)
			.addPoolSize(VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 20)
			.addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 20)
			.addPoolSize(VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 20)
			.addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 20)
			.addPoolSize(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 20)
			.addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 20)
			.addPoolSize(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 20)
			.addPoolSize(VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 20)
			.build();
		ubo->map();

		setLayout = DescriptorSetLayout::Builder(device)
			.addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
			.addBinding(1, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
			.build();
		for (int i = 0; i < descriptorSets.size(); ++i) {
			auto uboInfo = ubo->descriptorInfo();
			auto matInfo = materialBuffer->descriptorInfo();
			DescriptorWriter(*setLayout, *descriptorPool)
				.writeBuffer(0, &uboInfo)
				.writeBuffer(1, &matInfo)
				.build(descriptorSets[i]);
		}

		voxelStage.init(setLayout->getDescriptorSetLayout(), renderer.getRenderPass());
		//outlineStage.init(setLayout->getDescriptorSetLayout(), renderer.getRenderPass());

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsLight();

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForVulkan(window.getGlWindow(), false);
		ImGui_ImplVulkan_InitInfo init_info = {};
		init_info.Instance = device.getInstance();
		init_info.PhysicalDevice = device.getPhysivcalDevice();
		init_info.Device = device.getVkDevice();
		init_info.QueueFamily = device.findPhysicalQueueFamilies().graphicsFamily;
		init_info.Queue = device.graphicsQueue();
		init_info.DescriptorPool = descriptorPool->getVkDescriptorPool();
		init_info.MinImageCount = SwapChain::MAX_FRAMES_IN_FLIGHT;
		init_info.ImageCount = 3;
		init_info.MSAASamples = device.getMaxUsableSampleCount();
		ImGui_ImplVulkan_Init(&init_info, renderer.getRenderPass());

		VkCommandBuffer command_buffer = device.beginSingleTimeCommands();
		ImGui_ImplVulkan_CreateFontsTexture(command_buffer);
		device.endSingleTimeCommands(command_buffer);
		UIModule::add([this](){
			ImGui::Text("Instance count:%d", instanceCount);
			ImGui::Text("Mapped: %s", (stagingBuffer->getMappedMemory()==nullptr)?"false":"true");
		});
	}

	VisualContext::~VisualContext(){
		vkDeviceWaitIdle(device.getVkDevice());
		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void VisualContext::setCamera(Camera* cam) {
		camera = cam;
		cam->setPerspectiveProjection(glm::radians(50.f), renderer.getAspectRatio(), .1f, 35.f);
	}

	bool VisualContext::addInstance(obj::Voxel::Instance instance){
		if(stagingBuffer->getMappedMemory()==nullptr)
			stagingBuffer->map();
		stagingBuffer->writeToIndex(&instance, instanceCount);
		return (++instanceCount < INSTANCEMAX);
	}

	void VisualContext::renderFrame(){
		if (auto commandBuffer = renderer.startFrame()) {
			ImGui_ImplVulkan_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			ImGui::ShowDemoWindow();
			ImGui::Begin("Debug window");
			UIModule::render();

			if(stagingBuffer->getMappedMemory()!=nullptr)
				stagingBuffer->unmap();
			device.copyBuffer(stagingBuffer->getVkBuffer(), instanceBuffer->getVkBuffer(), stagingBuffer->getInstanceSize()*instanceCount);

			
			int frameIndex = renderer.getFrameIndex();

			UniformBuffer data;
			data.projectionView = camera->getProjection() * camera->getView();
			ubo->writeToIndex(&data, frameIndex);
			ubo->flushIndex(frameIndex);

			auto now = std::chrono::steady_clock::now();
			std::chrono::duration<float> delta = now - last;
			last = now;

			//rendering stage
			FrameInfo frameInfo{
				.frameIndex = frameIndex,
				.frameTime = delta.count(),
				.commandBuffer = commandBuffer,
				.instanceBuffer = instanceBuffer->getVkBuffer(),
				.camera = *camera,
				.descriptorSet = descriptorSets[frameIndex]
			};

			delta = now - start;
			ImGui::Text("FPS: %f", (frames++)/delta.count());
			if(delta.count()>1){
				start = now;
				frames = 0;
			}

			ImGui::End();
			ImGui::Render();
			renderer.startRenderPass(commandBuffer);
			voxelStage.renderVoxels(frameInfo, instanceCount);
			//outlineStage.renderOutlines(frameInfo, instanceCount);
			ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);
			renderer.endRenderPass(commandBuffer);
			renderer.endFrame();
		}
	}
}

