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
  string GetName();
  pair<size_t, size_t> GetPosition();
  Route* GetRoute();
  bool GetCollided();

private:
  string name_;
  Route* route_;
  pair<size_t, size_t> position_; // Leg index and the distance from the From station of the leg
  int speed_;
  bool collided_;
};
