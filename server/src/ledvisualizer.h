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
    boost::shared_ptr<LedLight> m_ledLight;
    std::atomic_bool m_terminated { false };

    boost::asio::io_service m_service;
};

#endif // LEDVISUALIZER_H
