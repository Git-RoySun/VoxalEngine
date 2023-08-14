#include "VisualContext.h"

#include "Voxel.h"

namespace vc {
	VisualContext::VisualContext():
		instanceBuffer{
		device,
			sizeof(Voxel::Instance),
			10000000,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
			1
		},
		ubo{
		device,
			sizeof(UniformBuffer),
			SwapChain::MAX_FRAMES_IN_FLIGHT,
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
			device.properties.limits.minUniformBufferOffsetAlignment
		} {
		descriptorPool = DescriptorPool::Builder(device)
			.setMaxSets(SwapChain::MAX_FRAMES_IN_FLIGHT)
			.addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, SwapChain::MAX_FRAMES_IN_FLIGHT)
			.build();
		ubo.map();

		setLayout = DescriptorSetLayout::Builder(device)
			.addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
			.build();

		for (int i = 0; i < descriptorSets.size(); ++i) {
			auto uboInfo = ubo.descriptorInfo();

			DescriptorWriter(*setLayout, *descriptorPool)
				.writeBuffer(0, &uboInfo)
				.build(descriptorSets[i]);
		}

		voxelStage.init(setLayout->getDescriptorSetLayout(), renderer.getRenderPass());
	}

	VisualContext::~VisualContext(){
		vkDeviceWaitIdle(device.getVkDevice());
	}

	void VisualContext::setCamera(Camera* cam) {
		camera = cam;
		cam->setPerspectiveProjection(glm::radians(50.f), renderer.getAspectRatio(), .1f, 35.f);
	}

	bool VisualContext::addInstance(Voxel::Instance instance){
		bool result = false;
		if(instanceBuffer.getMappedMemory()==nullptr)
			instanceBuffer.map();
		instanceBuffer.writeToIndex(&instance, instanceCount);
		instanceBuffer.flushIndex(instanceCount);
		instanceCount++;

		return true;//TODO return actual result when it means something (it should fail if not enough space)
	}

	void VisualContext::clearInstances(){
		instanceCount = 0;
	}

	void VisualContext::renderFrame(){
		if (auto commandBuffer = renderer.startFrame()) {
			if(instanceBuffer.getMappedMemory()!=nullptr)
				instanceBuffer.unmap();
			int frameIndex = renderer.getFrameIndex();

			UniformBuffer data;
			data.projectionView = camera->getProjection() * camera->getView();
			ubo.writeToIndex(&data, frameIndex);
			ubo.flushIndex(frameIndex);

			auto now = std::chrono::steady_clock::now();
			std::chrono::duration<float> delta = now - last;
			last = now;

			//rendering stage
			FrameInfo frameInfo{
				.frameIndex = frameIndex,
				.frameTime = delta.count(),
				.commandBuffer = commandBuffer,
				.instanceBuffer = instanceBuffer.getVkBuffer(),
				.camera = *camera,
				.descriptorSet = descriptorSets[frameIndex]
			};

			renderer.startRenderPass(commandBuffer);

			voxelStage.renderVoxels(frameInfo, instanceCount);

			renderer.endRenderPass(commandBuffer);
			renderer.endFrame();
		}
	}
}

