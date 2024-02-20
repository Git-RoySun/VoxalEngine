#include "DeviceBuffer.h"
#include "Graphic.h"

namespace gm {
  DeviceBuffer::DeviceBuffer(VkBufferUsageFlagBits usage, VkDeviceSize instanceSize, uint32_t instanceCount, VkDeviceSize offset) {
    localBuffer = std::make_unique<Buffer>(
      DEVICE,
      usage | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
      instanceSize,
      instanceCount,
      offset
    );
    stagingBuffer = std::make_unique<Buffer>(
      DEVICE,
      usage | VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
      instanceSize,
      instanceCount,
      offset
    );
  }

  void DeviceBuffer::synchronize() {
    stagingBuffer->flush();
    stagingBuffer->transfer(localBuffer->getVkBuffer());
  }
}
