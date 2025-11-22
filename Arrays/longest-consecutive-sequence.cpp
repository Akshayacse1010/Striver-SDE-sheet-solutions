/*
    Problem: Longest Consecutive Sequence (LeetCode 128)
    Given an unsorted array of integers nums, return the length of the longest 
    consecutive elements sequence.

    Approaches:
    ---------------------------------------------------------
    1) Brute Force
       - For every element x, search the entire array for x+1, x+2...
       - Keep track of the max length found.
       
       Time Complexity: O(N^3) or O(N^2) depending on search implementation.
       Space Complexity: O(1) (if no extra space used).
       Result: TLE (Time Limit Exceeded).

    2) Better Approach: Sorting
       - Sort the array first: O(N log N).
       - Iterate through the sorted array.
       - If current == previous + 1, increment current_streak.
       - If current == previous, continue (handle duplicates).
       - Else, reset current_streak to 1.
       
       Time Complexity: O(N log N) due to sorting.
       Space Complexity: O(1) or O(N) depending on sort implementation.

    3) Optimal Approach: Hash Set (Your Solution)
       - Put all numbers into an unordered_set for O(1) lookups.
       - Iterate through the set.
       - INTELLIGENT CHECK: Only start counting a sequence if the current number
         is the *start* of a sequence.
         How do we know? If (num - 1) is NOT in the set.
       - If (num - 1) exists, we skip this number (it's part of a sequence we 
         will count later).
       - If (num - 1) does NOT exist, we loop (num + 1), (num + 2)... 
         until the sequence breaks.

       Time Complexity: O(N)
                        - We insert all elements: O(N).
                        - We iterate the set: O(N).
                        - The inner while loop runs only when we hit the start of a sequence.
                        - Each number is visited at most twice (once in outer loop, 
                          once in inner loop). Total operations ≈ 2N -> O(N).
       Space Complexity: O(N)
                        - To store the hash set.
*/

#include <vector>
#include <unordered_set>
#include <algorithm>

using namespace std;

class Solution {
public:
    int longestConsecutive(vector<int>& nums) {
        if (nums.empty()) return 0;

        // 1. Insert all numbers into hashset for O(1) lookups
        unordered_set<int> numSet(nums.begin(), nums.end());
        
        int longestStreak = 0;

        // 2. Iterate through the set to find sequence starts
        for (int num : numSet) {
            // Check if 'num' is the start of a sequence
            // If (num - 1) exists, then 'num' is NOT the start. Skip it.
            if (numSet.find(num - 1) == numSet.end()) {
                
                int currentNum = num;
                int currentStreak = 1;

                // Count consecutive numbers
                while (numSet.find(currentNum + 1) != numSet.end()) {
                    currentNum += 1;
                    currentStreak += 1;
                }

                longestStreak = max(longestStreak, currentStreak);
            }
        }

        return longestStreak;
    }
};

/*
    ===========================================================================
    VISUALIZATION OF THE "START OF SEQUENCE" LOGIC
    ===========================================================================
    
    Input Array:  [ 100, 4, 200, 1, 3, 2 ]
    Hash Set:     { 100, 4, 200, 1, 3, 2 }

    We iterate through the set (order in unordered_set is random, let's assume random):

    -----------------------------------------------------------------------
    1. Pick 100
    -----------------------------------------------------------------------
       Check: Is (100 - 1 = 99) in Set? -> NO.
       Conclusion: 100 is a START.
       Loop:
         - 101 in set? No.
       Streak = 1.  (Max = 1)

    -----------------------------------------------------------------------
    2. Pick 4
    -----------------------------------------------------------------------
       Check: Is (4 - 1 = 3) in Set? -> YES.
       Conclusion: 4 is NOT a start (it's part of 3's sequence).
       Action: SKIP (continue)

    -----------------------------------------------------------------------
    3. Pick 200
    -----------------------------------------------------------------------
       Check: Is (200 - 1 = 199) in Set? -> NO.
       Conclusion: 200 is a START.
       Loop:
         - 201 in set? No.
       Streak = 1.  (Max = 1)

    -----------------------------------------------------------------------
    4. Pick 1  <-- IMPORTANT
    -----------------------------------------------------------------------
       Check: Is (1 - 1 = 0) in Set? -> NO.
       Conclusion: 1 is a START.
       Loop:
         - 2 in set? Yes. (Streak = 2)
         - 3 in set? Yes. (Streak = 3)
         - 4 in set? Yes. (Streak = 4)
         - 5 in set? No.
       Streak = 4.  (Max = 4)

    -----------------------------------------------------------------------
    5. Pick 3
    -----------------------------------------------------------------------
       Check: Is (3 - 1 = 2) in Set? -> YES.
       Action: SKIP.

    -----------------------------------------------------------------------
    6. Pick 2
    -----------------------------------------------------------------------
       Check: Is (2 - 1 = 1) in Set? -> YES.
       Action: SKIP.

    -----------------------------------------------------------------------
    Final Result: 4
    
    Why O(N)? 
    Notice that 2, 3, and 4 were visited in the outer loop but SKIPPED immediately.
    They were only processed inside the while loop when we started at 1.
    Every number is processed effectively once.
    ===========================================================================
*/