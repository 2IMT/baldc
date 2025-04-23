#ifndef BALDC_ASSERT_H
#define BALDC_ASSERT_H

#ifndef BALDC_RELEASE
void bc_assert_fail(const char* file, int line, const char* assertion);
#  define BC_ASSERT(expr) \
      (expr) ? (void)0 : bc_assert_fail(__FILE__, __LINE__, #expr)
#else
#  define BC_ASSERT(expr)
#endif

#define BC_ASSERT_NOT_NULL(expr) BC_ASSERT((expr) != NULL)

#endif
