#ifndef DEFL_H_
#define DEFL_H_

struct Item{
    char o_path[1000];
    char t_path[1000];
    char test_path[1000];
    char name[1000];
    int size;
    // int size;
    struct Item * next;
    
};

struct Feature{
    char name[1000];
    struct Item* head;
    int tot;
    struct Feature * next;
};
#endif