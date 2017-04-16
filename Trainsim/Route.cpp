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

Leg * Route::GetLeg(size_t legIndex)
{
  Leg* leg = nullptr;
  if (legIndex < legs_.size())
  {
    leg = legs_.operator[](legIndex).first;
  }
  return leg;
}

int Route::GetDirection(size_t legIndex)
{
  return legs_.operator[](legIndex).second;
}

size_t Route::GetDistance(size_t legIndex)
{
  return legs_.operator[](legIndex).first->GetDistance();
}

pair<size_t, size_t> Route::GetBegin()
{
  size_t fraction = 0;
  if (legs_.operator[](0).second == -1) /// Reversed
  {
    fraction = legs_.operator[](0).first->GetDistance();
  }
  return make_pair(0, fraction);
}

pair<size_t, size_t> Route::GetEnd()
{
  size_t fraction = (legs_.operator[](legs_.size() - 1)).first->GetDistance();
  if ((legs_.operator[](legs_.size() - 1)).second == -1) // Traverse last leg reversed
  {
    fraction = 0;
  }
  return make_pair(legs_.size()-1, fraction);
}
