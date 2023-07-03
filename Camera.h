#pragma once
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
namespace vc {
	class Camera {
		glm::mat4 projection{1.f};
		glm::mat4 view{1.f};
	public:
		void setOrthographicProjection(float left, float bottom, float near, float right, float top, float far);
		void setPerspectiveProjection(float vfov, float aspect, float near, float far);
		void setViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up = { 0.f,-1.f,0.f });
		void setViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up = { 0.f,-1.f,0.f });
		void setRotation(glm::vec3 position, glm::vec3 rotation);
		const glm::mat4& getProjection() const { return projection; };
		const glm::mat4& getView() const { return view; };
	};
}


