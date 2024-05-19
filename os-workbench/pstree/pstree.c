#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <dirent.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

#define VERSION "Version: 0.0.1"
#define HORIZONTAL "-+"
#define VERTICAL "|"

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
	char opt;
	int opt_index;
	struct node *head;

	head = node_malloc();
	buildProcessTree(head);

	if (argc == 1) {
		printProcessTree(head, 0, 0);
	}
	else {
		while ((opt = getopt_long(argc, argv, "pnV", longopts, &opt_index)) != -1) {
			switch (opt) {
			case 'V':
				printVersion();
				break;
			case 'p':
				printProcessTree(head, 0, 1);
				break;
			case 'n':
				printProcessTree(head, 0, 2);
				break;
			default:
				printf("Pstree:\n");
				printf("You can choose options from [-p, -n, -V] or [--show-pids, --numeric-sort, --version].\n");
				break;
			}
		}
	}

	node_free(head);

	return 0;
}

/**
 * The implementation of the pre-defined functions
*/

struct node *node_malloc() {
	struct node *n = (struct node *)malloc(sizeof(struct node));
	n->ps = (struct process_state){0, { }, 'S', 0}; 
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
	if (p != NULL) {
		for ( ; p != NULL; ) {
			struct node *tmp = p;
			p = p->next;
			node_free(p);
		}
	}
	free(fn);
}

int isNumber(char *str) {
	/* If the input string are all numbers, return 1, o.w. return 0 */
	for (char *c = str; *c != '\0'; c++) {
		if (isdigit(*c) == 0) {
			return 0;
		}
	}
	return 1;
}

void buildProcessTree(struct node *head) {
	DIR *d;
	struct dirent *dir;
	FILE *f;
	char path[100] = "/proc/";

	/* Open the proc directory */
	d = opendir(path);
	if (!d) {
		fprintf(stderr, "Directory %s doesn't exist!\n", path);
		exit(1);
	}
	else {
		while ((dir = readdir(d)) != NULL) {
			if (isNumber(dir->d_name)) {
				strcat(path, dir->d_name);
				strcat(path, "/stat");

				/* read the `/stat` file of a specific process */
				f = open(path, "r");
				if (!f) {
					fprintf(stderr, "Pstree failed when open file %s!\n", path);
					continue;
				}
				else {
					struct node *node = node_malloc();
					fscanf(f, "%d (%[^)]) %c %d", &node->ps.pid, &node->ps.name, &node->ps.state, &node->ps.ppid);
					node_insert(node, head);
					fclose(f);
				}

			}
		}
		closedir(d);
	}
}

void printProcessTree(struct node *node, int depth, int type) {
	if (node->ps.pid != 0) {
		printf("%s", HORIZONTAL);
		printProcess(node, type);
		depth++;
	}

	for (struct node *p = node->child; p != NULL; p = p->next) {
		for (int i = 0; i < depth; i++) {
			printf("   ");
		}
		printf("%s", VERTICAL);
		printProcessTree(p, depth, type);
		
	}
}

void printProcess(struct node *node, int type) {
	if (type == 0) {
		fprintf(stdout, "%s\n", node->ps.name);
	}
	else if (type == 1) {
		fprintf(stdout, "%s(%d)\n", node->ps.name, node->ps.pid);
	}
}


void printVersion() {
	printf("pstree ");
	fprintf(stdout, VERSION);
}