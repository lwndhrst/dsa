pub fn add(left: usize, right: usize) -> usize {
    left + right
}

pub struct MinHeap {
    length: usize,
    data: Vec<usize>,
}

impl MinHeap {
    pub fn new() -> Self {
        return Self {
            length: 0,
            data: vec![],
        };
    }

    pub fn push(&mut self, value: usize) {
        self.data.push(value);
        self.heapify_up(self.length);
        self.length = self.length + 1;
    }

    pub fn pop(&mut self) -> Option<usize> {
        if self.length == 0 {
            return None;
        }

        let out = self.data.first().expect("a value").to_owned();

        if self.length == 1 {
            self.length = 0;
            self.data.clear();
            return Some(out);
        }

        self.length = self.length - 1;

        let last_val = self.data.pop().expect("a value");
        let first_val = self.data.first_mut().expect("a value");
        *first_val = last_val;
        self.heapify_down(0);

        Some(out)
    }

    fn heapify_up(&mut self, idx: usize) {
        let mut current_idx = idx;
        let current_val = self.data.get(current_idx).expect("a value").to_owned();

        while current_idx > 0 {
            let parent_idx = self.parent(current_idx);
            let parent_val = self.data.get(parent_idx).expect("a value").to_owned();

            if current_val < parent_val {
                let p = self.data.get_mut(parent_idx).expect("a value");
                *p = current_val;
                let c = self.data.get_mut(current_idx).expect("a value");
                *c = parent_val;
                current_idx = parent_idx;
            } else {
                return;
            }
        }
    }

    fn heapify_down(&mut self, idx: usize) {
        let mut current_idx = idx;
        let current_val = self.data.get(current_idx).expect("a value").to_owned();

        while current_idx < self.length - 1 {
            let left_idx = self.left_child(current_idx);
            if left_idx >= self.length {
                return;
            }
            let left_val = self.data.get(left_idx).expect("a value").to_owned();

            let right_idx = self.right_child(current_idx);
            let right_val = self.data.get(right_idx);

            // holy shit i hate myself
            match right_val {
                Some(&right_val) => {
                    if left_val < right_val && left_val < current_val {
                        let l = self.data.get_mut(left_idx).expect("a value");
                        *l = current_val;
                        let c = self.data.get_mut(current_idx).expect("a value");
                        *c = left_val;
                        current_idx = left_idx;
                    } else if right_val < current_val {
                        let r = self.data.get_mut(right_val).expect("a value");
                        *r = current_val;
                        let c = self.data.get_mut(current_idx).expect("a value");
                        *c = right_val;
                        current_idx = right_idx;
                    } else {
                        return;
                    }
                }
                None => {
                    if left_val < current_val {
                        let l = self.data.get_mut(left_idx).expect("a value");
                        *l = current_val;
                        let c = self.data.get_mut(current_idx).expect("a value");
                        *c = left_val;
                        current_idx = left_idx;
                    } else {
                        return;
                    }
                }
            }
        }
    }

    fn parent(&self, idx: usize) -> usize {
        (idx - 1) / 2
    }

    fn left_child(&self, idx: usize) -> usize {
        2 * idx + 1
    }

    fn right_child(&self, idx: usize) -> usize {
        2 * idx + 2
    }
}

#[cfg(test)]
mod test {
    use super::*;

    #[test]
    fn min_heap() {
        let mut min_heap = MinHeap::new();

        min_heap.push(10);
        assert_eq!(min_heap.length, 1);
        assert_eq!(min_heap.data, vec![10]);

        min_heap.push(7);
        assert_eq!(min_heap.length, 2);
        assert_eq!(min_heap.data, vec![7, 10]);
        
        min_heap.push(13);
        assert_eq!(min_heap.length, 3);
        assert_eq!(min_heap.data, vec![7, 10, 13]);

        min_heap.push(3);
        assert_eq!(min_heap.length, 4);
        assert_eq!(min_heap.data, vec![3, 7, 13, 10]);

        let val = min_heap.pop();
        assert_eq!(val, Some(3));
        assert_eq!(min_heap.length, 3);
        assert_eq!(min_heap.data, vec![7, 10, 13]);

        let val = min_heap.pop();
        assert_eq!(val, Some(7));
        assert_eq!(min_heap.length, 2);
        assert_eq!(min_heap.data, vec![10, 13]);

        let val = min_heap.pop();
        assert_eq!(val, Some(10));
        assert_eq!(min_heap.length, 1);
        assert_eq!(min_heap.data, vec![13]);

        let val = min_heap.pop();
        assert_eq!(val, Some(13));
        assert_eq!(min_heap.length, 0);
        assert_eq!(min_heap.data, vec![]);

        let val = min_heap.pop();
        assert_eq!(val, None);
        assert_eq!(min_heap.length, 0);
        assert_eq!(min_heap.data, vec![]);
    }
}
