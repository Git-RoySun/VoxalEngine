#pragma once
#include "Buffer.h"

namespace gm {
  class DeviceBuffer {
  protected:
    std::unique_ptr<Buffer> localBuffer;
    std::unique_ptr<Buffer> stagingBuffer;

  public:
    DeviceBuffer(VkBufferUsageFlagBits usage, VkDeviceSize instanceSize, uint32_t instanceCount, VkDeviceSize offset = 0);
    Buffer& getStagingBuffer() const { return *stagingBuffer; }
    Buffer& getLocalBuffer() const { return *localBuffer; }
    void    synchronize();
  };
}
