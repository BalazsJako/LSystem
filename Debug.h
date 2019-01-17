
#pragma once

#ifdef _DEBUG
#  ifdef _MSC_VER
#    define ASSERT(x) (void)((!!(x)) || (__debugbreak(), (false)))
#  else
#    define ASSERT(x) assert(x)
#  endif
#else
#  define ASSERT(x)
#endif

