#include <cstdint>

#if __APPLE__

#include <assert.h>
#include <CoreServices/CoreServices.h>
#include <mach/mach.h>
#include <mach/mach_time.h>

struct StopWatch
{

    StopWatch()
    {
        mach_timebase_info(&m_tbinfo);
        lap();
    }

    double lap()
    {
        m_start = m_end;
        m_end = mach_absolute_time();
        uint64_t elapsed = m_end - m_start;
        return elapsed * m_tbinfo.numer / m_tbinfo.denom * 1.e-9;
    }

    mach_timebase_info_data_t m_tbinfo;
    uint64_t m_start = 0;
    uint64_t m_end = 0;

}; /* end struct StopWatch */

#elif __linux__

#include <time.h>

struct StopWatch
{
    StopWatch()
    {
        clock_getres(CLOCK_PROCESS_CPUTIME_ID, &m_res);
        lap();
    }

    double lap()
    {
        m_start = m_end;
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &m_end);
        return diff(m_start, m_end) * 1.e-9;
    }

    int64_t diff(timespec const & t1, timespec const & t2)
    {
        timespec temp;
        int64_t nsec;
        if ((t2.tv_nsec - t1.tv_nsec)<0)
        {
            nsec = 1000000000 + t2.tv_nsec - t1.tv_nsec;
            nsec += 1000000000 * (t2.tv_sec - t1.tv_sec - 1);
        }
        else
        {
            nsec = t2.tv_nsec - t1.tv_nsec;
            nsec += 1000000000 * (t2.tv_sec - t1.tv_sec);
        }
        return nsec;
    }

    timespec m_res;
    timespec m_start;
    timespec m_end;

}; /* end struct StopWatch */

#endif // __APPLE__

// vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4:
