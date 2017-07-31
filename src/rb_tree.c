#include<stdio.h>
#include<unistd.h>


// #define RB_TREE(name)  struct name{ \
//    struct name *left;              \
//    struct name*right;             \
//    int color;                     \
//    struct name *parent;           \
//    void *buf;                     \
// }; 
#include<string.h>
#include<errno.h>
#include<sys/types.h>



void unix_error(char *msg){
      frpintf(stderr,"%s %s",msg,strerror(errno));
      exit(0);
}



 
typedef  struct _rb_node_t {
         struct _rb_node_t *left;
         struct _rb_node_t *right;
         struct _rb_node_t *parent;
         int color;
         void * buf;
} rb_node_t;



/*@rb_tree_alloc()@*/
 
 rb_node_t *alloc_node(void *data){
      
      rb_node_t * node=(rb_node_t *)malloc(sizeof(rb_node_t));
      if(node==NULL){
 
      }
 }


/*@end@*/

/*@left rotate start@*/
static  rb_node_t *rotate_left(rb_node_t *node,rb_node_t *root){
        
         rb_node_t *right=node->right;

         if((node->right=right->left)){
               right->left->parent=node;
         }

         right->left=node;

         if((right->parent=node->parent)){
                    if(node->parent->right==node){
                        node->parent->right=right;
                    }else{
                        node->parent->left=right;
                    }
         }else{
             root=right; 
         }
         return root;                

}
/*rotate left end*/


static rb_node_t * rotate_right(rb_node_t *node,rb_node_t *root){
                
                rb_node_t *left=node->left;
                if(node->left=left->right){
                     left->right->parent=node;
                }                
                left->right=node;
                if(left->parent=node->parent){
                            if(node->parent->left==node){
                                node->parent->left=left;
                            }else{
                                node->parent->right=left;
                            }
               }else{
                   root=left;
               }
            return root;
}




    



/*@main start@*/
int main(){

    printf("hello world!");
    return 0;
}
/*@end@*/