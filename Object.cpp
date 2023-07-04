#include "Object.h"

id_t Object::nextId = 0;
Object::Object():instanceId(nextId++){}
Object::Object(std::unique_ptr<vc::Model> model, Transform transform) :instanceId{ nextId++ }, model{ std::move(model) }, transform{ transform } {}

