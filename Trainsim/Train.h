#pragma once

// Train.h: defines the Train class

#include <utility>
#include <vector>
using namespace std;

#include "Leg.h"

class Train
{
public:
  Train(string name, Route* route, int speed);
  ~Train();
  vector<Train*>::iterator Drive(vector<Train*>* trains);

private:
  string name_;
  Route* route_;
  pair<Leg*, int> position_; // Leg (From and To station) and the distance from the From station of the leg
  int speed_;
};
