// Trainsim.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <conio.h>
#include <iostream>
using namespace std;

#include "Route.h"
#include "Train.h"

void driveTrains(vector<Train*>* trains)
{
  for (vector<Train*>::iterator itTrainVec = trains->begin(); itTrainVec != trains->end(); itTrainVec++)
  {
    vector<Train*>::iterator itOtherTrain = (*itTrainVec)->drive(trains);
    if ((*itOtherTrain) != *itTrainVec) // Conflict
    {
      trains->erase(itOtherTrain);
      trains->erase(itTrainVec);
    }
  }
}

void printTrainStates(vector<Train*>* trains)
{
  if (trains->size() <= 0)
  {
    cout << "No trains in simulation." << endl;
  }
  for (vector<Train*>::iterator itTrainVec = trains->begin(); itTrainVec != trains->end(); itTrainVec++)
  {
    pair<size_t, size_t> position = (*itTrainVec)->getPosition();
    string fromStation = (*itTrainVec)->getRoute()->getLeg(position.first)->getFrom()->getName();
    string toStation = (*itTrainVec)->getRoute()->getLeg(position.first)->getTo()->getName();
    size_t distance = (*itTrainVec)->getRoute()->getLeg(position.first)->getDistance();
    size_t fraction = position.second;
    cout << "State of train " << (*itTrainVec)->getName() << ":" << endl;
    if (fraction == 0)
    {
      cout << "At station: " << fromStation << endl;
    }
    else if (fraction == (*itTrainVec)->getRoute()->getLeg(position.first)->getDistance())
    {
      cout << "At station: " << toStation << endl;
    }
    else
    {
      cout << "On leg between " << fromStation << " and " << toStation << "(" << fraction << "/" << distance << ")" << endl;
    }

    if ((*itTrainVec)->getPosition() == (*itTrainVec)->getRoute()->getEnd())
    {
      cout << "Destination reached!" << endl;
    }
  }
}

// Clean up (delete from train vector) all trains that have collided or have reached their destination
void cleanUp(vector<Train*>* trains)
{
}

int main()
{
  int speed = 1;

  // Construct the stations:
  Station* amsterdam = new Station("Amsterdam");
  Station* zaandam = new Station("Zaandam");
  Station* alkmaar = new Station("Alkmaar");
  Station* haarlem = new Station("Haarlem");
  Station* leiden = new Station("Leiden");
  Station* denHaag = new Station("Den Haag");
  Station* utrecht = new Station("Utrecht");

  // Construct the legs:
  Leg* leg1 = new Leg(amsterdam, haarlem, 20);
  Leg* leg2 = new Leg(amsterdam, zaandam, 15);
  Leg* leg3 = new Leg(zaandam, alkmaar, 40);
  Leg* leg4 = new Leg(haarlem, leiden, 30);
  Leg* leg5 = new Leg(leiden, denHaag, 10);
  Leg* leg6 = new Leg(denHaag, utrecht, 70);
  Leg* leg7 = new Leg(utrecht, amsterdam, 50);

  // Construct the route of train1:
  Route* route1 = new Route;

  // Add the legs:
  route1->AddLeg(leg1, 1);
  route1->AddLeg(leg4, 1);
  route1->AddLeg(leg5, 1);

  Train* train1 = new Train("train1", route1, speed);

  vector<Train*> trainVector;
  trainVector.push_back(train1);

  cout << "Welcome to trainsim, a basic train simulation program." << endl;
  cout << "Initial states of the trains:" << endl;

  bool continueSim = true;
  while (continueSim)
  {
    printTrainStates(&trainVector);
    cleanUp(&trainVector);
    cout << "Press <enter> to continue the simulation or press s(top) <enter> to stop." << endl;
    if (cin.get() == 's')
    {
      continueSim = false;
    }
    if (continueSim)
    {
      driveTrains(&trainVector);
    }
  }

  cout << "Press any key to exit." << endl;
  _getch();
  return 0;
}

