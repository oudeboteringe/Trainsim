// Train.cpp defines the methods of the Train class

#include "Route.h"
#include "Train.h"

Train::Train(string name, Route* route, int speed)
{
  name_ = name;
  route_ = route;
  speed_ = speed;
  position_ = make_pair(route_->GetBegin().first, 0);
}

Train::~Train()
{
}

vector<Train*>::iterator Train::drive(vector<Train*>* trains)
{
  // Move the position of the train by the speed:
  for (int iStep = 0; iStep < speed_; iStep++)
  {
    Leg* currentLeg = position_.first;
    int direction = route_->GetDirection(currentLeg);
    position_.second = position_.second + direction;

    // Have we reached the end of the leg?
    // TODO
  }
  return trains->begin(); // TODO: return iterator to ourself (default) or to conflicting train (collision).
}

string Train::getName()
{
  return name_;
}

pair<Leg*, int>* Train::getPosition()
{
  return &position_;
}
