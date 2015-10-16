#include "ledvisualizer.h"

#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

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
        std::cout << m_ledLight->getState() << " " << m_ledLight->getColor() << " "
                  << m_ledLight->getRate() << std::endl;

        t->expires_at(t->expires_at() + boost::posix_time::seconds(1));

        t->async_wait(boost::bind(&LedVisualizer::update,
                                 this,
                                 boost::asio::placeholders::error,
                                 t));
    }
}

