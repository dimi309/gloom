/*
 * Enemy.hpp
 *
 * Created on: 14 Oct 2019
 *     Author: Dimitri Kourkoulis
 *    License: BSD 3-Clause License (see LICENSE file)
 */
#pragma once

#define GLM_FORCE_RADIANS
#include <small3d/SceneObject.hpp>
#include <small3d/Math.hpp>

class Enemy : public small3d::SceneObject {

public:
  small3d::Vec3i sectorPosition;
  small3d::Vec3 worldPosition;
  small3d::Vec3 rotation;
  float dotp = 0.0f;
  int diffSectorX = 0;
  int diffSectorZ = 0;
  bool inRange = false;
  bool dead = false;
  Enemy(SceneObject &sceneObject);

};


