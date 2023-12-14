#include "Object.h"

namespace obj {
	uint32_t Base::nextId = 0;
	Base::Base(Transform transform) :instanceId{ nextId++ }, transform{ transform } {}
}