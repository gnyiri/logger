/*!
 * \file logger.cxx
 * \brief general purpose logger class
 * \author gergely.nyiri@gmail.com
 */

#include "logger.hxx"

namespace base
{
  unsigned int logger::m_LogLevel = logger::GetLevel();
  log_counter logger::m_LogCounter = logger::GetInitialCounter();
  const char* logger::m_LogPrefix = "[LOGGER]";
  std::string time_meas::s_Name;
  double time_meas::s_Times[TIME_GUARD] = { static_cast<double>(clock()) };

  //-------------------------------------------------------
  void log_(log_level p_LogLevel, const char* p_Msg, ...)
  {
    if(logger::Active(p_LogLevel))
    {
      va_list t_Argp;

      va_start(t_Argp, p_Msg);
      logger::GetInstance().Log(p_LogLevel, p_Msg, t_Argp);
      va_end(t_Argp);
    }
  }
  //-------------------------------------------------------
  void log_error(const char* p_Msg, ...)
  {
    if(logger::Active(LOG_ERROR))
    {
      va_list t_Argp;

      va_start(t_Argp, p_Msg);
      logger::GetInstance().Log(LOG_ERROR, p_Msg, t_Argp);
      va_end(t_Argp);
    }
  }
  //-------------------------------------------------------
  void log_warning(const char* p_Msg, ...)
  {
    if(logger::Active(LOG_WARNING))
    {
      va_list t_Argp;

      va_start(t_Argp, p_Msg);
      logger::GetInstance().Log(LOG_WARNING, p_Msg, t_Argp);
      va_end(t_Argp);
    }
  }
  //-------------------------------------------------------
  void log_debug(const char* p_Msg, ...)
  {
    if(logger::Active(LOG_DEBUG))
    {
      va_list t_Argp;

      va_start(t_Argp, p_Msg);
      logger::GetInstance().Log(LOG_DEBUG, p_Msg, t_Argp);
      va_end(t_Argp);
    }
  }
  //-------------------------------------------------------
  void log_special(const char* p_Msg, ...)
  {
    if(logger::Active(LOG_SPECIAL))
    {
      va_list t_Argp;

      va_start(t_Argp, p_Msg);
      logger::GetInstance().Log(LOG_SPECIAL, p_Msg, t_Argp);
      va_end(t_Argp);
    }
  }
}
