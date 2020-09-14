/* _*_ Mode:C++; c_file_style:"gnu"; indent_tabs_mode:nil; _*_ */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111_1307  USA
 */

#include "ns3/core-module.h"
#include "ns3/simulator-module.h"
#include "ns3/node-module.h"
#include "ns3/rapidnet-module.h"
#include "ns3/values-module.h"
#include "ns3/helper-module.h"
#include "ns3/smoke-module.h"
#include "ns3/smoke-query-module.h"
#include "ns3/list-value.h"
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <bits/stdc++.h>
#include <boost/algorithm/string.hpp>

#define SmokeTrain \
  "./data/smoke/smoking-train.db"

#define SmokeTest \
  "./data/smoke/smoking-test.db"

#define smoke(local, person)                    \
  rtuple(Smoke::SMOKE,                          \
         attr("smoke_attr1", Ipv4Value, local), \
         attr("smoke_attr2", Int32Value, person))

#define cancer(local, person)                    \
  rtuple(Smoke::CANCER,                          \
         attr("cancer_attr1", Ipv4Value, local), \
         attr("cancer_attr2", Int32Value, person))

#define friends(local, person1, person2)             \
  rtuple(Smoke::FRIENDS,                             \
         attr("friends_attr1", Ipv4Value, local),    \
         attr("friends_attr2", Int32Value, person1), \
         attr("friends_attr3", Int32Value, person2))

#define insertsmoke(local, person) \
  app(local)->Insert(smoke(addr(local), person))

#define insertcancer(local, person) \
  app(local)->Insert(cancer(addr(local), person))

#define insertfriends(local, person1, person2) \
  app(local)->Insert(friends(addr(local), person1, person2))

#define tupleQuery(local, name, attr1, attr2)   \
  rtuple(SmokeQuery::TUPLE,                     \
         attr("tuple_attr1", Ipv4Value, local), \
         attr("tuple_attr2", StrValue, name),   \
         attr("tuple_attr3", Ipv4Value, attr1), \
         attr("tuple_attr4", Int32Value, attr2))

#define inserttuple(local, name, attr1, attr2) \
  queryNode->Insert(tupleQuery(queryNode->GetAddress(), name, addr(attr1), attr2))

using namespace std;
using namespace ns3;
using namespace ns3::rapidnet;
using namespace ns3::rapidnet::smoke;
using namespace ns3::rapidnet::smokequery;

ApplicationContainer apps;
ApplicationContainer queryapps;

map<string, int> people;

void initApps()
{
  NodeContainer mainAppNodes;
  mainAppNodes.Create(3);

  NodeContainer queryAppNodes;
  queryAppNodes.Create(1);

  NodeContainer csmaNodes;
  csmaNodes.Add(mainAppNodes);
  csmaNodes.Add(queryAppNodes);

  CsmaHelper csma;

  NetDeviceContainer csmaDevices;
  csmaDevices = csma.Install(csmaNodes);

  InternetStackHelper stack;
  stack.Install(csmaNodes);

  Ipv4AddressHelper address;
  Ipv4Address base = "10.1.1.0";

  address.SetBase(base, "255.255.255.0");
  address.Assign(csmaDevices);

  apps.Add(Create<SmokeHelper>()->Install(mainAppNodes));
  queryapps.Add(Create<SmokeQueryHelper>()->Install(queryAppNodes));

  SetMaxJitter(apps, 0.001);
  SetMaxJitter(queryapps, 0.001);
}

void parseLine(const string &line)
{
  vector<string> splits;
  boost::split(splits, line, boost::is_any_of("(), "));

  vector<string> words;
  for (int i = 0; i < splits.size(); i++)
  {
    if (splits[i].size() != 0)
    {
      words.push_back(splits[i]);
    }
  }

  string predicate = words[0];
  if (predicate == "//")
  {
    return;
  }
  else if (predicate == "Friends")
  {
    string person1 = words[1];
    string person2 = words[2];
    if (people.find(person1) == people.end())
    {
      people[person1] = people.size();
    }
    if (people.find(person2) == people.end())
    {
      people[person2] = people.size();
    }
    cout << "friends" << people[person1] << '_' << people[person2] << " 1" << endl;
    insertfriends(1, people[person1], people[person2]);
  }
  else if (predicate == "Smokes")
  {
    string person = words[1];
    if (people.find(person) == people.end())
    {
      people[person] = people.size();
    }
    cout << "smoke" << people[person] << " 1" << endl;
    insertsmoke(1, people[person]);
  }
  else if (predicate == "Cancer")
  {
    string person = words[1];
    if (people.find(person) == people.end())
    {
      people[person] = people.size();
    }
    cout << "cancer" << people[person] << " 1" << endl;
    insertcancer(1, people[person]);
  }
}

void train()
{
  ifstream fp(SmokeTrain);
  string line;

  while (getline(fp, line))
  {
    if (!line.empty())
    {
      int i = 0;
      while (i < line.size() && line[i] == ' ')
      {
        i++;
      }
      if (i != 0)
      {
        line = line.substr(i, line.size() - i);
      }
      parseLine(line);
    }
  }
}

void printMap(map<string, int> m)
{
  for (map<string, int>::iterator it = m.begin(); it != m.end(); it++)
  {
    cout << it->first << ' ' << it->second << endl;
  }
}

void Print()
{
  printMap(people);
  PrintRelation(apps, Smoke::FRIENDS);
  PrintRelation(apps, Smoke::SMOKE);
  PrintRelation(apps, Smoke::CANCER);
  PrintRelation(queryapps, SmokeQuery::TUPLE);
  PrintRelation(queryapps, SmokeQuery::RECORDS);
}

int n = 1;

void SingleTupleToQuery()
{
  Ptr<RapidNetApplicationBase> queryNode = queryapps.Get(0)->GetObject<RapidNetApplicationBase>();
  inserttuple(1, "cancer", 1, 2);
}

void TupleToQuery()
{
  Ptr<RapidNetApplicationBase> queryNode = queryapps.Get(0)->GetObject<RapidNetApplicationBase>();
  // inserttuple(1, "cancer", 1, 2);
  // inserttuple(1, "cancer", 1, 6);
  inserttuple(1, "cancer", 1, n);
  n++;
}

int main(int argc, char *argv[])
{

  initApps();

  apps.Start(Seconds(0.0));
  apps.Stop(Seconds(100.0));
  queryapps.Start(Seconds(0.0));
  queryapps.Stop(Seconds(100.0));

  schedule(2.0, train);
  for (int i = 0; i < 8; i++)
  {
    schedule(5 + i * 5.0, TupleToQuery);
  }
  // schedule (5.0, SingleTupleToQuery);
  schedule(99.0, Print);

  // schedule(1.0, train);
  // schedule(2.0, TupleToQuery);
  // schedule(5.0, Print);

  Simulator::Run();
  Simulator::Destroy();

  return 0;
}
