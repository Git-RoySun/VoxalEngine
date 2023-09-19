#pragma once
#include <vector>
#include <glm/vec3.hpp>
namespace vc {
	class Material {
		uint32_t materialId;
		static uint32_t nextId;
		//TODO add textures
	public:
		struct Data {
			glm::vec3 colour;
			float albedo;
			alignas(16) float alpha;
		};

		static Material WHITE;
		static Material RED;
		static Material GREEN;
		static Material BLUE;

		static const std::vector<Material> MATERIALS;


		Material(glm::vec3 colour = glm::vec3{ 0.f }, float albedo = 0.f, float alpha = 1.f);
		uint32_t getId() const { return materialId; }
		Data getData() { return data; }
	private:
		Data data;
	};
}


