#ifndef LEDVISUALIZER_H
#define LEDVISUALIZER_H

#include <ledlight.h>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <atomic>

class LedVisualizer
{
public:
    LedVisualizer(const boost::shared_ptr<LedLight>& ledLight);
    void terminate();

    void operator()();

    void update(const boost::system::error_code& e,
                boost::asio::deadline_timer* t);
private:
    bool isUpdated(std::string state, std::string color, short rate);
    void printMessage(std::string state, std::string color, short rate);
    void clearScreen();
private:
    boost::shared_ptr<LedLight> m_ledLight;
    std::atomic_bool m_terminated { false };

    std::atomic<int> m_tickCount;
    unsigned int m_messageLength = 0;
    std::string m_previousState;
    short m_previousRate;
    std::string m_previousColor;


    boost::asio::io_service m_service;
};

#endif // LEDVISUALIZER_H
