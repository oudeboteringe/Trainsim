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
  vector<Train*>::iterator drive(vector<Train*>* trains);
  string getName();
  pair<Leg*, int>* getPosition();

private:
  string name_;
  Route* route_;
  pair<Leg*, int> position_; // Leg (From and To station) and the distance from the From station of the leg
  int speed_;
};
