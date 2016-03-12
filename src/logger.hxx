/*!
 * \author gergely.nyiri@gmail.com
 * \file logger.hxx
 * \brief general purpose logger class
 */

#ifndef LOGGER_HXX
#define LOGGER_HXX

#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <ctime>

namespace base
{
  /*!
   * \enum log_level
   * \brief log modes
   * LOG_SPECIAL is a special mode always printed.
   * LOG_GUARD is the last element, acts as a guard when iterating through all the log modes.
   */
  enum log_level
  {
    LOG_NO_LOG = 0,
    LOG_ERROR,
    LOG_WARNING,
    LOG_DEBUG,
    LOG_SPECIAL,
    LOG_GUARD
  };
  /*!
   * \enumb time_modes
   * \brief time modes
   */
  enum time_modes
  {
    TIME_START,
    TIME_ELAPSED = 0,
    TIME_CUMULATED,
    TIME_AVERAGE,
    TIME_MAX,
    TIME_GUARD
  };
  /*!
   * \typedef log_stat
   * \brief logging attributes
   * first: name of the logging level
   * second: counter of occurences per logging level
   */
  typedef std::pair<std::string, unsigned int> log_stat;
  /*!
   * \typedef log_counter
   * \brief logging map
   */
  typedef std::map<log_level, log_stat> log_counter;
  /*!
   * \class logger
   * \brief Singleton logger class
   * Usage: log::logger::GetInstance().Log(level, message);
   * message shall be logged if level is greater than or equal to LOG_LEVEL expect for LOG_SPECIAL
   */
  class logger
  {
  public:

    /*!
     * \brief Creates the only logger instance
     */
    static logger &GetInstance()
    {
      static logger s_Logger;

      return s_Logger;
    }
    /*!
     * \brief For map initialization. Once you modify the log_level enum, always consider updating this function!
     */
    static log_counter GetInitialCounter()
    {
      log_counter t_Counter;

      t_Counter.insert(std::pair<log_level, log_stat>(LOG_ERROR, std::make_pair("[error  ]", 0)));
      t_Counter.insert(std::pair<log_level, log_stat>(LOG_WARNING, std::make_pair("[warning]", 0)));
      t_Counter.insert(std::pair<log_level, log_stat>(LOG_DEBUG, std::make_pair("[debug  ]", 0)));
      t_Counter.insert(std::pair<log_level, log_stat>(LOG_SPECIAL, std::make_pair("[special]", 0)));

      return t_Counter;
    }
    /*!
     * \brief Get LOG_LEVEL shell variable.
     */
    static int GetLevel()
    {
      unsigned int t_Level = LOG_ERROR;
      const char* t_Env = getenv("LOG_LEVEL");

      if(t_Env)
      {
        std::stringstream t_Str(t_Env);
        t_Str >> t_Level;

        if(t_Str.fail())
        {
          std::cout << "LOG_LEVEL extraction failed!" << std::endl;
          t_Level = LOG_ERROR;
        }
      }

      std::cout << "LOG_LEVEL = " << t_Level << std::endl;

      return t_Level;
    }
    /*!
     * \brief Active
     * \param p_LogLevel
     * \return
     */
    static bool Active(const log_level p_LogLevel)
    {
      if(p_LogLevel != LOG_SPECIAL)
      {
        if(p_LogLevel > m_LogLevel || p_LogLevel >= LOG_GUARD || p_LogLevel <= LOG_NO_LOG || m_LogCounter.find(p_LogLevel) == m_LogCounter.end())
        {
          return false;
        }
      }

      return true;
    }
    /*!
     * \brief Print out statistical summary
     */
    void Summary() const
    {
      std::cout << "SUMMARY\n-------------------------------------------------\n";

      for(log_counter::const_iterator t_It = m_LogCounter.begin(); t_It != m_LogCounter.end(); t_It++)
      {
        std::cout << t_It->second.first << " = ";
        std::cout.width(5);
        std::cout << t_It->second.second << std::endl;
      }
    }
    /*!
     * \brief Log a given log message if needed.
     */
    void Log(const log_level p_LogLevel, const char* p_Msg, ...)  const
    {
      if(!Active(p_LogLevel))
      {
        return;
      }

      char t_Buff[300];

      va_list t_Arglist;
      va_start(t_Arglist,p_Msg);
      vsnprintf(t_Buff, 300, p_Msg, t_Arglist);
      va_end(t_Arglist);

      std::stringstream t_Str;
      t_Str << m_LogPrefix;
      t_Str << m_LogCounter[p_LogLevel].first << "[";
      t_Str.width(5);
      t_Str << m_LogCounter[p_LogLevel].second << "] - " << t_Buff;

      m_LogCounter[p_LogLevel].second++;

      Log(t_Str.str().c_str());
    }
    /*!
     * \brief Log
     * \param p_LogLevel
     * \param p_Msg
     * \param p_Argp
     */
    void Log(const log_level p_LogLevel, const char* p_Msg, va_list p_Argp) const
    {
      char t_Buff[300];
      vsnprintf(t_Buff, 300, p_Msg, p_Argp);

      std::stringstream t_Str;
      t_Str << m_LogPrefix;
      t_Str << m_LogCounter[p_LogLevel].first << "[";
      t_Str.width(5);
      t_Str << m_LogCounter[p_LogLevel].second << "] - " << t_Buff;

      m_LogCounter[p_LogLevel].second++;

      Log(t_Str.str().c_str());
    }
    /*!
     * \brief Log a given log message
     * \param p_Msg message to be logged
     */
    void Log(const char* p_Msg) const
    {
      std::cout << p_Msg << std::endl;
    }
    /*!
     * \brief Destructor calling Summary method
     */
    ~logger()
    {
      Summary();
    }

  private:
    /*!
     * \brief log level
     */
    static unsigned int m_LogLevel;
    /*!
     * \brief log counter
     */
    static log_counter m_LogCounter;
    /*!
     * \brief prefix for each log output
     */
    static const char* m_LogPrefix;
    /*!
     * \brief CTOR (private to avoid explicit construction)
     */
    logger() { }
    /*!
     * \brief DTOR (private)
     */
    logger(logger const &);
    /*!
     * \brief = operator (private to avoid explict calls)
     */
    logger &operator=(logger const &);
  };
  /*!
   * \brief Short version of logging
   */
  void log_(log_level p_LogLevel, const char* p_Msg, ...);
  /*!
   * \brief Short version of error logging
   */
  void log_error(const char* p_Msg, ...);
  /*!
   * \brief Short version of warning logging
   */
  void log_warning(const char* p_Msg, ...);
  /*!
   * \brief Short version of debug logging
   */
  void log_debug(const char* p_Msg, ...);
  /*!
   * \brief Short version of special logging
   */
  void log_special(const char* p_Msg, ...);
  /*!
   * \class time_meas
   * \brief Time measurement for a single block surrounded by {}
   * Elapsed time in sec will be printed out at block end
   */
  class time_meas
  {
  public:
    /*!
     * \brief CTOR
     */
    time_meas(const char* p_Name) :
      m_Name(p_Name),
      m_TimeS(clock())
    {
    }
    /*!
     * \brief DTOR
     */
    ~time_meas()
    {
      clock_t t_TimeE = clock();

      s_Times[TIME_ELAPSED]   = static_cast<double>(t_TimeE - m_TimeS) / CLOCKS_PER_SEC;
      s_Times[TIME_CUMULATED] = static_cast<double>(t_TimeE - s_Times[TIME_START]) / CLOCKS_PER_SEC;

      log_special("Elapsed time of %s = %2.4f sec (%2.4f cumulative)", m_Name.c_str(), s_Times[0], s_Times[1]);

      s_Name = m_Name;
    }
    /*!
     * \brief Get time event
     */
    static double getTime(const time_modes p_TimeMode = TIME_ELAPSED)
    {
      return s_Times[p_TimeMode];
    }
  private:
    /*!
     * \brief Name of the block to be measured
     */
    std::string m_Name;
    /*!
     * \brief Name of the last block that was measured
     */
    static std::string s_Name;
    /*!
     * \brief Start time of block
     */
    clock_t m_TimeS;
    /*!
     * \brief Last elapsed times (normal and cumulated)
     */
    static double s_Times[TIME_GUARD];
  };
}

#define TIMEMEAS(X) base::time_meas timemeas((X))

#endif
