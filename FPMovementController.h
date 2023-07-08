#pragma once
#include <deque>

#include "InputModule.h"
#include "DynamicObject.h"
#include "Updatable.h"
#include "Coordinates.h"

typedef ic::KeyEvent::IHandler IKeyHandler;
typedef ic::MouseEvent::IHandler IMouseHandler;

class FPMovementController: public Updatable, public IKeyHandler, public IMouseHandler {
	std::deque<glm::vec3> directionQueues[3] = {};
	float sensibility = 1000.f;
	DynamicObject* target;

	void KeyCallBack(int, int);
	void MouseCallBack(double, double);
protected:
	void update(float delta) override;
public:
	FPMovementController(DynamicObject* target) :target{ target } {};
	void EventCallback(int action, int mods) override { KeyCallBack(action, mods); };
	void EventCallback(double xpos, double ypos) override { MouseCallBack(xpos, ypos); };
};

