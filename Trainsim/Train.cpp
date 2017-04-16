// Train.cpp defines the methods of the Train class

#include "Route.h"
#include "Train.h"

Train::Train(string name, Route* route, int speed):collidedWith_(nullptr)
{
  name_ = name;
  route_ = route;
  speed_ = speed;
  int fraction = 0;
  position_ = make_pair(route_->GetBegin().first, route_->GetBegin().second);
}

Train::~Train()
{
  delete route_;
}

void Train::Drive(vector<Train*>* trains)
{
  // If the train hasn't collided and hasn't reached its destination yet, move it's position by the speed:
  for (int iStep = 0; (iStep < speed_ && (collidedWith_ == nullptr) && !(position_ == route_->GetEnd())); iStep++)
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

    // Any other trains on our leg?
    for (vector<Train*>::iterator itTrains = trains->begin(); itTrains != trains->end(); itTrains++)
    {
      if (*(itTrains) != this) // Other train
      {

        // On what leg is the other train?
        pair<size_t, size_t> posOtherTrain = (*(itTrains))->GetPosition();
        Leg* legOtherTrain = (*(itTrains))->GetRoute()->GetLeg(posOtherTrain.first);
        if (legOtherTrain == route_->GetLeg(position_.first)) // Same leg
        {
          if (posOtherTrain.second == position_.second) // Same position
          {
            collidedWith_ = *(itTrains);
            (*(itTrains))->SetCollidedWith(this);
          }
        }
      }
    }
  }
}

string Train::GetName()
{
  return name_;
}

pair<size_t, size_t> Train::GetPosition()
{
  return position_;
}

Route * Train::GetRoute()
{
  return route_;
}

Train* Train::GetCollidedWith()
{
  return collidedWith_;
}

void Train::SetCollidedWith(Train* otherTrain)
{
  collidedWith_ = otherTrain;
}
