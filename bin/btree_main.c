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
    printf("%d: ", level);
    while (!isEmpty(head) ) {
        
        long c_offset = delete_q (&head, &tail); //read the next offset.
        btree_node *node = read_btree_node(fp, c_offset, btree_order); 
        nodes_this_level-- ; 
        //for each children,
        if (node == NULL) {
            
            return ; 
        }
        
        for (int j = 0 ; j < node->n ; j++){
            printf("%d, ", node->key[j]);
        }
        for (int i=0 ; i <= node->n ; i++ ){
        
            if (node->child[i] !=0)
            {
                enqueue(&head, &tail, node->child[i]);
                nodes_next_level++ ;  
            }
        }

        if (nodes_this_level == 0 ){
            level++ ; 
            printf("\n%d: ", level);
            nodes_this_level = nodes_next_level ; 
            nodes_next_level = 0 ; 
        }
    }
}

int main (int argc, char **argv) {


    FILE *fp; /* Input file stream */ 
    btree_order = atoi(argv[2]);
    fp = fopen(argv[1], "r+b");
    if (fp == NULL) {
        
        root_offset = -1 ; 
        fp = fopen(argv[1], "w+b");
        fwrite(&root_offset, sizeof(long),1,fp);
    }
    else {
        debug_printf("file already existing: root_offset:%ld\n", root_offset);
        fread(&root_offset, sizeof(long), 1, fp);
    }
    char buffer[100];
    while(1) {
        if (fgets(buffer, sizeof(buffer), stdin)) {
            char *temp = buffer ; 
            buffer[strlen(buffer)-1] = '\0' ;
            debug_printf("buffer : %s, strlen(buffer) = %lu\n", buffer, strlen(buffer)) ;
            /*Close index file, availability file, student file etc. */         
            if (!strcmp(buffer,"end") ){
                debug_printf("exiting.\n"); 
                fseek(fp, 0, SEEK_SET) ;
                debug_printf("Writing root offset back to file: %ld\n", root_offset);
                fwrite(&root_offset, sizeof(long),1,fp);
                fclose( fp );
                exit(1) ;
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
                        int s = 0 ; 
                        btree_node *node = newNode(btree_order);
                        node->key[s] = ret->median ;
                        node->child[s] = ret->right_offset ; 
                        node->child[0] = ret->left_offset ; 
                        node->n++ ;
                        //compare the new length 
                        if (node->n <= btree_order -1){
                            //just copy the new list in parent. 
                            root_offset = write_btree_node(fp, node, -1, btree_order) ; 
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
                    debug_printf("printing index\n") ; 
                    print_btree(fp, root_offset);
                }
                else {
                    fprintf(stderr, "Wrong token\n") ;
                }

            }
        }
    }
} 
