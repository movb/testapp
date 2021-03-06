#include "ledlight.h"

#include <boost/lexical_cast.hpp>

LedLight::LedLight()
{
}

void LedLight::setState(const std::string &state)
{
    State newState;
    if (state == "on") {
        newState = State::On;
    } else if (state == "off") {
        newState = State::Off;
    } else {
        throw LedLightException("Unknow state " + state );
    }

    if (newState == m_state)
        throw LedLightException("State already is " + state );

    m_state = newState;
}

void LedLight::setColor(const std::string &color)
{
    Color newColor;
    if (color == "red") {
        newColor = Color::Red;
    } else if (color == "green") {
        newColor = Color::Green;
    } else if (color == "blue") {
        newColor = Color::Blue;
    } else {
        throw LedLightException("Unknow color " + color );
    }

    if (newColor == m_color)
        throw LedLightException("Color already is " + color );

    m_color = newColor;
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

    if (rateNumber == m_rate)
        throw LedLightException("Rate already is " + rate );

    m_rate = rateNumber;
}

std::string LedLight::getState()
{
    State state = m_state;

    if(state == State::On)
        return "on";
    else
        return "off";
}

std::string LedLight::getColor()
{
    Color color = m_color;

    if(color == Color::Red)
        return "red";
    else if (color == Color::Green)
        return "green";
    else if (color == Color::Blue)
        return "blue";
}

std::string LedLight::getRate()
{
    return boost::lexical_cast<std::string>(m_rate);
}

short LedLight::getRateInt()
{
    return m_rate;
}

