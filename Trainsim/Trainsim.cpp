// Trainsim.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <conio.h>
#include <iostream>
using namespace std;

#include "Route.h"
#include "Train.h"

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
    for (vector<Train*>::iterator itTrainVec = trainVector.begin(); itTrainVec != trainVector.end(); itTrainVec++)
    {
      vector<Train*>::iterator itOtherTrain = (*itTrainVec)->Drive(&trainVector);
      if ((*itOtherTrain) != *itTrainVec) // Conflict
      {
        trainVector.erase(itOtherTrain);
        trainVector.erase(itTrainVec);
      }
    }
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

