/*
    Problem: Majority Element II (element > n/3 times)

    Approaches:
    ---------------------------------------------------------
    1) Brute Force   → O(n^2) time, O(1) space
       Check freq of each element with nested loops.

    2) Better        → O(n) time, O(n) space
       Use a hashmap to store counts, then collect elements > n/3.

    3) Optimal (Boyer–Moore Extended Algorithm)
       ---------------------------------------------------------
       At most 2 elements can have a frequency > n/3.
       We maintain:
            el1, cnt1  — candidate 1 and its count
            el2, cnt2  — candidate 2 and its count

       For each number:
            Case 1: if cnt1 == 0 and current element ≠ el2
                    → assign it to el1
            Case 2: if cnt2 == 0 and current element ≠ el1
                    → assign it to el2
            Case 3: if it matches el1  → cnt1++
            Case 4: if it matches el2  → cnt2++
            Case 5: otherwise          → cnt1-- and cnt2--

       Why the checks (el2 != nums[i]) and (el1 != nums[i])?
       ---------------------------------------------------------
       When one count becomes 0, we must NOT overwrite it with the
       other candidate's value. If we did, both el1 and el2 could
       become the same element, breaking the algorithm.
       These checks ensure the two candidates stay distinct.

       After the candidate selection phase, we do one more pass:
       Count actual occurrences of el1 and el2.
       Only push those whose frequency > n/3.

       Time Complexity  : O(n)
       Space Complexity : O(1)
*/

class Solution {
public:
    vector<int> majorityElement(vector<int>& nums) {
        int cnt1 = 0, cnt2 = 0 ,el1,el2;
        for(int i = 0; i < nums.size(); i++)
        {
            if(cnt1 == 0 && el2 != nums[i])
            {
                cnt1++;
                el1 = nums[i];
            }
            else if(cnt2 == 0 && el1 != nums[i])
            {
                cnt2++;
                el2 = nums[i];
            }
            else if(el1 == nums[i]) cnt1++;
            else if(el2 == nums[i]) cnt2++;
            else{
                cnt1--;
                cnt2--;
            }
        }
        cnt1 = 0;
        cnt2 = 0;
        for(int i = 0; i < nums.size(); i++)
        {
            if(nums[i] == el1)  cnt1++;
            else if(nums[i] == el2)  cnt2++;
        }
        vector<int>ans;
        if(cnt1 > nums.size() / 3)  ans.push_back(el1);
        if(cnt2 > nums.size() / 3)  ans.push_back(el2);
        return ans;
    }
};