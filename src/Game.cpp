/*
 * Game.cpp
 *
 * Created on: 12 Oct 2019
 *     Author: Dimitri Kourkoulis
 *    License: BSD 3-Clause License (see LICENSE file)
 */

#include "Game.hpp"
#include <small3d/Logger.hpp>
#include <cmath>
#include <small3d/BasePath.hpp>
#include <small3d/WavefrontFile.hpp>

#define CAMERA_ROTATION_SPEED 0.08f
#define CAMERA_SPEED 0.5f
#define ENEMY_SPEED 0.05f
#define TOUCH_DISTANCE 1.7f
#define SHOOT_DURATION 12

using namespace small3d;

Game::Game() {

  manRunning = new SceneObject("manRunning", "resources/anthropoid_run/anthropoid", 11, 0);

  manRunning->setFrameDelay(8);

  manRunning->offset = glm::vec3(1.0f, -1.0f, -3.0f);
  manRunning->startAnimating();

  gun = new SceneObject("gun", "resources/gun.obj");

  small3d::initLogger();

  renderer = &small3d::Renderer::getInstance("Gloom", 0, 0, 0.785f,
    1.0f, 60.0f, "resources/shaders/", 240);

  map.load(getBasePath() + "resources/map.txt");

  xMapSize = map.getXsize();
  yMapSize = map.getYsize();

  WavefrontFile w1("resources/cube.obj");
  cube = Model(w1, "");
  WavefrontFile w2("resources/plane.obj");
  plane = Model(w2, "");

  renderer->generateTexture("tileTexture", Image("resources/images/tile.png"));
  renderer->cameraPosition.y = -0.1f;

  Enemy enemy;

  enemy.sectorPosition = glm::ivec3(5, 0, 5);
  enemy.position = glm::vec3(1.0f, 0.0f, 1.0f);

  enemies.push_back(enemy);

  enemy.sectorPosition = glm::ivec3(5, 0, 7);
  enemy.position = glm::vec3(1.0f, 0.0f, 1.0f);
  enemies.push_back(enemy);

  enemy.sectorPosition = glm::ivec3(5, 0, 9);
  enemy.position = glm::vec3(1.0f, 0.0f, 1.0f);
  enemies.push_back(enemy);

  enemy.sectorPosition = glm::ivec3(10, 0, 10);
  enemy.position = glm::vec3(1.0f, 0.0f, 1.0f);
  enemies.push_back(enemy);

  enemy.sectorPosition = glm::ivec3(8, 0, 7);
  enemy.position = glm::vec3(1.0f, 0.0f, 1.0f);
  enemies.push_back(enemy);

  gunshot = Sound("resources/sounds/0438.ogg");

  renderer->createRectangle(titleRect, glm::vec3(-0.8f, 0.6f, -1.0f), 
    glm::vec3(0.8f, 0.0f, -1.0f));

  renderer->createRectangle(outcomeRect, glm::vec3(-0.6f, -0.2f, -1.0f),
    glm::vec3(0.6f, -0.4f, -1.0f));

  renderer->createRectangle(instructionsRect, glm::vec3(-0.8f, -0.5f, -1.0f), 
    glm::vec3(0.8f, -0.7f, -1.0f));

  renderer->generateTexture(MSG_TITLE, "GLOOM!", glm::vec3(1.0f, 0.0f, 0.0f));
  renderer->generateTexture(MSG_INSTRUCTIONS, "Press enter to play, esc to quit", 
    glm::vec3(1.0f, 0.0f, 0.0f));

}

Game::~Game() {
  delete manRunning;
  delete gun;
}

GLFWwindow* Game::getWindow() {
  return renderer->getWindow();
}

void Game::init() {

  playerSector = glm::ivec2(3, 3);

  for (std::vector<Enemy>::iterator enemy = enemies.begin(); enemy != enemies.end(); ++enemy) {
    enemy->dead = false;
    enemy->position = glm::vec3(1.0f, 0.0f, 1.0f);
  }
  enemies[0].sectorPosition = glm::ivec3(7, 0, 7);
  enemies[1].sectorPosition = glm::ivec3(4, 0, 8);
  enemies[2].sectorPosition = glm::ivec3(5, 0, 9);
  enemies[3].sectorPosition = glm::ivec3(3, 0, 10);
  enemies[4].sectorPosition = glm::ivec3(5, 0, 11);
  inMenu = false;
  died = false;
  won = false;
  numDead = 0;
}

void Game::terminate() {
  inMenu = true;
  if (won) {
    renderer->generateTexture(MSG_OUTCOME, "You won", glm::vec3(1.0f, 0.0f, 0.0f));
  }
  else if (died) {
    renderer->generateTexture(MSG_OUTCOME, "You died", glm::vec3(1.0f, 0.0f, 0.0f));
  }
}

void Game::process(const KeyInput& input) {

  if (!inMenu) {
    if (dieCount > 0) {
      renderer->rotateCamera(glm::vec3(0.0f, -1.0f, 0.0f));
      --dieCount;
      if (dieCount == 0) {
        died = true;
        terminate();
      }
    }
    else if (winCount > 0) {
      if (winCount > 0) {
        --winCount;
        if (winCount == 0) {
          won = true;
          terminate();
        }
      }
    }
    else {
      if (input.space && shootCount == 0) {
        shootCount = SHOOT_DURATION;
        gunshot.play();
      }


      if (input.left) {
        renderer->rotateCamera(glm::vec3(0.0f, CAMERA_ROTATION_SPEED, 0.0f));
      }
      else if (input.right) {
        renderer->rotateCamera(glm::vec3(0.0f, -CAMERA_ROTATION_SPEED, 0.0f));
      }

      if (input.up) {

        renderer->cameraPosition.x += renderer->getCameraOrientation().x * CAMERA_SPEED;
        renderer->cameraPosition.z += renderer->getCameraOrientation().z * CAMERA_SPEED;

      }
      else if (input.down) {
        renderer->cameraPosition.x -= renderer->getCameraOrientation().x * CAMERA_SPEED;
        renderer->cameraPosition.z -= renderer->getCameraOrientation().z * CAMERA_SPEED;
      }

      if (renderer->cameraPosition.x < -sectorRadius + 2.0f &&
        map.getLocation(playerSector.x - 1, playerSector.y) == '#') {
        renderer->cameraPosition.x = -sectorRadius + 2.0f;
      }
      else if (renderer->cameraPosition.x < -sectorRadius) {
        if (playerSector.x > 0) {
          --playerSector.x;
          renderer->cameraPosition.x = sectorRadius;
        }
        else {
          renderer->cameraPosition.x = -sectorRadius;
        }
      }

      if (renderer->cameraPosition.x > sectorRadius - 2.0f &&
        map.getLocation(playerSector.x + 1, playerSector.y) == '#') {
        renderer->cameraPosition.x = sectorRadius - 2.0f;
      }
      else if (renderer->cameraPosition.x > sectorRadius) {
        if (playerSector.x < xMapSize - 1) {
          ++playerSector.x;
          renderer->cameraPosition.x = -sectorRadius;
        }
        else {
          renderer->cameraPosition.x = sectorRadius;
        }
      }

      if (renderer->cameraPosition.z > sectorRadius - 2.0f &&
        map.getLocation(playerSector.x, playerSector.y + 1) == '#') {
        renderer->cameraPosition.z = sectorRadius - 2.0f;
      }
      else if (renderer->cameraPosition.z > sectorRadius) {
        if (playerSector.y < yMapSize - 1) {
          ++playerSector.y;
          renderer->cameraPosition.z = -sectorRadius;
        }
        else {
          renderer->cameraPosition.z = sectorRadius;
        }
      }

      if (renderer->cameraPosition.z < -sectorRadius + 2.0f &&
        map.getLocation(playerSector.x, playerSector.y - 1) == '#') {
        renderer->cameraPosition.z = -sectorRadius + 2.0f;
      }
      else if (renderer->cameraPosition.z < -sectorRadius) {
        if (playerSector.y > 0) {
          --playerSector.y;
          renderer->cameraPosition.z = sectorRadius;
        }
        else {
          renderer->cameraPosition.z = -sectorRadius;
        }
      }

      auto gunRotation = renderer->getCameraRotationXYZ();
      gunRotation.x += shootCount * 0.3f;
      gun->setRotation(gunRotation);
      
      gun->offset = renderer->cameraPosition;
      gun->offset.y -= 0.9f;
      gun->offset.x += renderer->getCameraOrientation().x * 1.8f;
      gun->offset.z += renderer->getCameraOrientation().z * 1.8f;

      bool killedOne = false;

      for (std::vector<Enemy>::iterator enemy = enemies.begin(); enemy != enemies.end(); ++enemy) {

        int diffxcoords = playerSector.x - enemy->sectorPosition.x;
        int diffycoords = playerSector.y - enemy->sectorPosition.z;

        if (!enemy->dead) {
          bool touchx = false, touchy = false;
          if (std::abs(diffxcoords) > 0) {
            enemy->position.x = enemy->position.x +
              ENEMY_SPEED * std::abs(diffxcoords) / diffxcoords;
            if (enemy->position.x < -enemySectorRadius) {
              if (map.getLocation(enemy->sectorPosition.x - 1, enemy->sectorPosition.z) == '#') {
                enemy->position.x = -enemySectorRadius;
              }
              else {
                --enemy->sectorPosition.x;
                enemy->position.x = enemySectorRadius;
              }
            }
            if (enemy->position.x > enemySectorRadius) {
              if (map.getLocation(enemy->sectorPosition.x + 1, enemy->sectorPosition.z) == '#') {
                enemy->position.x = enemySectorRadius;
              }
              else {
                ++enemy->sectorPosition.x;
                enemy->position.x = -enemySectorRadius;
              }
            }
          }
          else {
            float diffx = renderer->cameraPosition.x - enemy->position.x;
            if (std::abs(diffx) > TOUCH_DISTANCE) {
              enemy->position.x += ENEMY_SPEED * std::abs(diffx) / diffx;

            }
            else {
              touchx = true;
            }
          }

          if (std::abs(diffycoords) > 0) {
            enemy->position.z = enemy->position.z +
              ENEMY_SPEED * std::abs(diffycoords) / diffycoords;
            if (enemy->position.z < -enemySectorRadius) {
              if (map.getLocation(enemy->sectorPosition.x, enemy->sectorPosition.z - 1) == '#') {
                enemy->position.z = -enemySectorRadius;
              }
              else {
                --enemy->sectorPosition.z;
                enemy->position.z = enemySectorRadius;
              }


            }
            if (enemy->position.z > enemySectorRadius) {
              if (map.getLocation(enemy->sectorPosition.x, enemy->sectorPosition.z + 1) == '#') {
                enemy->position.z = enemySectorRadius;
              }
              else {
                ++enemy->sectorPosition.z;
                enemy->position.z = -enemySectorRadius;
              }

            }
          }
          else {
            float diffy = renderer->cameraPosition.z - enemy->position.z;
            if (std::abs(diffy) > TOUCH_DISTANCE) {
              enemy->position.z += ENEMY_SPEED * std::abs(diffy) / diffy;

            }
            else {
              touchy = true;
            }
          }
          if (touchx && touchy) {
            dieCount = 20;
          }
        }

        enemy->diffSectorX = playerSector.x - enemy->sectorPosition.x;
        enemy->diffSectorZ = playerSector.y - enemy->sectorPosition.z;

        enemy->inRange = pow(enemy->diffSectorX, 2) + pow(enemy->diffSectorZ, 2) < 2 * pow(localCoordRadius, 2);

        float distanceX = renderer->cameraPosition.x - enemy->position.x + enemy->diffSectorX * sectorLength;
        float distanceZ = renderer->cameraPosition.z - enemy->position.z + enemy->diffSectorZ * sectorLength;
        double distance = std::sqrt(std::pow(distanceX, 2) + std::pow(distanceZ, 2));

        glm::vec3 normVecToPlayer(distanceZ / distance, distanceX / distance, 0.0f);
        glm::vec3 normCamVec(cos(renderer->getCameraRotationXYZ().y), sin(renderer->getCameraRotationXYZ().y), 0.0f);
        enemy->dotp = normVecToPlayer.x * normCamVec.x + normVecToPlayer.y * normCamVec.y;

        if (!enemy->dead && enemy->dotp > 0.992f && shootCount == SHOOT_DURATION && !killedOne) {
          enemy->dead = true;
          killedOne = true;
          ++numDead;
          if (numDead == 5) {
            winCount = 20;
          }
        }
      }

      if (shootCount) {
        --shootCount;
      }
    }
  }
  else {
    if (input.enter) {
      init();
    }
  }
}

void Game::renderEnv() {

  const char* region = map.getRegion(playerSector.x, playerSector.y, localCoordRadius);

  int length = 2 * localCoordRadius + 1;

  for (int y = 0; y < length; ++y) {
    for (int x = 0; x < length; ++x) {


      switch (region[y * length + x]) {
      case '#':

        renderer->render(cube, glm::vec3(-54.0f + x * 12.0f, -1.2f, -42.0f + y * 12.0f),
          glm::vec3(0.0f, 0.0f, 0.0f), "tileTexture");
        break;
      default:
        renderer->render(plane, glm::vec3(-54.0f + x * 12.0f, -1.2f, -42.0f + y * 12.0f),
          glm::vec3(0.0f, 0.0f, 0.0f), "tileTexture");
        renderer->render(plane, glm::vec3(-54.0f + x * 12.0f, 2.8f, -42.0f + y * 12.0f),
          glm::vec3(0.0f, 0.0f, 0.0f), "tileTexture");

        break;
      }
    }
  }
}

void Game::render() {
  renderer->setBackgroundColour(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
  if (!inMenu) {

    renderEnv();
    renderer->render(*gun, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

    for (std::vector<Enemy>::iterator enemy = enemies.begin(); enemy != enemies.end(); ++enemy) {
      if (enemy->inRange) {
        manRunning->offset.x = -enemy->diffSectorX * sectorLength + enemy->position.x;
        manRunning->offset.z = -enemy->diffSectorZ * sectorLength + enemy->position.z;

        int ycoeff = 0;

        if (enemy->diffSectorX < 0 || (enemy->diffSectorX == 0 && renderer->cameraPosition.x - enemy->position.x < 0)) {
          manRunning->setRotation(glm::vec3(0.0f, -1.7f, 0.0f));
          if (std::abs(enemy->diffSectorX) <= std::abs(enemy->diffSectorZ)) {
            ycoeff = -1;
          }
        }
        else {
          manRunning->setRotation(glm::vec3(0.0f, 1.7f, 0.0f));
          if (std::abs(enemy->diffSectorX) <= std::abs(enemy->diffSectorZ)) {
            ycoeff = 1;
          }
        }

        if (enemy->diffSectorZ < 0 || (enemy->diffSectorZ == 0 && renderer->cameraPosition.z - enemy->position.z < 0)) {
          manRunning->rotate(glm::vec3(0.0f, ycoeff * 0.85f, 0.0f));
        }
        else {
          manRunning->rotate(glm::vec3(0.0f, -ycoeff * 0.85f, 0.0f));
        }

        if (enemy->dead) {
          manRunning->setRotation(glm::vec3(-1.75f, 0.0f, 0.0f));
          manRunning->offset.y = -0.9f;
          
        }
        else {

          auto manRotation = manRunning->getRotationXYZ();
          manRotation.x = 0.0f;

          manRunning->setRotation(manRotation);
          
          manRunning->animate();
        }
        renderer->render(*manRunning, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
      }
    }
  }
  else {
    renderer->setBackgroundColour(glm::vec4(0.5f, 0.0f, 0.0f, 1.0f));
    renderer->render(titleRect, MSG_TITLE, false);
    
    if (won || died) {
      renderer->render(outcomeRect, MSG_OUTCOME, false);
    }

    renderer->render(instructionsRect, MSG_INSTRUCTIONS, false);

  }
  renderer->swapBuffers();

}
