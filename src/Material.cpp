#include "Material.h"

namespace vc {

	Material::Material(glm::vec3 colour, float albedo, float alpha):
		data{.colour = colour,.albedo = albedo,.alpha = alpha}{
		materialId = nextId++;
	}

	uint32_t Material::nextId = 0;
	Material Material::WHITE = Material{ glm::vec3{1.f} };
	Material Material::RED = Material{ glm::vec3{1.f,0.f,0.f} };
	Material Material::GREEN = Material{ glm::vec3{0.f,1.f,0.f} };
	Material Material::BLUE = Material{ glm::vec3{0.f,0.f,1.f} };

	const std::vector<Material> Material::MATERIALS = {
		Material::WHITE,
		Material::RED,
		Material::GREEN,
		Material::BLUE,
	};
}
