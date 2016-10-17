#ifndef THREAD_UTIL_H
#define THREAD_UTIL_H

#include <boost/thread.hpp>

boost::mutex mutex;
boost::condition_variable returnReady;

#endif
