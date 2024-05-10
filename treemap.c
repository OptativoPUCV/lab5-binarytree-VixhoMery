#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
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
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) 
{
    TreeMap * arbol = (TreeMap *)malloc(sizeof(TreeMap));
    arbol->root = NULL;
    arbol->current = NULL;
    arbol->lower_than = lower_than;
    return arbol;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) 
{
  //si son iguales actualizar el nodo
  TreeNode *aux = tree->root;
  TreeNode *parent = NULL;

  while(aux != NULL)
    {
      parent = aux;

      if(tree->lower_than(key, aux->pair->key))
      {
        aux = aux->left;
      }
      else if(tree->lower_than(aux->pair->key, key))
      {
        aux = aux->right;
      }
      else return;
    }

  TreeNode *elemento = createTreeNode(key, value);
  elemento->parent = parent;

  if(parent == NULL)
  {
    tree->root = elemento;
  }

  else if(tree->lower_than(key, parent->pair->key))
  {
    parent->left = elemento;
  }
  else{
    parent->right = elemento;
  }

  tree->current = elemento;
  
}

TreeNode * minimum(TreeNode * x)
{
  if(x->left == NULL)
  {
    return x;
  }
  
  while(x->left != NULL)
    {
      x = x->left;
      
    }
  return x;
}


void removeNode(TreeMap * tree, TreeNode* node) 
{
  TreeNode *aux = tree->root;
  while(aux != NULL)
    {
      if(is_equal(tree, aux->pair->key, node->pair->key))
      {
        break;
      }
      else if(tree->lower_than(node->pair->key, aux->pair->key))
      {
        aux = aux->left;
      }
      else if(tree->lower_than(aux->pair->key, node->pair->key))
      {
        aux = aux->right;
      }
    }

  //Si es hoja
  if(aux->left == NULL && aux->right == NULL)
  {
    if(aux->parent->left == aux)
    {
      aux->parent->left = NULL;
    }
    else if(aux->parent->right == aux)
    {
      aux->parent->right = NULL;
    }
  }

  else if(aux->left != NULL && aux->right == NULL || aux->left == NULL && aux->right != NULL)
  {
    if(aux->parent->left == aux)
    {
      aux->parent->left = aux->left;
    }
    else if(aux->parent->right == aux)
    {
      aux->parent->right = aux->left;
    }
  }

  else if(aux->left != NULL && aux->right != NULL)
  {
    TreeNode *temporal = aux;
    while(temporal != NULL)
      {
        temporal = temporal->left;
      }
    
  }

}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    return NULL;
}

Pair * nextTreeMap(TreeMap * tree) 
{
  TreeNode *key = tree->current;
  if (key->right != NULL)
  {
    tree->current = minimum(key->right);
    return tree->current->pair;
  }
    
  else
  {
    key = tree->current;
    while(key->parent != NULL && key->parent->right == key)
      {
        key = key->parent;
        
      }
    tree->current = key->parent;
    return tree->current->pair;
  }
  return NULL;
}
