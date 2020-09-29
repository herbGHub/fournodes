#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/ndnSIM-module.h"
#include "ns3/point-to-point-module.h"

namespace ns3 {

	int
	main(int argc, char* argv[])
	{
		/* Ring topology with at least 3 nodes
		 *  n0 -- n1 --n3
		 *       /
		 *     n2
		 */
		int numNodes = 4;

		CommandLine cmd;
		cmd.AddValue ("numNodes", "number of nodes (minimal 4)", numNodes);
		cmd.Parse(argc, argv);

		NodeContainer nodes;
		nodes.Create (numNodes);

		// Create topology p2p
		PointToPointHelper p2p;
		p2p.Install(nodes.Get(0), nodes.Get(1));
		p2p.Install(nodes.Get(1), nodes.Get(2));
		p2p.Install(nodes.Get(1), nodes.Get(3));

		// Install NDN stack on all nodes
		ndn::StackHelper ndnHelper;
		ndnHelper.SetDefaultRoutes(true);
		ndnHelper.Install(nodes);

		// Set Forwarding Strategy
		ndn::StrategyChoiceHelper::Install(nodes, "/", "/localhost/nfd/strategy/multicast");

		// I was trying only with "StrategyChoiceHelper" in multicast and the consumers were droping the data
		// I saw in the NDNSim Grid example the use of GlobalRoutingHelper, so I tried to install the router in the GlobalRoutingHelper
		// Bu I don't undestand why
		ndn::GlobalRoutingHelper ndnGlobalRoutingHelper;
		ndnGlobalRoutingHelper.Install( nodes.Get(1));

		// Installing applications

		// Consumer 0
		// Consumer will request /prefix
		ndn::AppHelper consumerHelper0("ConsumerFournodes");
		consumerHelper0.Install(nodes.Get(0));                        		// first node

		// Consumer 1
		// Consumer will request /prefix/0, /prefix/1, ...
		ndn::AppHelper consumerHelper2("ConsumerFournodes");
		consumerHelper2.Install(nodes.Get(2));                        		// third node

		// Producer
		// Producer will reply to all requests starting with /prefix
		ndn::AppHelper producerHelper("ProducerFournodes");
		//ApplicationContainer app3 = producerHelper.Install(nodes.Get(3));
		producerHelper.Install(nodes.Get(3)); 						  		// last node

		// Add /prefix origins to ndn::GlobalRouter
		// I don't undesrtand. I think that only the router (node 1) should be added in global routing
  		ndnGlobalRoutingHelper.AddOrigins("/prefix", nodes.Get(3));

  		// Calculate and install FIBs
		ndn::GlobalRoutingHelper::CalculateRoutes();

		//Not necessary
		//app3.Start (Seconds (1.0));

		Simulator::Stop(Seconds(20.0));
		Simulator::Run();
		Simulator::Destroy();

		 return 0;

	}
}

main(int argc, char* argv[])
{
    return ns3::main(argc, argv);
}
