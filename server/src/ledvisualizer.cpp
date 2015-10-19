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
        std::string state = m_ledLight->getState();
        std::string color = m_ledLight->getColor();

        if(rate != 0 && state == "on") {
            ++m_tickCount;

            clearScreen();
            if(m_tickCount % 2 == 0) {
                printMessage(state, color, rate);
                m_tickCount = 0;
            }

            t->expires_at(t->expires_at() + boost::posix_time::milliseconds(1000/(rate*2)));
        }
        else {
            if(isUpdated(state, color, rate))
                clearScreen();

            if((state == "on" && m_messageLength == 0))
                printMessage(state, color, rate);

            t->expires_at(t->expires_at() + boost::posix_time::milliseconds(100));
        }

        m_previousState = state;
        m_previousRate = rate;
        m_previousColor = color;

        t->async_wait(boost::bind(&LedVisualizer::update,
                                 this,
                                 boost::asio::placeholders::error,
                                 t));
    }
}

bool LedVisualizer::isUpdated(std::string state, std::string color, short rate)
{
    bool updated = m_previousColor != color ||
            m_previousState != state ||
            m_previousRate != rate;

    return updated;
}

void LedVisualizer::printMessage(std::string state, std::string color, short rate)
{
    std::stringstream ss;
    ss << "\033[1;";
    if (color == "red")
        ss << "31m";
    else if(color == "green")
        ss << "32m";
    else if(color == "blue")
        ss << "36m";
    ss << state << " " << color << " " << rate;
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

    std::cout << ss.str() << std::flush;
    m_messageLength = 0;
}

