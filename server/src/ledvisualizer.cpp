#include "ledvisualizer.h"

#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <sstream>
#include <iostream>

LedVisualizer::LedVisualizer(const boost::shared_ptr<LedLight> &ledLight):
    m_ledLight(ledLight)
{

}

void LedVisualizer::terminate()
{
    m_terminated = true;
}

void LedVisualizer::operator()()
{
    boost::asio::deadline_timer t(m_service, boost::posix_time::seconds(1));
    t.async_wait(boost::bind(&LedVisualizer::update,
                             this,
                             boost::asio::placeholders::error,
                             &t));

    m_service.run();
}

void LedVisualizer::update(const boost::system::error_code & /*e*/,
                           boost::asio::deadline_timer *t)
{
    if(! m_terminated ) {
        short rate = m_ledLight->getRateInt();

        if(rate != 0 && m_ledLight->getState() == "on") {
            ++m_tickCount;

            if(m_tickCount % rate == 0) {
                printMessage();
                m_tickCount = 0;
            }
            else {
                clearScreen();
            }

            t->expires_at(t->expires_at() + boost::posix_time::milliseconds(1000/rate));
        }
        else {
            if(isUpdated()) {
                clearScreen();
                if(m_ledLight->getState() != "off")
                    printMessage();
            }
            t->expires_at(t->expires_at() + boost::posix_time::milliseconds(100));
        }

        t->async_wait(boost::bind(&LedVisualizer::update,
                                 this,
                                 boost::asio::placeholders::error,
                                 t));
    }
}

bool LedVisualizer::isUpdated()
{
    bool updated = m_previousColor != m_ledLight->getColor() ||
            m_previousState != m_ledLight->getState() ||
            m_previousRate != m_ledLight->getRate();
    m_previousState = m_ledLight->getState();
    m_previousRate = m_ledLight->getRate();
    m_previousColor = m_ledLight->getColor();

    return updated;
}

void LedVisualizer::printMessage()
{
    std::stringstream ss;
    ss << "\033[1;";
    std::string color = m_ledLight->getColor();
    if (color == "red")
        ss << "31m";
    else if(color == "green")
        ss << "32m";
    else if(color == "blue")
        ss << "36m";
    ss << m_ledLight->getState() << " " << m_ledLight->getColor() << " "
       << m_ledLight->getRate();
    ss << "\033[0m";

    std::string message = ss.str();
    m_messageLength = message.size();

    std::cout << message << std::flush;
}

void LedVisualizer::clearScreen()
{
    std::stringstream ss;

    for(int i=0;i<m_messageLength;++i)
        ss << "\b \b";

    //std::cout << "\e[A";
    std::cout << ss.str() << std::flush;
    m_messageLength = 0;
}

