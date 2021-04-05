/*
 * Car.cpp
 *
 *  Created on: 2018/11/10
 *      Author: Dimitri Kourkoulis
 *     License: BSD 3-Clause License (see LICENSE file)
 */

#include "Car.hpp"
#include <string>
#include <small3d/WavefrontFile.hpp>

Car::Car() :
  SceneObject("car", "resources/models/car/car.obj") {
  WavefrontFile w("resources/models/car/wheel.obj");
  wheel = Model(w, "");
  this->colour = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
}

Car::~Car() {
  if (renderer) {
    renderer->clearBuffers(wheel);
    renderer->clearBuffers(static_cast<SceneObject &>(*this));
  }
}

void Car::render(Renderer *renderer) {
  this->renderer = renderer;
  renderer->render(*this, colour);

  renderer->render(this->wheel,
		   glm::vec3(this->offset.x + (this->rotation.y > 0.0f ? 1.4f : 1.9f),
			     this->offset.y,
			     this->offset.z + 1.0f),
		   this->rotation,
		   glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

  renderer->render(this->wheel,
		   glm::vec3(this->offset.x - (this->rotation.y > 0.0f ? 1.9f : 1.4f),
			     this->offset.y,
			     this->offset.z + 1.0f),
		   this->rotation,
		   glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		   
  
}
   
void Car::move() {
  this->offset.x += this->speed;
}
