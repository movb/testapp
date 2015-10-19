#include <iostream>

#include <vector>
#include <string>
#include <initializer_list>
#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>

#include "ledclient.h"

using namespace std;
namespace po = boost::program_options;

struct Command {
    string name;
    string description;
    vector<string> params;

    Command(const char* name, const char* description, initializer_list<string> list):
        name( name ), description ( description ), params(list)
    {
    }
};

using Commands = vector<Command>;

Commands prepareCommands() {
    Commands commands;
    commands.push_back(Command {"set-led-state", "Set led state", {"on", "off"}});
    commands.push_back(Command {"get-led-state", "Get led state", {}});
    commands.push_back(Command {"set-led-color", "Set led color (red, green or blue)", {"red", "green", "blue"}});
    commands.push_back(Command {"get-led-color", "Get led color", {}});
    commands.push_back(Command {"set-led-rate", "Set led rate (0..5 Hz)", {"0", "1", "2", "3", "4", "5"}});
    commands.push_back(Command {"get-led-rate", "Get led rate", {}});

    return commands;
}

void displayMenu(const Commands& commands) {
    cout << "Choose action:" << endl;
    int index = 1;
    for(Command cmd: commands) {
        cout << index++ << ": " << cmd.description << endl;
    }
    cout << "0: exit" << endl;
}

void displaySubMenu(vector<string> params) {
    cout << "Choose value:" << endl;
    int index = 1;
    for(string param: params) {
        cout << index++ << ": " << param << endl;
    }
    cout << "0: back to menu" << endl;
}

short getNumber(int minNumber, int maxNumber) {
    while ( true ) {
        cout << "Enter your choise: ";
        string choose;
        cin >> choose;
        short chooseNumber;
        try {
            chooseNumber = boost::lexical_cast<short>(choose);
        } catch ( boost::bad_lexical_cast & /* ex */) {
            cout << "Bad number " << choose << endl;
            continue;
        }
        if(chooseNumber < minNumber || chooseNumber > maxNumber) {
            cout << "Bad choise, you need to select number between " << minNumber << " and " << maxNumber << endl;
            continue;
        }
        return chooseNumber;
    }
}

pair<int,int> getAction(const Commands& commands) {
    while ( true ) {
        displayMenu(commands);
        int choice = getNumber(0,commands.size());
        int subChoice = 0;
        if (choice == 0) return make_pair(-1,0);
        if (!commands[choice-1].params.empty()) {
            displaySubMenu(commands[choice-1].params);
            subChoice = getNumber(0,commands[choice-1].params.size());
            if (subChoice == 0)
                continue;
        }
        return make_pair(choice-1, subChoice-1);
    }
}

int main(int argc, char** argv)
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

    cout << "Connecting to " << host << " on port " << port << endl;

    LedClient client;
    try {
        client.connect(host, port);

        Commands commands = prepareCommands();
        while( true ) {
            pair<int,int> action = getAction(commands);
            if(action.first == -1)
                break;
            string commandString = commands[action.first].name;
            if(action.second != -1)
                commandString += " " + commands[action.first].params[action.second];

                cout << "Result: " << client.sendCommand(commandString) << endl;
        }
    } catch (LedClientException& ex) {
        cout << "Connection error: " << ex.what() << endl;
    }
    cout << "Good bye!" << endl;
    return 0;
}
