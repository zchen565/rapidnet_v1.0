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
#include "ns3/list-value.h"
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <list> 


#define SmokeTrain \
"./data/smoking-train.db"

#define SmokeTest \
"./data/smoking-test.db"


#define smoke(local, person) \
tuple (Smoke::SMOKEEVENT, \
attr ("smokeEvent_attr1", Ipv4Value, local), \
attr ("smokeEvent_attr2", Int32Value, person) \
)

#define cancer(local, person) \
tuple (Smoke::CANCER, \
attr ("cancer_attr1", Ipv4Value, local), \
attr ("cancer_attr2", Int32Value, person) \
)

#define friends(local, person1, person2) \
tuple (Smoke::Friends, \
attr ("friends_attr1", Ipv4Value, local), \
attr ("friends_attr2", Int32Value, person1), \
attr ("friends_attr3", Int32Value, person2) \
)



#define insertsmoke(local, person) \
app(local)->Insert(smoke(addr(local), person))

#define insertcancer(local, person) \
app(local)->Insert(cancer(addr(local), person))

#define insertfriends(local, person1, person2) \
app(local)->Insert(friends(addr(local), person1, person2))



using namespace std;
using namespace ns3;
using namespace ns3::rapidnet;
using namespace ns3::rapidnet::smoke;


ApplicationContainer apps;



void train() {
  

}




int main(int argc, char *argv[]){

	apps = InitRapidNetApps (3, Create<AcquaintanceHelper> ());
	SetMaxJitter (apps, 0.001);

	apps.Start (Seconds (0.0));
	apps.Stop (Seconds (10.0));

	schedule (2.0, train);
	schedule (5.0, Print);

	Simulator::Run ();
	Simulator::Destroy ();

	return 0;

}
	






