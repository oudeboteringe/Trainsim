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
  void AddLeg(Leg* leg, int direction); // Add a leg plus direction on that leg to a route
  int GetDirection(Leg* leg);
  pair<Leg*, int> GetBegin();

private:
  // The route consists of a number of legs.
  // Legs are represented by the first element of the <Leg*, int> pair and
  // are to be traversed in a default (+1) or reversed (-1) direction.
  // The direction is represented by the second element of the <Leg*, int> pair:
  vector<pair<Leg*, int> > legs_;
};
