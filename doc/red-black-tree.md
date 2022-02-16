# Document of Red-Black Tree
## Catelog
1. Introduction
	1. Binary Search Tree (BST)
	2. Self-Balancing Binary Search Tree
	3. Terminologies
	4. Associative array
	5. Red-Black Tree 
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
![](https://i.imgur.com/PU4WiKW.png)
* **fig1-1-1**: A simple binary search tree.

For an ideal binary search tree, whose height is logarithm of number of nodes in the tree, the operations including insert, delete and search have a best time complexity $O(log(n))$.

### 1.2 Self-Balancing Binary Search Tree
If it has no optimization for a binary search tree, it may occur **imbalance** in the tree.

For example, Assume that we create an empty tree and insert some data(or keys) in order, the bianry search tree will be a **skewed tree** then causing the time complexity of operations, such as insert, delete and search, becomes $O(n)$. When it occurs, it is an **imbalanced** binary search tree.

![](https://i.imgur.com/4bGrLQc.png)
* **fig1-2-1**: A skewed tree. If it inserts the data $\{10, 13, 23, 39, 50\}$ into an empty BST in order, the BST will be a bad data structure with time complexity $O(n)$ for BST operations.

To solve imbalance, **self-balancing** binary search tree has been derived from binary search tree. **Self-balancing** binary search tree is also a binary search tree but it keeps its height as small as possible when any arbitrary insertions and deletions happens. So, the tree can always be balanced and have the time complexity $O(log(n))$ for the operations mentioned in the previous paragraph.
![](https://i.imgur.com/mJUfPBH.png)
* **fig1-2-2**: An example of balancing. If imbalance occurs, the tree will adjust itself by its balance rules.

Every self-balancing BST has its own balance rules to maintain itself; here lists some known self-balancing BST for reference.
* AVL tree
* 2-3 tree
* B-tree
* **Red-Black tree** (The main tree in this repo.)
* Treap
* Weight-balanced tree
### 1.3 Terminologies
Because it mentions binary tree concepts in this document, users need to review the relevant terminologies about binary tree (BT). Here lists some basic terminologies.

**root**: The first node of a tree and it only exists one in the tree exactly.

**edge**: Because a tree is also a graph, so here also has the terminology **edge**, whcih is related to graph theory, and it is a connecting link between two nodes.

**leaf**: The node having no child is called **leaf**, and is also called **terminal node.**

**internal node**: It is inverse to leaf. The node having at least one child is called **internal node.**

![](https://i.imgur.com/w7LwUOY.png)
* **fig1-3-1**: Illustration for root, edge, internal nodes and leaves.
---
**parent**: Except for root node, for a tree structure, every node in a tree must be derived from another node, which is called **parent** node for the node.

**child**: It is corresponding to parent; If a node derives another node, then the derived node is called **child** for the node.

**siblings**: If certain nodes have same parent, they are called **sibling** for each other. For a BT, every node must have at most one sibling node exactly.

![](https://i.imgur.com/OvKqZ04.png)
* **fig1-3-2**: Illustration for parent, children and sibling for node 15.
---
**path**: the sequence of nodes and edges from one node to another node is called as **path** between that two nodes.

**height**: the total number of edges from leaf node to a particular node in the longest path is called as **height** of that node.

![](https://i.imgur.com/4cB1vbJ.png)
* **fig1-3-3**: Illustration for path and height
---
**subtree**: For every node, if it is removed, then it will form two **subtrees**, whose roots are the children of the removed node.
![](https://i.imgur.com/BZLu4e8.png)
* **fig1-3-4**: Illustration for subtrees. If it removes 20, two subtrees are formed and have roots 15 and 25, respectively.
### 1.4 Associatve array
### 1.5 Red-Black Tree
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