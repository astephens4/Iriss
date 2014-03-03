#ifndef IRISS_TIMESTAMP_H
#define IRISS_TIMESTAMP_H 1

namespace Utils {

class TimeStamp {
public:
    enum Month : int {
        JANUARY = 0,
        FEBRUARY,
        MARCH,
        APRIL,
        MAY,
        JUNE,
        JULY,
        AUGUST,
        SEPTEMBER,
        OCTOBER,
        NOVEMBER,
        DECEMBER
    };

    enum Day : int {
        SUNDAY = 0,
        MONDAY,
        TUESDAY,
        WEDNESDAY,
        THURSDAY,
        FRIDAY,
        SATURDAY
    };

    /**
     * Set the timestamp upon creation
     */
    TimeStamp(void);

    /**
     * Update this timestamp to the current time
     */
    void update(void);

    /**
     * Getters
     */
    unsigned int get_year() const { return m_year; };
    Month get_month() const { return m_month; };
    unsigned int get_calDay() const { return m_calDay; };
    Day get_weekDay() const { return m_weekDay; };
    unsigned int get_hour() const { return m_hour; };
    unsigned int get_minute() const { return m_minute; };
    unsigned int get_sec() const { return m_sec; };
   
private:
    unsigned int m_year;
    Month m_month;
    unsigned int m_calDay;
    Day m_weekDay;
    unsigned int m_hour;
    unsigned int m_minute;
    unsigned int m_sec;
};

} // end namespace Utils

#endif //IRISS_TIMESTAMP_H
