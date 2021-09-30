/*
 * Map.hpp
 *
 * Created on: 12 Oct 2019
 *     Author: Dimitri Kourkoulis
 *    License: BSD 3-Clause License (see LICENSE file)
 */

#pragma once

#include <vector>
#include <string>

class Map {

private:
  std::vector<std::string> mapData;
  char *region;
  const uint32_t maxRegionRadius = 10;
  int xsize = 0, ysize = 0;
  
public:
  Map();
  ~Map();
  int getXsize();
  int getYsize();
  void load(std::string filepath);
  const char* getRegion(int coordx, int coordy, uint32_t radius);
  char getLocation(int coordx, int coordy);
};
