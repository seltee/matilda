#ifndef UTILS_H
#define UTILS_H

#ifndef EXPORT
#define EXPORT __declspec(dllexport)
#endif

#ifndef nullptr
#define nullptr 0
#endif

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#endif