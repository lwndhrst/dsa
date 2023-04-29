#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define SIZE 1024

typedef struct MinHeap {
    size_t len;
    int data[SIZE];
} MinHeap;

size_t parent(size_t idx) {
    return (idx - 1) / 2;
}

size_t left_child(size_t idx) {
    return 2 * idx + 1;
}

size_t right_child(size_t idx) {
    return 2 * idx + 2;
}

int heapify_up(MinHeap *heap, size_t idx) {
    size_t current_idx = idx;
    int current_val = heap->data[current_idx];

    while (current_idx > 0) {
        size_t parent_idx = parent(current_idx);
        int parent_val = heap->data[parent_idx];

        if (current_val >= parent_val) 
            return 0;

        heap->data[parent_idx] = current_val;
        heap->data[current_idx] = parent_val;
        current_idx = parent_idx;
    }

    return 0;
}

int heapify_down(MinHeap *heap, size_t idx) {
    size_t current_idx = idx;
    int current_val = heap->data[current_idx];

    while (current_idx < heap->len - 1) {
        size_t left_idx = left_child(current_idx);
        if (left_idx >= heap->len)
            return 0;

        size_t right_idx = right_child(current_idx);
        size_t min_idx;
        if (right_idx >= heap->len) {
            min_idx = left_idx;
        } else {
            int left_val = heap->data[left_idx];
            int right_val = heap->data[right_idx];
            min_idx = left_val < right_val ? left_idx : right_idx;
        }

        int min_val = heap->data[min_idx];
        if (min_val >= current_val)
            return 0;

        heap->data[min_idx] = current_val;
        heap->data[current_idx] = min_val;
        current_idx = min_idx;
    }

    return 0;
}

int push(MinHeap *heap, int value) {
    heap->data[heap->len] = value;
    heapify_up(heap, heap->len);
    heap->len += 1;

    return 0;
}

int pop(MinHeap *heap) {
    if (heap->len == 0)
        return -1;

    int val = heap->data[0];

    if (heap->len == 1) {
        heap->len = 0;
        return val;
    }

    heap->len -= 1;
    heap->data[0] = heap->data[heap->len];
    heapify_down(heap, 0);

    return val;
}

int test_weak_order(MinHeap *heap, size_t max_idx) {
    for (int i = 0; i < max_idx; i += 1) {
        size_t left_idx = left_child(i);
        size_t right_idx = right_child(i);

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

    for (int i = 0; i < SIZE; i += 1) {
        push(&heap, rand());
        test_weak_order(&heap, i);
    }

    if (heap.len != SIZE)
        printf("Expected: heap.len == %zu but got %d instead\n", heap.len, SIZE);

    for (int i = 0; i < SIZE; i += 1) {
        int val = pop(&heap);
        test_weak_order(&heap, SIZE - i);
    }

    return 0;
}
