/*
    Problem: Reverse Pairs (LeetCode 493)
    Given an array nums, we call (i, j) a reverse pair if i < j and nums[i] > 2 * nums[j].
    Return the number of important reverse pairs.

    Approaches:
    ---------------------------------------------------------
    1) Brute Force
       - Iterate through each pair (i, j) such that i < j.
       - Check if nums[i] > 2 * nums[j].
       - Increment count if condition is met.
       
       Time Complexity: O(N^2) - Two nested loops.
       Space Complexity: O(1) - No extra space used.
       Result: TLE (Time Limit Exceeded) on large inputs.

    2) Optimal Approach: Modified Merge Sort (Divide and Conquer)
       - Similar to "Count Inversions", but the condition is different.
       - We split the array into two halves: left (low..mid) and right (mid+1..high).
       - We recursively sort and count pairs in both halves.
       - THE TRICK: Before merging the two sorted halves, we count the reverse pairs.
         Since both halves are sorted:
         For each element in the left half 'nums[i]', we count elements in the right half
         'nums[j]' such that nums[i] > 2 * nums[j].
       - After counting, we perform the standard merge operation to sort the array
         so that it can be used for future recursive calls.

       Time Complexity: O(2N log N) ≈ O(N log N)
                        - The merge sort takes O(N log N).
                        - The counting part (findpairs) takes O(N) for each merge step.
                        - Total = O(N log N + N log N) = O(N log N).
       Space Complexity: O(N)
                        - Temporary vector used during the merge step.

    Note on "2LL": We use 2LL * nums[right] to prevent integer overflow when 
    nums[right] is a large integer (e.g., INT_MAX).
*/

class Solution {
public:
    // Standard merge function to sort two halves
    void merge(int start, int mid, int end, vector<int>& nums)
    {
        vector<int> temp;
        int left = start;
        int right = mid + 1;
        
        // Comparison and merge
        while(left <= mid && right <= end)
        {
            if(nums[left] < nums[right])
            {
                temp.push_back(nums[left]);
                left++;
            }
            else{
                temp.push_back(nums[right]);
                right++;
            }
        }
        
        // Copy remaining elements
        while(left <= mid)
        {
            temp.push_back(nums[left]);
            left++;
        }
        while(right <= end)
        {
            temp.push_back(nums[right]);
            right++;
        }
        
        // Copy back to original array
        for(int i = start; i <= end; i++)
        {
            nums[i] = temp[i - start];
        }
    }

    // Function to count reverse pairs before merging
    int findpairs(int start, int mid, int end, vector<int>& nums)
    {
        int cnt = 0;
        int right = mid + 1;
        
        for(int i = start; i <= mid; i++)
        {
            // Since left and right parts are sorted, we can simply increase the right pointer
            while(right <= end && nums[i] > 2LL * nums[right])
            {
                right++;
            }
            // If nums[i] > 2*nums[right-1], then it is also > 2*nums[start...right-2]
            // The count is simply the distance the 'right' pointer moved past (mid+1)
            cnt += (right - (mid + 1));
        }
        return cnt;
    }

    // Recursive Merge Sort function
    int mergesort(int start, int end, vector<int>& nums)
    {
        int cnt = 0;
        if(start >= end) return cnt;
        
        int mid = (start + end) / 2;
        
        cnt += mergesort(start, mid, nums);        // Count in left half
        cnt += mergesort(mid + 1, end, nums);      // Count in right half
        
        cnt += findpairs(start, mid, end, nums);   // Count cross pairs
        
        merge(start, mid, end, nums);              // Merge to keep sorted order
        
        return cnt;
    }

    int reversePairs(vector<int>& nums) {
        return mergesort(0, nums.size() - 1, nums);
    }
};

/*
    ===========================================================================
    VISUALIZATION OF THE 'findpairs' LOGIC
    ===========================================================================
    
    Scenario: We are merging two SORTED halves.
    Left Part:  [6, 13, 21]   (Indices: i)
    Right Part: [1, 2, 3, 4]  (Indices: right)
    Condition:  nums[i] > 2 * nums[right]

    -----------------------------------------------------------------------
    Step 1: i points to 6
    -----------------------------------------------------------------------
    Left:  [ 6, 13, 21 ]      Right: [ 1, 2, 3, 4 ]
             ^ (i)                     ^ (right) starts here

    Check: 6 > 2*1? (Yes) -> right++
           6 > 2*2? (Yes) -> right++
           6 > 2*3? (No)  -> STOP

    Right pointer moved to index 2 (value 3).
    Pairs found: {1, 2}
    Count += (right - start_of_right) = (2 - 0) = 2

    -----------------------------------------------------------------------
    Step 2: i points to 13 (Move i forward)
    -----------------------------------------------------------------------
    Left:  [ 6, 13, 21 ]      Right: [ 1, 2, 3, 4 ]
                 ^ (i)                     ^ (right) RESUMES from here!

    *CRITICAL*: We do NOT reset 'right' to start. Why?
    Because if 6 > 2*x, then 13 (which is > 6) is DEFINITELY > 2*x.
    We only need to check NEW elements further down.

    Check: 13 > 2*3? (Yes) -> right++
           13 > 2*4? (Yes) -> right++
           Right hits END -> STOP

    Pairs found: {1, 2, 3, 4}
    Count += (right - start_of_right) = (4 - 0) = 4

    -----------------------------------------------------------------------
    Step 3: i points to 21
    -----------------------------------------------------------------------
    Left:  [ 6, 13, 21 ]      Right: [ 1, 2, 3, 4 ] (Exhausted)
                     ^ (i)                       ^ (right)

    Check: Right is already at end.
    Count += (4 - 0) = 4

    -----------------------------------------------------------------------
    Total Count = 2 + 4 + 4 = 10
    ===========================================================================
*/