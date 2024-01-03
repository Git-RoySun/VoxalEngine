#include "Rasterizer.h"
#include "Observer.h"
#include "SwapChain.h"
#include "Utils.hpp"
#include "Window.h"
#include "Buffer.h"

static std::vector<glm::vec3> vertices = {
	{-0.5f, -0.5f, -0.5f},
	//left bottom far     0
	{-0.5f, -0.5f, 0.5f},
	//left bottom close   1
	{-0.5f, 0.5f, -0.5f},
	//left top far        2
	{-0.5f, 0.5f, 0.5f},
	//left top close      3
	{0.5f, -0.5f, -0.5f},
	//right bottom far    4
	{0.5f, -0.5f, 0.5f},
	//right bottom close  5
	{0.5f, 0.5f, -0.5f},
	//right top far       6
	{0.5f, 0.5f, 0.5f},
	//right top close     7
};

static std::vector<uint32_t> indices = {0, 1, 3, 3, 2, 0, 5, 4, 6, 6, 7, 5, 1, 5, 7, 7, 3, 1, 4, 0, 2, 2, 6, 4, 3, 7, 6, 6, 2, 3, 0, 4, 5, 5, 1, 0};

static const uint32_t vertexCount = static_cast<uint32_t>(vertices.size());
static const uint32_t indexCount = static_cast<uint32_t>(indices.size());

namespace gm {
	std::unique_ptr<Buffer> Rasterizer::vertexBuffer = nullptr;
	std::unique_ptr<Buffer> Rasterizer::indexBuffer = nullptr;

	Rasterizer::Rasterizer() {
		instanceBuffer = std::make_unique<Buffer>(Module::getInstance().getDevice(), VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 1);

		if(!vertexBuffer || !instanceBuffer)initBuffers();
		initPipelineLayout();
		initPipeline();
	}

	void Rasterizer::initBuffers() {
		Buffer vertexStager{Module::getInstance().getDevice(), VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, sizeof(vertices[0]), vertexCount,};
		vertexStager.map();
		vertexStager.writeToBuffer(vertices.data());
		vertexStager.flush();
		vertexBuffer = std::make_unique<Buffer>(Module::getInstance().getDevice(), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, sizeof(vertices[0]), vertexCount);
		vertexStager.transfer(vertexBuffer->getVkBuffer());

		Buffer indexStager{Module::getInstance().getDevice(), VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, sizeof(indices[0]), indexCount,};
		indexStager.map();
		indexStager.writeToBuffer(indices.data());
		indexStager.flush();
		indexBuffer = std::make_unique<Buffer>(Module::getInstance().getDevice(), VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, sizeof(indices[0]), indexCount);
		indexStager.transfer(indexBuffer->getVkBuffer());
	}

	void Rasterizer::initPipelineLayout() {
		VkPushConstantRange pushConstantRange{.stageFlags = VK_SHADER_STAGE_VERTEX_BIT, .offset = 0, .size = sizeof(PushConstant)};

		auto& device = gm::Module::getInstance().getDevice();
		VkPipelineLayoutCreateInfo layoutInfo = {.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO, .setLayoutCount = 0, .pSetLayouts = nullptr, .pushConstantRangeCount = 1, .pPushConstantRanges = &pushConstantRange,};

		VK_CHECK_RESULT(vkCreatePipelineLayout(device.getVkDevice(), &layoutInfo, nullptr, &pipelineLayout), "Failed to create pipeline Layout!")
	};

	void Rasterizer::initPipeline() {
		RasterizationPipeline::FixedStageInfo configInfo{};
		auto pipelineConfig = RasterizationPipeline::defaultPipelineInfo(configInfo);
		pipelineConfig.multisampleInfo.rasterizationSamples = gm::Module::getInstance().getDevice().getMsaaSample();
		pipelineConfig.rasterizationInfo.cullMode = VK_CULL_MODE_BACK_BIT;
		pipelineConfig.renderPass = gm::Module::getInstance().getWindow().getSwapChain().getRenderPass();
		pipelineConfig.pipelineLayout = pipelineLayout;

		pipeline = std::make_unique<RasterizationPipeline>("shaders/vert.spv", "shaders/frag.spv", pipelineConfig);
	}

	void Rasterizer::bindInstances(std::vector<obj::Voxel::Instance> voxels) {
		instanceCount = voxels.size();
		Buffer stagingBuffer{Module::getInstance().getDevice(), VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, sizeof(voxels[0]), static_cast<uint32_t>(voxels.size())};

		stagingBuffer.map();
		stagingBuffer.writeToBuffer(voxels.data());
		stagingBuffer.flush();
		oldInstanceBuffer = std::move(instanceBuffer);
		instanceBuffer = std::make_unique<Buffer>(Module::getInstance().getDevice(), VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, sizeof(voxels[0]), static_cast<uint32_t>(voxels.size()));

		stagingBuffer.transfer(instanceBuffer->getVkBuffer());
	}

	void Rasterizer::render(Frame frameInfo) {
		if(frameInfo.observer->changed) {
			bindInstances(frameInfo.observer->getData());
			frameInfo.observer->changed = false;
		}
		vkCmdBindPipeline(frameInfo.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->getVkPipeline());

		VkBuffer buffers[] = {vertexBuffer->getVkBuffer(), instanceBuffer->getVkBuffer()};
		VkDeviceSize offsets[] = {0, 0};
		vkCmdBindVertexBuffers(frameInfo.commandBuffer, 0, 2, buffers, offsets);
		vkCmdBindIndexBuffer(frameInfo.commandBuffer, indexBuffer->getVkBuffer(), 0, VK_INDEX_TYPE_UINT32);

		PushConstant pushConstantsData{.transform = frameInfo.observer->getCamera().getProjection() * frameInfo.observer->getCamera().getView()};
		vkCmdPushConstants(frameInfo.commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(PushConstant), &pushConstantsData);

		vkCmdDrawIndexed(frameInfo.commandBuffer, indexCount, instanceCount, 0, 0, 0);
	}

	void Rasterizer::cleanup() {
		vkDeviceWaitIdle(gm::Module::getInstance().getDevice().getVkDevice());
		vertexBuffer = nullptr;
		indexBuffer = nullptr;
	}
}
