#include "TimeStamp.hpp"
#include <ctime>

namespace Utils {

TimeStamp::TimeStamp(void)
{
    // this will only be in Unix, so go crazy with sys calls
    time_t timer = time(NULL);
    struct tm *now = localtime(&timer);
    m_year = 1900 + now->tm_year;
    m_month = static_cast<Month>(now->tm_mon);
    m_calDay = now->tm_mday;
    m_weekDay = static_cast<Day>(now->tm_wday);
    m_hour = now->tm_hour;
    m_minute = now->tm_min;
    m_sec = now->tm_sec;
}

TimeStamp::TimeStamp(const TimeStamp& other) :
    m_year(other.m_year),
    m_month(other.m_month),
    m_calDay(other.m_calDay),
    m_weekDay(other.m_weekDay),
    m_hour(other.m_hour),
    m_minute(other.m_minute),
    m_sec(other.m_sec)
{
    
}

void TimeStamp::update(void)
{
    // this will only be in Unix, so go crazy with sys calls
    time_t timer = time(NULL);
    struct tm *now = localtime(&timer);
    m_year = 1900 + now->tm_year;
    m_month = static_cast<Month>(now->tm_mon);
    m_calDay = now->tm_mday;
    m_weekDay = static_cast<Day>(now->tm_wday);
    m_hour = now->tm_hour;
    m_minute = now->tm_min;
    m_sec = now->tm_sec;
}

} // end namespace Utils
