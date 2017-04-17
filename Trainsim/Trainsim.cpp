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
  string msg = "";
  pugi::xml_document networkConfigDoc;
  pugi::xml_parse_result result = networkConfigDoc.load_file(networkConfigFilename.c_str());
  if (!result)
  {
    msg = "Could not load network configuration file (Network.cfg): ";
    string errorDescription = string(result.description());
    msg += errorDescription + ".";
    if (result.status != 1) // Error other than file not found
    {
      msg += string(". Check character ") + to_string(result.offset) +
        string(" and further (counting from the beginning of the file).");
    }
    cout << msg << endl;
    clog << msg << endl;
    return nullptr;
  }

  Network* network = new Network();

  // Construct the legs including their stations:
  msg = "";
  int nLegs = 0;
  for (pugi::xml_node newLeg = networkConfigDoc.child("leg"); newLeg; newLeg = newLeg.next_sibling("leg"))
  {
    string legFrom = newLeg.child("from").text().as_string();
    if (legFrom == "")
    {
      msg += "Error in leg specification: invalid \"from\" station.\n";
    }
    string legTo = newLeg.child("to").text().as_string();
    if (legTo == "")
    {
      msg += "Error in leg specification: invalid \"to\" station.\n";
    }
    int legDist = -1;
    string legDistString = newLeg.child("distance").text().as_string();
    if (legDistString != "")
    {
      legDist = atoi(legDistString.c_str());
    }
    if (legDist <= 0)
    {
      msg += "Error in leg specification: invalid distance.\n";
    }
    if(msg == "") // No errors in leg
    {
      network->AddLeg(legFrom, legTo, legDist);
      nLegs++;
    }
    else
    {
      msg = string("Error in leg ") + to_string(nLegs+1) + " in network configuration file (Network.cfg):\n" + msg;
      cout << msg << endl;
      clog << msg << endl;
      return nullptr;
    }
  }

  // No valid legs found? => error
  if (nLegs == 0)
  {
    msg = "Error: no valid legs found in network configuration file (Network.cfg).";
    cout << msg << endl;
    clog << msg << endl;
    return nullptr;
  }

  return network;
}

vector<Train*>* readTrainConfig(string trainConfigFilename, Network* network)
{
  string msg = "";
  pugi::xml_document trainConfigDoc;
  pugi::xml_parse_result result = trainConfigDoc.load_file(trainConfigFilename.c_str());
  if (!result)
  {
    msg = "Could not load train configuration file (Trains.cfg): ";
    string errorDescription = string(result.description());
    msg += errorDescription + ".";
    if (result.status != 1) // Error other than file not found
    {
      msg += string(". Check character ") + to_string(result.offset) +
        string(" and further (counting from the beginning of the file).");
    }
    cout << msg << endl;
    clog << msg << endl;
    return nullptr;
  }

  vector<Train*>* trains = new vector<Train*>;

  // Retreive the data for the trains:
  msg = "";
  int nTrains = 0;
  for (pugi::xml_node newTrain = trainConfigDoc.child("train"); newTrain; newTrain = newTrain.next_sibling("train"))
  {
    string name = newTrain.child("name").text().as_string();
    if (name == "")
    {
      msg += string("Error in train specification of train ") + to_string(nTrains + 1) +
        " in train configuration file (Trains.cfg): invalid name\n";
    }
    int speed = -1;
    string speedString = newTrain.child("speed").text().as_string();
    if (speedString != "")
    {
      speed = atoi(speedString.c_str());
    }
    if (speed <= 0)
    {
      msg += string("Error in train specification of train ") + to_string(nTrains + 1) +
        " in train configuration file (Trains.cfg): invalid speed\n";
    }
    pugi::xml_node route = newTrain.child("route");
    if (route == nullptr)
    {
      msg += string("Error in specification of train ") + to_string(nTrains + 1) +
        " in train configuration file (Trains.cfg): invalid route\n";
    }

    pugi::xml_node prevStation = route.child("station");              // First time: first station
    pugi::xml_node nextStation = prevStation.next_sibling("station"); // First time: second station

    if (msg == "")
    {
      Route* newRoute = new Route; // Construct the route for this train

      while (nextStation && (msg == ""))
      {
        string prevStationName = prevStation.text().as_string();
        string nextStationName = nextStation.text().as_string();

        // Get this leg:
        pair<Leg*, int> legAndDir = network->GetLeg(prevStationName, nextStationName);

        if (legAndDir.first != nullptr)
        {
          // Add this leg to the route:
          newRoute->AddLeg(legAndDir.first, legAndDir.second);
        }
        else
        {
          msg += string("Error: route of train ") + name + " uses leg from " + prevStationName + " to " +
            nextStationName + ", but this leg is not specified in the network configuration file (Network.cfg).\n";
          msg += "This leg could not be added to the route of train " + name + ".\n";
        }

        prevStation = nextStation;
        nextStation = nextStation.next_sibling("station");
      }
      if (msg == "")
      {
        Train* train = new Train(name, newRoute, speed);
        trains->push_back(train);
        nTrains++;
      }
    }
    if (msg != "")
    {
      cout << msg << endl;
      clog << msg << endl;
      return nullptr;
    }
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

  ofstream logFile("Trainsim.log");

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
  else
  {
    cout << "Program termination, press any key to exit." << endl;
    _getch();
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

  delete network;
  while ( (trains != nullptr) && (trains->begin() != trains->end()) )
  {
    delete *(trains->begin()); // Delete the train
    trains->erase(trains->begin()); // Erase the entry from the vector<Train*>
  }
  delete trains;

  return 0;
}

