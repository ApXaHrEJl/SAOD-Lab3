#include "fib.h"
#include <stdio.h>

int main()
{
    fibheap *heap = make_heap();
    heap = fibheap_insert(heap, 5);
    heap = fibheap_insert(heap, 3);
    printf("%d\n", fibheap_min(heap)->key);
    printf("%d\n", heap->n);
    printf("%d\n", fibheap_delete_min(heap)->key);
    printf("%d\n", heap->n);
    return 0;
}
