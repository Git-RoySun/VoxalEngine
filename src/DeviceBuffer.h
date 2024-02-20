#pragma once
#include "Buffer.h"

namespace gm {
  class DeviceBuffer {
  protected:
    std::unique_ptr<Buffer> localBuffer;

  public:
    DeviceBuffer(VkBufferUsageFlagBits usage, VkDeviceSize instanceSize, uint32_t instanceCount, VkDeviceSize offset = 0, bool clearFlag = false);
    void*   getData();
    void    writeData(void* data, VkDeviceSize offset = 0, VkDeviceSize size = VK_WHOLE_SIZE);
    Buffer& getLocalBuffer() const { return *localBuffer; }
  };
}
