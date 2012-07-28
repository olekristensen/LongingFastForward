
#if defined(__APPLE__)

#include <CoreServices/CoreServices.h>
#include <mach/mach.h>
#include <mach/mach_time.h>
#include <unistd.h>

unsigned long long currentTimeNano() {
	uint64_t t = mach_absolute_time();
  Nanoseconds tNano = AbsoluteToNanoseconds(*(AbsoluteTime*)&t);
  return *(uint64_t *)&tNano;
}

unsigned long long currentTimeMillis() {
	return currentTimeNano()/1000000;
}

#elif defined(__linux)

unsigned long long currentTimeNano() {
  struct timespec t;
  clock_gettime(CLOCK_MONOTONIC, &t);
  return t.tv_sec*1000000000 + t.tv_nsec;
}

unsigned long long currentTimeMillis() {
  struct timespec t;
  clock_gettime(CLOCK_MONOTONIC, &t);
  return t.tv_sec*1000 + t.tv_nsec/1000000;
}

#elif defined(_WIN32)

#error "Not ported to Win32. Can you help?"

#endif

