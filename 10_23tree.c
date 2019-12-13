#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef enum NodeType {
    INTERNAL,
    LEAF
} NodeType;

typedef struct TwoThreeNode{
    NodeType type;
    struct TwoThreeNode* parent;
    // A node in a two three tree can
    // either be an internal node or
    // a leaf node. An internal node 
    // again can have either another
    // internal node or a leaf as its
    // children. To simply all that, 
    // we merge the two different kind
    // of nodes here using the union, 
    // and separate them by the NodeType
    // declared above.
    union {
        struct {
            int l, m, numchild;
            int highestFromRight;
            struct TwoThreeNode *children[3];
        };
        struct {
            int val;
        };
    };
} TwoThreeNode;

TwoThreeNode* ttn_create_node(NodeType type) {
    TwoThreeNode* ttn = (TwoThreeNode*)malloc(sizeof(TwoThreeNode));
    ttn->type = type;
    ttn->parent = NULL;
    switch(type) {
        case INTERNAL:
            ttn->l = ttn->m = ttn->numchild = ttn->highestFromRight = 0;
            ttn->children[0] = ttn->children[1] = ttn->children[2] = NULL;
            break;
        case LEAF:
            ttn->val = 0;
            break;
    }
    return ttn;
}

TwoThreeNode* ttn_create_leaf(int val) {
    TwoThreeNode* node = ttn_create_node(LEAF);
    node->val = val;
    return node;
}

TwoThreeNode* ttn_create_internal(int l, int m) {
    TwoThreeNode* node = ttn_create_node(INTERNAL);
    node->l = l; node->m = m;
    return node;
}

int ttn_get_highest_from_right(TwoThreeNode *node) {
    switch(node->type) {
        case INTERNAL:
            return node->highestFromRight;
        case LEAF:
            return node->val;
    }
}

// This method iteratively updates the L M values
// of all nodes in the path of the root of the
// tree to the argument node.
// To facilitate the process, each internal node
// stores a member named 'highestFromRight',
// which stores the highest value present in
// the rightmost subtree of the node.
// For a node N,
//      1. L value : highest value in the left subtree
//      2. M value : highest value in the mid subtree
//      3. highestFromRight : highest value present
//          in its rightmost subtree.
// This method iteratively adjusts the values up
// until the root, which ensures updation of
// the only nodes which were touched in the
// insertion or deletion process.
void ttn_update_lm(TwoThreeNode *node) {
    while(node != NULL) {
        TwoThreeNode *max = node->children[node->numchild - 1];
        node->highestFromRight = ttn_get_highest_from_right(max);
        node->l = ttn_get_highest_from_right(node->children[0]);
        node->m = ttn_get_highest_from_right(node->children[1]);
        node = node->parent;
    }
}

// Given a 'parent', and a 'pos' to insert 'value' in
// its children, this method performs the split, and
// adjusts the whole tree recursively.
void ttn_insert_internal(TwoThreeNode **root, TwoThreeNode *parent, TwoThreeNode *value, int pos) {
    if(parent->numchild == 2) {
        // we still have space for one child.
        // problem = solved.
        int i = 1;
        while(i >= pos) {
            parent->children[i + 1] = parent->children[i];
            i--;
        }
        parent->children[pos] = value;
        value->parent = parent;
        parent->numchild = 3;

        // update the L and M values of the parent
        ttn_update_lm(parent);
        return;
    }
    // Otherwise, we need to split it.

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
    TwoThreeNode *arr[4] = {parent->children[0], parent->children[1], parent->children[2], NULL};
    // Now we insert 'value' at pos, shifting all 
    // other elements to the right.
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
    // Now, we clear out the extra child of 'parent'.
    parent->children[2] = NULL;

    // update the L and M values of parent
    // and tmp_internal
    ttn_update_lm(parent);
    ttn_update_lm(tmp_internal);

    // Now, we add the tmp_internal node to the tree
    if(parent->parent == NULL) {
        // we are at the top level of the tree,
        // and hence, need to modify the root.
        *root = ttn_create_internal(0, 0);
        (*root)->children[0] = parent;
        (*root)->children[1] = tmp_internal;
        parent->parent = tmp_internal->parent = *root;
        (*root)->numchild = 2;

        // update the L and M values of the root
        ttn_update_lm(*root);
        // we're done
    } else {
        // find position of 'parent' in its parent's
        // children array, and insert it one right
        int i = 0;
        while(parent->parent->children[i] != parent) i++;
        ttn_insert_internal(root, parent->parent, tmp_internal, i + 1);
    }
}

TwoThreeNode* ttn_search_parent(TwoThreeNode *root, int val) {
    TwoThreeNode *present = root, *parent = NULL;
    while(present->type == INTERNAL) {
        parent = present;
        if(val <= parent->l) {
            present = parent->children[0];
        } else if(val <= parent->m || parent->numchild == 2) {
            present = parent->children[1];
        } else {
            present = parent->children[2];
        }
    }
    return parent;
}

void ttn_insert(TwoThreeNode **root, int val) {
    if(*root == NULL) { // tree contains no children
        *root = ttn_create_leaf(val);
    }
    else if((*root)->type != INTERNAL) {
        // tree contains one child
        TwoThreeNode *tmp1 = *root, *tmp2 = ttn_create_leaf(val);
        TwoThreeNode *left = tmp1, *middle = tmp1;
        if(tmp1->val < val) {
            middle = tmp2;
        } else {
            left = tmp2;
        }
        *root = ttn_create_internal(left->val, middle->val);
        (*root)->children[0] = left;
        (*root)->children[1] = middle;
        left->parent = middle->parent = *root;
        (*root)->numchild = 2;
        (*root)->highestFromRight = middle->val;
    } else {
        TwoThreeNode *parent = ttn_search_parent(*root, val);

        // Find the position of 'val' among the
        // children first.
        int pos = 0;
        while(pos < 3 && parent->children[pos] != NULL && parent->children[pos]->val < val) {
            pos++;
        }
        // Now we call ttn_insert_node_internal with
        // trembling hands and pray to God that it
        // does it's job correctly.
        ttn_insert_internal(root, parent, ttn_create_leaf(val), pos);
        // aaand hopefully we're done.
    }
}

void ttn_delete_internal(TwoThreeNode **root, TwoThreeNode *parent, int pos) {
    // First release the node
    free(parent->children[pos]);
    parent->children[pos] = NULL;
    // shift all other children to the left
    while(pos < 2) {
        parent->children[pos] = parent->children[pos + 1];
        parent->children[pos + 1] = NULL;
        pos++;
    }
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
            while(parent->parent->children[pos] != parent)
                pos++;
            // Backup the child
            TwoThreeNode *leftover = parent->children[0];
            // First, find the uncle of leftover
            TwoThreeNode *uncle = NULL;
            int toInsert = 0;
            if(pos == parent->parent->numchild - 1) {
                // we don't have any right uncle
                // so, we choose the nearest left uncle
                uncle = parent->parent->children[pos - 1];
                // we will insert leftover as its left uncle's
                // rightmost child
                toInsert = uncle->numchild;
                // if uncle has 3 children, it will automatically
                // trigger a split
            } else {
                // we have a right uncle
                uncle = parent->parent->children[parent->parent->numchild - 1];
                // we will insert leftover to its left
                toInsert = 0;
            }
            // Now, we first delete the 'parent'
            ttn_delete_internal(root, parent->parent, pos);
            // Then, we perform insertion on uncle
            ttn_insert_internal(root, uncle, leftover, toInsert);
        }
    }

}

int ttn_delete(TwoThreeNode **root, int val) {
    if((*root)->type == LEAF) {
        if((*root)->val == val) {
            free(*root);
            *root = NULL;
            return 1;
        }
        return 0;
    } else {
        TwoThreeNode *parent = ttn_search_parent(*root, val);
        // check whether 'val' actually exists in a child of
        // 'parent'
        int pos = 0, found = 0;
        while(pos < parent->numchild) {
            if(parent->children[pos]->val == val) {
                found = 1;
                break;
            }
            pos++;
        }
        if(!found) {
            return 0;
        }
        ttn_delete_internal(root, parent, pos);
        return 1;
    }
}

int ttn_print_dot_rec(TwoThreeNode *root, FILE *f, int *count) {
    if(root->type == LEAF) {
        fprintf(f, "\tchild%d[shape=circle,label=\"%d\"];\n", *count, root->val);
    } else {
        int a = ttn_print_dot_rec(root->children[0], f, count);
        int b = ttn_print_dot_rec(root->children[1], f, count);
        int c = -1;
        if(root->numchild > 2) {
            c = ttn_print_dot_rec(root->children[2], f, count);
        }
        fprintf(f, "\tchild%d[shape=box,label=\"%d:%d\"];\n", *count, root->l, root->m);
        fprintf(f, "\tchild%d -> child%d;\n\tchild%d -> child%d;\n", *count, a, *count, b);
        if(c != -1) {
            fprintf(f, "\tchild%d -> child%d;\n", *count, c);
        }
        fprintf(f, "\n");
    }
    return (*count)++;
}

void ttn_print_dot(TwoThreeNode* root, FILE *f, int graphno) {
    fprintf(f, "digraph twothreetree%d {\n", graphno);
    int childcount = 0;
    if(root != NULL)
        ttn_print_dot_rec(root, f, &childcount);
    fprintf(f, "}\n\n");
}

void populate_arr_random(int *arr, int count, int limit) {
    srand(time(NULL));
    while(count--) {
        arr[count] = (rand() % limit) + 1;
    }
}

int main () {
    TwoThreeNode *t = NULL;
    int size = 10;
    //int *values = (int*)malloc(sizeof(int)*size);
    int values[] = {79, 16, 10, 67, 7, 90, 23, 11, 2, 45};
    //populate_arr_random(values, size, size*2);
    int i = 0;
    for(i = 0;i < size;i++) {
        ttn_insert(&t, values[i]);
        //printf("// inserted %d\n", values[i]);
        //ttn_print_dot(t, stdout, i);
    }
    ttn_delete(&t, 90);
    ttn_delete(&t, 7);
    ttn_delete(&t, 2);
    ttn_delete(&t, 10);
    ttn_delete(&t, 23);
    ttn_delete(&t, 16);
    ttn_delete(&t, 67);
    ttn_delete(&t, 45);
    ttn_delete(&t, 79);
    ttn_delete(&t, 11);
    ttn_print_dot(t, stdout, 0);
}
