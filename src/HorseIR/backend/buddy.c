/* src: http://www.cnblogs.com/youxin/p/3694834.html */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef long long L;

typedef struct buddy_node {
    L size, level, value;
    struct buddy_node *left, *right, *parent;
}BN0,*BN;

BN newBlock(L n, BN p){
    BN z = (BN)malloc(sizeof(BN0));
    z->left   = NULL;
    z->right  = NULL;
    z->parent = p;
    z->size   = n;
    z->level  = n;
    z->value  = 0;
    return z;
}

void updateBlock(BN rt){
    if(rt && rt->parent){
        BN parent = rt->parent;
        parent->size = parent->left->size + parent->right->size;
        updateBlock(parent);
    }
}

BN addBlock(BN rt, L n){
    printf("n = %d, size = %d\n",n, rt->size);
    if(n <= rt->size){
        if(rt->left == NULL && rt->right == NULL){
            if((n<<1) <= rt->size){
                L half = (rt->size)>>1;
                rt->left  = newBlock(half,rt);
                rt->right = newBlock(half,rt);
                return addBlock(rt->left, n);
            }
            else {
                rt->size = 0;
                rt->value= n;
                updateBlock(rt);
                return rt;
            }
        }
        else if(rt->left->size < n){
            return addBlock(rt->right, n);
        }
        else if(rt->right->size < n){
            return addBlock(rt->left, n);
        }
        else if(rt->left->size < rt->right->size){
            return addBlock(rt->left, n);
        }
        else {
            return addBlock(rt->right, n);
        }
    }
    else {
        printf("full ...\n");
        exit(99);
    }
    return NULL; 
}

L removeBlock(BN rt){
    if(rt){
        rt->size = rt->level;
        rt->value= 0;
        updateBlock(rt);
    }
}

void freeBlock(BN rt){
    if(rt){
        freeBlock(rt->left);
        freeBlock(rt->right);
        free(rt);
    }
}

void printTree(BN rt){
    printf("(");
    if(rt){
        printf("%d ", rt->value);
        if(rt->left){
            printTree(rt->left);
            printTree(rt->right);
        }
    }
    printf(")");
}

void printRoot(BN rt){
    printTree(rt);
    printf("\n");
}

int main(){
    BN root   = newBlock(512, NULL);
    BN work_a = addBlock(root, 70); //add(A, 70)
    printRoot(root);
    BN work_b = addBlock(root, 35); //add(B, 35)
    printRoot(root);
    BN work_c = addBlock(root, 80); //add(C, 80)
    printRoot(root);
    removeBlock(work_a);            //end A
    printRoot(root);
    BN work_d = addBlock(root, 60); //add(D, 80)
    printRoot(root);
    removeBlock(work_b);            //end B
    printRoot(root);
    removeBlock(work_d);            //end D
    printRoot(root);
    removeBlock(work_c);            //end C
    printRoot(root);
    freeBlock(root);
    return 0;
}

