// Trainsim.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <conio.h>
#include <fstream>
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
    string msg = "Could not load network configuration file (Network.cfg): ";
    string errorDescription = string(result.description());
    msg += errorDescription + string(". Check character ") + std::to_string(result.offset) +
      string(" and further (counting from the beginning of the file).");
    cout << msg << endl;
    clog << msg << endl;
    return nullptr;
  }

  Network* network = new Network();

  // Construct the legs including their stations:
  for (pugi::xml_node newLeg = networkConfigDoc.child("leg"); newLeg; newLeg = newLeg.next_sibling("leg"))
  {
    string legFrom = newLeg.child("from").text().as_string();
    string legTo = newLeg.child("to").text().as_string();
    size_t legDist = (size_t)newLeg.child("distance").text().as_int();
    network->AddLeg(legFrom, legTo, legDist);
  }

  return network;
}

vector<Train*>* readTrainConfig(string trainConfigFilename, Network* network)
{
  pugi::xml_document trainConfigDoc;
  pugi::xml_parse_result result = trainConfigDoc.load_file(trainConfigFilename.c_str());
  if (!result)
  {
    string msg = "Could not load train configuration file (Trains.cfg). ";
    string errorDescription = string(result.description());
    msg += errorDescription + string(". Check character ") + std::to_string(result.offset) +
      string(" and further (counting from the beginning of the file).");
    cout << msg << endl;
    clog << msg << endl;
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
      pair<Leg*, int> legAndDir = network->GetLeg(prevStationName, nextStationName);

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

void printAndCleanUp(vector<Train*>* trains)
{
  if (trains->size() <= 0)
  {
    cout << "No trains in simulation." << endl;
  }
  
  vector<Train*>::iterator itTrains = trains->begin();
  while (itTrains < trains->end())
  {
    pair<size_t, size_t> position = (*itTrains)->GetPosition();
    string fromStation = (*itTrains)->GetRoute()->GetLeg(position.first)->GetFrom()->GetName();
    string toStation = (*itTrains)->GetRoute()->GetLeg(position.first)->GetTo()->GetName();
    size_t distance = (*itTrains)->GetRoute()->GetLeg(position.first)->GetDistance();
    size_t fraction = position.second;
    cout << "State of train " << (*itTrains)->GetName() << ":" << endl;
    if (fraction == 0)
    {
      cout << "At station: " << fromStation << endl;
    }
    else if (fraction == (*itTrains)->GetRoute()->GetLeg(position.first)->GetDistance())
    {
      cout << "At station: " << toStation << endl;
    }
    else
    {
      cout << "On leg between " << fromStation << " and " << toStation << "(" << fraction << "/" << distance << ")" << endl;
    }

    bool destReached = (*itTrains)->GetPosition() == (*itTrains)->GetRoute()->GetEnd();
    if (destReached)
    {
      cout << "Destination reached!" << endl;
    }

    // Did this train collide or reach its destination? => remove it from the simulation
    Train* otherTrain = (*itTrains)->GetCollidedWith();
    if ((otherTrain != nullptr) || destReached)
    {
      if ((otherTrain != nullptr) && (otherTrain != (*itTrains)))
      {
        string msg = string("Train ") + (*itTrains)->GetName() + " collided with train " + otherTrain->GetName();
        cout << msg << endl;
        clog << msg << endl;
        otherTrain->SetCollidedWith(otherTrain); // Trick to signal other train is also to be deleted, but nothing should be printed.
      }
      delete (*itTrains);
      itTrains = trains->erase(itTrains);
    }
    else
    {
      itTrains++;
    }
  }
}


int main()
{
  cout << "Welcome to trainsim, a basic train simulation program." << endl;

  ofstream logFile("trainsim.log");

  // Get the rdbuf of clog.
  // We need it to reset the value before exiting.
  auto old_rdbuf = clog.rdbuf();

  // Set the rdbuf of clog.
  clog.rdbuf(logFile.rdbuf());

  int speed = 1;
  string networkConfigFilename = "./Network.cfg";
  string trainConfigFilename = "./Trains.cfg";
  Network* network = readNetworkConfig(networkConfigFilename);
  vector<Train*>* trains = nullptr;
  if (network != nullptr)
  {
    trains = readTrainConfig(trainConfigFilename, network);
  }

  bool continueSim = (network && trains);

  if (continueSim)
  {
    cout << "Initial states of the trains:" << endl;
  }

  while (continueSim)
  {
    printAndCleanUp(trains);
    cout << "Press <enter> to continue the simulation or press s(top) <enter> to stop." << endl;
    if (cin.get() == 's')
    {
      continueSim = false;
    }
    if (continueSim)
    {
      for (vector<Train*>::iterator itTrains = trains->begin(); itTrains != trains->end(); itTrains++)
      {
        (*itTrains)->Drive(trains);
      }
    }
  }

  cout << "Press any key to exit." << endl;
  delete network;
  while ( (trains != nullptr) && (trains->begin() != trains->end()) )
  {
    delete *(trains->begin()); // Delete the train
    trains->erase(trains->begin()); // Erase the entry from the vector<Train*>
  }
  delete trains;

  // Reset the rdbuf of clog.
  clog.rdbuf(old_rdbuf);

  _getch();
  return 0;
}

