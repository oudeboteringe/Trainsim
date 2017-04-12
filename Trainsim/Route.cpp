#include "Route.h"

Route::Route()
{
}

Route::~Route()
{
}

void Route::AddLeg(Leg* leg, int direction)
{
  legs_.push_back(make_pair(leg, direction));
}

int Route::GetDirection(Leg * leg)
{

  // Search for this leg:
  for (vector<pair<Leg*, int> >::iterator itLegs = legs_.begin(); itLegs != legs_.end(); itLegs++)
  {
    if ((*itLegs).first == leg) // Leg found
    {
      if ((*itLegs).second == 1)
      {
        return 1;
      }
      else
      {
        return -1;
      }
    }
  }
  return 0; // leg not found
}

pair<Leg*, int> Route::GetBegin()
{
  Leg* firstLeg = (legs_.begin())->first;
  return make_pair(firstLeg, 0);
}
