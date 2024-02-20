#include "DeviceBuffer.h"
#include "Graphic.h"

namespace gm {
  DeviceBuffer::DeviceBuffer(VkBufferUsageFlagBits usage, VkDeviceSize instanceSize, uint32_t instanceCount, VkDeviceSize offset, bool clearFlag) {
    localBuffer = std::make_unique<Buffer>(
      DEVICE,
      usage | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
      instanceSize,
      instanceCount,
      offset
    );

    if(clearFlag) {
      auto commandBuffer = DEVICE.beginInstantCommands();
      vkCmdFillBuffer(commandBuffer, localBuffer->getVkBuffer(), 0, localBuffer->getBufferSize(), 0);
      DEVICE.endInstantCommands(commandBuffer);
    }
  }

  void* DeviceBuffer::getData() {
    Buffer stagingBuffer(
      DEVICE,
      VK_BUFFER_USAGE_TRANSFER_DST_BIT,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
      localBuffer->getInstanceSize(),
      localBuffer->getInstanceCount()
    );

    void* dst = malloc(stagingBuffer.getBufferSize());
    localBuffer->transfer(stagingBuffer.getVkBuffer());
    stagingBuffer.map();
    memcpy(stagingBuffer.getMappedMemory(), dst, stagingBuffer.getBufferSize());
    stagingBuffer.unmap();
    return dst;
  }

  void DeviceBuffer::writeData(void* data, VkDeviceSize offset, VkDeviceSize size) {
    Buffer stagingBuffer(
      DEVICE,
      VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
      size,
      1
    );
    stagingBuffer.map();
    stagingBuffer.writeToBuffer(data, size);
    stagingBuffer.flush();
    stagingBuffer.transfer(localBuffer->getVkBuffer());
    stagingBuffer.unmap();
  }
}
