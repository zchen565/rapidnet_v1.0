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
#include "ns3/vqap-module.h"
#include "ns3/vqap-query-module.h"
#include <time.h>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <map>

#define WordObs \
	"./data/vqa/skyscraper/word_obs.txt"

#define HasImgObs \
	"./data/vqa/skyscraper/hasimg_obs.txt"

#define HasQObs \
	"./data/vqa/skyscraper/hasq_obs.txt"

#define SimObs \
	"./data/vqa/skyscraper/sim_obs.txt"

#define AnsTar \
	"./data/vqa/skyscraper/ans_target.txt"

#define word(local, object)                      \
	rtuple(Vqap::WORD,                           \
		   attr("word_attr1", Ipv4Value, local), \
		   attr("word_attr2", StrValue, object))

#define hasImg(local, object1, relation, object2)    \
	rtuple(Vqap::HASIMG,                             \
		   attr("hasImg_attr1", Ipv4Value, local),   \
		   attr("hasImg_attr2", StrValue, object1),  \
		   attr("hasImg_attr3", StrValue, relation), \
		   attr("hasImg_attr4", StrValue, object2))

#define hasQ(local, object1, relation, object2)    \
	rtuple(Vqap::HASQ,                             \
		   attr("hasQ_attr1", Ipv4Value, local),   \
		   attr("hasQ_attr2", StrValue, object1),  \
		   attr("hasQ_attr3", StrValue, relation), \
		   attr("hasQ_attr4", StrValue, object2))

#define sim(local, object1, object2)             \
	rtuple(Vqap::SIM,                            \
		   attr("sim_attr1", Ipv4Value, local),  \
		   attr("sim_attr2", StrValue, object1), \
		   attr("sim_attr3", StrValue, object2))

#define candidate(local, candidate)                   \
	rtuple(Vqap::CANDIDATE,                           \
		   attr("candidate_attr1", Ipv4Value, local), \
		   attr("candidate_attr2", StrValue, candidate))

#define hasImgAns(local, candidate, object1, relation, object2) \
	rtuple(Vqap::HASIMGANS,                                     \
		   attr("hasImgAns_attr1", Ipv4Value, local),           \
		   attr("hasImgAns_attr2", StrValue, candidate),        \
		   attr("hasImgAns_attr3", StrValue, object1),          \
		   attr("hasImgAns_attr4", StrValue, relaiton),         \
		   attr("hasImgAns_attr5", StrValue, object2))

#define ans(local, answer)                      \
	rtuple(Vqap::ANS,                           \
		   attr("ans_attr1", Ipv4Value, local), \
		   attr("ans_attr2", StrValue, answer))
/*
#define sharesult(local, vid, content) \
rtuple (Vqap::SHARESULT, \
attr ("shaResult_attr1", Ipv4Value, local), \
attr ("shaResult_attr2", IdValue, vid), \
attr ("shaResult_attr3", StrValue, content))
*/

#define insertword(local, object) \
	app(local)->Insert(word(addr(local), object));

#define inserthasImg(local, object1, relation, object2) \
	app(local)->Insert(hasImg(addr(local), object1, relation, object2));

#define inserthasQ(local, object1, relation, object2) \
	app(local)->Insert(hasQ(addr(local), object1, relation, object2));

#define insertsim(local, object1, object2) \
	app(local)->Insert(sim(addr(local), object1, object2));

#define insertcandidate(local, candidate) \
	app(local)->Insert(candidate(addr(local), candidate));

#define inserthasImgAns(local, candidate, object1, relation, object2) \
	app(local)->Insert(hasImgAns(addr(local), candidate, object1, relation, object2));

#define insertans(local, answer) \
	app(local)->Insert(ans(addr(local), answer));

/*
#define insertsharesult(local, vid, content) \
app(local)->Insert(sharesult(addr(local), vid, content));
*/

//define the rtuple you would like to query and how to insert it
#define tupleQuery(loc, name, attr1, attr2)       \
	rtuple(VqapQuery::TUPLE,                      \
		   attr("tuple_attr1", Ipv4Value, loc),   \
		   attr("tuple_attr2", StrValue, name),   \
		   attr("tuple_attr3", Ipv4Value, attr1), \
		   attr("tuple_attr4", StrValue, attr2))

#define inserttuple(loc, name, attr1, attr2) \
	queryNode->Insert(tupleQuery(queryNode->GetAddress(), name, addr(attr1), attr2));

using namespace std;
using namespace ns3;
using namespace ns3::rapidnet;
using namespace ns3::rapidnet::vqap;
using namespace ns3::rapidnet::vqapquery;

ApplicationContainer apps;
ApplicationContainer queryapps;

void initApps()
{
	NodeContainer mainAppNodes;
	mainAppNodes.Create(1);

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

	apps.Add(Create<VqapHelper>()->Install(mainAppNodes));
	queryapps.Add(Create<VqapQueryHelper>()->Install(queryAppNodes));

	SetMaxJitter(apps, 0.001);
	SetMaxJitter(queryapps, 0.001);
}

vector<string>
readFile(string filename)
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

vector<string> split(string str, char delimiter)
{
	vector<string> v;
	stringstream ss(str); // Turn the string into a stream.
	string tok;

	while (getline(ss, tok, delimiter))
	{
		v.push_back(tok);
	}

	return v;
}

void parse(vector<string> word_obs, vector<string> hasImg_obs, vector<string> hasQ_obs, vector<string> sim_obs)
{

	// first parse word_obs
	cout << "word size: " << word_obs.size() << endl;
	for (int i = 0; i < word_obs.size(); i++)
	{
		string line = word_obs[i];
		if (line.size() == 0)
			continue;
		vector<string> tokens = split(line, '\t');
		double p = atof(tokens.back().c_str());
		insertword(1, tokens[0]);
	}

	// parse hasImg_obs
	cout << "hasImg size: " << hasImg_obs.size() << endl;
	for (int i = 0; i < hasImg_obs.size(); i++)
	{
		cout << hasImg_obs[i] << endl;
		string line = hasImg_obs[i];
		if (line.size() == 0)
			continue;
		vector<string> tokens = split(line, ' ');
		double p = atof(tokens.back().c_str());
		inserthasImg(1, tokens[0], tokens[1], tokens[2]);
	}

	//parse hasQ_obs
	cout << "hasQ size: " << hasQ_obs.size() << endl;
	for (int i = 0; i < hasQ_obs.size(); i++)
	{
		string line = hasQ_obs[i];
		if (line.size() == 0)
			continue;
		vector<string> tokens = split(line, ' ');
		cout << tokens[0] << endl;
		double p = atof(tokens.back().c_str());
		inserthasQ(1, tokens[0], tokens[1], tokens[2]);
	}

	//parse sim_obs
	cout << "sim size: " << sim_obs.size() << endl;
	for (int i = 0; i < sim_obs.size(); i++)
	{
		string line = sim_obs[i];
		if (line.size() == 0)
			continue;
		vector<string> tokens = split(line, '	');
		double p = atof(tokens.back().c_str());
		insertsim(1, tokens[0], tokens[1]);
	}
}

void Train()
{
	vector<string> word_obs = readFile(WordObs);
	vector<string> hasImg_obs = readFile(HasImgObs);
	vector<string> hasQ_obs = readFile(HasQObs);
	vector<string> sim_obs = readFile(SimObs);
	// vector<string> hasImg_obs, hasQ_obs, sim_obs;
	parse(word_obs, hasImg_obs, hasQ_obs, sim_obs);
}

vector<string> answers = readFile(AnsTar);
int n = 0;

void TupleToQuery()
{
	Ptr<RapidNetApplicationBase> queryNode = queryapps.Get(0)->GetObject<RapidNetApplicationBase>();
	inserttuple(1, "ans", 1, answers[n]);
	n++;
}

void SingleTupleToQuery()
{
	Ptr<RapidNetApplicationBase> queryNode = queryapps.Get(0)->GetObject<RapidNetApplicationBase>();
	inserttuple(1, "ans", 1, "skyscraper");
}

void Print()
{
	PrintRelation(apps, Vqap::WORD);
	PrintRelation(apps, Vqap::SIM);
	PrintRelation(apps, Vqap::HASIMG);
	PrintRelation(apps, Vqap::HASQ);
	PrintRelation(apps, Vqap::ANS);
	PrintRelation(apps, Vqap::CANDIDATE);
	PrintRelation(apps, Vqap::RRETURN);
	// PrintRelation(apps, Vqap::SHARESULT);

	// PrintRelation(apps, Vqap::PROV);
	PrintRelation(queryapps, VqapQuery::TUPLE);
	PrintRelation(queryapps, VqapQuery::TEMP);
	PrintRelation(apps, Vqap::PROVQUERY);
	PrintRelation(queryapps, VqapQuery::RECORDS);
}

int main(int argc, char *argv[])
{

	//LogComponentEnable("Vqap", LOG_LEVEL_INFO);
	//LogComponentEnable("VqapQuery", LOG_LEVEL_INFO);
	//LogComponentEnable("RapidNetApplicationBase", LOG_LEVEL_INFO);

	initApps();

	apps.Start(Seconds(0.0));
	apps.Stop(Seconds(100.0));
	queryapps.Start(Seconds(0.0));
	queryapps.Stop(Seconds(100.0));

	schedule(2.0, Train);
	for (int i = 0; i < answers.size() - 1; i++)
	{
		schedule(5.0 + i, TupleToQuery);
	}
	// schedule (5.0, SingleTupleToQuery);
	schedule(90.0, Print);

	Simulator::Run();
	Simulator::Destroy();

	return 0;
}
