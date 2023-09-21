#include "CameraObject.h"
namespace obj {
	void Camera::move(glm::vec3 direction) {
		setPosition(transform.position + direction * speed);
	};

	void Camera::rotate(glm::vec3 rotation) {
		setRotation(transform.rotation + rotation);
	};

	void Camera::setPosition(glm::vec3 pos) {
		transform.position = pos;
		camera.setRotation(pos, transform.rotation);
	};

	void Camera::setRotation(glm::vec3 rotation) {
		transform.rotation = rotation;
		camera.setRotation(transform.position, rotation);
	};
}
