#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define MIN_HEAP_SIZE 1024

typedef struct MinHeap {
    size_t len;
    int data[MIN_HEAP_SIZE];
} MinHeap;

size_t min_heap_parent_idx(size_t idx) {
    return (idx - 1) / 2;
}

size_t min_heap_left_child_idx(size_t idx) {
    return 2 * idx + 1;
}

size_t min_heap_right_child_idx(size_t idx) {
    return 2 * idx + 2;
}

int min_heap_heapify_up(MinHeap *heap, size_t idx) {
    size_t current_idx = idx;
    int current_val = heap->data[current_idx];

    while (current_idx > 0) {
        size_t parent_idx = min_heap_parent_idx(current_idx);
        int parent_val = heap->data[parent_idx];

        if (current_val >= parent_val) 
            return 0;

        heap->data[parent_idx]  = current_val;
        heap->data[current_idx] = parent_val;
        current_idx = parent_idx;
    }

    return 0;
}

int min_heap_heapify_down(MinHeap *heap, size_t idx) {
    size_t current_idx = idx;
    int current_val = heap->data[current_idx];

    while (current_idx < heap->len - 1) {
        size_t left_idx = min_heap_left_child_idx(current_idx);
        if (left_idx >= heap->len)
            return 0;

        size_t min_idx   = left_idx;
        size_t right_idx = min_heap_right_child_idx(current_idx);

        if (right_idx < heap->len) {
            int left_val  = heap->data[left_idx];
            int right_val = heap->data[right_idx];
            min_idx = left_val < right_val ? left_idx : right_idx;
        }

        int min_val = heap->data[min_idx];
        if (min_val >= current_val)
            return 0;

        heap->data[min_idx]     = current_val;
        heap->data[current_idx] = min_val;
        current_idx = min_idx;
    }

    return 0;
}

int min_heap_push(MinHeap *heap, int value) {
    heap->data[heap->len] = value;
    min_heap_heapify_up(heap, heap->len);
    heap->len += 1;

    return 0;
}

int min_heap_pop(MinHeap *heap) {
    if (heap->len == 0)
        return -1;

    int val = heap->data[0];

    if (heap->len == 1) {
        heap->len = 0;
        return val;
    }

    heap->len -= 1;
    heap->data[0] = heap->data[heap->len];
    min_heap_heapify_down(heap, 0);

    return val;
}

int _test_weak_order(MinHeap *heap) {
    for (int i = 0; i < heap->len; i += 1) {
        size_t left_idx  = min_heap_left_child_idx(i);
        size_t right_idx = min_heap_right_child_idx(i);

        if (left_idx >= heap->len)
            continue;

        if (heap->data[i] > heap->data[left_idx]) {
            printf("Weak order is borked! (left side)\n");
            return -1;
        }

        if (right_idx >= heap->len)
            continue;

        if (heap->data[i] > heap->data[right_idx]) {
            printf("Weak order is borked! (right side)\n");
            return -1;
        }
    }

    return 0;
}

int main(void) {
    srand(69420);

    MinHeap heap;
    heap.len = 0;

    for (int i = 0; i < MIN_HEAP_SIZE; i += 1) {
        min_heap_push(&heap, rand());
        _test_weak_order(&heap);
    }

    if (heap.len != MIN_HEAP_SIZE)
        printf("Expected: heap.len == %zu but got %d instead\n", heap.len, MIN_HEAP_SIZE);

    for (int i = 0; i < MIN_HEAP_SIZE; i += 1) {
        int val = min_heap_pop(&heap);
        _test_weak_order(&heap);
    }

    return 0;
}
