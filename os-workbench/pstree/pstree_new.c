#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define VERSION "Version: 0.0.1"
#define MAX_PID 4096
#define ROW_PATTERN "+-"
#define COL_PATTERN "|"

/* Use node to represent a process */
struct Node {
    int pid;
    struct Node *children;
    struct Node *sibling;
};

/**
 * Manipulate the process tree, like insert, print, ...
*/
struct Node *createNode(int pid) {
    struct Node *node = (struct Node *)malloc(sizeof(struct Node));
    node->pid = pid;
    node->children = NULL; node->sibling = NULL;
    return node;
}

void insertChild(struct Node *parent, struct Node *child) {
    if (parent->children == NULL) {
        parent->children = child;
    }
    else {
        struct Node *temp = parent->children;
        while (temp->sibling != NULL) {
            temp = temp->sibling;
        }
        temp->sibling = child;
    }
}

struct Node *buildTree() {

}

void printTree() {
    
}

/* Print the pstree with process sorted by pid */
void print_with_sort() {

}

/* Print the pstree with pid specified */
void print_with_pid() {

}

/* Print the version of the pstree */
void print_version() {
    fprintf(stdout, "Pstree %s\n", VERSION);
}

int main(int argc, char *argv[]) {
    for (int i = 0; i < argc; i++) {
        assert(argv[i]); 
        /* Use if-else statement to handle the input arguments */
        if (strcmp(argv[i], "-n") == 0 || strcmp(argv[i], "--numeric-sort") == 0) {
            print_with_sort();
        }
        else if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--show-pids") == 0) {
            print_with_pid();
        }
        else if (strcmp(argv[i], "-V") == 0 || strcmp(argv[i], "--version") == 0) {
            print_version();
        }
        else {
            fprintf(stderr, "Error arguments, Please choose from [-n, -p, -V] or [--numeric-sort, --show-pids, --version]");
        }
        
    }
    assert(!argv[argc]); 
    return 0;
}