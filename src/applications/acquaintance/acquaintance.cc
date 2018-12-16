/* A RapidNet application. Generated by RapidNet compiler. */

#include "acquaintance.h"
#include <cstdlib>
#include "ns3/nstime.h"
#include "ns3/simulator.h"
#include "ns3/type-ids.h"
#include "ns3/rapidnet-types.h"
#include "ns3/rapidnet-utils.h"
#include "ns3/assignor.h"
#include "ns3/selector.h"
#include "ns3/rapidnet-functions.h"

using namespace std;
using namespace ns3;
using namespace ns3::rapidnet;
using namespace ns3::rapidnet::acquaintance;

const string Acquaintance::KNOW = "know";
const string Acquaintance::KNOWEVENT = "knowEvent";
const string Acquaintance::LIKE = "like";
const string Acquaintance::LIKEEVENT = "likeEvent";
const string Acquaintance::LIVE = "live";
const string Acquaintance::LIVEEVENT = "liveEvent";
const string Acquaintance::RELATION = "relation";

NS_LOG_COMPONENT_DEFINE ("Acquaintance");
NS_OBJECT_ENSURE_REGISTERED (Acquaintance);

TypeId
Acquaintance::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::rapidnet::acquaintance::Acquaintance")
    .SetParent<RapidNetApplicationBase> ()
    .AddConstructor<Acquaintance> ()
    ;
  return tid;
}

Acquaintance::Acquaintance()
{
  NS_LOG_FUNCTION_NOARGS ();
}

Acquaintance::~Acquaintance()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
Acquaintance::DoDispose (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  RapidNetApplicationBase::DoDispose ();
}

void
Acquaintance::StartApplication (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  RapidNetApplicationBase::StartApplication ();
  RAPIDNET_LOG_INFO("Acquaintance Application Started");
}

void
Acquaintance::StopApplication ()
{
  NS_LOG_FUNCTION_NOARGS ();

  RapidNetApplicationBase::StopApplication ();
  RAPIDNET_LOG_INFO("Acquaintance Application Stopped");
}

void
Acquaintance::InitDatabase ()
{
  //RapidNetApplicationBase::InitDatabase ();

  AddRelationWithKeys (KNOW, attrdeflist (
    attrdef ("know_attr1", IPV4),
    attrdef ("know_attr2", INT32),
    attrdef ("know_attr3", INT32),
    attrdef ("know_attr4", INT32)));

  AddRelationWithKeys (KNOWEVENT, attrdeflist (
    attrdef ("knowEvent_attr1", IPV4),
    attrdef ("knowEvent_attr2", INT32),
    attrdef ("knowEvent_attr3", INT32),
    attrdef ("knowEvent_attr4", INT32)));

  AddRelationWithKeys (LIKE, attrdeflist (
    attrdef ("like_attr1", IPV4),
    attrdef ("like_attr2", INT32),
    attrdef ("like_attr3", INT32)));

  AddRelationWithKeys (LIKEEVENT, attrdeflist (
    attrdef ("likeEvent_attr1", IPV4),
    attrdef ("likeEvent_attr2", INT32),
    attrdef ("likeEvent_attr3", INT32)));

  AddRelationWithKeys (LIVE, attrdeflist (
    attrdef ("live_attr1", IPV4),
    attrdef ("live_attr2", INT32),
    attrdef ("live_attr3", INT32)));

  AddRelationWithKeys (LIVEEVENT, attrdeflist (
    attrdef ("liveEvent_attr1", IPV4),
    attrdef ("liveEvent_attr2", INT32),
    attrdef ("liveEvent_attr3", INT32)));

  AddRelationWithKeys (RELATION, attrdeflist (
    attrdef ("relation_attr1", IPV4),
    attrdef ("relation_attr2", INT32),
    attrdef ("relation_attr3", INT32)));

}

void
Acquaintance::DemuxRecv (Ptr<Tuple> tuple)
{
  RapidNetApplicationBase::DemuxRecv (tuple);

  if (IsInsertEvent (tuple, LIVEEVENT))
    {
      RaEca0Ins (tuple);
    }
  if (IsDeleteEvent (tuple, LIVEEVENT))
    {
      RaEca0Del (tuple);
    }
  if (IsInsertEvent (tuple, LIKEEVENT))
    {
      RbEca0Ins (tuple);
    }
  if (IsDeleteEvent (tuple, LIKEEVENT))
    {
      RbEca0Del (tuple);
    }
  if (IsInsertEvent (tuple, KNOWEVENT))
    {
      RcEca0Ins (tuple);
    }
  if (IsDeleteEvent (tuple, KNOWEVENT))
    {
      RcEca0Del (tuple);
    }
  if (IsInsertEvent (tuple, KNOWEVENT))
    {
      RdEca0Ins (tuple);
    }
  if (IsDeleteEvent (tuple, KNOWEVENT))
    {
      RdEca0Del (tuple);
    }
  if (IsInsertEvent (tuple, LIVEEVENT))
    {
      R1Eca0Ins (tuple);
    }
  if (IsDeleteEvent (tuple, LIVEEVENT))
    {
      R1Eca0Del (tuple);
    }
  if (IsInsertEvent (tuple, LIVE))
    {
      R1Eca1Ins (tuple);
    }
  if (IsDeleteEvent (tuple, LIVE))
    {
      R1Eca1Del (tuple);
    }
  if (IsInsertEvent (tuple, LIKEEVENT))
    {
      R2Eca0Ins (tuple);
    }
  if (IsDeleteEvent (tuple, LIKEEVENT))
    {
      R2Eca0Del (tuple);
    }
  if (IsInsertEvent (tuple, LIKE))
    {
      R2Eca1Ins (tuple);
    }
  if (IsDeleteEvent (tuple, LIKE))
    {
      R2Eca1Del (tuple);
    }
  if (IsInsertEvent (tuple, RELATION))
    {
      R4Eca0Ins (tuple);
    }
  if (IsDeleteEvent (tuple, RELATION))
    {
      R4Eca0Del (tuple);
    }
  if (IsInsertEvent (tuple, LIVEEVENT))
    {
      R5Eca0Ins (tuple);
    }
  if (IsDeleteEvent (tuple, LIVEEVENT))
    {
      R5Eca0Del (tuple);
    }
  if (IsInsertEvent (tuple, LIVE))
    {
      R5Eca1Ins (tuple);
    }
  if (IsDeleteEvent (tuple, LIVE))
    {
      R5Eca1Del (tuple);
    }
}

void
Acquaintance::RaEca0Ins (Ptr<Tuple> liveEvent)
{
  RAPIDNET_LOG_INFO ("RaEca0Ins triggered");

  Ptr<Tuple> result = liveEvent;

  result->Assign (Assignor::New ("Local",
    LOCAL_ADDRESS));

  result = result->Project (
    LIVE,
    strlist ("Local",
      "liveEvent_attr2",
      "liveEvent_attr3"),
    strlist ("live_attr1",
      "live_attr2",
      "live_attr3"));

  Insert (result);
}

void
Acquaintance::RaEca0Del (Ptr<Tuple> liveEvent)
{
  RAPIDNET_LOG_INFO ("RaEca0Del triggered");

  Ptr<Tuple> result = liveEvent;

  result->Assign (Assignor::New ("Local",
    LOCAL_ADDRESS));

  result = result->Project (
    LIVE,
    strlist ("Local",
      "liveEvent_attr2",
      "liveEvent_attr3"),
    strlist ("live_attr1",
      "live_attr2",
      "live_attr3"));

  Delete (result);
}

void
Acquaintance::RbEca0Ins (Ptr<Tuple> likeEvent)
{
  RAPIDNET_LOG_INFO ("RbEca0Ins triggered");

  Ptr<Tuple> result = likeEvent;

  result->Assign (Assignor::New ("Local",
    LOCAL_ADDRESS));

  result = result->Project (
    LIKE,
    strlist ("Local",
      "likeEvent_attr2",
      "likeEvent_attr3"),
    strlist ("like_attr1",
      "like_attr2",
      "like_attr3"));

  Insert (result);
}

void
Acquaintance::RbEca0Del (Ptr<Tuple> likeEvent)
{
  RAPIDNET_LOG_INFO ("RbEca0Del triggered");

  Ptr<Tuple> result = likeEvent;

  result->Assign (Assignor::New ("Local",
    LOCAL_ADDRESS));

  result = result->Project (
    LIKE,
    strlist ("Local",
      "likeEvent_attr2",
      "likeEvent_attr3"),
    strlist ("like_attr1",
      "like_attr2",
      "like_attr3"));

  Delete (result);
}

void
Acquaintance::RcEca0Ins (Ptr<Tuple> knowEvent)
{
  RAPIDNET_LOG_INFO ("RcEca0Ins triggered");

  Ptr<Tuple> result = knowEvent;

  result->Assign (Assignor::New ("Local",
    LOCAL_ADDRESS));

  result = result->Project (
    KNOW,
    strlist ("Local",
      "knowEvent_attr2",
      "knowEvent_attr3",
      "knowEvent_attr4"),
    strlist ("know_attr1",
      "know_attr2",
      "know_attr3",
      "know_attr4"));

  Insert (result);
}

void
Acquaintance::RcEca0Del (Ptr<Tuple> knowEvent)
{
  RAPIDNET_LOG_INFO ("RcEca0Del triggered");

  Ptr<Tuple> result = knowEvent;

  result->Assign (Assignor::New ("Local",
    LOCAL_ADDRESS));

  result = result->Project (
    KNOW,
    strlist ("Local",
      "knowEvent_attr2",
      "knowEvent_attr3",
      "knowEvent_attr4"),
    strlist ("know_attr1",
      "know_attr2",
      "know_attr3",
      "know_attr4"));

  Delete (result);
}

void
Acquaintance::RdEca0Ins (Ptr<Tuple> knowEvent)
{
  RAPIDNET_LOG_INFO ("RdEca0Ins triggered");

  Ptr<Tuple> result = knowEvent;

  result->Assign (Assignor::New ("Local",
    LOCAL_ADDRESS));

  result = result->Project (
    KNOW,
    strlist ("Local",
      "knowEvent_attr3",
      "knowEvent_attr2",
      "knowEvent_attr4"),
    strlist ("know_attr1",
      "know_attr2",
      "know_attr3",
      "know_attr4"));

  Insert (result);
}

void
Acquaintance::RdEca0Del (Ptr<Tuple> knowEvent)
{
  RAPIDNET_LOG_INFO ("RdEca0Del triggered");

  Ptr<Tuple> result = knowEvent;

  result->Assign (Assignor::New ("Local",
    LOCAL_ADDRESS));

  result = result->Project (
    KNOW,
    strlist ("Local",
      "knowEvent_attr3",
      "knowEvent_attr2",
      "knowEvent_attr4"),
    strlist ("know_attr1",
      "know_attr2",
      "know_attr3",
      "know_attr4"));

  Delete (result);
}

void
Acquaintance::R1Eca0Ins (Ptr<Tuple> liveEvent)
{
  RAPIDNET_LOG_INFO ("R1Eca0Ins triggered");

  Ptr<RelationBase> result;

  result = GetRelation (LIVE)->Join (
    liveEvent,
    strlist ("live_attr3", "live_attr1"),
    strlist ("liveEvent_attr3", "liveEvent_attr1"));

  result->Assign (Assignor::New ("S",
    ValueExpr::New (Int32Value::New (1))));

  result->Assign (Assignor::New ("Local",
    LOCAL_ADDRESS));

  result = result->Select (Selector::New (
    Operation::New (RN_NEQ,
      VarExpr::New ("liveEvent_attr2"),
      VarExpr::New ("live_attr2"))));

  result = result->Project (
    KNOWEVENT,
    strlist ("Local",
      "liveEvent_attr2",
      "live_attr2",
      "S"),
    strlist ("knowEvent_attr1",
      "knowEvent_attr2",
      "knowEvent_attr3",
      "knowEvent_attr4"));

  Insert (result);
}

void
Acquaintance::R1Eca0Del (Ptr<Tuple> liveEvent)
{
  RAPIDNET_LOG_INFO ("R1Eca0Del triggered");

  Ptr<RelationBase> result;

  result = GetRelation (LIVE)->Join (
    liveEvent,
    strlist ("live_attr3", "live_attr1"),
    strlist ("liveEvent_attr3", "liveEvent_attr1"));

  result->Assign (Assignor::New ("S",
    ValueExpr::New (Int32Value::New (1))));

  result->Assign (Assignor::New ("Local",
    LOCAL_ADDRESS));

  result = result->Select (Selector::New (
    Operation::New (RN_NEQ,
      VarExpr::New ("liveEvent_attr2"),
      VarExpr::New ("live_attr2"))));

  result = result->Project (
    KNOWEVENT,
    strlist ("Local",
      "liveEvent_attr2",
      "live_attr2",
      "S"),
    strlist ("knowEvent_attr1",
      "knowEvent_attr2",
      "knowEvent_attr3",
      "knowEvent_attr4"));

  Delete (result);
}

void
Acquaintance::R1Eca1Ins (Ptr<Tuple> live)
{
  RAPIDNET_LOG_INFO ("R1Eca1Ins triggered");

  Ptr<RelationBase> result;

  result = GetRelation (LIVEEVENT)->Join (
    live,
    strlist ("liveEvent_attr3", "liveEvent_attr1"),
    strlist ("live_attr3", "live_attr1"));

  result->Assign (Assignor::New ("S",
    ValueExpr::New (Int32Value::New (1))));

  result->Assign (Assignor::New ("Local",
    LOCAL_ADDRESS));

  result = result->Select (Selector::New (
    Operation::New (RN_NEQ,
      VarExpr::New ("liveEvent_attr2"),
      VarExpr::New ("live_attr2"))));

  result = result->Project (
    KNOWEVENT,
    strlist ("Local",
      "liveEvent_attr2",
      "live_attr2",
      "S"),
    strlist ("knowEvent_attr1",
      "knowEvent_attr2",
      "knowEvent_attr3",
      "knowEvent_attr4"));

  Insert (result);
}

void
Acquaintance::R1Eca1Del (Ptr<Tuple> live)
{
  RAPIDNET_LOG_INFO ("R1Eca1Del triggered");

  Ptr<RelationBase> result;

  result = GetRelation (LIVEEVENT)->Join (
    live,
    strlist ("liveEvent_attr3", "liveEvent_attr1"),
    strlist ("live_attr3", "live_attr1"));

  result->Assign (Assignor::New ("S",
    ValueExpr::New (Int32Value::New (1))));

  result->Assign (Assignor::New ("Local",
    LOCAL_ADDRESS));

  result = result->Select (Selector::New (
    Operation::New (RN_NEQ,
      VarExpr::New ("liveEvent_attr2"),
      VarExpr::New ("live_attr2"))));

  result = result->Project (
    KNOWEVENT,
    strlist ("Local",
      "liveEvent_attr2",
      "live_attr2",
      "S"),
    strlist ("knowEvent_attr1",
      "knowEvent_attr2",
      "knowEvent_attr3",
      "knowEvent_attr4"));

  Delete (result);
}

void
Acquaintance::R2Eca0Ins (Ptr<Tuple> likeEvent)
{
  RAPIDNET_LOG_INFO ("R2Eca0Ins triggered");

  Ptr<RelationBase> result;

  result = GetRelation (LIKE)->Join (
    likeEvent,
    strlist ("like_attr3", "like_attr1"),
    strlist ("likeEvent_attr3", "likeEvent_attr1"));

  result->Assign (Assignor::New ("S",
    ValueExpr::New (Int32Value::New (1))));

  result->Assign (Assignor::New ("Local",
    LOCAL_ADDRESS));

  result = result->Select (Selector::New (
    Operation::New (RN_NEQ,
      VarExpr::New ("likeEvent_attr2"),
      VarExpr::New ("like_attr2"))));

  result = result->Project (
    KNOWEVENT,
    strlist ("Local",
      "likeEvent_attr2",
      "like_attr2",
      "S"),
    strlist ("knowEvent_attr1",
      "knowEvent_attr2",
      "knowEvent_attr3",
      "knowEvent_attr4"));

  Insert (result);
}

void
Acquaintance::R2Eca0Del (Ptr<Tuple> likeEvent)
{
  RAPIDNET_LOG_INFO ("R2Eca0Del triggered");

  Ptr<RelationBase> result;

  result = GetRelation (LIKE)->Join (
    likeEvent,
    strlist ("like_attr3", "like_attr1"),
    strlist ("likeEvent_attr3", "likeEvent_attr1"));

  result->Assign (Assignor::New ("S",
    ValueExpr::New (Int32Value::New (1))));

  result->Assign (Assignor::New ("Local",
    LOCAL_ADDRESS));

  result = result->Select (Selector::New (
    Operation::New (RN_NEQ,
      VarExpr::New ("likeEvent_attr2"),
      VarExpr::New ("like_attr2"))));

  result = result->Project (
    KNOWEVENT,
    strlist ("Local",
      "likeEvent_attr2",
      "like_attr2",
      "S"),
    strlist ("knowEvent_attr1",
      "knowEvent_attr2",
      "knowEvent_attr3",
      "knowEvent_attr4"));

  Delete (result);
}

void
Acquaintance::R2Eca1Ins (Ptr<Tuple> like)
{
  RAPIDNET_LOG_INFO ("R2Eca1Ins triggered");

  Ptr<RelationBase> result;

  result = GetRelation (LIKEEVENT)->Join (
    like,
    strlist ("likeEvent_attr3", "likeEvent_attr1"),
    strlist ("like_attr3", "like_attr1"));

  result->Assign (Assignor::New ("S",
    ValueExpr::New (Int32Value::New (1))));

  result->Assign (Assignor::New ("Local",
    LOCAL_ADDRESS));

  result = result->Select (Selector::New (
    Operation::New (RN_NEQ,
      VarExpr::New ("likeEvent_attr2"),
      VarExpr::New ("like_attr2"))));

  result = result->Project (
    KNOWEVENT,
    strlist ("Local",
      "likeEvent_attr2",
      "like_attr2",
      "S"),
    strlist ("knowEvent_attr1",
      "knowEvent_attr2",
      "knowEvent_attr3",
      "knowEvent_attr4"));

  Insert (result);
}

void
Acquaintance::R2Eca1Del (Ptr<Tuple> like)
{
  RAPIDNET_LOG_INFO ("R2Eca1Del triggered");

  Ptr<RelationBase> result;

  result = GetRelation (LIKEEVENT)->Join (
    like,
    strlist ("likeEvent_attr3", "likeEvent_attr1"),
    strlist ("like_attr3", "like_attr1"));

  result->Assign (Assignor::New ("S",
    ValueExpr::New (Int32Value::New (1))));

  result->Assign (Assignor::New ("Local",
    LOCAL_ADDRESS));

  result = result->Select (Selector::New (
    Operation::New (RN_NEQ,
      VarExpr::New ("likeEvent_attr2"),
      VarExpr::New ("like_attr2"))));

  result = result->Project (
    KNOWEVENT,
    strlist ("Local",
      "likeEvent_attr2",
      "like_attr2",
      "S"),
    strlist ("knowEvent_attr1",
      "knowEvent_attr2",
      "knowEvent_attr3",
      "knowEvent_attr4"));

  Delete (result);
}

void
Acquaintance::R4Eca0Ins (Ptr<Tuple> relation)
{
  RAPIDNET_LOG_INFO ("R4Eca0Ins triggered");

  Ptr<Tuple> result = relation;

  result->Assign (Assignor::New ("S",
    ValueExpr::New (Int32Value::New (0))));

  result->Assign (Assignor::New ("Local",
    LOCAL_ADDRESS));

  result = result->Select (Selector::New (
    Operation::New (RN_NEQ,
      VarExpr::New ("relation_attr2"),
      VarExpr::New ("relation_attr3"))));

  result = result->Project (
    KNOWEVENT,
    strlist ("Local",
      "relation_attr2",
      "relation_attr3",
      "S"),
    strlist ("knowEvent_attr1",
      "knowEvent_attr2",
      "knowEvent_attr3",
      "knowEvent_attr4"));

  Insert (result);
}

void
Acquaintance::R4Eca0Del (Ptr<Tuple> relation)
{
  RAPIDNET_LOG_INFO ("R4Eca0Del triggered");

  Ptr<Tuple> result = relation;

  result->Assign (Assignor::New ("S",
    ValueExpr::New (Int32Value::New (0))));

  result->Assign (Assignor::New ("Local",
    LOCAL_ADDRESS));

  result = result->Select (Selector::New (
    Operation::New (RN_NEQ,
      VarExpr::New ("relation_attr2"),
      VarExpr::New ("relation_attr3"))));

  result = result->Project (
    KNOWEVENT,
    strlist ("Local",
      "relation_attr2",
      "relation_attr3",
      "S"),
    strlist ("knowEvent_attr1",
      "knowEvent_attr2",
      "knowEvent_attr3",
      "knowEvent_attr4"));

  Delete (result);
}

void
Acquaintance::R5Eca0Ins (Ptr<Tuple> liveEvent)
{
  RAPIDNET_LOG_INFO ("R5Eca0Ins triggered");

  Ptr<RelationBase> result;

  result = GetRelation (LIVE)->Join (
    liveEvent,
    strlist ("live_attr1"),
    strlist ("liveEvent_attr1"));

  result->Assign (Assignor::New ("S",
    ValueExpr::New (Int32Value::New (0))));

  result->Assign (Assignor::New ("Local",
    LOCAL_ADDRESS));

  result = result->Select (Selector::New (
    Operation::New (RN_NEQ,
      VarExpr::New ("liveEvent_attr3"),
      VarExpr::New ("live_attr3"))));

  result = result->Select (Selector::New (
    Operation::New (RN_NEQ,
      VarExpr::New ("liveEvent_attr2"),
      VarExpr::New ("live_attr2"))));

  result = result->Project (
    KNOWEVENT,
    strlist ("Local",
      "liveEvent_attr2",
      "live_attr2",
      "S"),
    strlist ("knowEvent_attr1",
      "knowEvent_attr2",
      "knowEvent_attr3",
      "knowEvent_attr4"));

  Insert (result);
}

void
Acquaintance::R5Eca0Del (Ptr<Tuple> liveEvent)
{
  RAPIDNET_LOG_INFO ("R5Eca0Del triggered");

  Ptr<RelationBase> result;

  result = GetRelation (LIVE)->Join (
    liveEvent,
    strlist ("live_attr1"),
    strlist ("liveEvent_attr1"));

  result->Assign (Assignor::New ("S",
    ValueExpr::New (Int32Value::New (0))));

  result->Assign (Assignor::New ("Local",
    LOCAL_ADDRESS));

  result = result->Select (Selector::New (
    Operation::New (RN_NEQ,
      VarExpr::New ("liveEvent_attr3"),
      VarExpr::New ("live_attr3"))));

  result = result->Select (Selector::New (
    Operation::New (RN_NEQ,
      VarExpr::New ("liveEvent_attr2"),
      VarExpr::New ("live_attr2"))));

  result = result->Project (
    KNOWEVENT,
    strlist ("Local",
      "liveEvent_attr2",
      "live_attr2",
      "S"),
    strlist ("knowEvent_attr1",
      "knowEvent_attr2",
      "knowEvent_attr3",
      "knowEvent_attr4"));

  Delete (result);
}

void
Acquaintance::R5Eca1Ins (Ptr<Tuple> live)
{
  RAPIDNET_LOG_INFO ("R5Eca1Ins triggered");

  Ptr<RelationBase> result;

  result = GetRelation (LIVEEVENT)->Join (
    live,
    strlist ("liveEvent_attr1"),
    strlist ("live_attr1"));

  result->Assign (Assignor::New ("S",
    ValueExpr::New (Int32Value::New (0))));

  result->Assign (Assignor::New ("Local",
    LOCAL_ADDRESS));

  result = result->Select (Selector::New (
    Operation::New (RN_NEQ,
      VarExpr::New ("liveEvent_attr3"),
      VarExpr::New ("live_attr3"))));

  result = result->Select (Selector::New (
    Operation::New (RN_NEQ,
      VarExpr::New ("liveEvent_attr2"),
      VarExpr::New ("live_attr2"))));

  result = result->Project (
    KNOWEVENT,
    strlist ("Local",
      "liveEvent_attr2",
      "live_attr2",
      "S"),
    strlist ("knowEvent_attr1",
      "knowEvent_attr2",
      "knowEvent_attr3",
      "knowEvent_attr4"));

  Insert (result);
}

void
Acquaintance::R5Eca1Del (Ptr<Tuple> live)
{
  RAPIDNET_LOG_INFO ("R5Eca1Del triggered");

  Ptr<RelationBase> result;

  result = GetRelation (LIVEEVENT)->Join (
    live,
    strlist ("liveEvent_attr1"),
    strlist ("live_attr1"));

  result->Assign (Assignor::New ("S",
    ValueExpr::New (Int32Value::New (0))));

  result->Assign (Assignor::New ("Local",
    LOCAL_ADDRESS));

  result = result->Select (Selector::New (
    Operation::New (RN_NEQ,
      VarExpr::New ("liveEvent_attr3"),
      VarExpr::New ("live_attr3"))));

  result = result->Select (Selector::New (
    Operation::New (RN_NEQ,
      VarExpr::New ("liveEvent_attr2"),
      VarExpr::New ("live_attr2"))));

  result = result->Project (
    KNOWEVENT,
    strlist ("Local",
      "liveEvent_attr2",
      "live_attr2",
      "S"),
    strlist ("knowEvent_attr1",
      "knowEvent_attr2",
      "knowEvent_attr3",
      "knowEvent_attr4"));

  Delete (result);
}

