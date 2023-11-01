#pragma once
#include <memory>
#include <glm/vec3.hpp>

#include "Buffer.h"
#include "Descriptor.h"
#include "Device.h"
#include "Pipeline.h"
#include "RenderSystem.h"
#include "SwapChain.h"
#include "Voxel.h"

namespace vc {
	struct RTPushConstant{
		glm::vec4  clearColor;
		glm::vec3  lightPosition;
		float lightIntensity;
	};

	struct ScratchBuffer {
		uint64_t deviceAddress = 0;
		VkBuffer handle = VK_NULL_HANDLE;
		VkDeviceMemory memory = VK_NULL_HANDLE;
	};

	struct AccelerationStructure {
		VkAccelerationStructureKHR handle;
		uint64_t deviceAddress = 0;
		VkDeviceMemory memory;
		VkBuffer buffer;
	};

	struct AABB {
		glm::vec3 min;
		glm::vec3 max;
	};

	class ShaderBindingTable : public Buffer {
	public:
		ShaderBindingTable(
			Device& device,
			VkDeviceSize size,
			VkBufferUsageFlags usageFlags,
			VkMemoryPropertyFlags memoryPropertyFlags):Buffer(device,size,1,usageFlags,memoryPropertyFlags){
		};

		VkStridedDeviceAddressRegionKHR stridedDeviceAddressRegion{};
	};


	class VoxelRayTracer{
		VkPhysicalDeviceRayTracingPipelinePropertiesKHR  rayTracingPipelineProperties{};
		VkPhysicalDeviceAccelerationStructureFeaturesKHR accelerationStructureFeatures{};

		// Enabled features and properties
		VkPhysicalDeviceBufferDeviceAddressFeatures enabledBufferDeviceAddresFeatures{
			.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES,
			.bufferDeviceAddress = VK_TRUE
		};

		VkPhysicalDeviceRayTracingPipelineFeaturesKHR enabledRayTracingPipelineFeatures{
			.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR,
			.rayTracingPipeline = VK_TRUE,
		};

		VkPhysicalDeviceAccelerationStructureFeaturesKHR enabledAccelerationStructureFeatures{
			.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR,
			.accelerationStructure = VK_TRUE,
		};

		Device& device;

		AccelerationStructure bottomLevelAS{};
		AccelerationStructure topLevelAS{};

		std::vector<VkRayTracingShaderGroupCreateInfoKHR> shaderGroups{};
		std::vector<VkShaderModule> shaderModules{};

		std::unique_ptr<Buffer> ubo;
		std::unique_ptr<DescriptorPool> descriptorPool{};
		std::unique_ptr<DescriptorSetLayout> setLayout{};
		VkDescriptorSet descriptorSet;

		struct ShaderBindingTables {
			std::unique_ptr<ShaderBindingTable> raygen;
			std::unique_ptr <ShaderBindingTable> miss;
			std::unique_ptr <ShaderBindingTable> hit;
		} shaderBindingTables;

		struct StorageImage {
			VkDeviceMemory memory = VK_NULL_HANDLE;
			VkImage image = VK_NULL_HANDLE;
			VkImageView view = VK_NULL_HANDLE;
			VkFormat format;
		} storageImage;

		struct UniformBuffer{
			glm::mat4 view;
			glm::mat4 proj;

			glm::vec4  clearColor;
			glm::vec3  lightPosition;
			float lightIntensity;
		};

		std::unique_ptr<Buffer> stagingBuffer;

		const int INSTANCEMAX = 1000000;
		int instanceCount = 0;
		bool changed = false;

		VkPipeline pipeline;
		VkPipelineLayout pipelineLayout;

		void enableExtension();
		void createBottomLevelAS();
		void createTopLevelAS();
		void createShaderBindingTables();
		void createRayTracingPipeline();
		void createDescriptorSets(SwapChain& swapchain, Buffer& iBuffer, Buffer& mBuffer);
		VkAccelerationStructureInstanceKHR createInstance(obj::Voxel::Instance& instance);

		//helper function (in parent class)
		ScratchBuffer createScratchBuffer(VkDeviceSize size);
		void deleteScratchBuffer(ScratchBuffer& scratchBuffer);
		void createAccelerationStructure(AccelerationStructure& accelerationStructure, VkAccelerationStructureTypeKHR type, VkAccelerationStructureBuildSizesInfoKHR buildSizeInfo);
		void deleteAccelerationStructure(AccelerationStructure& accelerationStructure);
		std::unique_ptr<ShaderBindingTable> createShaderBindingTable(uint32_t handleCount);
		uint64_t getBufferDeviceAddress(VkBuffer buffer);
		VkStridedDeviceAddressRegionKHR getSbtEntryStridedDeviceAddressRegion(VkBuffer buffer, uint32_t handleCount);
		VkPipelineShaderStageCreateInfo loadShader(std::string fileName, VkShaderStageFlagBits stage);
		void createStorageImage(VkFormat format, VkExtent3D extent);
		void deleteStorageImage();
	public:
		static std::vector<const char*> requiredExtensions;

		VoxelRayTracer(Device& device);
		~VoxelRayTracer();

		void init(SwapChain& swapchain, Buffer& iBuffer, Buffer& mBuffer);
		void render(FrameInfo info, SwapChain& swapchain, Buffer& iBuffer, Buffer& mBuffer);
		void addInstance(obj::Voxel::Instance& instance);
	};
}

