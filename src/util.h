#ifndef UTIL_H
#define UTIL_H

// returns -1 from calling function if the called function does not return 0
#define ASSERT(fn) if (fn != 0) { fprintf(stderr, "Error in function '" #fn "'\n"); return -1; }

#endif
