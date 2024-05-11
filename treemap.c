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
  // TreeNode *aux = tree->root;
  // TreeNode *parent = NULL;

  
  // while (aux != NULL) 
  // {
  //   if (is_equal(tree, aux->pair->key, node->pair->key)) 
  //   {
  //       break; 
  //   } 
  //   else if (tree->lower_than(node->pair->key, aux->pair->key))
  //   {
  //       parent = aux;
  //       aux = aux->left;
  //   } 
  //   else 
  //   {
  //     parent = aux;
  //     aux = aux->right;
  //   }
  // }

  // //Si es hoja
  // if(aux->left == NULL && aux->right == NULL)
  // {
  //   if(parent->left == aux)
  //   {
  //     parent->left = NULL;
  //   }
  //   else if(parent->right == aux)
  //   {
  //     parent->right = NULL;
  //   }
  // }
    
  // //Si tiene un hijo
  // else if((aux->left != NULL && aux->right == NULL) || (aux->left == NULL && aux->right != NULL))
  // {
  //   if(parent->left == aux)
  //   {
  //     if (aux->left == NULL) { parent->left = aux->right; } 
  //     else { parent->left = aux->left; }
  //   }
    
  //   if(parent->right == aux)
  //   {
  //     if (aux->left == NULL) { parent->right = aux->right; }
  //     else { parent->right = aux->left; }
  //   }
    
  // }
    
  // // //Si tiene dos hijos
  // // else if(aux->left != NULL && aux->right != NULL)
  // // {
  // //   TreeNode *max = 

  // // }

      if (tree == NULL || node == NULL || tree->root == NULL) {
          return; // Verifica la validez de los parámetros y si el árbol está vacío
      }

      TreeNode *aux = tree->root;
      TreeNode *parent = NULL;

      // Busca el nodo a eliminar en el árbol
      while (aux != NULL) {
          if (is_equal(tree, aux->pair->key, node->pair->key)) {
              break; // Nodo encontrado
          } else if (tree->lower_than(node->pair->key, aux->pair->key)) {
              parent = aux;
              aux = aux->left;
          } else {
              parent = aux;
              aux = aux->right;
          }
      }

      // Si el nodo no fue encontrado en el árbol
      if (aux == NULL) {
          return; // Nodo no encontrado, termina la función
      }

      // Caso 1: Nodo es una hoja (no tiene hijos)
      if (aux->left == NULL && aux->right == NULL) {
          if (parent == NULL) {
              // El nodo es la raíz del árbol
              tree->root = NULL;
          } else if (parent->left == aux) {
              // El nodo es hijo izquierdo de su padre
              parent->left = NULL;
          } else {
              // El nodo es hijo derecho de su padre
              parent->right = NULL;
          }
          freeTreeNode(aux); // Libera la memoria del nodo eliminado
      }
      // Caso 2: Nodo tiene un solo hijo
      else if (aux->left != NULL && aux->right == NULL) {
          if (parent == NULL) {
              // El nodo es la raíz del árbol
              tree->root = aux->left;
          } else if (parent->left == aux) {
              // El nodo es hijo izquierdo de su padre
              parent->left = aux->left;
          } else {
              // El nodo es hijo derecho de su padre
              parent->right = aux->left;
          }
          if (aux->left != NULL) {
              aux->left->parent = parent; // Actualiza el padre del hijo
          }
          freeTreeNode(aux); // Libera la memoria del nodo eliminado
      } else if (aux->left == NULL && aux->right != NULL) {
          if (parent == NULL) {
              // El nodo es la raíz del árbol
              tree->root = aux->right;
          } else if (parent->left == aux) {
              // El nodo es hijo izquierdo de su padre
              parent->left = aux->right;
          } else {
              // El nodo es hijo derecho de su padre
              parent->right = aux->right;
          }
          if (aux->right != NULL) {
              aux->right->parent = parent; // Actualiza el padre del hijo
          }
          freeTreeNode(aux); // Libera la memoria del nodo eliminado
      }
      // Caso 3: Nodo tiene dos hijos
      else {
          TreeNode *successor = minimum(aux->right); // Encuentra el sucesor del nodo a eliminar
          aux->pair = successor->pair; // Copia el par del sucesor al nodo actual
          removeNode(tree, successor); // Elimina el sucesor recursivamente
      }
  }
}

void eraseTreeMap(TreeMap * tree, void* key)
{
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) 
{
  TreeNode *aux = tree->root;
  while(aux != NULL)
    {
      if(is_equal(tree, aux->pair->key, key))
      {
        tree->current = aux;
        return aux->pair;
      }
      else if(tree->lower_than(key, aux->pair->key))
      {
        aux = aux->left;
      }
      else if(tree->lower_than(aux->pair->key, key))
      {
        aux = aux->right;
      }
      
    }
  return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    return NULL;
}

Pair *nextTreeMap(TreeMap *tree) {
    if (tree == NULL || tree->current == NULL) 
    {
        return NULL; // Verifica la validez de los parámetros y el nodo actual
    }

    TreeNode *key = tree->current;

    // Caso 1: Si el nodo actual tiene un hijo derecho
    if (key->right != NULL) 
    {
        // Encuentra el nodo más a la izquierda del subárbol derecho (mínimo)
        tree->current = minimum(key->right);
        if (tree->current != NULL) 
        {
            return tree->current->pair; // Retorna el par asociado al nodo mínimo encontrado
        }
        return NULL; // Si no se encontró ningún nodo mínimo, retorna NULL
    }

    // Caso 2: Si el nodo actual no tiene hijo derecho
    // Retrocede por los ancestros hasta encontrar el primer ancestro izquierdo
    while (key->parent != NULL && key->parent->right == key) {
        key = key->parent;
    }

    // Si se llegó al nodo raíz y no hay más nodos por visitar
    if (key->parent == NULL) 
    {
        tree->current = NULL; // No hay más nodos en el árbol para visitar
        return NULL;
    }

    // El próximo nodo en orden es el ancestro izquierdo encontrado
    tree->current = key->parent;
    return tree->current->pair; // Retorna el par asociado al nodo ancestro izquierdo
}
