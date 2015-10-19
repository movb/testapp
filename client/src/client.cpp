#include <iostream>

#include <vector>
#include <string>
#include <initializer_list>
#include <boost/lexical_cast.hpp>

#include "ledclient.h"

using namespace std;

struct Command {
    std::string name;
    std::string description;
    std::vector<std::string> params;

    Command(const char* name, const char* description, std::initializer_list<std::string> list):
        name( name ), description ( description ), params(list)
    {
    }
};

using Commands = std::vector<Command>;

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

int main()
{
    LedClient client;
    try {
        client.connect("127.0.0.1", 2048);

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
