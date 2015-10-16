#include <iostream>

#include "syncserver.h"
#include "ledlight.h"
#include "ledvisualizer.h"
#include "commands.h"

int main()
{
    boost::shared_ptr<LedLight> light { new LedLight };

    LedVisualizer visualizer(light);
    boost::thread t(boost::ref(visualizer));

    SyncServer server(light, "127.0.0.1", 2048);
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
