#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <dirent.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

#define VERSION "Version: 0.0.1"

struct process_state {
	/* define one process of the os */
	pid_t pid;
	char name[30];
	char state;
	pid_t ppid;
};

struct node {
	/* node */
	struct process_state ps;
	struct node *next;
	struct node *child;
};

/**
 * The manipulation of nodes
*/
struct node *node_malloc();
int node_insert(struct node *sn, struct node *fn);
void node_free(struct node *fn);

static struct option longopts[] = {
	{"show-pids", 0, NULL, 'p'},
	{"numeric-sort", 0, NULL, 'n'},
	{"version", 0, NULL, 'V'},
	{0, 0, 0, 0}
};

/**
 * Build and print the process tree
*/

int isNumber(char *str);
void buildProcessTree(struct node *head);
void printProcessTree(struct node *head, int depth, int type);
void printProcess(struct node *node, int type);
void printVersion();


int main(int argc, char *argv[]) {
	for (int i = 0; i < argc; i++) {
		assert(argv[i]); // C 标准保证
		printf("argv[%d] = %s\n", i, argv[i]);
	}
	assert(!argv[argc]); // C 标准保证
	return 0;
}

/**
 * The implementation of the pre-defined functions
*/

struct node *node_malloc() {
	struct node *n = (struct node *)malloc(sizeof(struct node));
	n->ps = (struct process_state){0, { }, 0}; 
	n->next = NULL;
	n->child = NULL;

	return n;
}

int node_insert(struct node *sn, struct node *fn) {
	struct node *p;
	if (sn->ps.ppid == fn->ps.pid) {
		if (fn->child == NULL) {
			fn->child = sn;
		} 
		else {
			for (p = fn->child; p->next != NULL; p = p->next);
			p->next = sn;
		}
		return 1;
	}
	else {
		for (p = fn->child; p != NULL; p = p->next) {
			if (node_insert(sn, p)) {
				return 1;
			}
		}
	}
	return 0;
}

void node_free(struct node *fn) {
	struct node *p = fn->child;
	if(p != NULL) {
		for( ; p != NULL; ) {
			struct node *tmp = p;
			p = p->next;
			node_free(p);
		}
	}
	free(fn);
}

