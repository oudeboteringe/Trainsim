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

Leg * Route::getLeg(size_t legIndex)
{
  return legs_.operator[](legIndex).first;
}

int Route::getDirection(size_t legIndex)
{
  return legs_.operator[](legIndex).second;
}

size_t Route::getDistance(size_t legIndex)
{
  return legs_.operator[](legIndex).first->getDistance();
}

pair<size_t, size_t> Route::GetBegin()
{
  return make_pair(0, 0);
}

pair<size_t, size_t> Route::getEnd()
{
  size_t fraction = (legs_.operator[](legs_.size() - 1)).first->getDistance();
  if ((legs_.operator[](legs_.size() - 1)).second == -1) // Traverse last leg reversed
  {
    fraction = 0;
  }
  return make_pair(legs_.size()-1, fraction);
}
