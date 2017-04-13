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
  for (vector<Train*>::iterator itTrainVec = trains->begin(); itTrainVec != trains->end(); itTrainVec++)
  {
    pair<Leg*, int>* position = (*itTrainVec)->getPosition();
    string fromStation = position->first->getFrom()->getName();
    string toStation = position->first->getTo()->getName();
    int distance = position->first->getDistance();
    int fraction = position->second;
    cout << "State of train " << (*itTrainVec)->getName() << ":" << endl;
    cout << "On leg from: " << fromStation << " To: " << toStation << "(" << fraction << "/"  << distance << ")" << endl;
  }
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

  bool nextStep = true;
  char character = 'c';
  while ((trainVector.size() > 0) && nextStep)
  {
    driveTrains(&trainVector);
    printTrainStates(&trainVector);
    cout << "Press <enter> to simulate next step or s(top) to stop." << endl;
    cin >> character;
    if (character == 's')
    {
      nextStep = false;
    }
  }

  cout << "Press any key to exit." << endl;
  _getch();
  return 0;
}

