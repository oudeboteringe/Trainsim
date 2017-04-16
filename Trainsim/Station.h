#pragma once

// Station.h: defines the Station class

#include <string>
using namespace std;

class Station
{
public:
  Station(string name);
  ~Station();
  string GetName();

private:
  string name_;
};
