#include <iostream>
#include <boost/program_options.hpp>

#include "syncserver.h"
#include "ledlight.h"
#include "ledvisualizer.h"
#include "commands.h"

using namespace std;
namespace po = boost::program_options;

int main(int argc, char **argv)
{
    string host = "127.0.0.1";
    unsigned int port = 2048;

    po::options_description desc("Allowed options");
    desc.add_options()
        ("host", po::value<string>(), "server host")
        ("port", po::value<unsigned int>(), "server port")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if(vm.count("host"))
        host = vm["host"].as<string>();
    if(vm.count("port"))
        port = vm["port"].as<unsigned int>();

    boost::shared_ptr<LedLight> light { new LedLight };

    LedVisualizer visualizer(light);
    boost::thread t(boost::ref(visualizer));

    SyncServer server(light, host, port);

    server.registerCommandProcessor("set-led-state", CommandPtr(new SetLedStateCommand()));
    server.registerCommandProcessor("get-led-state", CommandPtr(new GetLedStateCommand()));
    server.registerCommandProcessor("set-led-color", CommandPtr(new SetLedColorCommand()));
    server.registerCommandProcessor("get-led-color", CommandPtr(new GetLedColorCommand()));
    server.registerCommandProcessor("set-led-rate", CommandPtr(new SetLedRateCommand()));
    server.registerCommandProcessor("get-led-rate", CommandPtr(new GetLedRateCommand()));
    server.registerCommandProcessor("default", CommandPtr(new DefaultCommand()));
    server.run();

    return 0;
}
