/*
    Problem: Unique Paths (m x n grid)
    Move only Right or Down from (0,0) → (m-1,n-1)

    ---------------------------------------------------------------
    1) Brute Force – Recursion (Exponential Time)
    ---------------------------------------------------------------
    Explore all possible paths using DFS.

        function f(i, j):
            if (i == m-1 && j == n-1): return 1        // reached destination
            if (i >= m || j >= n): return 0           // out of bounds

            // move down + move right
            return f(i+1, j) + f(i, j+1)

    Time Complexity  : O(2^(m+n)) 
    Space Complexity : O(m+n) recursion stack


    ---------------------------------------------------------------
    2) Better Approach – Dynamic Programming (O(m*n))
    ---------------------------------------------------------------
    dp[i][j] = number of ways to reach cell (i, j)

        for i in range(0, m):
            for j in range(0, n):
                if (i == 0 && j == 0): 
                    dp[i][j] = 1
                else:
                    up   = (i > 0) ? dp[i-1][j] : 0
                    left = (j > 0) ? dp[i][j-1] : 0
                    dp[i][j] = up + left

    return dp[m-1][n-1]

    Time Complexity  : O(m*n)
    Space Complexity : O(m*n) [can be optimized to O(n)]


    ---------------------------------------------------------------
    3) Optimal Approach – Combinatorics (O(min(m, n)) time, O(1) space)
    ---------------------------------------------------------------
    To reach bottom-right:
        - You MUST take (m-1) Down moves
        - And (n-1) Right moves
      Total steps = (m-1) + (n-1) = m + n - 2 = N
      Choose which positions are Down moves:

            Answer = C(N, m-1)   or   C(N, n-1)

    Efficient computation:
        r = min(m-1, n-1)
        C(N, r) = (N-r+1)(N-r+2)...(N) / (1 · 2 · 3 ... r)

    This avoids overflow by multiplying/dividing step-by-step.

    Time Complexity  : O(min(m, n))
    Space Complexity : O(1)
*/
class Solution {
public:
    int uniquePaths(int m, int n) {
        int N = m + n - 2;
        long ans = 1;
        int r = min(n , m) - 1;
        for(int i = 1; i <= r; i++)
        {
            ans = ans * (N - r + i) / i;
        }
        return ans;
    }
};

/*
    ---------------------------------------------------------------
    3) Optimal Approach (Combinatorics) — matches handwritten notes
    ---------------------------------------------------------------

    To go from (0,0) → (m-1,n-1):

        You must move:
            - (m-1) steps Down
            - (n-1) steps Right

        Total mandatory moves = (m-1) + (n-1)
                             = m + n - 2
                             = N   (total moves)

    Now out of these N positions, choose where you place the Down steps
    OR choose where you place the Right steps.

        Why choose?  
        Because once positions of Down are fixed, the remaining moves
        are automatically Right (and vice-versa).

        So answer = number of ways to arrange:
            (m-1) D's and (n-1) R's

        → Answer = C(N, m-1)    // choose positions for Down
        → OR Answer = C(N, n-1) // choose positions for Right
        (both are mathematically identical)

    ---------------------------------------------------------------
    Your notes about combination trick:
    ---------------------------------------------------------------
    To compute C(N, r) efficiently:

        C(N, r) = N! / (r! * (N-r)!)

    But computing factorials overflows.  
    So we use the multiplicative form in decreasing order:

        C(N, r) = (N-r+1)(N-r+2)...(N) / (1·2·3…r)

    Example from your notes:
        10C3 = (8 × 9 × 10) / (3 × 2 × 1)

    We multiply only the top r numbers in decreasing order
    to avoid overflow and reduce operations.

    Also from your notes:
        If we choose R steps first (e.g., 3 R's),
        the remaining places must be D's — no choice left.
        That's why combinations count only the choice of R or D,
        not both.

    ---------------------------------------------------------------
    Time Complexity  : O(min(m, n))
    Space Complexity : O(1)
*/
