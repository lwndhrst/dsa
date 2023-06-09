package min_heap

import "core:fmt"

MinHeap :: struct {
	len: u64,
	data: ^[dynamic]u64,
}

push :: proc(heap: ^MinHeap, value: u64) {
	append(heap.data, value)
	heapify_up(heap, heap.len)
	heap.len += 1
}

pop :: proc(heap: ^MinHeap) -> Maybe(u64) {
	if heap.len == 0 {
		return nil
	}

	val := heap.data[0]

	if heap.len == 1 {
		heap.len  = 0
		return val
	}

	heap.len -= 1
	heap.data[0] = heap.data[heap.len]
	heapify_down(heap, 0)

	return val
}

heapify_up :: proc(heap: ^MinHeap, idx: u64) {
	current_idx := idx
	current_val := heap.data[current_idx]

	for current_idx > 0 {
		parent_idx := parent(current_idx)
		parent_val := heap.data[parent_idx]

		if current_val >= parent_val {
			return
		}

		heap.data[parent_idx]  = current_val
		heap.data[current_idx] = parent_val
		current_idx = parent_idx
	}
}

heapify_down :: proc(heap: ^MinHeap, idx: u64) {
	current_idx := idx
	current_val := heap.data[current_idx]

	for current_idx < heap.len - 1 {
		left_idx := left_child(current_idx)

		if left_idx >= heap.len {
			return
		}
		
		min_idx	  := left_idx
		right_idx := right_child(current_idx)

		if right_idx < heap.len {
			left_val  := heap.data[left_idx]
			right_val := heap.data[right_idx]
			min_idx = left_val < right_val ? left_idx : right_idx
		}

		min_val := heap.data[min_idx]

		if min_val >= current_val {
			return
		}

		heap.data[min_idx]		= current_val
		heap.data[current_idx] 	= min_val
		current_idx = min_idx
	}
}

parent :: proc(idx: u64) -> u64 {
	return (idx - 1) / 2
}

left_child :: proc(idx: u64) -> u64 {
	return 2 * idx + 1
}

right_child :: proc(idx: u64) -> u64 {
	return 2 * idx + 2
}



////////////////////////////////////////////////////////////////////

import "core:testing"
import "core:math/rand"

expect :: testing.expect

@(test)
test_push_pop :: proc(t: ^testing.T) {
	data := make([dynamic]u64)
	defer delete(data)

	heap := MinHeap { 0, &data }

	expect_slice: []u64

	push(&heap, 10)
	expect_slice = []u64{10}
	expect(t, 
		compare_slices(heap.data[:heap.len], expect_slice),
		fmt.tprintf("Expected: %v, but got %v instead\n", expect_slice, heap.data[:]),
	)

	push(&heap, 7)
	expect_slice = []u64{7, 10}
	expect(t, 
		compare_slices(heap.data[:heap.len], expect_slice),
		fmt.tprintf("Expected: %v, but got %v instead\n", expect_slice, heap.data[:]),
	)

	push(&heap, 13)
	expect_slice = []u64{7, 10, 13}
	expect(t, 
		compare_slices(heap.data[:heap.len], expect_slice),
		fmt.tprintf("Expected: %v, but got %v instead\n", expect_slice, heap.data[:]),
	)

	push(&heap, 3)
	expect_slice = []u64{3, 7, 13, 10}
	expect(t, 
		compare_slices(heap.data[:heap.len], expect_slice),
		fmt.tprintf("Expected: %v, but got %v instead\n", expect_slice, heap.data[:]),
	)

	val		  : Maybe(u64)
	expect_val: Maybe(u64)

	val = pop(&heap)
	expect_val = 3
	expect(t, 
		val == expect_val,
		fmt.tprintf("Expected: %v, but got %v instead\n", expect_val, val),
	)
	expect_slice = []u64{7, 10, 13}
	expect(t, 
		compare_slices(heap.data[:heap.len], expect_slice),
		fmt.tprintf("Expected: %v, but got %v instead\n", expect_slice, heap.data[:]),
	)

	val = pop(&heap)
	expect_val = 7
	expect(t, 
		val == expect_val,
		fmt.tprintf("Expected: %v, but got %v instead\n", expect_val, val),
	)
	expect_slice = []u64{10, 13}
	expect(t, 
		compare_slices(heap.data[:heap.len], expect_slice),
		fmt.tprintf("Expected: %v, but got %v instead\n", expect_slice, heap.data[:]),
	)

	val = pop(&heap)
	expect_val = 10
	expect(t, 
		val == expect_val,
		fmt.tprintf("Expected: %v, but got %v instead\n", expect_val, val),
	)
	expect_slice = []u64{13}
	expect(t, 
		compare_slices(heap.data[:heap.len], expect_slice),
		fmt.tprintf("Expected: %v, but got %v instead\n", expect_slice, heap.data[:]),
	)

	val = pop(&heap)
	expect_val = 13
	expect(t, 
		val == expect_val,
		fmt.tprintf("Expected: %v, but got %v instead\n", expect_val, val),
	)
	expect_slice = []u64{}
	expect(t, 
		compare_slices(heap.data[:heap.len], expect_slice),
		fmt.tprintf("Expected: %v, but got %v instead\n", expect_slice, heap.data[:]),
	)

	val = pop(&heap)
	expect_val = nil
	expect(t, 
		val == expect_val,
		fmt.tprintf("Expected: %v, but got %v instead\n", expect_val, val),
	)
	expect_slice = []u64{}
	expect(t, 
		compare_slices(heap.data[:heap.len], expect_slice),
		fmt.tprintf("Expected: %v, but got %v instead\n", expect_slice, heap.data[:]),
	)
}

@(test)
test_n_rand_elems :: proc(t: ^testing.T) {
	n_elems :: 1024

	data := make([dynamic]u64, 0, n_elems)
	defer delete(data)

	heap := MinHeap { 0, &data }
	
	rng  := rand.create(0)
	rand.init_as_system(&rng)

	for i in 0..<n_elems {
		val := rand.uint64(&rng)
		push(&heap, val)
		check_weak_order(&heap, t)
	}

	expect(t, 
		heap.len == n_elems,
		fmt.tprintf("Expected: %v, but got %v instead\n", n_elems, heap.len),
	)

	for i in 0..<n_elems {
		pop(&heap)
		check_weak_order(&heap, t)
	}

	check_weak_order :: proc(heap: ^MinHeap, t: ^testing.T) {
		for val, i in heap.data {
			idx := cast(u64)i

			left_idx := left_child(idx)
			if left_idx >= heap.len {
				continue
			}
			expect(t, 
				val <= heap.data[left_idx],
				fmt.tprintf("Weak order is borked (left side)\n"),
			)

			right_idx := right_child(idx)
			if right_idx >= heap.len {
				continue
			}
			expect(t, 
				val <= heap.data[right_idx],
				fmt.tprintf("Weak order is borked (right side)\n"),
			)
		}
	}
}

compare_slices :: proc(a: []u64, b: []u64) -> bool {
	// fmt.printf("Comparing slices:\n  A: %v\n  B: %v\n", a, b)

	if len(a) != len(b) {
		return false
	}

	for v, i in a {
		if v != b[i] {
			return false
		}
	}

	return true
}
