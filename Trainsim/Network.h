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
  void AddStation(Station* station);
  void AddLeg(string from, string to, size_t distance);
  pair<Leg*, int> GetLeg(string from, string to);

private:
  map<string, Station*> stations_;
  vector<Leg*> legs_;
};
