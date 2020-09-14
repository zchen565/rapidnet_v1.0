#include "ns3/core-module.h"
#include "ns3/simulator-module.h"
#include "ns3/node-module.h"
#include "ns3/rapidnet-module.h"
#include "ns3/values-module.h"
#include "ns3/helper-module.h"
#include "ns3/trust-module.h"
#include "ns3/trust-query-module.h"
#include <time.h>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <list>

#define TrustTrain \
	"./data/trust/sample.csv"

#define TrustTrain_5 \
	"./data/trust/sample_5.csv"

#define TrustTrain_10 \
	"./data/trust/sample_10.csv"

#define TrustTrain_20 \
	"./data/trust/sample_20.csv"

#define TrustTrain_30 \
	"./data/trust/sample_30.csv"

#define TrustTrain_40 \
	"./data/trust/sample_40.csv"

#define TrustTrain_50 \
	"./data/trust/sample_50.csv"

#define TrustTrain_60 \
	"./data/trust/sample_60.csv"

#define TrustTrain_70 \
	"./data/trust/sample_70.csv"

#define TrustTrain_80 \
	"./data/trust/sample_80.csv"

#define TrustTrain_90 \
	"./data/trust/sample_90.csv"

#define TrustTrain_100 \
	"./data/trust/sample_100.csv"

#define trust(local, person1, person2)               \
	rtuple(Trust::TRUST,                             \
		   attr("trust_attr1", Ipv4Value, local),    \
		   attr("trust_attr2", Int32Value, person1), \
		   attr("trust_attr3", Int32Value, person2))

#define inserttrust(local, person1, person2) \
	app(local)->Insert(trust(addr(local), person1, person2));

//define the rtuple you would like to query and how to insert it
#define tupleQuery(loc, name, attr1, attr2, attr3) \
	rtuple(TrustQuery::TUPLE,                      \
		   attr("tuple_attr1", Ipv4Value, loc),    \
		   attr("tuple_attr2", StrValue, name),    \
		   attr("tuple_attr3", Ipv4Value, attr1),  \
		   attr("tuple_attr4", Int32Value, attr2), \
		   attr("tuple_attr5", Int32Value, attr3))

#define inserttuple(loc, name, attr1, attr2, attr3) \
	queryNode->Insert(tupleQuery(queryNode->GetAddress(), name, addr(attr1), attr2, attr3));

using namespace std;
using namespace ns3;
using namespace ns3::rapidnet;
using namespace ns3::rapidnet::trust;
using namespace ns3::rapidnet::trustquery;

ApplicationContainer apps;
ApplicationContainer queryapps;

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

	apps.Add(Create<TrustHelper>()->Install(mainAppNodes));
	queryapps.Add(Create<TrustQueryHelper>()->Install(queryAppNodes));

	SetMaxJitter(apps, 0.001);
	SetMaxJitter(queryapps, 0.001);
}

vector<string> readFile(string filename)
{
	vector<string> re;
	string s;
	ifstream f(filename.c_str(), ios_base::in);

	while (!f.eof())
	{
		getline(f, s);
		re.push_back(s);
	}
	f.close();
	return re;
}

int stringToInt(string s)
{
	stringstream ss(s);
	int x = 0;
	ss >> x;
	return x;
}

double stringToDouble(string s)
{
	stringstream ss(s);
	double x = 0.0;
	ss >> x;
	return x;
}

void parse(vector<string> trust_obs)
{
	for (int i = 1; i < trust_obs.size() - 1; i++)
	{
		string trust = trust_obs[i];
		if (trust.size() == 0)
			continue;
		int l = 0;
		while (trust.at(l) != ',')
			l++;
		string person1 = trust.substr(0, l);
		l++;
		int p = l;
		while (trust.at(l) != ',')
			l++;
		string person2 = trust.substr(p, l);
		l++;
		string weight = trust.substr(l, trust.length() - l);

		int p1 = stringToInt(person1);
		int p2 = stringToInt(person2);

		inserttrust(1, p1, p2);
	}
	cout << endl;
}

void TupleToQuery()
{
	Ptr<RapidNetApplicationBase> queryNode = queryapps.Get(0)->GetObject<RapidNetApplicationBase>();
	inserttuple(1, "mutualTrustPath", 1, 1, 2);
}

void Print()
{
	// PrintRelation(apps, Trust::TRUST);
	// PrintRelation(apps, Trust::TRUSTPATH);
	PrintRelation(apps, Trust::MUTUALTRUSTPATH);

	// PrintRelation (queryapps, TrustQuery::TUPLE);
	PrintRelation(queryapps, TrustQuery::RECORDS); //modify: add col tuple's vid (hash)
}

void train()
{
	vector<string> trust_train = readFile(TrustTrain_30);
	parse(trust_train);
}

int main(int argc, char *argv[])
{

	// LogComponentEnable("Trust", LOG_LEVEL_INFO);
	// LogComponentEnable("TrustQuery", LOG_LEVEL_INFO);
	// LogComponentEnable("RapidNetApplicationBase", LOG_LEVEL_INFO);
	// for (int i=0; i<2; i++){

	initApps();

	apps.Start(Seconds(0.0));
	apps.Stop(Seconds(10.0));
	queryapps.Start(Seconds(0.0));
	queryapps.Stop(Seconds(10.0));

	schedule(1.0, TupleToQuery);
	schedule(2.0, train);
	schedule(5.0, Print);

	Simulator::Run();
	Simulator::Destroy();

	// }
	return 0;
}
