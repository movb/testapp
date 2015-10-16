#include "commands.h"

std::string SetLedStateCommand::processCommand(const boost::shared_ptr<LedLight> ledLight, std::list<std::string> &params)
{
    if(params.size() !=1)
        return error();

    try {
        ledLight->setState(*params.begin());
        return success();
    } catch (LedLightException& /*ex*/) {
        return error();
    }
}


std::string GetLedStateCommand::processCommand(const boost::shared_ptr<LedLight> ledLight, std::list<std::string> &params)
{
    if(params.size() !=0)
        return error();

    try {
        return success(ledLight->getState());
    } catch (LedLightException& /*ex*/) {
        return error();
    }
}


std::string SetLedColorCommand::processCommand(const boost::shared_ptr<LedLight> ledLight, std::list<std::string> &params)
{
    if(params.size() !=1)
        return error();

    try {
        ledLight->setColor(*params.begin());
        return success();
    } catch (LedLightException& /*ex*/) {
        return error();
    }
}


std::string GetLedColorCommand::processCommand(const boost::shared_ptr<LedLight> ledLight, std::list<std::string> &params)
{
    if(params.size() !=0)
        return error();

    try {
        return success(ledLight->getColor());
    } catch (LedLightException& /*ex*/) {
        return error();
    }
}


std::string SetLedRateCommand::processCommand(const boost::shared_ptr<LedLight> ledLight, std::list<std::string> &params)
{
    if(params.size() !=1)
        return error();

    try {
        ledLight->setRate(*params.begin());
        return success();
    } catch (LedLightException& /*ex*/) {
        return error();
    }
}


std::string GetLedRateCommand::processCommand(const boost::shared_ptr<LedLight> ledLight, std::list<std::string> &params)
{
    if(params.size() !=0)
        return error();

    try {
        return success(ledLight->getRate());
    } catch (LedLightException& /*ex*/) {
        return error();
    }
}


std::string DefaultCommand::processCommand(const boost::shared_ptr<LedLight> ledLight, std::list<std::string> &params)
{
    return error();
}
