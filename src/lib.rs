pub fn add(left: usize, right: usize) -> usize {
    left + right
}

#[cfg(test)]
mod test {
    use super::*;
    
    #[test]
    fn test_add() {
        assert_eq!(4, add(2, 2));
    }
}
