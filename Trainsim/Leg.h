#pragma once

// Leg.h: defines the Leg class

#include "Station.h"

class Leg
{
public:
  Leg(Station* from, Station* to, int distance);
  ~Leg();
  Station* getFrom();
  Station* getTo();
  size_t getDistance();

private:
  Station* from_;
  Station* to_;
  int distance_;
};
