#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/*
	AVL tree is a special type of balanced BST where
	each node has a balance factor of -1, 0 or +1 where
	for a node N, balance factor is defined as 
		balance(N) = height of the left subtree of N
					- height of the right subtree of N
*/

// To facilitate generalized input from the user
// a value is represented using tagged structure.
// The 'type' tag of a value determines the implementation 
// of the operations performed on it, i.e. print, comparison
// and input.

// Value type
enum ValueType {
	Number,
	String
};

// Value structure
typedef struct Value {
	ValueType type;	// type of the value stored in the structure
	union {		// union is used to reduce space usage, since for
				// any Value variable, only one type of value will
				// be present
		double dval;	// double value
		char *sval;		// string value, covers characters also
	};
} Value;

// Due to limitations of scanf, maximum characters
// that can be read from stdin is limited to 100
#define MAX_CHARS 100

// Inputs a value from the stdin.
// 'type' determines the type of value
// to be read. 
// Returns the appropiate value packed
// in a Value struct.
Value value_input(ValueType type) {
	Value v;
	v.type = type;
	char sinput[100]; // buffer for the string input
	switch(type) {
		case Number: scanf("%lf", &v.dval); break;
		case String: scanf("%s", sinput); 
					v.sval = strdup(sinput);
					break;
	}
	return v;
}

// Comapares between two Value structs. Since 
// this function is called internally, it is assumed
// that always the type of the values would be same.
// Otherwise the behaviour is undefined.
// The function takes two 'Value's as input, and
// returns an integer denoting the following situations :
//		-1 : v1 is numerically or lexicographically
//			smaller than v2
//		 0 : v1 is numerically or lexicographically
//			equal to v2
//		 1 : v1 is numercially or lexicographically
//			greater than v2
int value_compare(Value v1, Value v2) {
	switch(v1.type) {
		// if both the values are numbers, 
		// compare them manually
		case Number:
			if(v1.dval == v2.dval)
				return 0;
			if(v1.dval > v2.dval)
				return 1;
			return -1;
		// otherwise call strcmp, as it handles
		// the comparison with exact semantics
		case String:
			return strcmp(v1.sval, v2.sval);
	}
}

// Generates a random string of size 'size' and
// stores in the buffer provided in the argument.
char *rand_string(char *str, size_t size) {
	// character set to be used in the generated
	// string
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    if (size) {
        --size;
        // fill up the whole string
        for (size_t n = 0; n < size; n++) {
        	// generate a randpm integer between 0 and
        	// size of the character set, which will point
        	// to the index of the character in the 
        	// character set to be stored in the buffer
            int key = rand() % (int) (sizeof charset - 1);
            // store the character in buffer
            str[n] = charset[key];
        }
        // terminate the string
        str[size] = '\0';
    }
    return str;
}

// Generates a random value of given 'type'
// and returns the value.
Value value_random(ValueType type) {
	Value v;
	v.type = type;
	switch(type) {
		case Number:
			// since rand() always returns an integer, 
			// the resulting number is divided by RAND_MAX,
			// which converts it into a double between 0 and 
			// 1, which is finally scaled up by a factor of
			// 100, to generate a double between 0.0 and 100.0
			v.dval = ((double)rand()/(double)(RAND_MAX)) * 100;
			break;
		case String:
			// allocates the buffer
			v.sval = (char*)malloc(10);
			// generates a random string
			v.sval = rand_string(v.sval, 10);
			break;
	}
	return v;
}

// Prints the value based on its type
void value_print(Value v) {
	switch(v.type) {
		case Number:
			// its a number
			printf("%g", v.dval);
			break;
		case String:
			// its a string
			printf("%s", v.sval);
			break;
	}
}

// Structure containing AVL tree
typedef struct Avl{
	Value val; // value
	struct Avl* left, *right; // pointers to the left and right subtrees
} Avl;

// Allocates a new AVL node, and stores
// 'val' inside it
Avl* avl_new(Value val) {
	Avl* node = (Avl*)malloc(sizeof(Avl));
	// manually reset the left and right links
	node->left = node->right = NULL;
	node->val = val;
	return node;
}

// Rotate the head to left.
// Initial tree :
//	    A <- head
//	   / \
//	 <T1> B
//		 / \
//	  <T2>	C
//
// After left rotation :
//		B <- head
//	   / \
//	  A	 C
//	 / \
//<T1> <T2>
void avl_rotate_left(Avl **head) {
	// B will be the new head
	Avl *newhead = (*head)->right;
	// Assign T2 to right of A
	(*head)->right = newhead->left;
	// Assign A to left of B
	newhead->left = *head;
	// Make B the new head
	*head = newhead;
}


// Rotate the head to right.
// Initial tree :
//	    A <- head
//	   / \
//	  B <T1>
//	 / \
//	C <T2>
//
// After right rotation :
//		B <- head
//	   / \
//	  C	  A
//	     / \
//    <T2> <T1>
void avl_rotate_right(Avl **head) {
	// B will be the new head
	Avl *newhead = (*head)->left;
	// Assign T2 to left of A
	(*head)->left = newhead->right;
	// Assign A to right of B
	newhead->right = *head;
	// Assign B to head
	*head = newhead;
}

// Prints a node and its children
// recursively. 'level' denotes the
// level of the node from the root, 
// which is used for indentation.
void avl_print_rec(int level, Avl *head) {
	printf("\n");
	for(int i = 0; i < level; i++) {
		printf("    ");
	}
	printf("|--");
	if(head == NULL) {
		// no value to print
		printf(" X");
	}
	else {
		value_print(head->val);
		// if both the children are empty, it
		// is a leaf. So don't print two Xs in the
		// next line.
		if(head->left != NULL || head->right != NULL) {
			avl_print_rec(level + 1, head->left);
			avl_print_rec(level + 1, head->right);
		}
	}
}

// Initiates the recursive print function
// with a level 0.
void avl_print(Avl *head) {
	avl_print_rec(0, head);
}

// Since we do not really need either 
// the balance or the height of a node
// except for adjust and reordering 
// anyway, this structure packs the 
// two of them together, so both of 
// them can be returned from the same 
// function at the same time.
typedef struct AvlInfo {
	int height, balance;
} AvlInfo;

// This function checks a tree T whether
// it is AVL or not by calcutating the 
// balance factor of the node stored at
// *head. 
// address of the root stored at *head
AvlInfo avl_check(Avl **head) {
	if(*head == NULL) {
		return (AvlInfo){0, 0};
	}
	
	AvlInfo leftInfo = avl_check(&(*head)->left);
	AvlInfo rightInfo = avl_check(&(*head)->right);
		
	int balance = leftInfo.height - rightInfo.height;
	int height = rightInfo.height > leftInfo.height ? rightInfo.height : leftInfo.height;
	height++;
	
	// After any type of corrective rotations,
	// height decreases by 1, and the balance 
	// becomes 0
	
	if(balance < -1) {
		// the problem is in the right side
		if(rightInfo.balance == -1) {
			// right high
			avl_rotate_left(head);
			return (AvlInfo){height - 1, 0};
		} else if(rightInfo.balance == 1) {
			// left high
			avl_rotate_right(&(*head)->right);
			avl_rotate_left(head);
			return (AvlInfo){height - 1, 0};
		}
	} else if(balance > 1) {
		// the problem is in the right side
		if(leftInfo.balance == 1) {
			// right high reverse
			avl_rotate_right(head);
			return (AvlInfo){height - 1, 0};
		} else if(leftInfo.balance == -1) {
			// left high reverse
			avl_rotate_left(&(*head)->left);
			avl_rotate_right(head);
			return (AvlInfo){height - 1, 0};
		}
	}
	return (AvlInfo){height, balance};
}

void avl_insert(Avl **head, Value val) {
	Avl **nhead = head;
	int doNotInsert = 0;
	while(*nhead != NULL) {
		int res = value_compare(val, (*nhead)->val);
		if(res == -1) {
			nhead = &(*nhead)->left;
		} else if(res == 0) {
			return;
		}
		else {
			nhead = &(*nhead)->right;
		}
	}
	*nhead = avl_new(val);
	avl_check(head);
}

AvlInfo avl_check_balance(Avl **head) {
	if(*head == NULL) {
		return (AvlInfo){0, 0};
	}
	
	AvlInfo leftInfo = avl_check_balance(&(*head)->left);
	AvlInfo rightInfo = avl_check_balance(&(*head)->right);
		
	int balance = leftInfo.height - rightInfo.height;
	
	if(balance < -1 || balance > 1) {
		printf("\nError in node %d -> balance %d!", (*head)->val, balance);
		exit(1);
	}
	
	int height = rightInfo.height > leftInfo.height ? rightInfo.height : leftInfo.height;
	height++;
	return (AvlInfo){height, balance};
}

Avl* avl_generate_random(int elements, ValueType type) {
	Avl *head = NULL;
	srand(time(NULL));
	while(elements--)
		avl_insert(&head, value_random(type));
	return head;
}

Avl* avl_generate_user() {
	Avl *head = NULL;
	int el;
	char c;
	ValueType type = Number;
	printf("Enter number of elements : ");
	scanf("%d", &el);
	printf("Enter element type\n1. Number\n2. String : ");
	scanf(" %c", &c);
	switch(c) {
		case '2':
			type = String; break;
		case '1':
			type = Number; break;
		default:
			printf("\n[Error] Wrong choice! Defaulting to Number!\n");
			break;
	}
	printf("Enter %d %s values :\n", el, type == Number ? "numeric" : "string");
	while(el--) {
		avl_insert(&head, value_input(type));
	}
	return head;
}

int main() {
	Avl *head = avl_generate_random(10, Number);
	avl_check_balance(&head);
	printf("\nGenerated tree : ");
	avl_print(head);
}
