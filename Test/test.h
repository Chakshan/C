#include <stdio.h>
#include <stdlib.h>

static inline void handle_assert_failure(const char *x, const char *func, const char *file, int line)
{
	fprintf(stderr, "Assertion failed: %s, function %s, file %s, line %d\n", x, func, file, line);
	#ifdef ABORT_AFTER_FAILURE
		abort();
	#endif
}

#define ASSERT(x) \
	((x) ? (void)0 : handle_assert_failure(#x, __func__, __FILE__, __LINE__))

static void setup();
static void cleanup();

#define RUN_TEST(x) \
	do { \
		printf("Running: " #x); \
		setup(); \
		x(); \
		cleanup(); \
		printf("...completed\n"); \
	} while (0)
	