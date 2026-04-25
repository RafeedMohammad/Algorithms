# B+ Tree — Quick Reference & Summary

> Order = 4 → **max 3 keys**, **max 4 children** per node.

---

## Core Idea in One Paragraph

A B+ Tree is a sorted, balanced multi-way tree.
**Internal nodes** are just routers (keys only, no data).
**Leaf nodes** hold the real data AND are connected in a linked list →
making range scans trivial. The tree grows **upward** — a new root is
created when the old root splits.

---

## Data Structures

```
Node {
  bool          leaf     // true = leaf, false = internal
  int[]         keys     // sorted keys in this node
  int[]         values   // data payload — LEAF ONLY (parallel to keys)
  Node*[]       child    // child pointers — INTERNAL ONLY (len = keys+1)
  Node*         next     // linked list pointer — LEAF ONLY
}

BPlusTree {
  Node*  root
  int    order    // max children  (= 4)
  int    maxKeys  // max keys      (= order-1 = 3)
}
```

---

## Function Map

| Function                        | Public/Private | One-line purpose                             |
| ------------------------------- | -------------- | -------------------------------------------- |
| `insert(key, val)`              | public         | Entry point — handles empty tree & full root |
| `search(key)`                   | public         | Root-to-leaf lookup, returns value or -1     |
| `printTree()`                   | public         | BFS level-order display                      |
| `printLeaves()`                 | public         | Walk leaf linked list left→right             |
| `insertNonFull(node, key, val)` | private        | Recursive descent + leaf insertion           |
| `splitChild(parent, idx)`       | private        | Splits a full child, promotes key up         |

---

## 1. `search(key)` — Read-only lookup

**What it does:** Walks root → leaf using keys as routing guides. Checks for exact match only at the leaf.

**Steps:**

```
1. Start at root, cur = root
2. LOOP while cur != null:
   a. Find slot i: scan keys while key > keys[i], stop at first keys[i] >= key
   b. IF leaf:
        return values[i]  if keys[i] == key
        return -1         otherwise
   c. IF internal:
        cur = child[i]    (descend)
3. return -1  (empty tree)
```

**Key rule:** `child[i]` holds all keys in range `[keys[i-1], keys[i])`.

---

## 2. `insert(key, val)` — Public entry point

**What it does:** Handles the two special cases before handing off to `insertNonFull`.

**Steps:**

```
1. IF tree is empty:
     create root as leaf, store key+val, return

2. IF root is FULL (keys.size() == maxKeys):       ← Pitfall 4 & 5
     newRoot = new internal node
     newRoot.child[0] = old root
     splitChild(newRoot, 0)                         ← splits old root
     root = newRoot                                 ← tree grows taller here

3. insertNonFull(root, key, val)
```

> Root is the only node with no parent. It must be split _before_ descending so the promotion has somewhere to go.

---

## 3. `insertNonFull(node, key, val)` — Recursive descent

**What it does:** Descends the tree and inserts into the correct leaf. Splits any full child it encounters _before_ entering it (proactive/top-down).

**Steps:**

```a
IF node is a leaf:
  find sorted position i (scan while key > keys[i])
  insert key at keys[i]
  insert val at values[i]        ← keep parallel arrays in sync
  return

IF node is internal:
  find child index i (same scan)
  IF child[i] is FULL (keys.size() == maxKeys):   ← Pitfall 5
    splitChild(node, i)                            ← split before entering
    IF key > node.keys[i]:  ++i                   ← Pitfall 3: re-check side
  insertNonFull(node.child[i], key, val)           ← recurse
```

> By splitting on the way **down**, we guarantee the node we're about to insert into always has room. No backtracking needed.

---

## 4. `splitChild(parent, idx)` — The heart of the tree

**What it does:** Takes a full child at `parent.child[idx]` and splits it into two nodes, promoting one key up to `parent`.

### Branch A — Splitting a Leaf (Copy-Up)

```
mid = order / 2   (= 2 for order-4)

sibling.keys   = child.keys[mid .. end]      ← right half moves out
child.keys     = child.keys[0 .. mid-1]      ← left half stays

sibling.values = child.values[mid .. end]    ← data follows keys
child.values   = child.values[0 .. mid-1]

sibling.next = child.next                    ← Pitfall 2: do FIRST
child.next   = sibling                       ← then re-link

promote = sibling.keys[0]                    ← Pitfall 1: COPY of first right key
parent.keys.insert(at idx, promote)          ← key still lives in leaf too!
```

**Before / After (leaf, keys = [5, 7, 10]):**

```
Before:  child = [5, 7, 10]
After:   child = [5, 7]    sibling = [10]    promoted = 10 (copy)
Linked:  child ──next──▶ sibling
```

---

### Branch B — Splitting an Internal Node (Push-Up)

```
mid = maxKeys / 2   (= 1 for maxKeys=3)

promo = child.keys[mid]                          ← save BEFORE erase! (Pitfall 6)

sibling.keys  = child.keys[mid+1 .. end]         ← right of mid
child.keys    = child.keys[0 .. mid-1]           ← left of mid (mid removed)

sibling.child = child.child[mid+1 .. end]        ← mirror child pointers
child.child   = child.child[0 .. mid]

parent.keys.insert(at idx, promo)                ← key is GONE from child
```

**Before / After (internal, keys = [k0, k1, k2], children = [c0,c1,c2,c3]):**

```
Before:  child = keys[k0, k1, k2]  children[c0, c1, c2, c3]

After:   child   = keys[k0]      children[c0, c1]
         sibling = keys[k2]      children[c2, c3]
         promoted = k1  ← REMOVED from child (push-up, unlike leaf copy-up)
```

---

## 5. `printTree()` — Level-order (BFS)

**What it does:** Prints each level of the tree on its own line.

```
level = [root]
WHILE level not empty:
  next = []
  FOR each node n in level:
    print n.keys as "[k0 k1 k2]"
    IF n is internal: add n.children to next
  print newline
  level = next
```

---

## 6. `printLeaves()` — Leaf chain

**What it does:** Finds leftmost leaf, then walks the `next` linked list.

```
p = root
WHILE p is not leaf:  p = p.child[0]     ← go left all the way down

WHILE p != null:
  print all (key, value) pairs in p
  p = p.next                              ← follow linked list
```

---

## The 6 Pitfalls — At a Glance

| #   | Pitfall                   | Wrong                                          | Correct                                                               |
| --- | ------------------------- | ---------------------------------------------- | --------------------------------------------------------------------- |
| 1   | Leaf promotion key        | Remove median from leaf                        | **Copy** `sibling.keys[0]` (key stays in leaf)                        |
| 2   | Leaf `next` pointer order | `child->next = sibling` first                  | `sibling->next = child->next` **first**, then `child->next = sibling` |
| 3   | Child index after split   | Keep same `i`                                  | Re-check: `if (key > node.keys[i]) ++i`                               |
| 4   | Root overflow             | No special handling                            | Split root **before** descending (it has no parent)                   |
| 5   | Split trigger             | `>= maxKeys` or `> maxKeys`                    | Exactly `== maxKeys` (when node is full)                              |
| 6   | Internal `mid` & promo    | Use `order/2` as promo index; read after erase | Save `promo = keys[mid]` **before** any erase; use `maxKeys/2` as mid |

---

## Copy-Up vs Push-Up — The Core Difference

```
LEAF SPLIT   → Copy-Up:
  promoted key is a COPY → it stays in the leaf AND goes to parent

INTERNAL SPLIT → Push-Up:
  promoted key is MOVED → it leaves the child and goes to parent only
```

This is why leaves always have a full set of keys for scanning, while internal nodes only hold routing separators.

---

_Quick reference for `bPlus-tree.cpp` — B+ Tree, Order 4, C++17_
