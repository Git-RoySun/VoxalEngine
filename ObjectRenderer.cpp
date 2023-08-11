#include "ObjectRenderer.h"

namespace vc {
	void ObjectRenderer::renderObjects(FrameInfo info, std::vector<Object>& objects) {
		pipeline->bind(info.commandBuffer);
		//for (auto& obj : objects) {
		//	PushConstantData push{
		//		.transform = obj.getTransform(),
		//	};

		//	vkCmdPushConstants(
		//		info.commandBuffer,
		//		pipelineLayout,
		//		VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
		//		0,
		//		sizeof(PushConstantData),
		//		&push);

		//	vkCmdBindDescriptorSets(
		//		info.commandBuffer,
		//		VK_PIPELINE_BIND_POINT_GRAPHICS,
		//		pipelineLayout,
		//		0, 1,
		//		&info.descriptorSet,
		//		0, nullptr);

		//	vkCmdDrawIndexed(info.commandBuffer,static_cast<uint32_t>(obj.getModel().getIndices().size()), 1, obj.getModel().getIndexOffset(), 0, 0);
		//}
	}
}
