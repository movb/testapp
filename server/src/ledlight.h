#ifndef LEDLIGHT_H
#define LEDLIGHT_H

#include <string>
#include <atomic>
#include <stdexcept>

class LedLightException : public std::runtime_error
{
public:
    LedLightException(const std::string& message): runtime_error( message ) {}
};

class LedLight
{
public:
    LedLight();

    void setState(const std::string& state);
    void setColor(const std::string& color);
    void setRate(const std::string& rate);

    std::string getState();
    std::string getColor();
    std::string getRate();

private:
    enum class State { On, Off };
    enum class Color { Red, Green, Blue };

    constexpr static unsigned int minRate { 0 };
    constexpr static unsigned int maxRate { 5 };

    std::atomic<State> m_state { State::Off };
    std::atomic<Color> m_color { Color::Red };
    std::atomic<short> m_rate { 0 };
};

#endif // LEDLIGHT_H
