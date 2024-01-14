#pragma once
#include "volk.h"
#include <cassert>
#include <iostream>
#include <chrono>
#include <fstream>
#include <vulkan/vk_enum_string_helper.h>

#define VK_CHECK_RESULT(f, msg){\
	VkResult res = (f);\
	if (res != VK_SUCCESS){\
		std::cerr << "[FATAL]: VkResult is \" "<< string_VkResult(res) << "\" in " << __FILE__ << " at line " << __LINE__ << "\n";\
		assert(res == VK_SUCCESS && msg);\
	}\
}

#undef DECL_ENUM_ELEMENT
#undef BEGIN_ENUM
#undef END_ENUM

#ifndef GENERATE_ENUM_STRINGS
#define DECL_ENUM_ELEMENT( element ) element
#define BEGIN_ENUM( ENUM_NAME ) typedef enum tag##ENUM_NAME
#define END_ENUM( ENUM_NAME ) ENUM_NAME; \
            char* get##ENUM_NAME(enum tag##ENUM_NAME index);
#else
#define DECL_ENUM_ELEMENT( element ) #element
#define BEGIN_ENUM( ENUM_NAME ) char* gs_##ENUM_NAME [] =
#define END_ENUM( ENUM_NAME ) ; char* get##ENUM_NAME(enum \
            tag##ENUM_NAME index){ return gs_##ENUM_NAME [index]; }
#endif

struct debugTimer {
  std::string                           process;
  std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
};

class Utils {
public:
  static void warn(const std::string& msg) {
    std::cerr << "[WARN]: " << msg << std::endl;
  }

  static void info(const std::string& msg) {
    std::cout << "[INFO]: " << msg << std::endl;
  }

  static debugTimer startTimer(std::string process) {
    return debugTimer{process};
  }

  static void endTimer(debugTimer& timer) {
    auto                         now   = std::chrono::steady_clock::now();
    std::chrono::duration<float> delta = now - timer.start;
    auto                         time  = std::chrono::duration_cast<std::chrono::milliseconds>(delta).count();
    std::cout << std::format("[Timer]: {} took {} ms", timer.process, time) << std::endl;
  }

  static std::vector<char> readFile(const std::string& filePath) {
    std::ifstream file{filePath, std::ios::ate | std::ios::binary};
    //^ this line will call abort (crash) if file is invalid and not throw an error
    if(!file.is_open()) {
      throw std::runtime_error("Pipeline failed to open file " + filePath);
    }
    size_t            fileSize = static_cast<size_t>(file.tellg());
    std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();
    return buffer;
  }
};
