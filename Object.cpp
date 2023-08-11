#include "Object.h"

id_t Object::nextId = 0;
Object::Object(Transform transform) :instanceId{ nextId++ }, transform{ transform } {}

