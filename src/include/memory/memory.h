/*
 * Module: memory
 * File: memory.h
 * Created:
 * May 03, 2021
 * Author: Andrew Porter [<caritasdedeus@gmail.com>](mailto:caritasdedeus@gmail.com)
 *
 * Copyright &copy; 2021 Christi Crucifixi, LLC. All rights reserved.
 *
 * License: See LICENSE.txt
 *
 * A robust and efficient BC memory model implementation.
 */


#ifndef PROJECT_AQUINAS_MEMORY_H
#define PROJECT_AQUINAS_MEMORY_H

#include "platform.h"
#include <stdalign.h>
#include "dynarray.h"
#include "m_object.h"

/*
 * A memory interface for partitioning an infinite address space in $[0, +\infty)$.
 */
typedef struct memory_interface {
    /*
     * Reserves a memory partition if applicable.
     * Calling this function tells the memory interface:
     * "I need this much memory right now."
     *
     * See memory.h:m_reserve for more information.
     */
    m_object *(*m_reserve)(uqword const bits);
    
    /*
     * Resizes a memory partition if applicable.
     * Calling this function tells the memory interface:
     * "I need a different amount of memory right now."
     *
     * See memory.h:m_resize for more information.
     */
    m_object *(*m_resize)(uqword const context, uqword const identifier, udqword const bits);
    
    /*
     * Relinquishes a memory partition if applicable.
     * Calling this function tells the memory interface:
     * "I don't need this memory anymore."
     *
     * See memory.h:m_relinquish for more information.
     */
    m_object *(*m_relinquish)(uqword const context, uqword const identifier);
} memory_interface;

/*
 * Reserves a partition to be associated with the given context and identifier with at least
 * the given number of bits. The given m_object contains the context and identifier pointers
 * per the C memory model and are implementation-defined values. In BC, however, the context
 * with identifier and m_object are one and the same.
 *
 * With the BC memory model, a valid replacement for garbage collection is viable through
 * facilitated manual memory management: memory is a single, contiguous vector of state
 * with unique, user-defined identifiers. In this way, I can simply refer to state as state,
 * and provide a means to organize it for the sake of software which, by necessity, must
 * _usually_ share a single global address space on modern hardware.
 *
 * For the C implementation, the returned m_object is guaranteed to have the given context
 * and identifier provided that they were generated with m_push_context() and
 * m_push_identifier(). A void m_object may be formed through the corresponding
 * pointer_reconstruct() function in m_object.h. Note that these functions are only
 * necessary for use in C and will be decoupled from code written in AD.
 */
m_object m_reserve(uqword const context, uqword const identifier, udqword const bits);

/*
 * Resizes the partition associated with the given context and identifier to be at least the number
 * of specified bits. The given m_object contains the context and identifier pointers per the
 * C memory model and are implementation-defined values.
 *
 * With the BC memory model, a valid replacement for garbage collection is viable through
 * facilitated manual memory management: memory is a single, contiguous vector of state
 * with unique, user-defined identifiers. In this way, I can simply refer to state as state,
 * and provide a means to organize it for the sake of software which, by necessity, must
 * _usually_ share a single global address space on modern hardware.
 */
m_object m_resize(uqword const context, uqword const identifier, udqword const bits);

/*
 * Relinquishes the partition associated with the given context and identifier. The associated
 * m_object does not need to be freed with free(); the m_relinquish implementation handles
 * such calls for you. Note that use of the unmodified m_object after this call is
 * to be considered undefined behavior according to ISO C. In the BC memory model, however,
 * this is impossible since the context with identifier are also the associated numerical variable
 * name.
 *
 * With the BC memory model, a valid replacement for garbage collection is viable through
 * facilitated manual memory management: memory is a single, contiguous vector of state
 * with unique, user-defined identifiers. In this way, I can simply refer to state as state,
 * and provide a means to organize it for the sake of software which, by necessity, must
 * _usually_ share a single global address space on modern hardware.
 */
void m_relinquish(uqword const context, uqword const identifier);

#endif //PROJECT_AQUINAS_MEMORY_H
