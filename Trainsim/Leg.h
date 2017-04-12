#pragma once

// Leg.h: defines the Leg class

#include "Station.h"

class Leg
{
public:
  Leg();
  ~Leg();

private:
  pair<Station, Station> fromTo_;
};
