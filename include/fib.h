#ifndef FIB_H
#define FIB_H

typedef struct fibnode{
    struct fibnode *parent;
    int key;
    int mark;
    int degree;
    struct fibnode *left;
    struct fibnode *right;
    struct fibnode *child;
}fibnode;

typedef struct fibheap{
    int n;
    fibnode* min;
}fibheap;

fibheap* make_heap();

fibheap *fibheap_insert(fibheap *heap, int key);

fibnode *fibheap_min(fibheap *heap);

fibheap *fibheap_union(fibheap *heap1, fibheap *heap2);

fibnode *fibheap_delete_min(fibheap *heap);

fibheap *fibheap_decrease_key(fibheap *heap, fibnode *node, int newkey);

void fibheap_delete(fibheap *heap, fibnode* node);

#endif
