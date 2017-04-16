#pragma once

// Leg.h: defines the Leg class

#include "Station.h"

class Leg
{
public:
  Leg(Station* from, Station* to, size_t distance);
  ~Leg();
  Station* GetFrom();
  Station* GetTo();
  size_t GetDistance();

private:
  Station* from_;
  Station* to_;
  size_t distance_;
};
