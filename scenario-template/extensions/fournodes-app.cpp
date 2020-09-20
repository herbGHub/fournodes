// helloworld-app.cpp

#include "fournodes-app.hpp"

#include "ns3/ptr.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"

#include "ns3/ndnSIM/helper/ndn-stack-helper.hpp"
#include "ns3/ndnSIM/helper/ndn-fib-helper.hpp"

#include "ns3/random-variable-stream.h"

NS_LOG_COMPONENT_DEFINE("FournodesApp");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED(FournodesApp);

// register NS-3 type
TypeId FournodesApp::GetTypeId()
{
  static TypeId tid = TypeId("FournodesApp").SetParent<ndn::App>().AddConstructor<HelloworldApp>();
  return tid;
}

// Processing upon start of the application
void FournodesApp::StartApplication()
{
  // initialize ndn::App
  ndn::App::StartApplication();

  // Add entry to FIB for `/helloworld`
  //ndn::FibHelper::AddRoute(GetNode(), "/helloworld", m_face, 0);

  // Schedule send of first interest
  Simulator::Schedule(Seconds(1.0), &FournodesApp::SendInterest, this);
}

// Processing when application is stopped
void FournodesApp::StopApplication()
{
  // cleanup ndn::App
  ndn::App::StopApplication();
}

void
FournodesApp::SendInterest()
{
  /////////////////////////////////////
  // Sending one Interest packet out //
  /////////////////////////////////////

  // Create and configure ndn::Interest
  auto interest = std::make_shared<ndn::Interest>("/prefix");
  Ptr<UniformRandomVariable> rand = CreateObject<UniformRandomVariable>();
  interest->setNonce(rand->GetValue(0, std::numeric_limits<uint32_t>::max()));
  interest->setCanBePrefix(false);
  interest->setInterestLifetime(ndn::time::seconds(1));

  NS_LOG_DEBUG("Sending Interest packet for " << *interest);

  // Call trace (for logging purposes)
  m_transmittedInterests(interest, this, m_face);

  m_appLink->onReceiveInterest(*interest);
}

// Callback that will be called when Interest arrives
void FournodesApp::OnInterest(std::shared_ptr<const ndn::Interest> interest)
{
  ndn::App::OnInterest(interest);

  NS_LOG_DEBUG("Received Interest packet for " << interest->getName());

  auto data = std::make_shared<ndn::Data>(interest->getName());
  data->setFreshnessPeriod(ndn::time::milliseconds(1000));
  data->setContent(std::make_shared< ::ndn::Buffer>(1024));

  ndn::StackHelper::getKeyChain().sign(*data);

  // Call trace (for logging purposes)
  m_transmittedDatas(data, this, m_face);

  m_appLink->onReceiveData(*data);
}
// Callback that will be called when Data arrives
void FournodesApp::OnData(std::shared_ptr<const ndn::Data> data)
{
  NS_LOG_DEBUG("Receiving Data packet for " << data->getName());
}

} // namespace ns3
