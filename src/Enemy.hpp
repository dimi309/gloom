/*
 * Enemy.hpp
 *
 * Created on: 14 Oct 2019
 *     Author: Dimitri Kourkoulis
 *    License: BSD 3-Clause License (see LICENSE file)
 */
#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

class Enemy {

public:
  glm::ivec3 sectorPosition = glm::ivec3(0, 0, 0);
  glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
  float dotp = 0.0f;
  int diffSectorX = 0;
  int diffSectorZ = 0;
  bool inRange = false;
  bool dead = false;
  Enemy();


};


