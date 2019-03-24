/*
 * managed.h
 * ---------
 * This file defines a template class named Managed that can be used to define
 * classes that perform checks on how their objects are allocated.
 *
 * @author Marty Stepp
 * @version 2018/12/04
 * - initial version
 */

#ifndef _managed_h
#define _managed_h

#include <cstddef>
#include <typeinfo>
#include <error.h>
#include <map.h>
#include <memory.h>
#include <set.h>
#include <strlib.h>

/**
 * The Managed template class can be used to define classes that perform checks
 * on how their objects are allocated. For example, in a Managed subclass you
 * can forbid objects from being allocated on the stack
 */
template <typename T>
class Managed {
public:
    /**
     * Overloaded new operator tracks heap memory allocated to newly created
     * objects.
     */
    void* operator new(size_t n) {
        void* p = ::operator new(n);
        s_allocatedUsingNew().add(p);
        s_freed().remove(p);
        return p;
    }

    /**
     * Overloaded delete operator tracks what pointers were deleted and can
     * print errors if the client, for example, tries to delete the same pointer
     * twice by mistake.
     */
    void operator delete(void* p) {
        ::operator delete(p);
        if (s_freed().contains(p)) {
            error("You are trying to delete the same pointer twice: " + pointerToString(p));
        }
//        if (!s_allocated().contains(p)) {
//            error("You are trying to delete a pointer that was never allocated: " + pointerToString(p));
//        }
        s_allocated().remove(p);
        s_freed().add(p);
        s_deleteCount()++;
    }

protected:
    /**
     * Returns the total number of objects of this type that have been created.
     */
    static int getInstanceCount() {
        return s_instanceCount();
    }

    /**
     * Returns the total number of objects of this type that have been deleted.
     */
    static int getDeleteCount() {
        return s_deleteCount();
    }

    /**
     * Returns the total number of objects of this type that seem to be memory
     * leaks (the number of objects created minus the number deleted).
     */
    static int getLeakCount() {
        return getInstanceCount() - getDeleteCount();
    }

    /**
     * Returns true if this type has a probable memory leak
     * (seems to have some objects that have been created but not freed).
     */
    static bool hasMemoryLeak() {
        return getLeakCount() > 0;
    }

    /**
     * Sets the numbers of objects created and deleted back to zero.
     * This would be used by autograder programs between leak tests.
     */
    static void resetInstanceAndFreeCounts() {
        s_instanceCount() = 0;
        s_deleteCount() = 0;
    }

    /**
     * Sets whether this type allows its objects to be allocated on the heap.
     * Default true.
     */
    static void setHeapAllocationAllowed(bool allowed = true) {
        s_heapAllowed() = allowed;
    }

    /**
     * Sets whether this type allows its objects to be allocated on the heap
     * but without the use of the 'new' operator.
     * This is to handle the subtle case of an outer object containing this
     * object, where the outer object is heap-allocated but the inner object
     * is declared as an object rather than as a pointer.
     * Setting this to false ensures that all uses of this class must be done
     * via pointers and new rather than declaring objects directly.
     * Default true.
     */
    static void setHeapAllocationWithoutNewAllowed(bool allowed = true) {
        s_heapWithoutNewAllowed() = allowed;
    }

    /**
     * Sets whether this type allows its objects to be allocated on the stack,
     * such as local variables declared in functions.
     */
    static void setStackAllocationAllowed(bool allowed) {
        s_stackAllowed() = allowed;
    }

    /**
     * Sets whether this type allows its objects to be allocated in static
     * storage, such as static variables declared in functions.
     */
    static void setStaticAllocationAllowed(bool allowed) {
        s_staticAllowed() = allowed;
    }

    /**
     * Constructor that logs the construction of this object for later use.
     */
    Managed() {
        notifyOfConstruction(this);
    }

    /**
     * Called when a new object is constructed.
     * @throw ErrorException if this object is created in an illegal way.
     */
    static void notifyOfConstruction(void* const p) {
        checkAllocation(p);
        s_allocated().add(p);
        s_instanceCount()++;
    }

    /**
     * Called when an object is deleted.
     */
    static void notifyOfDelete(T* const /*p*/) {
        // empty
    }

private:
    /**
     * Checks how this object was allocated.
     * @throw ErrorException if object was allocated in an illegal way.
     */
    static void checkAllocation(void* const p) {
        unsigned long int stackDist, heapDist, staticDist;
        stanfordcpplib::memory::computeMemoryDistances(p, stackDist, heapDist, staticDist);
        if (stackDist < heapDist && stackDist < staticDist) {
            if (!s_stackAllowed()) {
                const std::type_info& type = typeid(T);
                error(std::string("Stack allocation not allowed for type: ") + type.name()
                      + " (must allocate objects of this type using pointers and 'new')");
            }
        } else if (heapDist < stackDist && heapDist < staticDist) {
            if (!s_heapAllowed()) {
                const std::type_info& type = typeid(T);
                error(std::string("Heap allocation not allowed for type: ") + type.name()
                      + " (must allocate objects of this type on the stack, without pointers or 'new')");
            } else if (!s_heapWithoutNewAllowed() && !s_allocatedUsingNew().contains(p)) {
                const std::type_info& type = typeid(T);
                error(std::string("Invalid allocation for type: ") + type.name()
                      + " (must allocate objects of this type using pointers and 'new')");
            }
        } else {
            if (!s_staticAllowed()) {
                const std::type_info& type = typeid(T);
                error(std::string("Static allocation not allowed for type: ") + type.name()
                      + " (must allocate objects of this type using pointers and 'new')");
            }
        }
    }

    /**
     * A set of pointers of this type that are currently allocated.
     * @private
     */
    static Set<void*>& s_allocated() {
        static Set<void*> _s_allocated;
        return _s_allocated;
    }

    /**
     * A set of pointers of this type that are currently allocated using the
     * 'new' operator.
     * @private
     */
    static Set<void*>& s_allocatedUsingNew() {
        static Set<void*> _s_allocatedUsingNew;
        return _s_allocatedUsingNew;
    }

    /**
     * A set of pointers of this type that have been deleted.
     * @private
     */
    static Set<void*>& s_deleted() {
        static Set<void*> _s_deleted;
        return _s_deleted;
    }

    /**
     * A count of how many pointers of this type have been deleted.
     * @private
     */
    static int& s_deleteCount() {
        static int _s_deleteCount = 0;
        return _s_deleteCount;
    }

    /**
     * A count of how many objects of this type have been allocated.
     * @private
     */
    static int& s_instanceCount() {
        static int _s_instanceCount = 0;
        return _s_instanceCount;
    }

    /**
     * Whether this type allows allocating objects on the heap.
     * @private
     */
    static bool& s_heapAllowed() {
        static bool _s_heapAllowed = true;
        return _s_heapAllowed;
    }

    /**
     * Whether this type allows allocating objects on the heap
     * without the 'new' operator.
     * @private
     */
    static bool& s_heapWithoutNewAllowed() {
        static bool _s_heapWithoutNewAllowed = true;
        return _s_heapWithoutNewAllowed;
    }

    /**
     * Whether this type allows allocating objects on the stack.
     * @private
     */
    static bool& s_stackAllowed() {
        static bool _s_stackAllowed = true;
        return _s_stackAllowed;
    }

    /**
     * Whether this type allows allocating objects in static storage.
     * @private
     */
    static bool& s_staticAllowed() {
        static bool _s_staticAllowed = true;
        return _s_staticAllowed;
    }

    /**
     * Returns a string representing the name of this type.
     * @private
     */
    static std::string typeName() {
        const std::type_info& type = typeid(T);
        std::string name = type.name();

// TODO: demangle names using the function below
//       (I didn't implement this yet because the platform-dependent issues are tricky)
//        int status;
//        char* demangled = abi::__cxa_demangle(name, /* buffer */ nullptr,
//                                              /* length pointer */ nullptr, &status);
//        if (status == 0 && demangled) {
//            symname = demangled;
//        }

        return name;
    }
};

#endif // _managed_h
