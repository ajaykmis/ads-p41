#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "assn4.h"
#include<assert.h>
#include "queue.h"


void print_btree(FILE *fp, long offset){

    //push the children into a queue. 
    head = tail = NULL ; 
    int level = 1;
    enqueue(&head, &tail, offset);
    int nodes_this_level = 1 ; 
    int nodes_next_level = 0 ; 
    printf(" %d: ", level);
    debug_printf("Printing btree\n");
    while (!isEmpty(head) ) {
        
        long c_offset = delete_q (&head, &tail); //read the next offset.
        btree_node *node = read_btree_node(fp, c_offset, btree_order); 
        nodes_this_level-- ; 
        //for each children,
        if (node == NULL) {
            
            return ; 
        }
        
        for (int j = 0 ; j < (node->n -1) ; j++){
            printf("%d,", node->key[j]);
        }
        printf("%d ", node->key[node->n -1]); 
        for (int i=0 ; i <= node->n ; i++ ){
        
            if (node->child[i] !=0)
            {
                enqueue(&head, &tail, node->child[i]);
                nodes_next_level++ ;  
            }
        }

        if (nodes_this_level == 0 ){
            level++ ; 
            nodes_this_level = nodes_next_level ; 
            nodes_next_level = 0 ; 
            if (nodes_this_level != 0 )
                printf("\n %d: ", level);
        }
    }
    debug_printf("Exiting Btree\n");
    printf("\n");
}

int main (int argc, char **argv) {


    FILE *fp; /* Input file stream */ 
    
    if (argc < 3 ) {
        
        fprintf(stderr, "Insufficient Arguments: Usage: ./assn4 <index filename> <order>\n");
        exit(-1);
    }
    
    btree_order = atoi(argv[2]);
    fp = fopen(argv[1], "r+b");
    count = 0 ; 
    
    if (fp == NULL) {
        
        root_offset = -1 ; 
        fp = fopen(argv[1], "w+b");
        fwrite(&root_offset, sizeof(long),1,fp);
    }
    else {
        fread(&root_offset, sizeof(long), 1, fp);
        debug_printf("file already existing: root_offset:%ld\n", root_offset);
    }
    char buffer[100];
    while(1) {
        if (fgets(buffer, sizeof(buffer), stdin)) {
            char *temp = buffer ; 
            buffer[strlen(buffer)-1] = '\0' ;
            //debug_printf("buffer : %s, strlen(buffer) = %lu\n", buffer, strlen(buffer)) ;
            /*Close index file, availability file, student file etc. */         
            
            
            if (buffer[strlen(buffer)-1] == '\r'){
                buffer[strlen(buffer)-1] = '\0';
            }

            if (!strcmp(buffer,"end") ){
                debug_printf("exiting.\n"); 
                fseek(fp, 0, SEEK_SET) ;
                debug_printf("Writing root offset back to file: %ld\n", root_offset);
                fwrite(&root_offset, sizeof(long),1,fp);
                fclose( fp );
                exit(1) ;
            }

            if (!strcmp(buffer, "print")) {
                    print_btree(fp, root_offset);
                    continue ; 
            }

            if( (temp = strchr(buffer,' ') ) ) {
                *temp = '\0' ; //nullterminate the string here.  
                temp++ ;  // remaining string pointed by temp. 
                debug_printf("buffer : %s, temp: %s\n", buffer, temp) ;

                if(!strcmp(buffer,"add")) {
                    int sid = atoi(temp) ; 
                    struct offset_info *ret ; 
                    debug_printf("Key to Add: %d\n", sid);
                    ret = add_key(fp, sid, root_offset);
                    if (ret != NULL) 
                    {
                        btree_node *node = newNode(btree_order);
                        node->key[node->n++] = ret->median ;
                        node->child[0] = ret->left_offset ; 
                        node->child[1] = ret->right_offset ; 

                        //compare the new length 
                        if (node->n <= btree_order -1){
                            //just copy the new list in parent. 
                            root_offset = write_btree_node(fp, node, -1, btree_order) ; 
                            file_offset[count++] = root_offset;
                            debug_printf("New Root node created at offset:%ld \n", root_offset);
                            free(ret);
                        } else {
                           debug_printf("Shouldnt split here\n");
                            assert(0);
                        }
                        free_node(node);
                    }
                }

                else if (!strcmp(buffer, "find")) {
                    int sid = atoi(temp) ; 
                    int ret = find_key(fp, sid, root_offset) ; 
                    if (ret == EXIT_SUCCESS) {

                        printf("Entry with key=%d exists\n",sid);
                    }
                    else {
                        printf("Entry with key=%d does not exist\n", sid);
                    }
                }
                else if(!strcmp(buffer,"print")) {
                    debug_printf("printing %s\n", temp) ; 
                    if (!strcmp(temp, "offset")){
                        
                        //print_buffer(file_offset, count);
                    }else {
                        long c_offset = atol(temp);
                        btree_node *node = read_btree_node(fp, c_offset, btree_order);
                        print_node(node); 
                        free(node);
                    }
                }
                else {
                    fprintf(stderr, "Wrong token\n") ;
                }

            }
        }
    }
} 
