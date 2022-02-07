# Documentation of Red-Black Tree
## Catelog
1. Introduction
	1. Binary Search Tree (BST)
	2. Self-Balancing Binary Search Tree
	3. Terminologies
	4. Red-Black Tree 
2. Rotation
	1. Why does it need rotation?
	2. Left Rotation
	3. Right Rotation
3. Insertion
	1. Insertion Rules
	2. Fixup for Insertion
		1. Case 1
		2. Case 2
		3. Case 3
4. Erasion
	1. Erasion Strategies
	2. Fixup for Erasion
		1. Case 1
		2. Case 2
		3. Case 3
		4. Case 4
5. Manipulations of Red-Black Tree
6. Reference
## 1. Introduction
### 1.1 Binary Search Tree (BST)
In computer science, a **binary search tree** is a **binary tree** data structure that each internal node stores a key greater than all the keys in the node's left subtree and less than those in its right subtree.

It is also called **ordered**/**sorted** binary tree so that it can provide **search** function to find a particular key in the tree. (That is why we calls it binary search tree.)

For an ideal binary search tree, whose height is logarithm of number of nodes in the tree, the operations including insert, delete and search have a best time complexity $O(log(n))$.
### 1.2 Self-Balancing Binary Search Tree
If there has no optimization for a binary search tree, it may occur **inbalance** in the tree.

For example, Assume that we create an empty tree and insert some data(or keys) in order, the bianry search tree will be a **skewed tree** then causing the time complexity of operations such as insert, delete and search becomes $O(n)$. When it occurs, it is an **imbalance** binary search tree.

To solve imbalance, **self-balancing** binary search tree has been derived from binary search tree. **Self-balancing** binary search tree is also a binary search tree but it keeps its height small when any arbitrary insertions and deletions happens. So, the tree can always be balance and have the time complexity $O(log(n))$ for the operations mentioned in the previous paragraph.


### 1.3 Terminologies
## 2. Rotation
### 2.1 Why does it need rotation?
### 2.2 Left Rotation
### 2.3 Right Rotation
## 3. Insertion
### 3.1 Insertion Rules
### 3.2 Fixup for Insertion
#### 3.2.1 Case 1
#### 3.2.2 Case 2
#### 3.2.3 Case 3
## 4. Erasion
### 4.1 Erasion Strategies
### 4.2 Fixup for Erasion
#### 4.2.1 Case 1
#### 4.2.2 Case 2
#### 4.2.3 Case 3
#### 4.2.4 Case 4
## 5. Manipulations of Red-Black Tree
## 6. Reference
* [BTech Smart Class - binary-tree](http://www.btechsmartclass.com/data_structures/binary-tree.html)
* [Wiki - Binary search tree](https://en.wikipedia.org/wiki/Binary_search_tree)
* [Wiki - Self-balancing binary search tree](https://en.wikipedia.org/wiki/Self-balancing_binary_search_tree)