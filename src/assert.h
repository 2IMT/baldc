#ifndef BALDC_ASSERT_H
#define BALDC_ASSERT_H

#ifndef BALDC_RELEASE

void bc_assert_fail(const char* file, int line, const char* assertion);

void bc_assert_unreachable_fail(const char* file, int line);

#  define BC_ASSERT(expr) \
      (expr) ? (void)0 : bc_assert_fail(__FILE__, __LINE__, #expr)

#  define BC_ASSERT_UNREACHABLE() bc_assert_unreachable_fail(__FILE__, __LINE__)

#else

#  define BC_ASSERT(expr)

#  define BC_ASSERT_UNREACHABLE()

#endif

#endif
