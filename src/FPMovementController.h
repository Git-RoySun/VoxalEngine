#pragma once
#include <deque>

#include "IMovable.h"
#include "InputModule.h"
#include "Updatable.h"
#include "IRotatable.h"

namespace ic {
	class FPMovementController : public Updatable, public IKeyHandler, public IMouseHandler {
		std::deque<glm::vec3> directionQueues[3] = {};
		float sensibility = 1000.f;
		IMovable* movable;
		IRotatable* rotatable;

		void KeyCallBack(int, int);
		void MouseCallBack(double, double);
	protected:
		void update(float delta) override;
	public:
		FPMovementController(IMovable* movable, IRotatable* rotatable) :movable{ movable }, rotatable{ rotatable } {};
		void EventCallback(int action, int mods) override { KeyCallBack(action, mods); };
		void EventCallback(double xpos, double ypos) override { MouseCallBack(xpos, ypos); };
	};

}