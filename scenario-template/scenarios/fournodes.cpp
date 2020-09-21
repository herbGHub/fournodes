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

		PointToPointHelper p2p;
		p2p.Install(nodes.Get(0), nodes.Get(1));
		p2p.Install(nodes.Get(1), nodes.Get(2));
		p2p.Install(nodes.Get(1), nodes.Get(3));

		// 3. Install NDN stack on all nodes
		ndn::StackHelper ndnHelper;
		ndnHelper.SetDefaultRoutes(true);
		ndnHelper.Install(nodes);

		// 4. Set Forwarding Strategy
		ndn::StrategyChoiceHelper::Install(nodes, "/", "/localhost/nfd/strategy/multicast");

		// Installing applications




		// Consumer0
		//ndn::AppHelper consumerHelper0("ns3::ndn::ConsumerCbr");
		// Consumer will request /prefix/0, /prefix/1, ...
		ndn::AppHelper consumerHelper0("ConsumerFournodes");
		//consumerHelper0.SetPrefix("/prefix");
		//consumerHelper0.SetAttribute("Frequency", StringValue("10")); // 10 interests a second

		consumerHelper0.Install(nodes.Get(0));                        // first node

		// Consumer1
		ndn::AppHelper consumerHelper2("ConsumerFournodes");
		// Consumer will request /prefix/0, /prefix/1, ...
		//consumerHelper2.SetPrefix("/prefix");
		//consumerHelper2.SetAttribute("Frequency", StringValue("10")); // 10 interests a second
		consumerHelper2.Install(nodes.Get(2));                        // third node


		// Producer
		//ndn::AppHelper producerHelper("ns3::ndn::Producer");
		ndn::AppHelper producerHelper("ProducerFournodes");
		// Producer will reply to all requests starting with /prefix
		//producerHelper.SetPrefix("/prefix");
		//producerHelper.SetAttribute("PayloadSize", StringValue("1024"));
		//producerHelper.
		producerHelper.Install(nodes.Get(3)); // last node

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
