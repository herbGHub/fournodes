// helloworld-app.cpp

#include "ConsumerFournodes.hpp"

#include "ns3/ptr.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"

#include "ns3/ndnSIM/helper/ndn-stack-helper.hpp"
#include "ns3/ndnSIM/helper/ndn-fib-helper.hpp"

#include "ns3/random-variable-stream.h"

NS_LOG_COMPONENT_DEFINE("ConsumerFournodes");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED(ConsumerFournodes);

// register NS-3 type
TypeId ConsumerFournodes::GetTypeId()
{
  static TypeId tid = TypeId("ConsumerFournodes").SetParent<ndn::App>().AddConstructor<ConsumerFournodes>();
  return tid;
}

// Processing upon start of the application
void ConsumerFournodes::StartApplication()
{
  // initialize ndn::App
  ndn::App::StartApplication();

  // Add entry to FIB for `/helloworld`
  //ndn::FibHelper::AddRoute(GetNode(), "/helloworld", m_face, 0);

  // Schedule send of first interest
  Simulator::Schedule(Seconds(1.1), &ConsumerFournodes::SendInterest, this);
}

// Processing when application is stopped
void ConsumerFournodes::StopApplication()
{
  // cleanup ndn::App
  ndn::App::StopApplication();
}

void
ConsumerFournodes::SendInterest()
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


// Callback that will be called when Data arrives
void ConsumerFournodes::OnData(std::shared_ptr<const ndn::Data> data)
{
  NS_LOG_DEBUG("Receiving Data packet for " << data->getName());
}

} // namespace ns3
