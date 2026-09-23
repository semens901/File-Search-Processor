#include "thread_safe_queue.h"

// ThreadSafeQueue implementation note:
// The functional implementation lives in the header to allow inlining and
// easy templating. This translation unit is intentionally empty so that
// build systems that expect a .cpp file for each module remain satisfied.
