#pragma once

// Route.h: defines the Route class

#include <vector>
using namespace std;

#include "Leg.h"

class Route
{
public:
  Route();
  ~Route();
  void AddLeg(Leg* leg, int direction); // Add a leg (plus direction) on that leg to a route
  Leg* getLeg(size_t legIndex);
  int getDirection(size_t legIndex);
  size_t getDistance(size_t legIndex);
  pair<size_t, size_t> GetBegin();
  pair<size_t, size_t> getEnd();

private:
  // The route consists of a number of legs.
  // Legs are represented by the first element of the <size_t, size_t> pair and
  // are to be traversed in a default (+1) or reversed (-1) direction.
  // The direction is represented by the second element of the <size_t, size_t> pair:
  vector<pair<Leg*, int> > legs_;
};
