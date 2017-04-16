#include "Network.h"

// Network.cpp defines the methods of the Network class

Network::Network()
{
}

Network::~Network()
{
  // Delete the legs:
  while (legs_.begin() != legs_.end())
  {
    legs_.erase(legs_.begin());
  }

  // Delete the stations:
  while (stations_.begin() != stations_.end())
  {
    delete stations_.begin()->second; // Delete the station
    stations_.erase(stations_.begin()); // Erase the entry from the map<string, Station*>
  }
}

void Network::AddStation(Station * station)
{
  stations_[station->GetName()] = station;
}

void Network::AddLeg(string from, string to, size_t distance)
{
  if (stations_[from] == nullptr)
  {
    stations_[from] = new Station(from);
  }
  if (stations_[to] == nullptr)
  {
    stations_[to] = new Station(to);
  }
  legs_.push_back(new Leg(stations_[from], stations_[to], distance));
}

pair<Leg*, int> Network::GetLeg(string from, string to)
{
  Leg* leg = nullptr;
  int dir = 0;
  for (vector<Leg*>::iterator itLegs = legs_.begin(); itLegs != legs_.end(); itLegs++)
  {
    if ( ((*itLegs)->GetFrom()->GetName() == from) && ((*itLegs)->GetTo()->GetName() == to))
    {
      leg = *itLegs;
      dir = 1;
    }
    else if (((*itLegs)->GetFrom()->GetName() == to) && ((*itLegs)->GetTo()->GetName() == from)) // Use leg reversed
    {
      leg = *itLegs;
      dir = -1;
    }
  }
  return pair<Leg*, int>(leg, dir);
}
