#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include "assn4.h"
#include <sys/types.h>
#include <sys/stat.h> 

void print_node(btree_node *node){

    #ifdef DEBUG_PRINT 

    printf("keys:\n");
    for(int i=0 ; i < (node->n) ; i++) {
        printf("%d ",node->key[i] );
    }
    printf("\nChild Offsets:\n");
    for(int i=0 ; i<= node->n ; i++) {
        printf("%ld ", node->child[i]);
    }
    printf("\n");

    #endif
}

btree_node* read_btree_node(FILE *fp, long offset , int order) {
    
    //debug_printf("read_btree_node: offset == %ld\n", offset);
    if (offset == -1) {
        return NULL ; 
    }
    fseek( fp, offset, SEEK_SET ); 
    btree_node *node = (btree_node *)malloc(sizeof(btree_node));
    fread(&(node->n), sizeof(int), 1, fp);
    node->key = (int *) calloc( order, sizeof( int ) ); 
    node->child = (long *) calloc( order + 1, sizeof( long ) );
    fread(node->key, sizeof(int), order - 1, fp);
    fread(node->child, sizeof(long), order , fp );
    //print_node(node);
    return node ; 
}

int write_btree_node(FILE *fp, btree_node *node, long offset, int order ) {

    if (offset != -1) 
        fseek( fp, offset, SEEK_SET ); 
    else 
        fseek(fp, 0, SEEK_END);
    offset = ftell(fp);
    debug_printf("Writing at offset: %ld\n", offset);
    print_node(node); 
    fwrite( &(node->n), sizeof( int ), 1, fp ) ; 
    fwrite( (node->key), sizeof( int ), order - 1, fp ) ; 
    fwrite( (node->child), sizeof( long ), order , fp ) ; 
    debug_printf("returning offset:%ld\n", offset);
    return offset ; 
}

