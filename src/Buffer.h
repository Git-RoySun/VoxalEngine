#pragma once
#include "Device.h"
#include "volk.h"

namespace gm{
  class Buffer {
    static VkDeviceSize getAlignment(VkDeviceSize instanceSize, VkDeviceSize minOffsetAlignment);

    Device& device;
    void* mapped = nullptr;
    VkBuffer buffer = VK_NULL_HANDLE;
    VkDeviceMemory memory = VK_NULL_HANDLE;

    VkDeviceSize bufferSize;
    uint32_t instanceCount;
    VkDeviceSize instanceSize;
    VkDeviceSize alignmentSize;
    VkBufferUsageFlags usageFlags;
    VkMemoryPropertyFlags memoryPropertyFlags;
  public:
    Buffer(
      Device& device,
      VkBufferUsageFlags usageFlags,
      VkMemoryPropertyFlags memoryPropertyFlags,
      VkDeviceSize instanceSize,
      uint32_t instanceCount = 1,
      VkDeviceSize minOffsetAlignment = 1);
    ~Buffer();

    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;

    VkResult map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
    void unmap();

    void writeToBuffer(void* data, VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
    VkResult flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
    VkDescriptorBufferInfo descriptorInfo(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
    VkResult invalidate(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

    void writeToIndex(void* data, int index);
    VkResult flushIndex(int index);
    VkDescriptorBufferInfo descriptorInfoForIndex(int index);
    VkResult invalidateIndex(int index);

    VkBuffer getVkBuffer() const { return buffer; }
    void* getMappedMemory() const { return mapped; }
    uint32_t getInstanceCount() const { return instanceCount; }
    VkDeviceSize getInstanceSize() const { return instanceSize; }
    VkDeviceSize getAlignmentSize() const { return alignmentSize; }
    VkBufferUsageFlags getUsageFlags() const { return usageFlags; }
    VkMemoryPropertyFlags getMemoryPropertyFlags() const { return memoryPropertyFlags; }
    VkDeviceSize getBufferSize() const { return bufferSize; }

    uint64_t getDeviceAddress() const;
		VkStridedDeviceAddressRegionKHR getSbtEntryStridedDeviceAddressRegion(const VkPhysicalDeviceRayTracingPipelinePropertiesKHR& rayTracingPipelineProperties);

    void transfer(VkBuffer dstBuffer);
  };
}
