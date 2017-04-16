#include "Station.h"
// Station.cpp defines the methods of the Station class

Station::Station(string name)
{
  name_ = name;
}

Station::~Station()
{
}

string Station::GetName()
{
  return name_;
}
