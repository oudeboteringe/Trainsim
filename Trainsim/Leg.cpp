#include "Leg.h"

// Leg.cpp defines the methods of the Leg class

Leg::Leg(Station* from, Station* to, size_t distance):from_(from), to_(to), distance_(distance)
{
}

Leg::~Leg()
{
}

Station * Leg::getFrom()
{
  return from_;
}

Station * Leg::getTo()
{
  return to_;
}

size_t Leg::getDistance()
{
  return distance_;
}
