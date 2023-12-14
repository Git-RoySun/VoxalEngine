#pragma once
#include "volk.h"
#include <vector>
#include <memory>

#include "Container.h"

#ifdef NDEBUG
#define vl {}
#else
#define vl { "VK_LAYER_KHRONOS_validation" }
#endif

class Observer;
namespace gm{
	class Device;
	class Window;
	class Buffer;

	class Module {
		VkInstance instance;
		VkDebugUtilsMessengerEXT debugMessenger;
		std::vector<const char*> validationLayers = vl;
		std::vector<const char*> instanceExtensions = {};
		void createVkInstance();

		std::unique_ptr<Device> device{};
		std::unique_ptr<Window> window{};

		Module();
		~Module();
	public:
		static Module& getInstance();
		void addExtension(const char* name) { instanceExtensions.push_back(name); }
		void addExtension(std::vector<const char*> names) { instanceExtensions.insert(instanceExtensions.begin(),names.begin(),names.end()); }

		const VkInstance& getVkInstance() const { return instance; }
		Window& getWindow() const { return *window; }
		Device& getDevice() const { return *device; }

	};
}
