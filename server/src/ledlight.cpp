#include "ledlight.h"

#include <boost/lexical_cast.hpp>

LedLight::LedLight()
{
}

void LedLight::setState(const std::string &state)
{
    if (state == "on") {
        m_state = State::On;
    } else if (state == "off") {
        m_state = State::Off;
    } else {
        throw LedLightException("Unknow state " + state );
    }
}

void LedLight::setColor(const std::string &color)
{
    if (color == "red") {
        m_color = Color::Red;
    } else if (color == "green") {
        m_color = Color::Green;
    } else if (color == "blue") {
        m_color = Color::Blue;
    } else {
        throw LedLightException("Unknow color " + color );
    }
}

void LedLight::setRate(const std::string &rate)
{
    short rateNumber;
    try {
        rateNumber = boost::lexical_cast<short>(rate);
    } catch ( boost::bad_lexical_cast & /* ex */) {
        throw LedLightException("Bad rate " + rate );
    }

    if( rateNumber < minRate || rateNumber > maxRate )
        throw LedLightException("Rate " + rate + " not in 0..5 range");

    m_rate = rateNumber;
}

std::string LedLight::getState()
{
    if(m_state == State::On)
        return "on";
    else
        return "off";
}

std::string LedLight::getColor()
{
    if(m_color == Color::Red)
        return "red";
    else if (m_color == Color::Green)
        return "green";
    else if (m_color == Color::Blue)
        return "blue";
}

std::string LedLight::getRate()
{
    return boost::lexical_cast<std::string>(m_rate);
}

