/*!
 * \author gergely.nyiri@gmail.com
 * \file main.cxx
 * \brief test of logger class
 */

#include "logger.hxx"

int main()
{
  base::log_(base::LOG_ERROR, "log_error");
  base::log_(base::LOG_DEBUG, "log_debug");
  base::log_(base::LOG_WARNING, "log_warning");
  base::log_(base::LOG_SPECIAL, "log_special");

  return 0;
}
