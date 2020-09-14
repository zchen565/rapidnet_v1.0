#include "ns3/core-module.h"
#include "ns3/simulator-module.h"
#include "ns3/node-module.h"
#include "ns3/rapidnet-module.h"
#include "ns3/values-module.h"
#include "ns3/helper-module.h"
#include "ns3/messf-module.h"
#include "ns3/messf-query-module.h"
#include <time.h>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <cstdio>
#include <fstream>
#include <sstream>
#include "ns3/rapidnet-utils.h"

#define source(node, content, score)                             \
  ns3::rapidnet::rtuple(Messf::SOURCE,                           \
                        attr("source_attr1", Ipv4Value, node),   \
                        attr("source_attr2", StrValue, content), \
                        attr("source_attr3", RealValue, score))

#define msg(node, content, score, ttl)                        \
  ns3::rapidnet::rtuple(Messf::MSG,                           \
                        attr("msg_attr1", Ipv4Value, node),   \
                        attr("msg_attr2", StrValue, content), \
                        attr("msg_attr3", RealValue, score),  \
                        attr("msg_attr4", Int32Value, ttl))

#define link(node1, node2, score)                             \
  ns3::rapidnet::rtuple(Messf::LINK,                          \
                        attr("link_attr1", Ipv4Value, node1), \
                        attr("link_attr2", Ipv4Value, node2), \
                        attr("link_attr3", RealValue, score))

#define insertsource(node, content, score) \
  app(node)->Insert(source(addr(node), content, score));

#define insertlink(node1, node2, score) \
  app(node1)->Insert(link(addr(node1), addr(node2), score));

#define tupleQuery(loc, name, attr1, attr2, attr3, attr4) \
  rtuple(MessfQuery::TUPLE,                               \
         attr("tuple_attr1", Ipv4Value, loc),             \
         attr("tuple_attr2", StrValue, name),             \
         attr("tuple_attr3", Ipv4Value, attr1),           \
         attr("tuple_attr4", StrValue, attr2),            \
         attr("tuple_attr5", Int32Value, attr3),          \
         attr("tuple_attr6", Int32Value, attr4))

#define inserttuple(loc, name, attr1, attr2, attr3, attr4) \
  queryNode->Insert(tupleQuery(queryNode->GetAddress(), name, addr(attr1), attr2, attr3, attr4));

#define insertquerytuple(loc, name, ipv4addr, attr2, attr3, attr4) \
  queryNode->Insert(tupleQuery(queryNode->GetAddress(), name, ipv4addr, attr2, attr3, attr4));

using namespace std;
using namespace ns3;
using namespace ns3::rapidnet;
using namespace ns3::rapidnet::messf;
using namespace ns3::rapidnet::messfquery;

ApplicationContainer apps;
ApplicationContainer queryapps;

void initApps()
{
  NodeContainer mainAppNodes;
  mainAppNodes.Create(100);

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

  apps.Add(Create<MessfHelper>()->Install(mainAppNodes));
  queryapps.Add(Create<MessfQueryHelper>()->Install(queryAppNodes));

  SetMaxJitter(apps, 0.001);
  SetMaxJitter(queryapps, 0.001);
}
// NodeContainer mainAppNodes;
// NodeContainer queryAppNodes;
void initp2pApps()
{
  // cout << "initing p2p apps" << endl;
  NodeContainer mainAppNodes;
  mainAppNodes.Create(100);

  NodeContainer queryAppNodes;
  queryAppNodes.Create(1);
  int link[505][2] = {0};
  ifstream myfile;
  myfile.open("examples/messf_tables/link5.csv");
  string store;
  int node1 = 0;
  int node2 = 0;
  int lossy = 10;
  float lossy1 = 1.0;
  int i = 0;
  while (getline(myfile, store, ','))
  {
    // node1 = atoi(store.c_str())+1;
    node1 = atoi(store.c_str());
    getline(myfile, store, ',');
    // node2 = atoi(store.c_str())+1;
    node2 = atoi(store.c_str());
    getline(myfile, store);
    lossy = atoi(store.c_str());
    link[i][0] = node1;
    link[i][1] = node2;
    // cout << node1 << " -> " << node2 << endl;
    i += 1;
  }
  // cout << "link matrix done" << endl;
  NodeContainer *pNodeC;
  pNodeC = new NodeContainer[505];
  // cout << "node containers init" << endl;
  for (int j = 0; j < 505; j++)
  {
    // cout << j << "th node container: " << link[j][0] << " -> " << link[j][1] << endl;
    pNodeC[j] = NodeContainer(mainAppNodes.Get(link[j][0]), mainAppNodes.Get(link[j][1]));
  }
  // cout << "node containers done" << endl;
  NodeContainer nq = NodeContainer(mainAppNodes.Get(1), queryAppNodes.Get(0));
  InternetStackHelper internet;
  internet.Install(mainAppNodes);
  internet.Install(queryAppNodes);

  PointToPointHelper p2p;
  p2p.SetDeviceAttribute("DataRate", StringValue("100Mbps"));
  p2p.SetDeviceAttribute("Mtu", UintegerValue(65500));
  p2p.SetChannelAttribute("Delay", StringValue("2ms"));

  NetDeviceContainer *pNetDC;
  pNetDC = new NetDeviceContainer[505];
  for (int j = 0; j < 505; j++)
  {
    pNetDC[j] = p2p.Install(pNodeC[j]);
  }
  NetDeviceContainer ndcq = p2p.Install(nq);
  uint16_t mtu = 65500;

  for (int j = 0; j < 505; j++)
  {
    pNetDC[j].Get(0)->SetMtu(mtu);
  }

  for (int j = 0; j < 505; j++)
  {
    pNetDC[j].Get(1)->SetMtu(mtu);
  }
  ndcq.Get(0)->SetMtu(mtu);
  ndcq.Get(1)->SetMtu(mtu);

  Ipv4AddressHelper ipv4;

  for (int j = 0; j < 505; j++)
  {
    string a = "10.";
    string b = ".0";
    stringstream stream, stream2;
    stream << (j / 255 + 1);
    string n;
    stream >> n;
    a += n;
    a += ".";
    string m;
    stream2 << (j);
    stream2 >> m;
    a += m;
    a += b;
    string c = "255.255.255.0";
    ipv4.SetBase(a.c_str(), c.c_str());
    ipv4.Assign(pNetDC[j]);
  }
  ipv4.SetBase("10.6.0.0", "255.255.255.0");
  ipv4.Assign(ndcq);

  Ipv4GlobalRoutingHelper::PopulateRoutingTables();

  apps.Add(Create<MessfHelper>()->Install(mainAppNodes));
  queryapps.Add(Create<MessfQueryHelper>()->Install(queryAppNodes));

  SetMaxJitter(apps, 0.001);
  SetMaxJitter(queryapps, 0.001);
}

void TupleToQuery()
{
  Ptr<RapidNetApplicationBase> queryNode = queryapps.Get(0)->GetObject<RapidNetApplicationBase>();
  //inserttuple(1, "msg", 40, "Hello", 1, 5);

  //inserttuple(1, "link", 1, 3);
}

void TupleToQuery1()
{
  Ptr<RapidNetApplicationBase> queryNode = queryapps.Get(0)->GetObject<RapidNetApplicationBase>();
  // cout << "inserting rtuple to query" << endl;
  inserttuple(1, "msg", 1, "Hello", 1, 0);
  // insertquerytuple(1, "msg", Ipv4Address("10.2.30.2"), "Hello", 1, 3);
  //insertquerytuple(1, "msg", Ipv4Address("10.1.38.2"), "Hello", 1, 6);
  // inserttuple(1, "msg", 14, "Hello", 1, 5);
  // inserttuple(1, "msg", 96, "Hello", 1, 5);

  //inserttuple(1, "link", 1, 3);
}

/*void SerializeProv(string storepath)
{
  vector<string> relNames;
  relNames.push_back("ruleExec");
  relNames.push_back("prov");
  app(1)->SerializeRel(relNames, 1, storepath);
}*/

void UpdateTables1()
{
  insertsource(1, "Hello", 1.0);
  // cout << "inserting links" << endl;
  ifstream myfile;
  myfile.open("examples/messf_tables/link5.csv");
  string store;
  int node1 = 0;
  int node2 = 0;
  int lossy = 10;
  float lossy1 = 1.0;

  while (getline(myfile, store, ','))
  {
    node1 = atoi(store.c_str()) + 1;
    getline(myfile, store, ',');
    node2 = atoi(store.c_str()) + 1;
    getline(myfile, store);
    lossy = atoi(store.c_str());
    lossy1 = float(lossy) / 10;
    if (lossy1 > 1)
    {
      lossy1 = 1.0;
    }
    insertlink(node1, node2, lossy1);
  }
}
void UpdateTables2()
{
  // cout << "inserting source" << endl;
  Ptr<RapidNetApplicationBase> firstnode = apps.Get(0)->GetObject<RapidNetApplicationBase>();
  insertsource(1, "Hello", 1.0);
}

void Print()
{
  PrintRelation(apps, Messf::PROV);
  //PrintRelation (apps, Messf::RULEEXEC);
  PrintRelation(apps, Messf::MSG);

  //PrintRelation (apps, Messf::RQLIST);
  //PrintRelation (apps, Messf::PQLIST);

  PrintRelation(queryapps, MessfQuery::TUPLE);
  PrintRelation(queryapps, MessfQuery::PROVQUERY);
  PrintRelation(queryapps, MessfQuery::RECORDS);
  PrintRelation(queryapps, MessfQuery::RESULTS);
  //PrintRelation (apps, Messf::LINK);
}

int main(int argc, char *argv[])
{

  //LogComponentEnable("Messf", LOG_LEVEL_INFO);
  // LogComponentEnable("MessfQuery", LOG_LEVEL_INFO);
  //LogComponentEnable("RapidNetApplicationBase", LOG_LEVEL_INFO);*/

  //initApps();
  initp2pApps();
  //  string storepath = "output/";

  apps.Start(Seconds(0.0));
  apps.Stop(Seconds(2000.0));
  queryapps.Start(Seconds(0.0));
  queryapps.Stop(Seconds(2000.0));

  schedule(1.0, TupleToQuery1);
  schedule(2.0, UpdateTables1);
  //schedule (4.0, UpdateTables2);
  //schedule (500.0, TupleToQuery1);

  schedule(1000.0, Print);
  //Simulator::Schedule (Seconds(1500.0), SerializeProv, storepath);

  Simulator::Run();
  Simulator::Destroy();

  return 0;
}
