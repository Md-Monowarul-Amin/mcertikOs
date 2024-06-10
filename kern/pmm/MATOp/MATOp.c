#include <lib/debug.h>
#include "import.h"

#define PAGESIZE 4096
#define VM_USERLO 0x40000000
#define VM_USERHI 0xF0000000
#define VM_USERLO_PI (VM_USERLO / PAGESIZE)
#define VM_USERHI_PI (VM_USERHI / PAGESIZE)

/**
 * Allocate a physical page.
 *
 * 1. First, implement a naive page allocator that scans the allocation table (AT)
 *    using the functions defined in import.h to find the first unallocated page
 *    with normal permissions.
 *    (Q: Do you have to scan the allocation table from index 0? Recall how you have
 *    initialized the table in pmem_init.)
 *    Then mark the page as allocated in the allocation table and return the page
 *    index of the page found. In the case when there is no available page found,
 *    return 0.
 * 2. Optimize the code using memoization so that you do not have to
 *    scan the allocation table from scratch every time.
 */

unsigned int last_free = VM_USERLO_PI;
unsigned int palloc()
{
    if (get_nps() == 0)
    {
        return 0;
    }

    // TODO: memoization
    for (unsigned int i = last_free; i < VM_USERHI_PI; i++)
    {
        if (at_is_norm(i) > 0 && !at_is_allocated(i))
        {
            at_set_allocated(i, 1);
            last_free = i + 1;
            return i;
        }
    }
    return 0;
}

/**
 * Free a physical page.
 *
 * This function marks the page with given index as unallocated
 * in the allocation table.
 *
 * Hint: Simple.
 */
void pfree(unsigned int pfree_index)
{
    at_set_allocated(pfree_index, 0);
    if (pfree_index < last_free)
    {
        last_free = pfree_index;
    }
}
