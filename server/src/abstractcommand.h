#ifndef ABSTRACTCOMMAND_H
#define ABSTRACTCOMMAND_H

#include <list>
#include <string>

#include <boost/shared_ptr.hpp>

#include "ledlight.h"

class AbstractCommand {
public:
    AbstractCommand() {}
    virtual ~AbstractCommand() {}

    virtual std::string processCommand(const boost::shared_ptr<LedLight> ledLight, std::list<std::string>& params) = 0;
protected:
    std::string error() { return "FAILED"; }

    std::string success(const std::string& message = "") {
        if(message.size() !=0)
            return "OK " + message;
        else
            return "OK";
    }
};

#endif // ABSTRACTCOMMAND_H

