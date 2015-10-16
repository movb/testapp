#ifndef COMMANDS_H
#define COMMANDS_H

#include <abstractcommand.h>


class SetLedStateCommand : public AbstractCommand
{
public:
    std::string processCommand(const boost::shared_ptr<LedLight> ledLight, std::list<std::string>& params) override;
};

class GetLedStateCommand : public AbstractCommand
{
public:
    std::string processCommand(const boost::shared_ptr<LedLight> ledLight, std::list<std::string>& params) override;
};

class SetLedColorCommand : public AbstractCommand
{
public:
    std::string processCommand(const boost::shared_ptr<LedLight> ledLight, std::list<std::string>& params) override;
};

class GetLedColorCommand : public AbstractCommand
{
public:
    std::string processCommand(const boost::shared_ptr<LedLight> ledLight, std::list<std::string>& params) override;
};

class SetLedRateCommand : public AbstractCommand
{
public:
    std::string processCommand(const boost::shared_ptr<LedLight> ledLight, std::list<std::string>& params) override;
};

class GetLedRateCommand : public AbstractCommand
{
public:
    std::string processCommand(const boost::shared_ptr<LedLight> ledLight, std::list<std::string>& params) override;
};

class DefaultCommand : public AbstractCommand
{
public:
    std::string processCommand(const boost::shared_ptr<LedLight> ledLight, std::list<std::string>& params) override;
};

#endif // COMMANDS_H
