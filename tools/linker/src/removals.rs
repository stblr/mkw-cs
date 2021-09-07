// SPDX-License-Identifier: Apache-2.0 OR MIT

use crate::removal::{Removal, RemovalMergeError, RemovalRemoveError};
use crate::section::Section;

#[derive(Debug)]
pub struct Removals {
    removals: Vec<Removal>,
}

impl Removals {
    pub fn new() -> Removals {
        Removals { removals: vec![] }
    }

    pub fn try_insert(&mut self, mut new_removal: Removal) -> Result<(), RemovalInsertError> {
        if new_removal.size() == 0 {
            return Ok(());
        }

        let new_idx = self.removals.binary_search_by_key(&new_removal.start(), |removal| removal.start());
        let new_idx = match new_idx {
            Ok(_) => return Err(RemovalInsertError::Duplicate),
            Err(new_idx) => new_idx,
        };

        let merged_with_prev = self.try_merge_with_prev(&mut new_removal, new_idx)?;
        let merged_with_next = self.try_merge_with_next(&mut new_removal, new_idx)?;

        if !merged_with_prev && !merged_with_next {
            self.removals.insert(new_idx, new_removal);
        } else if merged_with_prev && merged_with_next {
            self.removals.remove(new_idx);
        }

        Ok(())
    }

    fn try_merge_with_prev(&mut self, new_removal: &mut Removal, new_idx: usize) -> Result<bool, RemovalInsertError> {
        if new_idx == 0 {
            return Ok(false);
        }

        let prev_removal = match self.removals.get_mut(new_idx - 1) {
            Some(prev_removal) => prev_removal,
            None => return Ok(false),
        };

        match prev_removal.try_merge(*new_removal) {
            Ok(merged) => {
                *prev_removal = merged;
                *new_removal = merged;
                Ok(true)
            },
            Err(RemovalMergeError::Overlapping) => return Err(RemovalInsertError::Overlapping),
            Err(_) => Ok(false),
        }
    }

    fn try_merge_with_next(&mut self, new_removal: &mut Removal, new_idx: usize) -> Result<bool, RemovalInsertError> {
        let next_removal = match self.removals.get_mut(new_idx) {
            Some(next_removal) => next_removal,
            None => return Ok(false),
        };

        match new_removal.try_merge(*next_removal) {
            Ok(merged) => {
                *new_removal = merged;
                *next_removal = merged;
                Ok(true)
            },
            Err(RemovalMergeError::Overlapping) => return Err(RemovalInsertError::Overlapping),
            Err(_) => Ok(false),
        }
    }

    pub fn try_remove(&mut self, size: u32, section: Section) -> Result<u32, RemovalRemoveError> {
        for (idx, removal) in self.removals.iter_mut().enumerate() {
            if removal.section() != section {
                continue;
            }

            let start = match removal.try_remove(size) {
                Ok(start) => start,
                Err(_) => continue,
            };

            if removal.size() == 0 {
                self.removals.remove(idx);
            }

            return Ok(start);
        }

        Err(RemovalRemoveError::NotEnoughSpace)
    }

    pub fn iter(&self) -> impl Iterator<Item=&Removal> {
        self.removals.iter()
    }
}

#[derive(Debug)]
pub enum RemovalInsertError {
    Duplicate,
    Overlapping,
}
