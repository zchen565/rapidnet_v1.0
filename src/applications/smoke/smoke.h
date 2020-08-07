/* A RapidNet application. Generated by RapidNet compiler. */

#ifndef SMOKE_H
#define SMOKE_H

#include <string>
#include <iostream>
#include "ns3/ptr.h"
#include "ns3/event-id.h"
#include "ns3/ipv4-address.h"
#include "ns3/rapidnet-header.h"
#include "ns3/relation-base.h"
#include "ns3/database.h"
#include "ns3/rapidnet-application-base.h"
#include "ns3/aggregator.h"
#include "ns3/aggwrap.h"

using namespace std;
using namespace ns3;

namespace ns3 {

class Socket;
class Packet;

namespace rapidnet {
namespace smoke {

class Smoke : public RapidNetApplicationBase
{
public:
  static const string CANCER;
  static const string EPRETURN;
  static const string EPROVQUERY;
  static const string ERRETURN;
  static const string ERULEQUERY;
  static const string ECANCER;
  static const string ECANCERDELETE;
  static const string ECANCERTEMP;
  static const string ECANCERTEMPDELETE;
  static const string ESHARESULT;
  static const string ESHARESULTDELETE;
  static const string ESHARESULTTEMP;
  static const string ESHARESULTTEMPDELETE;
  static const string ESMOKE;
  static const string ESMOKEDELETE;
  static const string ESMOKETEMP;
  static const string ESMOKETEMPDELETE;
  static const string FRIENDS;
  static const string PITERATE;
  static const string PQLIST;
  static const string PRESULTTMP;
  static const string PRETURN;
  static const string PROV;
  static const string PROVQUERY;
  static const string RITERATE;
  static const string RQLIST;
  static const string RRESULTTMP;
  static const string RRETURN;
  static const string RULEEXEC;
  static const string RULEQUERY;
  static const string SHARESULT;
  static const string SMOKE;
  static const string SMOKEEVENT;

  static TypeId GetTypeId (void);

  Smoke ();

  virtual ~Smoke ();

protected:

  virtual void DoDispose (void);

  virtual void StartApplication (void);

  virtual void StopApplication (void);

  virtual void InitDatabase (void);

  virtual void DemuxRecv (Ptr<Tuple> tuple);

  virtual void Prov_r1_1Eca0Ins (Ptr<Tuple> smoke);

  virtual void Prov_r1_1Eca0Del (Ptr<Tuple> smoke);

  virtual void Prov_r1_2_ecaAdd (Ptr<Tuple> ecancerTemp);

  virtual void Prov_r1_2_ecaDel (Ptr<Tuple> ecancerTempDelete);

  virtual void Prov_r1_3_ecaAdd (Ptr<Tuple> ecancerTemp);

  virtual void Prov_r1_3_ecaDel (Ptr<Tuple> ecancerTempDelete);

  virtual void Prov_r1_4_ecaAdd (Ptr<Tuple> ecancer);

  virtual void Prov_r1_4_ecaDel (Ptr<Tuple> ecancerDelete);

  virtual void Prov_r1_5_ecaAdd (Ptr<Tuple> ecancer);

  virtual void Prov_r1_5_ecaDel (Ptr<Tuple> ecancerDelete);

  virtual void Prov_r2_1Eca0Ins (Ptr<Tuple> friends);

  virtual void Prov_r2_1Eca0Del (Ptr<Tuple> friends);

  virtual void Prov_r2_1Eca3Ins (Ptr<Tuple> smoke);

  virtual void Prov_r2_1Eca3Del (Ptr<Tuple> smoke);

  virtual void Prov_r2_2_ecaAdd (Ptr<Tuple> esmokeTemp);

  virtual void Prov_r2_2_ecaDel (Ptr<Tuple> esmokeTempDelete);

  virtual void Prov_r2_3_ecaAdd (Ptr<Tuple> esmokeTemp);

  virtual void Prov_r2_3_ecaDel (Ptr<Tuple> esmokeTempDelete);

  virtual void Prov_r2_4_ecaAdd (Ptr<Tuple> esmoke);

  virtual void Prov_r2_4_ecaDel (Ptr<Tuple> esmokeDelete);

  virtual void Prov_r2_5_ecaAdd (Ptr<Tuple> esmoke);

  virtual void Prov_r2_5_ecaDel (Ptr<Tuple> esmokeDelete);

  virtual void Prov_r3_1Eca0Ins (Ptr<Tuple> friends);

  virtual void Prov_r3_1Eca0Del (Ptr<Tuple> friends);

  virtual void Prov_r3_1Eca3Ins (Ptr<Tuple> smoke);

  virtual void Prov_r3_1Eca3Del (Ptr<Tuple> smoke);

  virtual void Prov_r11_1Eca0Ins (Ptr<Tuple> smoke);

  virtual void Prov_r11_1Eca0Del (Ptr<Tuple> smoke);

  virtual void Prov_r11_2_ecaAdd (Ptr<Tuple> eshaResultTemp);

  virtual void Prov_r11_2_ecaDel (Ptr<Tuple> eshaResultTempDelete);

  virtual void Prov_r11_3_ecaAdd (Ptr<Tuple> eshaResultTemp);

  virtual void Prov_r11_3_ecaDel (Ptr<Tuple> eshaResultTempDelete);

  virtual void Prov_r11_4_ecaAdd (Ptr<Tuple> eshaResult);

  virtual void Prov_r11_4_ecaDel (Ptr<Tuple> eshaResultDelete);

  virtual void Prov_r11_5_ecaAdd (Ptr<Tuple> eshaResult);

  virtual void Prov_r11_5_ecaDel (Ptr<Tuple> eshaResultDelete);

  virtual void Prov_r12_1Eca0Ins (Ptr<Tuple> cancer);

  virtual void Prov_r12_1Eca0Del (Ptr<Tuple> cancer);

  virtual void Prov_r13_1Eca0Ins (Ptr<Tuple> friends);

  virtual void Prov_r13_1Eca0Del (Ptr<Tuple> friends);

  virtual void Prov_r14_1Eca0Ins (Ptr<Tuple> smokeEvent);

  virtual void Prov_r14_1Eca0Del (Ptr<Tuple> smokeEvent);

  virtual void Prov_edb_1Eca1Ins (Ptr<Tuple> friends);

  virtual void Prov_edb_1Eca1Del (Ptr<Tuple> friends);

  virtual void Prov_edb_2Eca1Ins (Ptr<Tuple> smokeEvent);

  virtual void Prov_edb_2Eca1Del (Ptr<Tuple> smokeEvent);

  virtual void Edb1_eca (Ptr<Tuple> provQuery);

  virtual void Idb1a_eca (Ptr<Tuple> provQuery);

  virtual void Idb1b_eca (Ptr<Tuple> provQuery);

  virtual void Idb2_eca (Ptr<Tuple> provQuery);

  virtual void Idb3_eca (Ptr<Tuple> provQuery);

  virtual void Idb4_eca (Ptr<Tuple> pIterate);

  virtual void Idb5_eca (Ptr<Tuple> pIterate);

  virtual void Idb6_eca (Ptr<Tuple> eRuleQuery);

  virtual void Idb7_eca (Ptr<Tuple> rReturn);

  virtual void Idb8Eca0Ins (Ptr<Tuple> pResultTmp);

  virtual void Idb8Eca1Ins (Ptr<Tuple> pQList);

  virtual void Idb9a_eca (Ptr<Tuple> ePReturn);

  virtual void Rv1_eca (Ptr<Tuple> ruleQuery);

  virtual void Rv2_eca (Ptr<Tuple> ruleQuery);

  virtual void Rv3_eca (Ptr<Tuple> ruleQuery);

  virtual void Rv4_eca (Ptr<Tuple> rIterate);

  virtual void Rv5_eca (Ptr<Tuple> rIterate);

  virtual void Rv6_eca (Ptr<Tuple> eProvQuery);

  virtual void Rv7_eca (Ptr<Tuple> pReturn);

  virtual void Rv8Eca0Ins (Ptr<Tuple> rResultTmp);

  virtual void Rv8Eca1Ins (Ptr<Tuple> rQList);

  virtual void Rv9_eca (Ptr<Tuple> eRReturn);

};

} // namespace smoke
} // namespace rapidnet
} // namespace ns3

#endif // SMOKE_H
