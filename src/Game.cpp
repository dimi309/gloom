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
#include <small3d/GlbFile.hpp>

#define CAMERA_ROTATION_SPEED 0.08f
#define CAMERA_SPEED 0.5f
#define ENEMY_SPEED 0.1f
#define ENEMY_FRAME_DELAY 4
#define TOUCH_DISTANCE 1.7f
#define SHOOT_DURATION 12
#define ENEMY_Y_POS -1.0f
#define ENEMY_Y_DEAD_POS -0.7f

using namespace small3d;

Game::Game() {

  small3d::initLogger();

  gun = std::make_unique<SceneObject>("gun", Model(GlbFile("resources/gun.glb")));

#if !defined(NDEBUG) 
  renderer = &small3d::Renderer::getInstance("Gloom", 1024, 768, 0.785f,
    1.0f, 60.0f, "resources/shaders/", 240);

#else
  renderer = &small3d::Renderer::getInstance("Gloom", 0, 0, 0.785f,
    1.0f, 60.0f, "resources/shaders/", 240);
#endif

  renderer->shadowsActive = true;
  renderer->shadowCamTransformation = glm::rotate(glm::mat4x4(1.0f), 1.07f, glm::vec3(1.0f, 0.0f, 0.0f)) *
    glm::translate(glm::mat4x4(1.0f), glm::vec3(0.0f, -10.0f, 1.0f));

  map.load(getBasePath() + "resources/map.txt");

  xMapSize = map.getXsize();
  yMapSize = map.getYsize();

  cube = Model(GlbFile("resources/cube.glb"));
  cube.noShadow = true;
  plane = Model(GlbFile("resources/plane.glb"));
  plane.noShadow = true;

  renderer->generateTexture("tileTexture", Image("resources/images/tile.png"));
  renderer->cameraPosition.y = -0.1f;

  SceneObject sobj("enemy", Model(GlbFile("resources/anthropoid.glb")));

  Enemy enemy(sobj);

  enemy.setFrameDelay(ENEMY_FRAME_DELAY);
  enemy.sectorPosition = glm::ivec3(5, 0, 5);
  enemy.worldPosition = glm::vec3(1.0f, ENEMY_Y_POS, 1.0f);
  enemy.startAnimating();

  enemies.push_back(enemy);

  enemy.sectorPosition = glm::ivec3(5, 0, 7);
  enemy.worldPosition = glm::vec3(1.0f, ENEMY_Y_POS, 1.0f);
  enemies.push_back(enemy);

  enemy.sectorPosition = glm::ivec3(5, 0, 9);
  enemy.worldPosition = glm::vec3(1.0f, ENEMY_Y_POS, 1.0f);
  enemies.push_back(enemy);

  enemy.sectorPosition = glm::ivec3(10, 0, 10);
  enemy.worldPosition = glm::vec3(1.0f, ENEMY_Y_POS, 1.0f);
  enemies.push_back(enemy);

  enemy.sectorPosition = glm::ivec3(8, 0, 7);
  enemy.worldPosition = glm::vec3(1.0f, ENEMY_Y_POS, 1.0f);
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

GLFWwindow* Game::getWindow() {
  return renderer->getWindow();
}

void Game::init() {

  playerSector = glm::ivec2(3, 3);

  for (auto &enemy: enemies) {
    enemy.dead = false;
    enemy.worldPosition = glm::vec3(1.0f, ENEMY_Y_POS, 1.0f);
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

void Game::process(KeyInput& input) {

#if _WIN32
  if (input.prtscr) {
    renderer->screenCapture = true;
    input.prtscr = false;
  }
#endif

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
        gunshot.stop();
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

      gun->position = renderer->cameraPosition;
      gun->position.y -= 0.9f;
      gun->position.x += renderer->getCameraOrientation().x * 1.8f;
      gun->position.z += renderer->getCameraOrientation().z * 1.8f;

      bool killedOne = false;

      for (auto &enemy : enemies) {

        int diffxcoords = playerSector.x - enemy.sectorPosition.x;
        int diffycoords = playerSector.y - enemy.sectorPosition.z;

        if (!enemy.dead) {
          bool touchx = false, touchy = false;
          if (std::abs(diffxcoords) > 0) {
            enemy.worldPosition.x = enemy.worldPosition.x +
              ENEMY_SPEED * std::abs(diffxcoords) / diffxcoords;
            if (enemy.worldPosition.x < -enemySectorRadius) {
              if (map.getLocation(enemy.sectorPosition.x - 1, enemy.sectorPosition.z) == '#') {
                enemy.worldPosition.x = -enemySectorRadius;
              }
              else {
                --enemy.sectorPosition.x;
                enemy.worldPosition.x = enemySectorRadius;
              }
            }
            if (enemy.worldPosition.x > enemySectorRadius) {
              if (map.getLocation(enemy.sectorPosition.x + 1, enemy.sectorPosition.z) == '#') {
                enemy.worldPosition.x = enemySectorRadius;
              }
              else {
                ++enemy.sectorPosition.x;
                enemy.worldPosition.x = -enemySectorRadius;
              }
            }
          }
          else {
            float diffx = renderer->cameraPosition.x - enemy.worldPosition.x;
            if (std::abs(diffx) > TOUCH_DISTANCE) {
              enemy.worldPosition.x += ENEMY_SPEED * std::abs(diffx) / diffx;

            }
            else {
              touchx = true;
            }
          }

          if (std::abs(diffycoords) > 0) {
            enemy.worldPosition.z = enemy.worldPosition.z +
              ENEMY_SPEED * std::abs(diffycoords) / diffycoords;
            if (enemy.worldPosition.z < -enemySectorRadius) {
              if (map.getLocation(enemy.sectorPosition.x, enemy.sectorPosition.z - 1) == '#') {
                enemy.worldPosition.z = -enemySectorRadius;
              }
              else {
                --enemy.sectorPosition.z;
                enemy.worldPosition.z = enemySectorRadius;
              }


            }
            if (enemy.worldPosition.z > enemySectorRadius) {
              if (map.getLocation(enemy.sectorPosition.x, enemy.sectorPosition.z + 1) == '#') {
                enemy.worldPosition.z = enemySectorRadius;
              }
              else {
                ++enemy.sectorPosition.z;
                enemy.worldPosition.z = -enemySectorRadius;
              }

            }
          }
          else {
            float diffy = renderer->cameraPosition.z - enemy.worldPosition.z;
            if (std::abs(diffy) > TOUCH_DISTANCE) {
              enemy.worldPosition.z += ENEMY_SPEED * std::abs(diffy) / diffy;

            }
            else {
              touchy = true;
            }
          }
          if (touchx && touchy) {
            dieCount = 20;
          }
        }

        enemy.diffSectorX = playerSector.x - enemy.sectorPosition.x;
        enemy.diffSectorZ = playerSector.y - enemy.sectorPosition.z;

        enemy.inRange = pow(enemy.diffSectorX, 2) + pow(enemy.diffSectorZ, 2) < 2 * pow(localCoordRadius, 2);

        float distanceX = renderer->cameraPosition.x - enemy.worldPosition.x + enemy.diffSectorX * sectorLength;
        float distanceZ = renderer->cameraPosition.z - enemy.worldPosition.z + enemy.diffSectorZ * sectorLength;
        double distance = std::sqrt(std::pow(distanceX, 2) + std::pow(distanceZ, 2));

        glm::vec3 normVecToPlayer(distanceZ / distance, distanceX / distance, 0.0f);
        glm::vec3 normCamVec(cos(renderer->getCameraRotationXYZ().y), sin(renderer->getCameraRotationXYZ().y), 0.0f);
        enemy.dotp = normVecToPlayer.x * normCamVec.x + normVecToPlayer.y * normCamVec.y;

        if (!enemy.dead && enemy.dotp > 0.992f && shootCount == SHOOT_DURATION && !killedOne) {
          enemy.dead = true;
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

    for (auto &enemy : enemies) {
      if (enemy.inRange) {
        enemy.position.x = -enemy.diffSectorX * sectorLength + enemy.worldPosition.x;
        enemy.position.z = -enemy.diffSectorZ * sectorLength + enemy.worldPosition.z;
        enemy.position.y = ENEMY_Y_POS;
        int ycoeff = 0;

        if (enemy.diffSectorX < 0 || (enemy.diffSectorX == 0 && renderer->cameraPosition.x - enemy.worldPosition.x < 0)) {
          enemy.setRotation(glm::vec3(0.0f, -1.7f, 0.0f));
          if (std::abs(enemy.diffSectorX) <= std::abs(enemy.diffSectorZ)) {
            ycoeff = -1;
          }
        }
        else {
          enemy.setRotation(glm::vec3(0.0f, 1.7f, 0.0f));
          if (std::abs(enemy.diffSectorX) <= std::abs(enemy.diffSectorZ)) {
            ycoeff = 1;
          }
        }

        if (enemy.diffSectorZ < 0 || (enemy.diffSectorZ == 0 && renderer->cameraPosition.z - enemy.worldPosition.z < 0)) {
          enemy.rotate(glm::vec3(0.0f, ycoeff * 0.85f, 0.0f));
        }
        else {
          enemy.rotate(glm::vec3(0.0f, -ycoeff * 0.85f, 0.0f));
        }

        if (enemy.dead) {
          auto pos = enemy.position;
          pos.y = ENEMY_Y_DEAD_POS;
          renderer->render(enemy.getModel(), pos, glm::vec3(-1.75f, 0.0f, 0.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
        }
        else {

          enemy.animate();
          renderer->render(enemy, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
        }

      }
    }
  }
  else {
    renderer->setBackgroundColour(glm::vec4(0.5f, 0.0f, 0.0f, 1.0f));
    renderer->render(titleRect, MSG_TITLE, 0, false);

    if (won || died) {
      renderer->render(outcomeRect, MSG_OUTCOME, 0, false);
    }

    renderer->render(instructionsRect, MSG_INSTRUCTIONS, 0, false);

  }
  renderer->swapBuffers();

}
