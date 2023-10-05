#include <assert.h>
#include "stack.h"

ELEM_T *my_memset(ELEM_T *destination, ELEM_T elem, size_t size)
{
  assert(destination != NULL);
  assert(size > 0);

  for (size_t i = 0; i < size; i++)
    destination[i] = elem;

  assert(destination != NULL);

  return destination;
}
