#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;

struct TreeNode {
    void* key;
    void * value;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->key = key;
    new->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
   
    TreeMap * treeuwu = (TreeMap*) malloc(sizeof(TreeMap));
    treeuwu->root = NULL;
    treeuwu->current = NULL;
    treeuwu->lower_than = lower_than;
    return treeuwu;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {

    if (tree == NULL || tree->root == NULL) return;
    TreeNode* aux = tree->root;
    TreeNode* parent = NULL;

    TreeNode * nodo = createTreeNode(key,value);

    if(!tree->root){

        tree->root = nodo;
        return;

    }
    else{

        while (aux!=NULL){

            parent = aux;

            if (tree->lower_than(key,aux->key)){
                aux = aux->left;
            }
            else if (tree->lower_than(aux->key, key)){
                aux = aux->right;
            }
            else{
                return;
            }
        }

    }

    nodo->parent = parent;
    
    if(tree->lower_than(key, parent->key)){
        
        parent->left = nodo;
        
    }
    else{
        
        parent->right = nodo;
        
    }
    
    tree->current = nodo;

}

TreeNode * minimum(TreeNode * x){
  if ((x->parent== NULL)&& (x == NULL)){
    return NULL;
  }
  else if (x->left== NULL){
    return x;
  }
  else{
    return(minimum(x->left));
  } 
}


void removeNode(TreeMap * tree, TreeNode* node) {
   TreeNode * papa = node->parent;
   if (node->left == NULL && node->right==NULL){
     if (papa->left==node){
       papa->left =NULL;
     }
     else {
       papa->right=NULL;
     }
   }
   else if (node->left == NULL || node -> right ==NULL){
     if (papa ->left == node){
       if (node ->left!=NULL){
         papa->left = node->left;
         node->left->parent=papa;
       }
       else {
         papa->left=node->right;
         node->right->parent=papa;
       }
     }
     else{
      if (node ->left!=NULL){
         papa->right = node->left;
         node->left->parent=papa;
       }
       else {
         papa->right=node->right;
         node->right->parent=papa;
       }
     }
   }
   else {
     TreeNode * min = minimum(node->right);
     node->key=min->key;
     node->value=min->value;
     removeNode(tree, min);
   }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




void * searchTreeMap(TreeMap * tree, void* key) {
  if (tree == NULL) return NULL;

  TreeNode * aux = tree->root;

  while (aux!=NULL){
 if (tree->lower_than(key,aux->key)){
   aux = aux->left;
 }else if (tree->lower_than(aux->key,key)){
 aux = aux->right;
 }
 else{
 tree->current = aux;
 return aux->value;
 }

}
    return NULL;
}


void * upperBound(TreeMap * tree, void* key) {
  if (tree == NULL) return NULL;

  TreeNode * aux = tree->root;
  TreeNode * ub_node = NULL;

  while (aux!=NULL){
    if (tree->lower_than(key,aux->key)){
      if (ub_node == NULL){ 
        ub_node = aux;
      }
      else if (tree->lower_than(aux->key,ub_node->key) || is_equal(tree,ub_node->key, aux->key)){
        ub_node = aux;
      }
      aux = aux->left;
    }else if (tree->lower_than(aux->key,key)){  
      aux = aux->right;
    }
    else{
      if (tree->lower_than(aux->key,ub_node->key) || is_equal(tree,ub_node->key, aux->key)){
        ub_node = aux;
      }
      tree->current = aux;
      return ub_node->value;
    }
  }
  if (ub_node==NULL) return NULL;
  return ub_node->value;  
}

void * firstTreeMap(TreeMap * tree) {
  TreeNode * aux = tree->root;

 do{

   aux = aux->left;
 }while (aux->left != NULL);
 return aux->value;
}


void * nextTreeMap(TreeMap * tree) {
    
    if ((tree == NULL) || (tree->root == NULL)) return NULL;
    
    TreeNode * aux = tree->current;
    
    if (aux->right != NULL){
            
        aux = aux->right;

        while(aux->left != NULL){
            
            aux = aux->left;
        }
        
        tree->current = aux;
        return aux->value;
                
    }
    else {
            
        while(aux->parent != NULL){
            
            if(tree->lower_than(aux->key, aux->parent->key)){
                
                tree->current = aux->parent;
                return aux->parent->value;
                
            }
            
            aux = aux->parent;
            
        }
    
    }
    
    return NULL;
}