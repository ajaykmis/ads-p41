#ifndef ASSN4_H
#define ASSN4_H 

/*Debug printf. To enable debug printing.*/
#ifdef DEBUG_PRINT
    #define debug_printf(fmt, ...) printf(fmt, ##__VA_ARGS__)
#else
    #define debug_printf(fmt, ...)
#endif

typedef struct { /* B-tree node */ 
    int n; /* Number of keys in node */ 
    int *key; /* Node's keys */ 
    long *child; /* Node's child subtree offsets */ 
} btree_node;

typedef struct {
    
    btree_node *node ; 
    int offset ; 
    int order ; 
} btree ; 

int btree_order ;
long root_offset ; 


struct offset_info {
    
    long right_offset ; 
    long left_offset ; 
    int median ; 

};

/*to track unique file offsets*/
long file_offset[1000];
int count ; 


btree_node * newNode(int order); 
int find_key(FILE *fp, int target, long offset);
void free_node(btree_node *node); 
struct offset_info  * add_key (FILE *fp, int key, long offset ); 
void print_node(btree_node *node);
btree_node* read_btree_node(FILE *fp, long offset , int order) ; 
int write_btree_node(FILE *fp, btree_node *node, long offset, int order ); 
void print_buffer(int *, int );
#endif 
