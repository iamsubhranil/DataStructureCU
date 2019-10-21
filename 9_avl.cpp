#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Avl{
	int val;
	struct Avl* left, *right;
} Avl;

Avl* avl_new(int val) {
	Avl* node = (Avl*)malloc(sizeof(Avl));
	node->left = node->right = NULL;
	node->val = val;
	return node;
}

void avl_rotate_left(Avl **head) {
	Avl *newhead = (*head)->right;
	(*head)->right = newhead->left;
	newhead->left = *head;
	*head = newhead;
}

void avl_rotate_right(Avl **head) {
	Avl *newhead = (*head)->left;
	(*head)->left = newhead->right;
	newhead->right = *head;
	*head = newhead;
}

typedef struct AvlInfo {
	int height, balance;
} AvlInfo;

void avl_print_rec(int level, Avl *head) {
	printf("\n");
	for(int i = 0; i < level; i++) {
		printf("    ");
	}
	printf("|--");
	if(head == NULL) {
		printf(" X");
	}
	else {
		printf("%2d", head->val);
		if(head->left != NULL || head->right != NULL) {
			avl_print_rec(level + 1, head->left);
			avl_print_rec(level + 1, head->right);
		}
	}
}

void avl_print(Avl *head) {
	avl_print_rec(0, head);
}

AvlInfo avl_check(Avl **head) {
	if(*head == NULL) {
		return (AvlInfo){0, 0};
	}
	
	AvlInfo leftInfo = avl_check(&(*head)->left);
	AvlInfo rightInfo = avl_check(&(*head)->right);
		
	int balance = leftInfo.height - rightInfo.height;
	
//	printf("\nChecking balance for %d : %d", (*head)->val, balance);
	
	if(balance < -1) {
	//	printf("\nProblem is in the right side");
		// the problem is in the right side
		if(rightInfo.balance == -1) {
	//		printf("\nRight high");
	//		printf("\nBefore");
	//		avl_print(*head);
			// right high
			avl_rotate_left(head);
	//		printf("\nAfter left rotation");
	//		avl_print(*head);
			return avl_check(head);
		} else if(rightInfo.balance == 1) {
	//		printf("\nLeft high");
	//		printf("\nBefore");
	//		avl_print(*head);
			// left high
			avl_rotate_right(&(*head)->right);
	//		printf("\nAfter right rotation of right");
//			avl_print(*head);
			avl_rotate_left(head);
	//		printf("\nAfter left rotation");
	//		avl_print(*head);
			return avl_check(head);
		}
	} else if(balance > 1) {
	//	printf("\nProblem is in the left side");
		// the problem is in the right side
		if(leftInfo.balance == 1) {
	//		printf("\nRight high reverse");
	//		printf("\nBefore");
	//		avl_print(*head);
			// right high reverse
			avl_rotate_right(head);
	//		printf("\nAfter right rotation");
	//		avl_print(*head);
			return avl_check(head);
		} else if(leftInfo.balance == -1) {
	//		printf("\nLeft high reverse");
	//		printf("\nBefore");
	//		avl_print(*head);
			// left high reverse
			avl_rotate_left(&(*head)->left);
	//		printf("\nAfter left rotation of left");
	//		avl_print(*head);
			avl_rotate_right(head);
	//		printf("\nAfter right rotation");
	//		avl_print(*head);
			return avl_check(head);
		}
	}
	
	int height = rightInfo.height > leftInfo.height ? rightInfo.height : leftInfo.height;
	height++;
	return (AvlInfo){height, balance};
}

void avl_insert(Avl **head, int val) {
	Avl **nhead = head;
	int doNotInsert = 0;
	while(*nhead != NULL) {
		if(val <= (*nhead)->val) {
			nhead = &(*nhead)->left;
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

int main() {
	int elements = 10;
	Avl *head = NULL;
	srand(time(NULL));
	while(elements--)
		avl_insert(&head, rand());
	avl_check_balance(&head);
	avl_print(head);
}
