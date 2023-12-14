#include "RasterizationPipeline.h"
#include "Device.h"
#include "Graphic.h"
#include "Utils.hpp"
#include "Voxel.h"

namespace gm{

	std::vector<VkVertexInputBindingDescription> RasterizationPipeline::bindings = {
	{0, sizeof(glm::vec3), VK_VERTEX_INPUT_RATE_VERTEX},
	{1, sizeof(obj::Voxel::Instance), VK_VERTEX_INPUT_RATE_INSTANCE },
	};

	std::vector<VkVertexInputAttributeDescription> RasterizationPipeline::attributes = {
			{0,0,VK_FORMAT_R32G32B32_SFLOAT, 0},
			{1,1,VK_FORMAT_R32G32B32_SFLOAT, offsetof(obj::Voxel::Instance, position)},
			{2,1,VK_FORMAT_R32_SFLOAT, offsetof(obj::Voxel::Instance, scale)},
			{3,1,VK_FORMAT_R32G32B32_SFLOAT, offsetof(obj::Voxel::Instance, rotation) },
			{4,1,VK_FORMAT_R32_UINT, offsetof(obj::Voxel::Instance, materialID)},
	};

	RasterizationPipeline::RasterizationPipeline(const std::string& vertPath, const std::string& fragPath, FixedStageInfo stageInfo){
		auto vertFile = Utils::readFile(vertPath);
		auto fragFile = Utils::readFile(fragPath);

		createShaderModule(vertFile, &vertShader);
		createShaderModule(fragFile, &fragShader);

		VkPipelineShaderStageCreateInfo shaderStages[2];
		shaderStages[0] = {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.stage = VK_SHADER_STAGE_VERTEX_BIT,
			.module = vertShader,
			.pName = "main",
			.pSpecializationInfo = nullptr,
		};

		shaderStages[1] = {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.stage = VK_SHADER_STAGE_FRAGMENT_BIT,
			.module = fragShader,
			.pName = "main",
			.pSpecializationInfo = nullptr,
		};

		VkPipelineVertexInputStateCreateInfo vertexInputInfo = {
				.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
				.vertexBindingDescriptionCount = static_cast<uint32_t>(bindings.size()),
				.pVertexBindingDescriptions = bindings.data(),
				.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributes.size()),
				.pVertexAttributeDescriptions = attributes.data(),
		};

		VkGraphicsPipelineCreateInfo pipelineInfo = {
			.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
			.stageCount = 2,
			.pStages = shaderStages,
			.pVertexInputState = &vertexInputInfo,
			.pInputAssemblyState = &stageInfo.inputAssemblyInfo,
			.pViewportState = &stageInfo.viewportInfo,
			.pRasterizationState = &stageInfo.rasterizationInfo,
			.pMultisampleState = &stageInfo.multisampleInfo,
			.pDepthStencilState = &stageInfo.depthStencilInfo,
			.pColorBlendState = &stageInfo.colorBlendInfo,
			.pDynamicState = &stageInfo.dynamicStateInfo,
			.layout = stageInfo.pipelineLayout,
			.renderPass = stageInfo.renderPass,
			.subpass = stageInfo.subpass,
			.basePipelineHandle = VK_NULL_HANDLE,
			.basePipelineIndex = -1,
		};

		VK_CHECK_RESULT(vkCreateGraphicsPipelines(gm::Module::getInstance().getDevice().getVkDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &vkPipeline), "Failed to create graphics pipeline!")
		
	}

	RasterizationPipeline::~RasterizationPipeline() {
		VkDevice device = gm::Module::getInstance().getDevice().getVkDevice();
		vkDestroyShaderModule(device, vertShader, nullptr);
		vkDestroyShaderModule(device, fragShader, nullptr);
		vkDestroyPipeline(device, vkPipeline, nullptr);
	};

	void RasterizationPipeline::createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule) {
		VkShaderModuleCreateInfo createInfo{
			.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
			.codeSize = code.size(),
			.pCode = reinterpret_cast<const uint32_t*>(code.data()),
		};

		VK_CHECK_RESULT(vkCreateShaderModule(gm::Module::getInstance().getDevice().getVkDevice(), &createInfo, nullptr, shaderModule),"Could not create shader module!")
	}

	RasterizationPipeline::FixedStageInfo RasterizationPipeline::defaultPipelineInfo(FixedStageInfo& configInfo) {
		configInfo.inputAssemblyInfo = {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
			.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
			.primitiveRestartEnable = VK_FALSE
		};

		configInfo.viewportInfo = {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
			.viewportCount = 1,
			.pViewports = nullptr,
			.scissorCount = 1,
			.pScissors = nullptr,
		};

		configInfo.rasterizationInfo = {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
			.depthClampEnable = VK_FALSE,
			.rasterizerDiscardEnable = VK_FALSE,
			.polygonMode = VK_POLYGON_MODE_FILL,
			.cullMode = VK_CULL_MODE_NONE,
			.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,
			.depthBiasEnable = VK_FALSE,
			.depthBiasConstantFactor = 0.0f,	//optional
			.depthBiasClamp = 0.0f,						//optional
			.depthBiasSlopeFactor = 0.0f,			//optional
			.lineWidth = 1.0f,
		};

		configInfo.multisampleInfo = {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
			.rasterizationSamples = gm::Module::getInstance().getDevice().getMsaaSample(),
			.sampleShadingEnable = VK_FALSE,
			.minSampleShading = 1.0f,
			.pSampleMask = nullptr,
			.alphaToCoverageEnable = VK_FALSE,
			.alphaToOneEnable = VK_FALSE,
		};

		configInfo.colorBlendAttachment = {
			.blendEnable = VK_FALSE,
			.srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
			.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
			.colorBlendOp = VK_BLEND_OP_ADD,
			.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
			.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
			.alphaBlendOp = VK_BLEND_OP_ADD,
			.colorWriteMask =
				VK_COLOR_COMPONENT_R_BIT |
				VK_COLOR_COMPONENT_G_BIT |
				VK_COLOR_COMPONENT_B_BIT |
				VK_COLOR_COMPONENT_A_BIT,
		};

		configInfo.colorBlendInfo = {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
			.logicOpEnable = VK_FALSE,
			.logicOp = VK_LOGIC_OP_COPY,
			.attachmentCount = 1,
			.pAttachments = &configInfo.colorBlendAttachment,
			.blendConstants = {0.0f,0.0f,0.0f,0.0f},
		};

		configInfo.depthStencilInfo = {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
			.depthTestEnable = VK_TRUE,
			.depthWriteEnable = VK_TRUE,
			.depthCompareOp = VK_COMPARE_OP_LESS,
			.depthBoundsTestEnable = VK_FALSE,
			.stencilTestEnable = VK_FALSE,
			.front = {},
			.back = {},
			.minDepthBounds = 0.0f,
			.maxDepthBounds = 1.0f,
		};

		configInfo.dynamicStateEnables = { VK_DYNAMIC_STATE_VIEWPORT,VK_DYNAMIC_STATE_SCISSOR };
		configInfo.dynamicStateInfo = {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
			.flags = 0,
			.dynamicStateCount = static_cast<uint32_t>(configInfo.dynamicStateEnables.size()),
			.pDynamicStates = configInfo.dynamicStateEnables.data()
		};

		return configInfo;
	}

}
