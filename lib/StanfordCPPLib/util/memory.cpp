/*
 * memory.cpp
 * ----------
 * This file defines utility functions dealing with memory regions and
 * allocation.
 *
 * @author Marty Stepp
 * @version 2018/12/04
 * - initial version
 */

#define INTERNAL_INCLUDE 1
#include <memory.h>
#include <cstddef>
#undef INTERNAL_INCLUDE

namespace stanfordcpplib {
namespace memory {

void computeMemoryDistances(
        void* const p,
        unsigned long int& stackDist,
        unsigned long int& heapDist,
        unsigned long int& staticDist) {
    static int si = 0;
    int i = 0;
    int* pi = new int(0);

    // don't use abs/subtraction here because it can over/underflow with very large integers
    stackDist = p > &i ?
            reinterpret_cast<std::ptrdiff_t>(p) - reinterpret_cast<std::ptrdiff_t >(&i) :
            reinterpret_cast<std::ptrdiff_t >(&i) - reinterpret_cast<std::ptrdiff_t >(p);
    heapDist = p > pi ?
            reinterpret_cast<std::ptrdiff_t >(p) - reinterpret_cast<std::ptrdiff_t >(pi) :
            reinterpret_cast<std::ptrdiff_t >(pi) - reinterpret_cast<std::ptrdiff_t >(p);
    staticDist = p > &si ?
            reinterpret_cast<std::ptrdiff_t >(p) - reinterpret_cast<std::ptrdiff_t >(&si) :
            reinterpret_cast<std::ptrdiff_t >(&si) - reinterpret_cast<std::ptrdiff_t >(p);
    delete pi;
}

bool isOnHeap(void* const p) {
    unsigned long int stackDist, heapDist, staticDist;
    computeMemoryDistances(p, stackDist, heapDist, staticDist);
    return heapDist < stackDist && heapDist < staticDist;
}

bool isOnStack(void* const p) {
    unsigned long int stackDist, heapDist, staticDist;
    computeMemoryDistances(p, stackDist, heapDist, staticDist);
    return stackDist < heapDist && stackDist < staticDist;
}

bool isOnStatic(void* const p) {
    unsigned long int stackDist, heapDist, staticDist;
    computeMemoryDistances(p, stackDist, heapDist, staticDist);
    return staticDist < stackDist && staticDist < heapDist;
}

} // namespace memory
} // namespace stanfordcpplib
