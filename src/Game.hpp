/*
 * Game.hpp
 *
 * Created on: 12 Oct 2019
 *     Author: Dimitri Kourkoulis
 *    License: BSD 3-Clause License (see LICENSE file)
 */
#pragma once

#include "Map.hpp"
#include <small3d/Renderer.hpp>
#include <small3d/Model.hpp>
#include "KeyInput.hpp"
#include <small3d/SceneObject.hpp>
#include "Enemy.hpp"
#include <vector>
#include <small3d/Sound.hpp>

class Game {

private:

  small3d::Renderer *renderer;
  Map map;
  small3d::Model cube;
  small3d::Model plane;
  const int localCoordRadius = 4;
  const float sectorRadius = 6.0f;
  const float sectorLength = 2 * sectorRadius;
  const float enemySectorRadius = 6.0f;
  std::string fontFile = "resources/fonts/CrusoeText/CrusoeText-Regular.ttf";

  int shootCount = 0;
  
  int xMapSize;
  int yMapSize;

  glm::ivec2 playerSector;
  
  small3d::SceneObject *manRunning;
  small3d::SceneObject *gun;

  small3d::Sound gunshot;
  
  void renderEnv();
  std::vector<Enemy> enemies;
  
  bool died = false;
  bool won = false;
  
  bool inMenu = true;
  
  int numDead = 0;
  
  int dieCount = 0;
  int winCount = 0;
  
  small3d::Model titleRect, outcomeRect, instructionsRect;

  const std::string MSG_TITLE = "titleTexture";
  const std::string MSG_OUTCOME = "outcomeTexture";
  const std::string MSG_INSTRUCTIONS = "instructionsTexture";

public:
  Game();
  ~Game();
  GLFWwindow* getWindow();
  void init();
  void terminate();
  void process(const KeyInput &input);
  void render();
};
