// Train.cpp defines the methods of the Train class

#include "Route.h"
#include "Train.h"

Train::Train(string name, Route* route, int speed):collided_(false)
{
  name_ = name;
  route_ = route;
  speed_ = speed;
  int fraction = 0;
  position_ = make_pair(route_->GetBegin().first, route_->GetBegin().second);
}

Train::~Train()
{
}

vector<Train*>::iterator Train::drive(vector<Train*>* trains)
{
  // If the train hasn't collided and hasn't reached its destination yet, move it's position by the speed:
  for (int iStep = 0; (iStep < speed_ && !collided_ && !(position_ == route_->GetEnd())); iStep++)
  {

    // Have we not yet reached the end of the route?
    if (!(position_ == route_->GetEnd()))
    {
      int direction = route_->GetDirection(position_.first);

      bool proceedToNextLeg = false;
      if ((direction == 1 && position_.second >= route_->GetDistance(position_.first)-1) || // Default
          (direction == -1 && position_.second <= 1)) // Reversed
      {

        // Proceed to next leg if there is one:
        proceedToNextLeg = (route_->GetLeg(position_.first + 1) != nullptr);
      }

      if (proceedToNextLeg)
      {
        position_.first++;
        if (route_->GetDirection(position_.first) == 1) // Default
        {
          position_.second = 0;
        }
        else // Reversed
        {
          position_.second = route_->GetDistance(position_.first);
        }
      }
      else // Move train on same leg
      {
        position_.second = position_.second + direction;
      }
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
