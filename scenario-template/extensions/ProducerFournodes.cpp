// ProducerFournodes.cpp

#include "ProducerFournodes.hpp"

#include "ns3/ptr.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"

#include "ns3/ndnSIM/helper/ndn-stack-helper.hpp"
#include "ns3/ndnSIM/helper/ndn-fib-helper.hpp"

#include "ns3/random-variable-stream.h"

NS_LOG_COMPONENT_DEFINE("ProducerFournodes");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED(ProducerFournodes);

// register NS-3 type
TypeId ProducerFournodes::GetTypeId()
{
  static TypeId tid = TypeId("ProducerFournodes").SetParent<ndn::App>().AddConstructor<ProducerFournodes>();
  return tid;
}

// Processing upon start of the application
void ProducerFournodes::StartApplication()
{
  // initialize ndn::App
  ndn::App::StartApplication();

  // Add entry to FIB for `/prefix`
  ndn::FibHelper::AddRoute(GetNode(), "/prefix", m_face, 0);

}

// Processing when application is stopped
void ProducerFournodes::StopApplication()
{
  // cleanup ndn::App
  ndn::App::StopApplication();
}


// Callback that will be called when Interest arrives
void ProducerFournodes::OnInterest(std::shared_ptr<const ndn::Interest> interest)
{
  ndn::App::OnInterest(interest);

  NS_LOG_DEBUG("Received Interest packet for " << interest->getName());

  auto data = std::make_shared<ndn::Data>(interest->getName());
  data->setFreshnessPeriod(ndn::time::milliseconds(1000));
  data->setContent(std::make_shared< ::ndn::Buffer>(1024));

  ndn::StackHelper::getKeyChain().sign(*data);

  NS_LOG_DEBUG("Sending Data packet for " << data->getName());

  // Call trace (for logging purposes)
  m_transmittedDatas(data, this, m_face);

  m_appLink->onReceiveData(*data);
}


} // namespace ns3
