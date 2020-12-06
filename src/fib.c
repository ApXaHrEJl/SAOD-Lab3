#include "fib.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

fibheap* make_heap()
{
    fibheap* heap = malloc(sizeof(fibheap));
    heap->n = 0;
    heap->min = NULL;
    return heap;
}

fibnode* add_node_to_root(fibnode* heap, fibnode* node)
{
    if (heap == NULL) {
        return node;
    }
    if (heap->left == heap) {
        heap->left = node;
        node->right = heap;
        node->left = heap;
        heap->right = node;
    } else {
        fibnode* lnode = heap->left;
        heap->left = node;
        node->right = heap;
        node->left = lnode;
        lnode->right = node;
    }
    return heap;
}

fibheap* fibheap_insert(fibheap* heap, int key)
{
    fibnode* new_node = malloc(sizeof(fibnode));
    new_node->key = key;
    new_node->degree = 0;
    new_node->mark = 0;
    new_node->parent = NULL;
    new_node->left = new_node;
    new_node->right = new_node;
    add_node_to_root(heap->min, new_node);
    if (heap->min == NULL) {
        heap->min = new_node;
    }
    heap->n += 1;
    return heap;
}

fibnode* fibheap_min(fibheap* heap)
{
    return heap->min;
}

fibheap* fibheaplinks(fibheap* heap1, fibheap* heap2)
{
    if (!heap1->min || !heap2->min) {
        return NULL;
    }
    fibnode* left1 = heap1->min->left;
    fibnode* left2 = heap2->min->left;
    left1->right = heap2->min;
    heap2->min->left = left1;
    heap1->min->left = left2;
    left2->right = heap1->min;
    return heap1;
}

fibheap* fibheap_union(fibheap* heap1, fibheap* heap2)
{
    fibheap* new_heap = malloc(sizeof(fibheap));
    new_heap->min = heap1->min;
    fibheaplinks(heap1, heap2);
    if (heap1->min == NULL
        || (heap2->min != NULL && heap2->min->key < new_heap->min->key)) {
        new_heap->min = heap2->min;
    }
    new_heap->n = heap1->n + heap2->n;
    free(heap1);
    free(heap2);
    return new_heap;
}

void delete_node_from_heap(fibnode* node)
{
    fibnode* left = node->left;
    node->left = node->right;
    node->right = left;
}

int D(int n)
{
    return floor(log2(n));
}

void heap_link(fibheap* heap, fibnode* x, fibnode* y)
{
    x->degree += 1;
    delete_node_from_heap(y);
    y->parent = x;
    add_node_to_root(y, x->child);
    y->mark = 0;
}

void consolidate(fibheap* heap)
{
    fibnode* a[D(heap->n)];
    for (int i = 0; i < D(heap->n); i++) {
        a[i] = NULL;
    }
    for (fibnode* i = heap->min; i->right != heap->min; i = i->right) {
        fibnode* x = i;
        int d = x->degree;
        while (a[d] != NULL) {
            fibnode* y = a[d];
            if (y->key < x->key) {
                fibnode* g = y;
                y = x;
                x = g;
            }
            heap_link(heap, x, y);
            a[d] = NULL;
            d++;
        }
        a[d] = x;
    }
    heap->min = NULL;
    for (int i = 0; i < D(heap->n); i++) {
        if (a[i] != NULL) {
            add_node_to_root(heap->min, a[i]);
            if (heap->min == NULL || a[i]->key < heap->min->key) {
                heap->min = a[i];
            }
        }
    }
}

fibnode* fibheap_delete_min(fibheap* heap)
{
    fibnode* min = heap->min;
    if (!min)
        return NULL;
    if (min->child != NULL) {
        for (fibnode* i = min->child; i->right != min->child; i = i->right) {
            add_node_to_root(heap->min, i);
            i->parent = NULL;
        }
    }
    delete_node_from_heap(min);
    if (min == min->right) {
        heap->min = NULL;
    } else {
        heap->min = min->right;
        consolidate(heap);
    }
    heap->n -= 1;
    return min;
}

void cut(fibheap* heap, fibnode* node, fibnode* parent)
{
    delete_node_from_heap(node);
    parent->degree -= 1;
    add_node_to_root(heap->min, node);
    node->parent = NULL;
    node->mark = 0;
}

void cascading_cut(fibheap* heap, fibnode* node)
{
    fibnode* parent = node->parent;
    if (!parent) {
        return;
    }
    if (node->mark == 0) {
        node->mark = 1;
    } else {
        cut(heap, node, parent);
        cascading_cut(heap, parent);
    }
}

fibheap* fibheap_decrease_key(fibheap* heap, fibnode* node, int newkey)
{
    if (newkey < node->key) {
        node->key = newkey;
        fibnode* y = node->parent;
        if (y != NULL && node->key < y->key) {
            cut(heap, node, y);
            cascading_cut(heap, y);
        }
        if (node->key < heap->min->key) {
            heap->min = node;
        }
    }
    return NULL;
}

void fibheap_delete(fibheap* heap, fibnode* node)
{
    fibheap_decrease_key(heap, node, -2147483648);
    fibheap_delete_min(heap);
}
