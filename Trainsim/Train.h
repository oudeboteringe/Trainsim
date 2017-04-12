#pragma once

// Train.h: defines the Train class

#include <utility>
#include <vector>
using namespace std;

#include "Leg.h"

class Train
{
public:
  Train();
  ~Train();

private:
  string name_;
  pair<Leg, int> position_;
  vector<Station> route_;
};
