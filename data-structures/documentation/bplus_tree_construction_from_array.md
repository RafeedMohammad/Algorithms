# 🌳 Constructing a B+ Tree from an Array

This guide demonstrates how to construct a B+ Tree step-by-step given an array of numbers. This is a common exercise in Data Structures exams.

## The Problem Setup

**Given Array:** `[10, 20, 5, 6, 12, 30, 7, 17]`
**Tree Order:** `d = 4`

### Important Rules for Order 4:
*   **Max children per internal node:** `d = 4`
*   **Max keys per node:** `d - 1 = 3`
*   **Overflow condition:** A node overflows when it gets `4` keys.
*   **Split index:** `d / 2 = 2`. When splitting `[A, B, C, D]`:
    *   **Left gets:** First 2 keys `[A, B]`
    *   **Right gets:** Last 2 keys `[C, D]`
*   **Promotion Rule:**
    *   **Leaf Split:** The *first key of the right leaf* (C) goes up. The key is *copied* to the parent.
    *   **Internal Split:** The *middle key* (C) goes up. The key is *removed* from the current level.

---

## Step-by-Step Construction

We will insert elements from the array one by one: `10 → 20 → 5 → 6 → 12 → 30 → 7 → 17`

### Step 1: Insert 10
The tree is empty. We create the root node as a leaf and add 10.

```
ROOT: [ 10 ]
```

---

### Step 2: Insert 20
The root has space (max 3 keys). We insert 20 in sorted order.

```
ROOT: [ 10 | 20 ]
```

---

### Step 3: Insert 5
The root still has space. We insert 5 in sorted order.

```
ROOT: [ 5 | 10 | 20 ]
```
*Note: The node is now FULL.*

---

### Step 4: Insert 6 (First Split!)
Inserting 6 into the root gives `[5, 6, 10, 20]`.
This is an **OVERFLOW** (4 keys). We must split the leaf.

**Split Process:**
*   Array to split: `[5, 6, 10, 20]`
*   Left leaf: `[5, 6]`
*   Right leaf: `[10, 20]`
*   Promotion key (first key of right leaf): `10`
*   Since the root split, we create a new internal node as the root.

**Tree After Insert 6:**
```
        [ 10 ]               <-- New Root (Internal)
       /      \
  [5|6] ---> [10|20]         <-- Leaves (Linked)
```

---

### Step 5: Insert 12
Start at root `[ 10 ]`. Since 12 > 10, go to the right child `[10 | 20]`.
Insert 12 in sorted order.

**Tree After Insert 12:**
```
          [ 10 ]
         /      \
    [5|6] ---> [10|12|20]
```
*Note: The right leaf is now FULL.*

---

### Step 6: Insert 30 (Leaf Split & Promotion!)
Start at root `[ 10 ]`. 30 > 10, go to right child `[10 | 12 | 20]`.
Insert 30: `[10, 12, 20, 30]`.
**OVERFLOW!** Split the leaf.

**Split Process:**
*   Array to split: `[10, 12, 20, 30]`
*   Left leaf: `[10, 12]`
*   Right leaf: `[20, 30]`
*   Promotion key: `20`
*   Promote `20` to the parent internal node `[ 10 ]`.

**Tree After Insert 30:**
```
          [ 10 | 20 ]        <-- Parent gets 20
         /     |     \
    [5|6] -> [10|12] -> [20|30]
```

---

### Step 7: Insert 7
Start at root `[ 10 | 20 ]`. 7 < 10, go to left child `[5 | 6]`.
Insert 7 in sorted order.

**Tree After Insert 7:**
```
          [ 10 | 20 ]
         /     |     \
  [5|6|7] -> [10|12] -> [20|30]
```

---

### Step 8: Insert 17
Start at root `[ 10 | 20 ]`. 10 < 17 < 20, go to middle child `[10 | 12]`.
Insert 17 in sorted order.

**Tree After Insert 17:**
```
          [ 10 | 20 ]
         /     |     \
  [5|6|7] -> [10|12|17] -> [20|30]
```

---

## Final B+ Tree Structure

Here is the final state of our B+ Tree after constructing it from the array `[10, 20, 5, 6, 12, 30, 7, 17]`:

```
                    [ 10 | 20 ]
                  /      |      \
                 /       |       \
        [5|6|7]   [10|12|17]   [20|30]
           |            |           |
           +------------+-----------+  (Next pointers linking leaves)
```

### Verification Checklist:
1. **Root keys:** `[10, 20]` (Order 4 allows up to 3 keys, so valid).
2. **Left child `[5|6|7]`:** All values < 10. Valid.
3. **Middle child `[10|12|17]`:** All values ≥ 10 and < 20. Valid.
4. **Right child `[20|30]`:** All values ≥ 20. Valid.
5. **Leaf keys:** All node sizes are between 1 (minimum) and 3 (maximum). Valid.
6. **Leaf list:** All elements from the original array are present in sorted order at the leaf level: `5, 6, 7, 10, 12, 17, 20, 30`.

## Advanced Example Triggering an Internal Split

What if we continue inserting and the *internal node* overflows? Let's add a few more elements to see an internal split.

### Let's Insert `25`, `26`, `27`

**Insert 25:**
Goes to rightmost leaf: `[20 | 25 | 30]`

**Insert 26:**
Leaf `[20 | 25 | 30]` overflows to `[20, 25, 26, 30]`.
Splits into `[20, 25]` and `[26, 30]`. Promotes `26`.
Root becomes `[10 | 20 | 26]` (Now FULL).

```
                 [ 10 | 20 | 26 ]
               /      |      |     \
        [5|6|7]  [10|12|17] [20|25] [26|30]
```

**Insert 27:**
Goes to rightmost leaf `[26 | 30]`. Inserts as `[26 | 27 | 30]`.

**Insert 28:**
Leaf `[26 | 27 | 30]` overflows to `[26, 27, 28, 30]`.
Splits into `[26, 27]` and `[28, 30]`. Promotes `28`.

Now we must insert `28` into the root `[10 | 20 | 26]`.
This causes an **INTERNAL NODE OVERFLOW**: `[10, 20, 26, 28]`

**Internal Split Process:**
*   Node to split: `[10, 20, 26, 28]` with 5 children `[C1, C2, C3, C4, C5]`
*   Mid index is 2 (key `26`).
*   Left internal node gets: `[10, 20]` and children `[C1, C2, C3]`
*   Right internal node gets: `[28]` and children `[C4, C5]`
*   **Promotion key (Middle key): `26`**. *Crucially, 26 leaves the internal level.*
*   Create a new root for `26`.

**Tree After Internal Split:**
```
                     [ 26 ]                  <-- New Root
                    /      \
            [ 10 | 20 ]      [ 28 ]          <-- Internal Nodes
           /     |     \      /    \
    [5|6|7] [10|12|17] [20|25] [26|27] [28|30] <-- Leaves
```
*Notice how `26` is in the root, but NO LONGER in the internal nodes below it. It only exists as actual data down in the leaf `[26|27]`.*
