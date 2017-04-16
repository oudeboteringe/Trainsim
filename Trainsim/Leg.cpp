#include "Leg.h"

// Leg.cpp defines the methods of the Leg class

Leg::Leg(Station* from, Station* to, size_t distance):from_(from), to_(to), distance_(distance)
{
}

Leg::~Leg()
{
}

Station * Leg::GetFrom()
{
  return from_;
}

Station * Leg::GetTo()
{
  return to_;
}

size_t Leg::GetDistance()
{
  return distance_;
}
