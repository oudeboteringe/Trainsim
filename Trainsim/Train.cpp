// Train.cpp defines the methods of the Train class

#include "Route.h"
#include "Train.h"

Train::Train(string name, Route* route, int speed):collided_(false)
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
  // If the train hasn't collided and hasn't reached its destination yet, move it's position by the speed:
  for (int iStep = 0; (iStep < speed_ && !collided_ && !(position_ == route_->getEnd())); iStep++)
  {

    // Have we not yet reached the end of the route?
    if (!(position_ == route_->getEnd()))
    {

      if (position_.second >= route_->getDistance(position_.first)) // Proceed to next leg:
      {
        position_.first++;
        if (route_->getDirection(position_.first) == 1) // Default
        {
          position_.second = 0;
        }
        else // Reversed traversal
        {
          position_.second = route_->getDistance(position_.first);
        }
      }

      // Move train
      int direction = route_->getDirection(position_.first);
      position_.second = position_.second + direction;
    }

    // TODO: Have we collided?
  }
  return trains->begin(); // TODO: return iterator to ourself (default) or to conflicting train (collision).
}

string Train::getName()
{
  return name_;
}

pair<size_t, size_t> Train::getPosition()
{
  return position_;
}

Route * Train::getRoute()
{
  return route_;
}
