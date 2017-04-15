#pragma once

// Network.h: defines the Network class


#include <map>
#include <vector>
using namespace std;

#include "Leg.h"
#include "Station.h"

class Network
{
public:
  Network();
  ~Network();
  void addStation(Station* station);
  void addLeg(string from, string to, size_t distance);
  pair<Leg*, int> getLeg(string from, string to);

private:
  map<string, Station*> stations_;
  vector<Leg*> legs_;
};
