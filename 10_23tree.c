#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// The type which distinguishes
// between two different kind of
// two three nodes sharing the
// same structure : internal nodes
// and the leaves.
typedef enum NodeType { INTERNAL, LEAF } NodeType;

// The core two three node structure
typedef struct TwoThreeNode {
	NodeType             type;   // the type of this node
	struct TwoThreeNode *parent; // pointer to its parent
	// A node in a two three tree can
	// either be an internal node or
	// a leaf node. An internal node
	// again can have either another
	// internal node or a leaf as its
	// children. To simplify all that,
	// we merge the two different kind
	// of nodes here using the union,
	// and separate them by the NodeType
	// declared above.
	union {
		// Structure of an internal node
		struct {
			int l;                            // L value
			int m;                            // M value
			int numchild;                     // Number of children of the node
			int highestFromRight;             // Highest value present in
			                                  // the rightmost subtree of the
			                                  // node
			struct TwoThreeNode *children[3]; // Children of the node
		};
		// Structure of a leaf
		struct {
			int val; // Value stored in the leaf
		};
	};
} TwoThreeNode;

// This method allocates memory for a TwoThreeNode
// with a given 'type', and resets all its members
// before returning it back.
TwoThreeNode *ttn_create_node(NodeType type) {
	// Allocate memory
	TwoThreeNode *ttn = (TwoThreeNode *)malloc(sizeof(TwoThreeNode));
	ttn->type         = type;
	ttn->parent       = NULL;
	// Reset the members
	switch(type) {
		case INTERNAL:
			ttn->l = ttn->m = ttn->numchild = ttn->highestFromRight = 0;
			ttn->children[0] = ttn->children[1] = ttn->children[2] = NULL;
			break;
		case LEAF: ttn->val = 0; break;
	}
	return ttn;
}

// This method creates a TwoThreeNode with 'type'
// set to LEAF, and given value.
TwoThreeNode *ttn_create_leaf(int val) {
	TwoThreeNode *node = ttn_create_node(LEAF);
	node->val          = val;
	return node;
}

// This method creates a TwoThreeNode with 'type'
// set to INTERNAL, and given L and M values.
TwoThreeNode *ttn_create_internal(int l, int m) {
	TwoThreeNode *node = ttn_create_node(INTERNAL);
	node->l            = l;
	node->m            = m;
	return node;
}

// Updates L and M values only for the given node.
// To facilitate the process, each internal node
// stores a member named 'highestFromRight',
// which stores the highest value present in
// the rightmost subtree of the node.
// For a node N,
//      1. L value : highest value in the left subtree
//      2. M value : highest value in the mid subtree
//      3. highestFromRight : highest value present
//          in its rightmost subtree.
void ttn_update_lm_only(TwoThreeNode *node) {
	// Get the rightmost child
	TwoThreeNode **children = node->children;
	TwoThreeNode * max      = children[node->numchild - 1];
	// Since all leaves of a two three tree reside
	// in the same level, if max is leaf, then all
	// children of 'node' are leaves.
	// Similary if max is an internal, all children
	// of 'node' are internals
	switch(max->type) {
		case LEAF:
			// Highest value of rightmost subtree
			node->highestFromRight = max->val;
			// Highest value of the left subtree
			node->l = children[0]->val;
			// Highest value of the mid subtree
			node->m = children[1]->val;
			break;
		case INTERNAL:
			node->highestFromRight = max->highestFromRight;
			node->l                = children[0]->highestFromRight;
			node->m                = children[1]->highestFromRight;
			break;
	}
}

// This method iteratively updates the L M values
// of all nodes in the path of the root of the
// tree to the argument node.
// This method iteratively adjusts the values up
// until the root, which ensures updation of
// the only nodes which were touched in the
// insertion or deletion process.
void ttn_update_lm(TwoThreeNode *node) {
	while(node != NULL) {
		ttn_update_lm_only(node);
		// Go for the parent
		node = node->parent;
	}
}

// Given a 'parent', and a 'pos' to insert 'value' in
// its children, this method does the actual insertion.
// This is different from 'ttn_insert' in so that
// all it bothers with is inserting 'value' at index
// 'pos' of 'parent'.
//
// This is required because in case of a split,
// all we need to do is that add a new child
// to the parent at an index one right to the
// present node.
void ttn_insert_internal(TwoThreeNode **root, TwoThreeNode *parent,
                         TwoThreeNode *value, int pos) {
	// First, we check the number of children
	// of parent
	if(parent->numchild == 2) {
		// we still have space for one child.
		// problem = solved
		// We shift children from 0 to pos to
		// the right
		int i = 1;
		while(i >= pos) {
			parent->children[i + 1] = parent->children[i];
			i--;
		}
		// And insert 'value' at pos
		parent->children[pos] = value;
		value->parent         = parent;

		// We update the number of children of
		// parent
		parent->numchild = 3;

		// And finally, we update the L and M
		// values of the parent
		ttn_update_lm(parent);
		// aand we're done
		return;
	}
	// Otherwise, parent contains 3 childern
	// already, so we need to split it.

	// Create the split node first. The split node
	// is going to be the next right child of the
	// parent of 'parent'.
	TwoThreeNode *tmp_internal = ttn_create_internal(0, 0);
	// Now, we need to divide the four children, 3
	// of 'parent' and the new 'value' into two sets
	// so that children can be evenly distributed
	// between 'tmp_internal' and 'parent'.

	// For that, we first create an array of size 4,
	// which contains all three children of 'parent'.
	TwoThreeNode *arr[4] = {parent->children[0], parent->children[1],
	                        parent->children[2], NULL};
	// We first shift elements from 0 to pos to the
	// right
	int bak = 3;
	while(bak > pos) {
		arr[bak] = arr[bak - 1];
		bak--;
	}
	// Now we insert 'value' at 'pos'.
	arr[pos] = value;
	// Now we put first two element of arr as first
	// two children of 'parent', and the next two as
	// first two children of 'tmp_internal'.
	parent->children[0] = arr[0];
	parent->children[1] = arr[1];
	arr[0]->parent = arr[1]->parent = parent;

	tmp_internal->children[0] = arr[2];
	tmp_internal->children[1] = arr[3];
	arr[2]->parent = arr[3]->parent = tmp_internal;

	// Now we set numchild of both of them to 2
	parent->numchild = tmp_internal->numchild = 2;
	// Now, we clear out the extra child of 'parent',
	// which contained three children before
	parent->children[2] = NULL;

	// Then, we update the L and M values of parent
	// and tmp_internal
	ttn_update_lm_only(parent);
	ttn_update_lm_only(tmp_internal);

	// Now, we need add the tmp_internal node to the tree
	if(parent->parent == NULL) {
		// We are at the top level of the tree,
		// and hence, need to modify the root.

		// We first create an internal node
		*root = ttn_create_internal(0, 0);
		// The we assign 'parent' and
		// 'tmp_internal' as its children,
		// and update their parent.
		(*root)->children[0] = parent;
		(*root)->children[1] = tmp_internal;
		parent->parent = tmp_internal->parent = *root;
		// Next, we update the number
		// of children of root.
		(*root)->numchild = 2;

		// Finally, update the L and M values of the root
		ttn_update_lm_only(*root);
		// aand we're done.
	} else {
		// 'parent' has a parent of its own.
		// So we first find position of 'parent'
		// in its parent's children array
		int i = 0;
		while(parent->parent->children[i] != parent) i++;
		// and insert 'tmp_internal' one right to it
		ttn_insert_internal(root, parent->parent, tmp_internal, i + 1);
		// and we're done.
	}
}

// This function searches for the parent node
// whose children should contain a leaf node
// containing 'val'
TwoThreeNode *ttn_search_parent(TwoThreeNode *root, int val) {
	TwoThreeNode *present = root, *parent = NULL, **children = NULL;
	while(present->type == INTERNAL) {
		parent   = present;
		children = parent->children;
		present  = children[parent->numchild - 1];
		if(val <= parent->m) {
			present = children[1];
			if(val <= parent->l)
				present = children[0];
		}
	}
	return parent;
}

// Given a two three tree with root pointed
// by 'root', this method inserts 'val' in
// the tree
void ttn_insert(TwoThreeNode **root, int val) {
	if(*root == NULL) {
		// tree contains no children
		// node containing the 'val'
		// will be the new leaf
		*root = ttn_create_leaf(val);
	} else if((*root)->type != INTERNAL) {
		// tree contains one child
		TwoThreeNode *tmp1 = *root, *tmp2 = ttn_create_leaf(val);
		// we need to determine which one
		// of them is gonna go on the left
		// and which one should go in
		// the middle
		TwoThreeNode *left = tmp1, *middle = tmp1;
		if(tmp1->val < val) {
			middle = tmp2;
		} else {
			left = tmp2;
		}
		// Now, we create an internal node,
		// the first one for this tree,
		// and make it the root. Since
		// it only contains two children, we
		// also know the L and M values
		// instantly.
		*root = ttn_create_internal(left->val, middle->val);
		// Now, we adjust the pointers
		(*root)->children[0] = left;
		(*root)->children[1] = middle;
		left->parent = middle->parent = *root;
		(*root)->numchild             = 2;
		// Finally, we mark the value of
		// the middle node as the highest value
		// of the rightmost subtree of root.
		(*root)->highestFromRight = middle->val;
	} else {
		// First, we search the parent where 'val'
		// can be inserted
		TwoThreeNode *parent = ttn_search_parent(*root, val);

		// Next, we determine the position of 'val'
		// among the children of 'parent'
		int pos = 0;
		if(parent->children[0] != NULL) {
			if(parent->children[0]->val < val)
				pos = 1;
			if(parent->children[1] != NULL) {
				if(parent->children[1]->val < val)
					pos = 2;
				if(parent->children[2] != NULL) {
					if(parent->children[2]->val < val) {
						pos = 3;
					}
				}
			}
		}
		// Now we call ttn_insert_node_internal with
		// trembling hands and pray to God that it
		// does it's job correctly.
		ttn_insert_internal(root, parent, ttn_create_leaf(val), pos);
		// aaand hopefully we're done.
	}
}

// This method deletes the children at index
// 'pos' of the 'parent' in the two three
// tree pointed by 'root'.
// This method is different from 'ttn_delete'
// in so that it does not bother retrieving
// the position of a given value in the tree.
// It needs to have the location specified,
// using which it just performs the deletion.
//
// This is required for the fact that deletion
// might be needed to propagate from a leaf
// to the root, and in those cases we are just
// bothered with deletion of a child in a
// specific index of a parent, and its
// consequences.
void ttn_delete_internal(TwoThreeNode **root, TwoThreeNode *parent, int pos) {
	// First release the child
	free(parent->children[pos]);
	parent->children[pos] = NULL;
	// shift all other children to the left
	while(pos < 2) {
		parent->children[pos]     = parent->children[pos + 1];
		parent->children[pos + 1] = NULL;
		pos++;
	}
	// Decrease the number of children of the parent
	parent->numchild--;
	// Now check the number of children of parent
	if(parent->numchild == 2) {
		// if the parent has two children now,
		// we're good. we just need to update L M
		// values, and call it a day.
		ttn_update_lm(parent);
		return;
	} else {
		// the parent has 1 child now
		if(*root == parent) {
			// if the parent is the root itself,
			// make the child new root, and release
			// the parent.
			*root = parent->children[0];
			free(parent);
			(*root)->parent = NULL;
		} else {
			// Otherwise, first find the position
			// of 'parent' in its parent.
			int pos = 0;
			while(parent->parent->children[pos] != parent) pos++;
			// Backup the child
			TwoThreeNode *leftover = parent->children[0];
			// First, find the uncle of leftover, and
			// the subsequent insertion position
			TwoThreeNode *uncle    = NULL;
			int           toInsert = 0;
			if(pos == parent->parent->numchild - 1) {
				// we don't have any right uncle of leftover
				// so, we choose the nearest left uncle
				uncle = parent->parent->children[pos - 1];
				// we will insert leftover as its left uncle's
				// rightmost child
				toInsert = uncle->numchild;
				// if uncle has 3 children, it will automatically
				// trigger a split
			} else {
				// we have a right uncle of leftover
				uncle = parent->parent->children[parent->parent->numchild - 1];
				// we will insert leftover to its left
				toInsert = 0;
			}
			// Now, we first delete the 'parent' from
			// its parent, which is located at index 'pos'
			ttn_delete_internal(root, parent->parent, pos);
			// Then, we perform insertion on uncle
			ttn_insert_internal(root, uncle, leftover, toInsert);
			// and we're done.
		}
	}
}

// Deletes a node with value 'val' from
// the two three tree with root pointed
// by 'root'. If the node is found in
// the tree, it returns 1, otherwise it
// returns 0.
int ttn_delete(TwoThreeNode **root, int val) {
	if(*root == NULL)
		return 2;
	if((*root)->type == LEAF) {
		// If there is only one node in the tree
		if((*root)->val == val) {
			free(*root);
			*root = NULL;
			return 1;
		}
		return 0;
	} else {
		// Search for the parent whose children
		// should contain val
		TwoThreeNode *parent = ttn_search_parent(*root, val);
		// check whether 'val' actually exists in a child of
		// 'parent'
		int pos = -1;
		if(parent->children[0]->val == val) {
			pos = 0;
		} else if(parent->children[1] != NULL) {
			if(parent->children[1]->val == val) {
				pos = 1;
			} else if(parent->children[2] != NULL) {
				if(parent->children[2]->val == val) {
					pos = 2;
				}
			}
		}

		// 'val' not found in any children of parent
		if(pos == -1) {
			return 0;
		}
		// 'val' found in children at 'pos' of
		// 'parent'. Perform the deletion.
		ttn_delete_internal(root, parent, pos);
		return 1;
	}
}

// Release the memory occupied by a 23tree
void ttn_free(TwoThreeNode **root) {
	if(*root == NULL)
		return;
	if((*root)->type == INTERNAL) {
		ttn_free(&(*root)->children[0]);
		ttn_free(&(*root)->children[1]);
		if((*root)->numchild == 3)
			ttn_free(&(*root)->children[2]);
	}
	free(*root);
	*root = NULL;
}

// This function recursively prints the subtree
// in the given 'level' using inorder traversal
//
// 'print_level' stores the information on
// whether or not to print | to denote a parent
// level.
void ttn_print_rec(TwoThreeNode *node, int level, int **print_level,
                   int *print_level_size) {
	// Extend the array for this level if required
	if(*print_level_size == level) {
		int bak             = *print_level_size;
		(*print_level_size) = level + 1;
		*print_level =
		    (int *)realloc(*print_level, sizeof(int) * (*print_level_size));
		while(bak < *print_level_size) {
			print_level[0][bak] = 0;
			bak++;
		}
	}
	if(node->type == INTERNAL) {
		ttn_print_rec(node->children[0], level + 1, print_level,
		              print_level_size);
	}

	// Initially, we'll print this level
	print_level[0][level] = 1;
	int bak               = 1;
	while(bak <= level) {
		printf("\t");
		if(print_level[0][bak])
			printf("|");
		else
			printf(" ");
		bak++;
	}

	printf("-- ");
	if(node->type == LEAF) {
		printf("%d\n", node->val);
	} else {
		printf("%d : %d\n", node->l, node->m);
		// Now we need to decide whether to print
		// the | to denote this level while
		// printing subsequent children
		if(node->parent == NULL) {
			// no parent, so we won't print
			print_level[0][level] = 0;
		} else {
			// if this is the left child, we'll print |
			if(node->parent->children[0] == node)
				print_level[0][level] = 1;
			// else if this is not the ultimate
			// child, we'll print |
			else if(node->parent->children[node->parent->numchild - 1] != node)
				print_level[0][level] = 1;
			// otherwise, we won't
			else
				print_level[0][level] = 0;
		}
		// Since there is more children, we'll
		// print the child's level regardless
		print_level[0][level + 1] = 1;
		ttn_print_rec(node->children[1], level + 1, print_level,
		              print_level_size);

		if(node->numchild == 3) {
			print_level[0][level + 1] = 1;
			if(node->children[2]->type != LEAF) {
				bak = 1;
				while(bak <= level + 1) {
					printf("\t");
					if(print_level[0][bak])
						printf("|");
					else
						printf(" ");
					bak++;
				}
				printf("\n");
			}
			ttn_print_rec(node->children[2], level + 1, print_level,
			              print_level_size);
		}
	}
	// finally, we reset this level's
	// separator which won't be turned on
	// until the parent explicitly does so.
	print_level[0][level] = 0;
}

// This method prints the whole tree
// pointed by 'root' into stdout
void ttn_print(TwoThreeNode *root) {
	if(root == NULL) {
		printf("<empty>\n");
	} else {
		int *print_level = NULL, level_size = 0;
		ttn_print_rec(root, 0, &print_level, &level_size);
		free(print_level);
	}
}

// This method recursively prints all the children of
// a two three tree pointed by the given root in
// dot language.
int ttn_print_dot_rec(TwoThreeNode *root, FILE *f, int *count) {
	if(root->type == LEAF) {
		fprintf(f, "\tchild%d[shape=circle,label=\"%d\"];\n", *count,
		        root->val);
	} else {
		int a = ttn_print_dot_rec(root->children[0], f, count);
		int b = ttn_print_dot_rec(root->children[1], f, count);
		int c = -1;
		if(root->numchild > 2) {
			c = ttn_print_dot_rec(root->children[2], f, count);
		}
		fprintf(f, "\tchild%d[shape=box,label=\"%d:%d\"];\n", *count, root->l,
		        root->m);
		fprintf(f, "\tchild%d -> child%d;\n\tchild%d -> child%d;\n", *count, a,
		        *count, b);
		if(c != -1) {
			fprintf(f, "\tchild%d -> child%d;\n", *count, c);
		}
		fprintf(f, "\n");
	}
	return (*count)++;
}

// Prints a given two three tree in 'dot' format.
// Since, it may be called to draw more than one
// graphs on the same tree, it needs a predefined
// 'childcount' which the recursive method
// 'ttn_print_dot_rec' increments to uniquely mark
// each node in the tree.
void ttn_print_dot(TwoThreeNode *root, FILE *f, int *childcount) {
	if(*childcount == 0) {
		fprintf(f, "digraph twothreetree {\n");
	}
	fprintf(f, "subgraph cluster_%d {\n", *childcount);
	fprintf(f, "\tlabel = \"graph%d\";\n", *childcount);
	if(root != NULL)
		ttn_print_dot_rec(root, f, childcount);
	fprintf(f, "}\n\n");
}

// Benchmarks insertion and deletion methods
void bench(char **argv) {
	TwoThreeNode *t        = NULL;
	long          inserted = 0;
	char *        end      = NULL;
	long          items    = strtol(argv[1], &end, 10);
	if(end[0] != 0 || items < 1) {
		printf("Invalid number of items : '%s'\n", argv[1]);
	} else {
		int *arr = (int *)malloc(sizeof(int) * items);
		while(inserted < items) {
			arr[inserted++] = rand();
		}
		clock_t begin = clock();
		inserted      = 0;
		while(inserted < items) {
			ttn_insert(&t, arr[inserted++]);
		}
		double insertion = (double)(clock() - begin) / CLOCKS_PER_SEC;
		inserted         = 0;
		while(inserted < items) {
			ttn_delete(&t, arr[inserted++]);
		}
		double deletion = (double)(clock() - begin) / CLOCKS_PER_SEC;
		printf("\nInsertion took: %0.5fs\nDeletion took: %0.5fs\n", insertion,
		       deletion);
	}
}

int main(int argc, char *argv[]) {
	TwoThreeNode *t = NULL;
	char          c;
	int           val, num, inserted = 0, bak;
	if(argc == 2) {
		bench(argv);
		return 0;
	}
	printf("Two three tree\n");
	printf("==============");
	while(1) {
		printf("\n\n1. Insert\n");
		printf("2. Delete\n");
		printf("3. Print : ");
		scanf(" %c", &c);
		printf("\n");
		switch(c) {
			case '1':
				printf("Enter number of values to insert : ");
				scanf("%d", &num);
				inserted = 0;
				if(num > 10) {
					printf("1. Use random values\n");
					printf("2. Enter manually : ");
					scanf(" %c", &c);
					switch(c) {
						case '2': break;
						default:
							inserted = 1;
							bak      = num;
							while(bak--) {
								val = (rand() % (num * 2)) + 1;
								ttn_insert(&t, val);
								printf("Insertion successful: %d!\n", val);
							}
							break;
					}
				}
				if(!inserted) {
					while(num--) {
						printf("\nEnter value : ");
						scanf("%d", &val);
						ttn_insert(&t, val);
						printf("Insertion successful: %d!", val);
					}
				}
				break;
			case '2':
				printf("Enter the value to delete : ");
				scanf("%d", &val);
				val = ttn_delete(&t, val);
				switch(val) {
					case 0:
						printf("Deletion failed: Value not found in the tree!");
						break;
					case 1: printf("Deletion successful!"); break;
					case 2: printf("Deletion failed: Tree is empty!"); break;
				}
				break;
			case '3': ttn_print(t); break;
			default:
				printf("Are you sure want to exit (y/n) ? ");
				scanf(" %c", &c);
				switch(c) {
					case 'y':
					case 'Y': ttn_free(&t); return 0;
				}
				break;
		}
	}
}
