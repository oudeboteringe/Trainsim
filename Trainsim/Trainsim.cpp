// Trainsim.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <conio.h>
#include <iostream>
using namespace std;

#include "Network.h"
#include "Route.h"
#include "Train.h"

Network* readNetworkConfig(string networkConfigFilename)
{
  Network* network = new Network();

  // Construct the legs including their stations:
  network->addLeg(string("Amsterdam"), string("Haarlem"), 20);
  network->addLeg(string("Amsterdam"), string("Zaandam"), 15);
  network->addLeg(string("Zaandam"), string("Alkmaar"), 40);
  network->addLeg(string("Haarlem"), string("Leiden"), 30);
  network->addLeg(string("Leiden"), string("Den Haag"), 10);
  network->addLeg(string("Den Haag"), string("Utrecht"), 70);
  network->addLeg(string("Utrecht"), string("Amsterdam"), 50);

  return network;
}

vector<Train*>* readTrainConfig(string trainConfigFilename, Network* network)
{
  vector<Train*>* trainVector = new vector<Train*>;

  // Construct the route of train1:
  Route* route1 = new Route;

  // Add the legs:
  pair<Leg*, int> legAndDir = network->getLeg(string("Amsterdam"), string("Haarlem"));
  route1->AddLeg(legAndDir.first, legAndDir.second);
  legAndDir = network->getLeg(string("Haarlem"), string("Leiden"));
  route1->AddLeg(legAndDir.first, legAndDir.second);
  legAndDir = network->getLeg(string("Leiden"), string("Den Haag"));
  route1->AddLeg(legAndDir.first, legAndDir.second);

  Train* train1 = new Train("train1", route1, 1);

  trainVector->push_back(train1);
  return trainVector;

}

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
  string networkConfigFilename = ".\\Network.cfg";
  string trainConfigFilename = ".\\Trains.cfg";
  Network* network = readNetworkConfig(networkConfigFilename);
  vector<Train*>* trainVector = readTrainConfig(trainConfigFilename, network);

  cout << "Welcome to trainsim, a basic train simulation program." << endl;
  cout << "Initial states of the trains:" << endl;

  bool continueSim = true;
  while (continueSim)
  {
    printTrainStates(trainVector);
    cleanUp(trainVector);
    cout << "Press <enter> to continue the simulation or press s(top) <enter> to stop." << endl;
    if (cin.get() == 's')
    {
      continueSim = false;
    }
    if (continueSim)
    {
      driveTrains(trainVector);
    }
  }

  cout << "Press any key to exit." << endl;
  _getch();
  return 0;
}

