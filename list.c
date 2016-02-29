#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct island {
    char *name;
    char *opens;
    char *closes;
    struct island *next;
} island;

void display(island *);

island *create(char *);

void release(island *);

int main() {
    printf("Game on\n");
    island *start = NULL;
    //i as a iterator
    island *i = NULL;
    island *node = NULL;
    char name[80];
    for (; fgets(name, 80, stdin); i = node) {
        if (name[strlen(name) - 1] == '\n')
            name[strlen(name) - 1] = 0;
        //node leads to a piece of storage which contains the new island,the 'node' pointer changes every time you create a new island.
        node = create(name);
        if (start == NULL) {
            start = node;
        }
        if (i != NULL) {
            i->next = node;
        }
    }
    display(start);
    release(start);
    return 0;
}

void display(island *start) {
    island *i = start;
    for (; i != NULL; i = i->next)
        printf("Name:%s\nopen:%s-%s\n", i->name, i->opens, i->closes);
}

island *create(char *name) {
    island *i = malloc(sizeof(island));
    i->name = strdup(name);
    i->opens = "09:00";
    i->closes = "17:00";
    i->next = NULL;
    return i;
}

void release(island *start) {
    island *i = start;
    island *next = NULL;
    for (; i != NULL; i = next) {
        next = i->next;
        free(i->name);
        free(i);
    }
    printf("finish releasing.\n");
}
