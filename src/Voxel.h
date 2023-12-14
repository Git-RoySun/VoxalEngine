#pragma once
#include "Object.h"

namespace obj{
	class Voxel: public Base{
	public:
		struct Instance{
			alignas(16) glm::vec3 position{};
			alignas(4) float scale = 1.f;
			alignas(16) glm::vec3 rotation{};
			alignas(4) uint32_t materialID = 0;
		};
	private:
		uint32_t materialID;
	public:
		Voxel(Transform transform = {}, uint32_t matId = 0):Base{transform}, materialID{matId} {};
		Instance toInstance() const { return Instance{ transform.position, 1, transform.rotation, materialID }; }
	};
}


