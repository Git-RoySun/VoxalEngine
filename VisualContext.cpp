#include "VisualContext.h"
namespace vc {
	VisualContext::VisualContext():
		ubo{
			device,
				sizeof(UniformBuffer),
				vc::SwapChain::MAX_FRAMES_IN_FLIGHT,
				VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
				device.properties.limits.minUniformBufferOffsetAlignment
	} {
		descriptorPool = vc::DescriptorPool::Builder(device)
			.setMaxSets(vc::SwapChain::MAX_FRAMES_IN_FLIGHT)
			.addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, vc::SwapChain::MAX_FRAMES_IN_FLIGHT)
			.build();
		ubo.map();

		setLayout = vc::DescriptorSetLayout::Builder(device)
			.addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
			.build();

		for (int i = 0; i < descriptorSets.size(); ++i) {
			auto bufferInfo = ubo.descriptorInfo();
			vc::DescriptorWriter(*setLayout, *descriptorPool)
				.writeBuffer(0, &bufferInfo)
				.build(descriptorSets[i]);
		}

		renderSystem.init(setLayout->getDescriptorSetLayout(), renderer.getRenderPass());
	}

	VisualContext::~VisualContext(){
		vkDeviceWaitIdle(device.getVkDevice());
	}

	void VisualContext::setCamera(Camera* cam) {
		camera = cam;
		cam->setPerspectiveProjection(glm::radians(50.f), renderer.getAspectRatio(), .1f, 15.f);
	}

	void VisualContext::renderFrame(std::vector<Object>& objects){
		if (auto commandBuffer = renderer.startFrame()) {
			int frameIndex = renderer.getFrameIndex();
			vc::UniformBuffer data;
			data.projectionView = camera->getProjection() * camera->getView();
			ubo.writeToIndex(&data, frameIndex);
			ubo.flushIndex(frameIndex);

			auto now = std::chrono::steady_clock::now();
			std::chrono::duration<float> delta = now - last;
			last = now;

			//rendering stage
			vc::FrameInfo frameInfo{
				.frameIndex = frameIndex,
				.frameTime = delta.count(),
				.commandBuffer = commandBuffer,
				.camera = *camera,
				.descriptorSet = descriptorSets[frameIndex]
			};

			renderer.startRenderPass(commandBuffer);
			renderSystem.renderObjects(frameInfo,objects,meshes);
			renderer.endRenderPass(commandBuffer);
			renderer.endFrame();
		}
	}

	void VisualContext::loadModel(Model& model){
		model.setIndexOffset(meshes.addMesh(model));
	}
}

