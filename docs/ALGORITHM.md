# Algorithm Deep Dive: Security Laser Beam Calculator

## Problem Statement

Given a 2D binary grid representing a bank floor plan where:
- `'1'` represents a security device
- `'0'` represents an empty cell

Calculate the total number of laser beams between security devices.

### Laser Beam Rules

1. Beams connect devices on **different rows** (r1 < r2)
2. **No devices** can exist in rows between connected devices
3. Each valid device pair creates **one beam**
4. Beams are **independent** (don't interfere)

## Solution Approach

### Optimized Algorithm - O(m×n)

The key insight: We only need to track the **previous row** with devices.

```cpp
int numberOfBeams(vector<string>& bank) {
    int prev = 0;      // Devices in previous active row
    int ans = 0;       // Total beams

    for (const string& row : bank) {
        int count = countDevices(row);  // O(n)

        if (count > 0) {
            ans += (prev * count);  // Beams = prev_devices × curr_devices
            prev = count;           // Update previous count
        }
    }

    return ans;
}
```

### Why This Works

**Mathematical Foundation:**

If row `i` has `a` devices and the next row `j` with devices has `b` devices:
- Every device in row `i` connects to every device in row `j`
- Total beams between these rows: `a × b`

**Key Optimization:**

Instead of tracking all device positions, we only need **counts**:
- Current row device count
- Previous active row device count

Empty rows are automatically skipped (they don't update `prev`).

### Complexity Analysis

**Time Complexity: O(m × n)**
- Iterate through m rows: O(m)
- Count devices in each row: O(n)
- Total: O(m × n)

**Space Complexity: O(1)**
- Only two integer variables (`prev`, `ans`)
- No additional data structures needed
- Input grid not modified

**Why Not O(m + n)?**

We must examine every cell at least once to count devices, requiring O(m × n) time.

## Examples Walkthrough

### Example 1: Standard Case

```
Grid:
  0 1 2 3 4 5
0 · █ █ · · █    3 devices
1 · · · · · ·    0 devices (skipped)
2 · █ · █ · ·    2 devices
3 · · █ · · ·    1 device

Calculation:
Row 0: prev=0, count=3 → beams=0×3=0, prev=3
Row 1: prev=3, count=0 → skipped
Row 2: prev=3, count=2 → beams=3×2=6, prev=2
Row 3: prev=2, count=1 → beams=2×1=2, prev=1

Total: 0 + 6 + 2 = 8 beams
```

### Example 2: No Beams

```
Grid:
0 · · ·    0 devices
1 █ █ █    3 devices (all in same row)
2 · · ·    0 devices

Calculation:
Row 0: prev=0, count=0 → skipped
Row 1: prev=0, count=3 → beams=0×3=0, prev=3
Row 2: prev=3, count=0 → skipped

Total: 0 beams
```

**Why no beams?** All devices are in the same row!

### Example 3: Maximum Beams

```
Grid:
0 █ █ █    3 devices
1 · · ·    0 devices
2 █ █ █    3 devices

Calculation:
Row 0: prev=0, count=3 → beams=0×3=0, prev=3
Row 1: prev=3, count=0 → skipped
Row 2: prev=3, count=3 → beams=3×3=9, prev=3

Total: 9 beams
```

## Edge Cases

### 1. Empty Grid
```
Input: ["000","000"]
Output: 0
Reason: No devices
```

### 2. Single Row
```
Input: ["111"]
Output: 0
Reason: Need at least 2 rows with devices
```

### 3. Single Device Per Row
```
Input: ["100","010","001"]
Output: 2
Calculation: 1×1=1 (row0→row1) + 1×1=1 (row1→row2) = 2
```

### 4. All Devices
```
Input: ["11","11","11"]
Output: 8
Calculation: 2×2=4 (row0→row1) + 2×2=4 (row1→row2) = 8
```

### 5. Alternating Patterns
```
Input: ["101","000","101","000","101"]
Output: 8
Rows 0,2,4 each have 2 devices
Beams: 2×2=4 (0→2) + 2×2=4 (2→4) = 8
```

## Alternative Approaches

### Approach 1: Track All Positions (Naive)
```cpp
// Time: O(m×n + k²) where k = device count
// Space: O(k)
vector<pair<int,int>> devices;
for (int i = 0; i < m; i++)
    for (int j = 0; j < n; j++)
        if (grid[i][j] == '1')
            devices.push_back({i, j});

// Check all pairs for valid beams
int beams = 0;
for (int i = 0; i < devices.size(); i++)
    for (int j = i+1; j < devices.size(); j++)
        if (isValidBeam(devices[i], devices[j]))
            beams++;
```

**Problems:**
- Higher time complexity: O(k²) checking pairs
- Higher space complexity: O(k) storing positions
- More complex validation logic

### Approach 2: Row-by-Row with Full Tracking
```cpp
// Time: O(m×n)
// Space: O(m)
vector<int> rowCounts(m);
for (int i = 0; i < m; i++)
    rowCounts[i] = countDevices(grid[i]);

int beams = 0;
int prevIdx = -1;
for (int i = 0; i < m; i++) {
    if (rowCounts[i] > 0) {
        if (prevIdx != -1)
            beams += rowCounts[prevIdx] * rowCounts[i];
        prevIdx = i;
    }
}
```

**Analysis:**
- Same time complexity but extra pass
- Unnecessary space for row counts
- Less efficient than single-pass solution

## Optimizations

### 1. Early Termination
```cpp
// If grid has < 2 rows, return 0 immediately
if (bank.size() < 2) return 0;
```

### 2. Skip Empty Rows Efficiently
```cpp
// Check first character as heuristic for all-zero rows
if (row[0] == '0' && row.find('1') == string::npos)
    continue;
```

### 3. Use std::count
```cpp
int count = std::count(row.begin(), row.end(), '1');
```

Modern compilers optimize `std::count` with SIMD instructions.

## Real-World Applications

### 1. Security System Design
- Calculate laser beam coverage
- Optimize sensor placement
- Minimize equipment costs

### 2. Network Topology
- Point-to-point connections
- Line-of-sight communication
- Mesh network design

### 3. Computer Vision
- Line detection algorithms
- Feature matching
- Object tracking

## Performance Characteristics

### Best Case: O(m)
- All rows are empty except one
- Each row processed in O(1) average time

### Average Case: O(m×n)
- Must check all cells
- Typical performance for random grids

### Worst Case: O(m×n)
- All cells must be examined
- Dense grids with many devices

### Memory Footprint
- Stack: O(1) - only local variables
- No heap allocations
- Cache-friendly sequential access

## Testing Strategy

### Unit Tests
1. Basic functionality
2. Edge cases (empty, single row, etc.)
3. Large grids (500×500)
4. Random grids (various densities)

### Performance Tests
1. Scaling: 10×10 to 500×500
2. Density: 0% to 100% devices
3. Memory usage validation
4. Comparison with naive approaches

### Correctness Verification
```cpp
void verifyBeams(const Grid& grid) {
    // Method 1: Optimized algorithm
    int fast = optimizedCount(grid);

    // Method 2: Brute force verification
    int slow = bruteForceCount(grid);

    assert(fast == slow);
}
```

## Conclusion

The optimized O(m×n) solution with O(1) space is:
- ✅ **Optimal** - Can't do better than examining each cell
- ✅ **Simple** - Easy to understand and implement
- ✅ **Efficient** - Minimal memory overhead
- ✅ **Robust** - Handles all edge cases correctly

This algorithm demonstrates the power of mathematical insight in transforming a seemingly complex problem into an elegant, efficient solution.

---

**Further Reading:**
- Dynamic Programming patterns
- Grid traversal algorithms
- Computational geometry
- Combinatorics in algorithm design
