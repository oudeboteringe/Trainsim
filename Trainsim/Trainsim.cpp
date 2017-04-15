// Trainsim.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <conio.h>
#include <iostream>
using namespace std;

#include "Network.h"
#include "Route.h"
#include "Train.h"
#include "pugixml.hpp"

Network* readNetworkConfig(string networkConfigFilename)
{

  pugi::xml_document networkConfigDoc;
  pugi::xml_parse_result result = networkConfigDoc.load_file(networkConfigFilename.c_str());
  if (!result)
  {
    cout << "Could not load network configuration file." << endl;
    return nullptr;
  }

  Network* network = new Network();

  // Construct the legs including their stations:
  for (pugi::xml_node newLeg = networkConfigDoc.child("leg"); newLeg; newLeg = newLeg.next_sibling("leg"))
  {
    string legFrom = newLeg.child("from").text().as_string();
    string legTo = newLeg.child("to").text().as_string();
    size_t legDist = (size_t)newLeg.child("distance").text().as_int();
    network->addLeg(legFrom, legTo, legDist);
  }

  return network;
}

vector<Train*>* readTrainConfig(string trainConfigFilename, Network* network)
{
  pugi::xml_document trainConfigDoc;
  pugi::xml_parse_result result = trainConfigDoc.load_file(trainConfigFilename.c_str());
  if (!result)
  {
    cout << "Could not load train configuration file." << endl;
    return nullptr;
  }

  vector<Train*>* trains = new vector<Train*>;

  // Retreive the data for the trains:
  for (pugi::xml_node newTrain = trainConfigDoc.child("train"); newTrain; newTrain = newTrain.next_sibling("train"))
  {
    string name = newTrain.child("name").text().as_string();
    int speed = newTrain.child("speed").text().as_int();
    pugi::xml_node route = newTrain.child("route");

    // Construct the route of this train:
    Route* newRoute = new Route;

    pugi::xml_node prevStation = route.child("station");              // First time: first station
    pugi::xml_node nextStation = prevStation.next_sibling("station"); // First time: second station

    while (nextStation)
    {
      string prevStationName = prevStation.text().as_string();
      string nextStationName = nextStation.text().as_string();

      // Get this leg:
      pair<Leg*, int> legAndDir = network->getLeg(prevStationName, nextStationName);

      if (true)
      {
        // Add this leg to the route:
        newRoute->AddLeg(legAndDir.first, legAndDir.second);
      }
      else // TODO: error handling
      {
        ;
      }

      prevStation = nextStation;
      nextStation = nextStation.next_sibling("station");
    }
    Train* train = new Train(name, newRoute, speed);
    trains->push_back(train);
  }

  return trains;
}

void driveTrains(vector<Train*>* trains)
{
  for (vector<Train*>::iterator itTrainVec = trains->begin(); itTrainVec != trains->end(); itTrainVec++)
  {
    vector<Train*>::iterator itOtherTrain = (*itTrainVec)->drive(trains);
    if ((*itOtherTrain) != *itTrainVec) // Conflict
    {
      //trains->erase(itOtherTrain);
      //trains->erase(itTrainVec);
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
    string fromStation = (*itTrainVec)->getRoute()->GetLeg(position.first)->getFrom()->getName();
    string toStation = (*itTrainVec)->getRoute()->GetLeg(position.first)->getTo()->getName();
    size_t distance = (*itTrainVec)->getRoute()->GetLeg(position.first)->getDistance();
    size_t fraction = position.second;
    cout << "State of train " << (*itTrainVec)->getName() << ":" << endl;
    if (fraction == 0)
    {
      cout << "At station: " << fromStation << endl;
    }
    else if (fraction == (*itTrainVec)->getRoute()->GetLeg(position.first)->getDistance())
    {
      cout << "At station: " << toStation << endl;
    }
    else
    {
      cout << "On leg between " << fromStation << " and " << toStation << "(" << fraction << "/" << distance << ")" << endl;
    }

    if ((*itTrainVec)->getPosition() == (*itTrainVec)->getRoute()->GetEnd())
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
  cout << "Welcome to trainsim, a basic train simulation program." << endl;

  int speed = 1;
  string networkConfigFilename = "C:/Temp/Network.cfg";
  string trainConfigFilename = "C:/Temp/Trains.cfg";
  Network* network = readNetworkConfig(networkConfigFilename);
  vector<Train*>* trainVector = readTrainConfig(trainConfigFilename, network);

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

