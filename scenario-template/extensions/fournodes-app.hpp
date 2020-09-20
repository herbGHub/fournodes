// helloworld-app.hpp

#ifndef FOURNODES_APP_H_
#define FOURNODES_APP_H_

#include "ns3/ndnSIM/apps/ndn-app.hpp"

namespace ns3 {

class FournodesApp : public ndn::App {
public:
  static TypeId GetTypeId();
  virtual void StartApplication();
  virtual void StopApplication();

  // (overridden from ndn::App) Callback that will be called when Interest arrives
  virtual void OnInterest(std::shared_ptr<const ndn::Interest> interest);
  virtual void OnData(std::shared_ptr<const ndn::Data> data);

private:
  void SendInterest();
};

} // namespace ns3

#endif // FOURNODES_APP_H_
