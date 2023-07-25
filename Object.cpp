#include "Object.h"

id_t Object::nextId = 0;
Object::Object():instanceId(nextId++){}
Object::Object(std::shared_ptr<vc::Model> model, Transform transform) :instanceId{ nextId++ }, model{ model }, transform{ transform } {}

