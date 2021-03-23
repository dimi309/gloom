/*
 *  GameLogic.cpp
 *
 *  Created on: 2014/11/09
 *      Author: Dimitri Kourkoulis
 *     License: BSD 3-Clause License (see LICENSE file)
 */


#define MAX_Z -1.0f
#define MIN_Z -24.0f
#define MAX_X 24.0f
#define MIN_X -24.0f

#define GROUND_Y -1.0f
#define FULL_ROTATION 6.28f // More or less 360 degrees in radians

#define BUG_TILT_SPEED 0.03f
#define BUG_ROTATION_SPEED 0.06f
#define BUG_SPEED 0.08f
#define BUG_START_ALTITUDE 2.6f

#define GOAT_ROTATION_SPEED 0.1f
#define GOAT_SPEED 0.05f

#define RESULT_TEXT_NAME "resutlText"

#include <memory>
#include <stdexcept>
#include "GameLogic.hpp"


using namespace small3d;

namespace ChaseTheGoat3D {

  GameLogic::GameLogic() :
    goat("goat", "resources/models/Goat/goatAnim",
	 19, 1, 3U),
    bug("bug", "resources/models/Bug/bugAnim", 9),
    bahSound("resources/sounds/bah.ogg") {

    renderer = &Renderer::getInstance("Chase the Goat 3D", 0, 0, 1.2f);

    renderer->createRectangle(startScreenRect, glm::vec3(-1.0f, 1.0f, 1.0f),
      glm::vec3(1.0f, -1.0f, 1.0f));
    renderer->createRectangle(msgRect,
      glm::vec3(-0.95f, -0.6f, -0.5f), glm::vec3(0.0f, -0.8f, -0.5f));
    renderer->createRectangle(skyRect, glm::vec3(-1.0f, 1.0f, 1.0f),
      glm::vec3(1.0f, -1.0f, 1.0f));
    renderer->createRectangle(groundRect, glm::vec3(-25.0f, GROUND_Y, MIN_Z),
      glm::vec3(25.0f, GROUND_Y, MAX_Z));

    Image startScreenTexture("resources/images/startScreen.png");
    renderer->generateTexture("startScreen", startScreenTexture);

    Image groundTexture("resources/images/grass.png");
    renderer->generateTexture("ground", groundTexture);

    Image goatTexture("resources/models/Goat/goat.png");
    renderer->generateTexture("goatTexture", goatTexture);

    bug.setFrameDelay(2);

    gameState = START_SCREEN;

    seconds = 0;

    lightModifier = -0.01f;

    goatState = WALKING_STRAIGHT;
  }

  GameLogic::~GameLogic() {
    renderer->clearBuffers(goat);
    renderer->clearBuffers(bug);
  }

  void GameLogic::initGame() {
    goat.offset = glm::vec3(-1.2f, GROUND_Y, -4.0f);
    bug.offset = glm::vec3(3.6f, GROUND_Y + BUG_START_ALTITUDE, -5.0f);
    bug.rotation = glm::vec3(0.0f, 0.0f, 0.0f);


    bug.startAnimating();
    goat.startAnimating();


    startSeconds = glfwGetTime();

  }

  void GameLogic::moveGoat() {
    goatState = TURNING;

    float xDistance = bug.offset.x - goat.offset.x;
    float zDistance = bug.offset.z - goat.offset.z;
    float distance = sqrt(xDistance * xDistance + zDistance * zDistance);

    float goatRelX = xDistance / distance;
    float goatRelZ = zDistance / distance;

    float goatDirectionX = sin(goat.rotation.y);
    float goatDirectionZ = cos(goat.rotation.y);

    float dotPosDir = goatRelX * goatDirectionX + goatRelZ * goatDirectionZ; // dot product

    if (dotPosDir > 0.98f) {
      goatState = TURNING;
    }
    else
      goatState = WALKING_STRAIGHT;

    if (goatState == TURNING) {
      goat.rotation.y += GOAT_ROTATION_SPEED;

    }

    if (goat.offset.z > MAX_Z) {
      goat.offset.z = MAX_Z;
      goatState = TURNING;
    }
    if (goat.offset.z < MIN_Z) {
      goat.offset.z = MIN_Z;
      goatState = TURNING;
    }
    if (goat.offset.x > MAX_X) {
      goat.offset.x = MAX_X;
      goatState = TURNING;
    }
    if (goat.offset.x < MIN_X) {
      goat.offset.x = MIN_X;
      goatState = TURNING;
    }

    goat.offset.x -= sin(goat.rotation.y) * GOAT_SPEED;
    goat.offset.z -= cos(goat.rotation.y) * GOAT_SPEED;
    goat.offset.y += sin(goat.rotation.x) * GOAT_SPEED;

    goat.animate();

  }

  void GameLogic::moveBug(const KeyInput& keyInput) {

    if (keyInput.left) {
      bug.rotation.y += BUG_ROTATION_SPEED;

      if (bug.rotation.y > FULL_ROTATION)
        bug.rotation.y = 0.0f;


    }
    else if (keyInput.right) {
      bug.rotation.y -= BUG_ROTATION_SPEED;

      if (bug.rotation.y < -FULL_ROTATION)
        bug.rotation.y = 0.0f;
    }

    if (keyInput.down) {

      bug.rotation.x += BUG_TILT_SPEED;

      if (bug.rotation.x > 0.75f)
        bug.rotation.x = 0.75f;


    }
    else if (keyInput.up) {

      bug.rotation.x -= BUG_TILT_SPEED;

      if (bug.rotation.x < -0.75f)
        bug.rotation.x = -0.75f;
    }

    if (keyInput.space) {
      bug.offset.x -= sin(bug.rotation.y) * BUG_SPEED;
      bug.offset.z -= cos(bug.rotation.y) * BUG_SPEED;
      bug.offset.y += sin(bug.rotation.x) * BUG_SPEED;
    }

    if (bug.offset.y < GROUND_Y + 0.5f)
      bug.offset.y = GROUND_Y + 0.5f;

    if (bug.offset.z > MAX_Z)
      bug.offset.z = MAX_Z;
    if (bug.offset.z < MIN_Z)
      bug.offset.z = MIN_Z;
    if (bug.offset.x > MAX_X)
      bug.offset.x = MAX_X;
    if (bug.offset.x < MIN_X)
      bug.offset.x = MIN_X;


    // Bug chase camera
    renderer->cameraPosition = bug.offset;
    renderer->cameraPosition.x += sin(bug.rotation.y) * 1.7f;
    renderer->cameraPosition.z += cos(bug.rotation.y) * 1.7f;
    renderer->cameraPosition.y -= sin(bug.rotation.x) * 1.7f;
    renderer->cameraRotation = bug.rotation;
    if (renderer->cameraPosition.y < GROUND_Y + 1.0f)
      renderer->cameraPosition.y = GROUND_Y + 1.0f;

    bug.animate();

    if (goat.contains(bug.offset)) {
      gameState = START_SCREEN;
      bahSound.play();
      seconds = (glfwGetTime() - startSeconds);
      renderer->generateTexture(RESULT_TEXT_NAME, "Goat not bitten for " + std::to_string(seconds) +
        " seconds", glm::vec3(0.5f, 1.0f, 0.0f));
    }
  }

  void GameLogic::processGame(const KeyInput& keyInput) {
    moveBug(keyInput);
    moveGoat();
  }

  void GameLogic::processStartScreen(const KeyInput& keyInput) {
    if (keyInput.enter) {
      initGame();
      gameState = PLAYING;
    }
  }

  void GameLogic::process(const KeyInput& keyInput) {
    switch (gameState) {
    case START_SCREEN:
      processStartScreen(keyInput);
      break;
    case PLAYING:
      processGame(keyInput);
      break;
    default:
      throw std::runtime_error("Urecognised game state");
    }
  }

  void GameLogic::render() {
    renderer->clearScreen(glm::vec4(0.4f, 0.37f, 1.0f, 1.0f));

    if (gameState == START_SCREEN) {

      renderer->render(startScreenRect, glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(0.0f, 0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), "startScreen", false);

      if (seconds != 0) {
        renderer->render(msgRect, RESULT_TEXT_NAME, false);
      }

    }
    else {

      // Draw the background

      renderer->render(groundRect, "ground", true);

      renderer->render(goat, "goatTexture");
      
      renderer->render(bug, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

    }
    renderer->swapBuffers();
  }

}
