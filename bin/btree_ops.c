#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assn4.h"
#include<math.h>

void print_buffer(int buffer[], int len  ) {
    
    for (int i =0 ; i < len ; i++ ) {
        printf("%d ", buffer[i]); 
    } 
    printf("\n") ; 
}
int find_key(FILE *fp, int target, long offset ){

    int s = 0 ; 
    btree_node *node =  read_btree_node(fp, offset, btree_order );
    
    if (node == NULL) {
        debug_printf("Empty tree. Key not found.\n");
        return EXIT_FAILURE ; 
    }
    while (s < node->n ) {
        debug_printf("While: s=%d, key:%d, num of keys= %d\n", s, node->key[s], node->n  );
        if (target == node->key[s]) {
            return EXIT_SUCCESS ; 
        }

        else if (target < node->key[s]) {
            break ; 
        }

        else {
            s++ ; 
        }
    }

    if ((node)->child[s] != 0 ) {

        debug_printf("Branching, Calling child at offset: %ld\n", node->child[s] );
        return find_key(fp, target, node->child[s]);
    } 
    else {/*read the btree node from the file. */
        return EXIT_FAILURE ;
    }
}

int cmpfunc (const void * a, const void * b)
{
    int *ptr1 = (int *)a ; 
    int *ptr2 = (int *)b ;    
    debug_printf("ptr1: %d, ptr2:%d\n", *ptr1, *ptr2);
    return ( *ptr1 - *ptr2 );
}

btree_node * newNode(int order) {
    
    btree_node *node = malloc(sizeof(*node));
    node->n = 0; 
    node->key = (int *) calloc( order , sizeof( int ) ); 
    node->child = (long *) calloc( (order + 1), sizeof( long ) );
    return node ; 
}

void free_node(btree_node *node) {

    free(node->key);
    free(node->child);
    free(node) ;

}

/*node to be splitted*/    
void split_node (FILE *fp, btree_node *node, long offset, long *left_offset, long *right_offset, int *median_value) {

    //find m
    int m = ceil((double)(node->n / 2)) ;
    debug_printf("node overflowed, splitting node");
    debug_printf("Median to be promoted up: %d, current offset: %ld, num_keys:%d\n", node->key[m], offset, node->n);
    /*split the node*/

    //Left Node : 
    int r_n = node->n ; 
    *left_offset = offset ; 
    node->n = m ;
    write_btree_node(fp, node, offset, btree_order);
    //create a right node and insert the elements in right node. 
    btree_node *r_node = newNode(btree_order);
    r_node->n =  r_n - m -1 ; 
    debug_printf("Number of keys right child:%d\n", r_node->n);
    for(int j = 0 ; j < r_node->n  ; j++) {
        r_node->key[j] =  node->key[j + m + 1];
    }
    /*copy the children*/
    for (int j= 0 ; j <= r_node->n ; j++ ) {
        r_node->child[j] = node->child[j + m +1];
    }
    file_offset[count++] = *right_offset ; 
    *right_offset = write_btree_node(fp, r_node, -1, btree_order  );
    debug_printf("Right Node offset: %ld\n", *right_offset);
    print_node(r_node);
    *median_value = node->key[m] ; 

}
//main will 
//
struct offset_info * add_key (FILE *fp, int key, long offset ) {

    btree_node *node = read_btree_node (fp, offset, btree_order);
    if (node == NULL) {
        debug_printf("No node found at offset:%ld, creating root\n", offset);
        node = newNode(btree_order) ;
        node->key[node->n++] = key ;
        root_offset = write_btree_node(fp, node, offset, btree_order);
        file_offset[count++] = root_offset ; 
        debug_printf("Root offset: %ld\n", root_offset);
        //push(&head, root_offset);
        return  NULL; 
    }
    int s = 0 ; 
    while (s < node->n ) {
        
        if (key == node->key[s]) {
            printf("Entry with key=%d already exists\n", key);
            return NULL ; 
        }

        else if (key < node->key[s]) {
            break ; 
        }

        else {
            s++ ; 
        }
    }

    if ((node)->child[s] != 0 ) {
         
        long child_offset = (node)->child[s];
        free_node(node); 
        node = NULL ;
        debug_printf("Calling add on child %d,child offset: %ld\n ", s, child_offset);
        struct offset_info *ret = add_key(fp, key,child_offset );
        btree_node *node = read_btree_node (fp, offset, btree_order);
        
        if (ret != NULL) 
        {
            
            debug_printf("Child node splitted, adding child to this node: child to be added: %d\n", ret->median);
            print_node(node);
            int temp_len = node->n ; 
            /*
            if (node->n == (btree_order -1)) { //Because we will be inserting one more element, splitting will \
                //take place. So copy 
                    temp_len++ ; 
                    debug_printf( "Copying at extra location %d\n",temp_len);
            }
            */
            for (int i = temp_len ; i > s ; i-- ){

                node->key[i] = node->key[i-1];
                node->child[i+1] = node->child[i];
            }
            node->key[s] = ret->median ;
            debug_printf("Adding child at offset: %ld\n",ret->right_offset);
            node->child[s] = ret -> left_offset ; 
            node->child[s+1] = ret->right_offset ; 
            node->n++ ; 
           
            debug_printf("Before Splitting offset:%ld\n", offset);
            print_node(node);

            //compare the new length 
            if (node->n <= btree_order -1){
                //just copy the new list in parent. 
                write_btree_node(fp, node, offset, btree_order) ; 
                free(ret);
                return NULL ; 
            }
            else { // if splitting took place 

                struct offset_info *retval= malloc(sizeof(struct offset_info)) ; 
                retval->left_offset = 0 ; 
                retval->right_offset = 0 ; 
                retval->median = 0 ; 
                split_node(fp, node, offset, &(retval->left_offset), &(retval->right_offset) , &(retval->median));
                return retval ; 
            }
        } 

        return NULL ; 

    }

    else { //this is a leaf node and the key needs to be inserted here. 

        node->key[node->n++] = key ; 
        //void qsort(void *base, size_t nitems, size_t size, int (*compar)(const void *, const void*))
        qsort(node->key, node->n , sizeof(int), cmpfunc ) ; 
        debug_printf("Printing %d the array after sorting\n", node->n);
        //print_buffer(node->key, node->n);
       
        //compare the new length 
        if (node->n <= btree_order -1){
            //just copy the new list in parent. 
            debug_printf("Writing  Node back, no splitting required");
            write_btree_node(fp, node, offset, btree_order) ; 
            return NULL ; 
        } 

        else {
            //copy child offsets as well. 
            struct offset_info *retval= malloc(sizeof(struct offset_info)) ; 
            retval->left_offset = 0 ; 
            retval->right_offset = 0 ; 
            retval->median = 0 ; 
            split_node(fp, node, offset, &(retval->left_offset), &(retval->right_offset) , &(retval->median));
            return retval ; 
        } 

    }

}
