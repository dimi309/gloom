/*
 * main.cpp
 *
 * Created on: 12 Oct 2019
 *     Author: Dimitri Kourkoulis
 *    License: BSD 3-Clause License (see LICENSE file)
 */

#include "KeyInput.hpp"
#include "Game.hpp"

KeyInput input;

bool downkey, leftkey, rightkey, upkey, esckey, enterkey;

const uint32_t framerate = 60;

void keyCallback(GLFWwindow* window, int key, int scancode, int action,
		 int mods) {
  if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
    input.down = true;
  if (key == GLFW_KEY_UP && action == GLFW_PRESS)
    input.up = true;
  if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
    input.left = true;
  if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
    input.right = true;
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    input.esc = true;
  if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
    input.enter = true;
  if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    input.space = true;
  if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE)
    input.down = false;
  if (key == GLFW_KEY_UP && action == GLFW_RELEASE)
    input.up = false;
  if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
    input.left = false;
  if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
    input.right = false;
  if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
    input.esc = false;
  if (key == GLFW_KEY_ENTER && action == GLFW_RELEASE)
    input.enter = false;
  if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
    input.space = false;
  
}

int main(int argc, char** argv) {

  Game game;
  
  GLFWwindow *window = game.getWindow();

  glfwSetKeyCallback(window, keyCallback);
  
  double seconds = glfwGetTime();
  double prevSeconds = seconds;
  double secondsInterval = 1.0 / framerate;
  
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    
    seconds = glfwGetTime();
    if (seconds - prevSeconds > secondsInterval) {
      prevSeconds = seconds;
      game.process(input);
      game.render();
      
      if (input.esc) {
        break;
      }
    }
  }
  
  return 0;
}
