/*
    Problem: Count Inversions (number of pairs (i,j) with i < j and arr[i] > arr[j])

    Approaches (short):
    ---------------------------------------------------------
    1) Brute force
       Check every pair (i,j) -> O(n^2) time, O(1) space.
       Simple but too slow for large n.

    2) Optimal: Modified Merge Sort (used here)
       - Use the divide-and-conquer structure of merge sort.
       - Count:
            inversions in left half
          + inversions in right half
          + cross-inversions during merge

       Key observation (merge step):
         When merging two sorted halves (left and right),
         if right[j] < left[i], then right[j] is smaller than ALL
         remaining elements in left (from i..mid). So we add:
                (mid - i + 1)
         to the inversion count and place right[j] into the merged array.

    Example (small):
        left = [2, 5, 7], right = [1, 3, 6]
        - 1 < 2 → add (3) inversions (pairs: (2,1),(5,1),(7,1))
        - 3 < 5 → add (2) inversions (pairs: (5,3),(7,3))
        - 6 < 7 → add (1) inversion (pair: (7,6))

    Pitfalls / notes:
    - Use (mid - left_index + 1) when counting cross-inversions.
    - Be careful copying merged values back into original array:
        copy for indices low..high inclusive.
    - Watch off-by-one errors in loops (<= vs <).
    - If large arrays, use 64-bit for counts (long long) to avoid overflow.
    - Time Complexity: O(n log n)
      Space Complexity: O(n) extra for merge temporary vector

    This file uses the merge-sort approach and returns the total inversion count.
    Scroll for the tree diagram
*/

class Solution {
public:
    long long mergeCount(vector<int> &arr, vector<int> &temp, int low, int mid, int high) {
        int left = low;
        int right = mid + 1;
        int idx = low;
        long long cnt = 0;

        while (left <= mid && right <= high) {
            // take left if <= so equal elements are NOT counted as inversions
            if (arr[left] <= arr[right]) {
                temp[idx++] = arr[left++];
            } else {
                // arr[left] > arr[right] -> all remaining left..mid are inversions
                temp[idx++] = arr[right++];
                cnt += (long long)(mid - left + 1);
            }
        }

        while (left <= mid)  temp[idx++] = arr[left++];
        while (right <= high) temp[idx++] = arr[right++];

        // copy back inclusive of high
        for (int i = low; i <= high; ++i) arr[i] = temp[i];

        return cnt;
    }

    long long mergeSortCount(vector<int> &arr, vector<int> &temp, int low, int high) {
        if (low >= high) return 0;
        int mid = low + (high - low) / 2;
        long long cnt = 0;
        cnt += mergeSortCount(arr, temp, low, mid);
        cnt += mergeSortCount(arr, temp, mid + 1, high);
        cnt += mergeCount(arr, temp, low, mid, high);
        return cnt;
    }

    // change return type if platform expects long long; otherwise cast to int
    long long inversionCount(vector<int> &arr) {
        int n = (int)arr.size();
        if (n <= 1) return 0;
        vector<int> temp(n);
        return mergeSortCount(arr, temp, 0, n - 1);
    }
};

/**                      5, 3, 2, 1]
                       /             \
                [5, 3]                [2, 1]
               /     \               /     \
            [5]     [3]           [2]     [1]
              \     /               \     /
            Merge                Merge
            -------              -------
            Left: [5]            Left: [2]
            Right:[3]            Right:[1]
            Result:[3,5]         Result:[1,2]
            Inv = 1              Inv = 1


                    Merge two sorted halves:
                    ------------------------
               [3,5] (left)      [1,2] (right)

               Compare 3 and 1 → 1 < 3 → add (mid-left+1) = 2
               Compare 3 and 2 → 2 < 3 → add 2
               Remaining elements: 3,5

               Final merged: [1,2,3,5]
               Cross inversions here = 4


Final inversion count = 1 (left) + 1 (right) + 4 (cross) = 6
**/