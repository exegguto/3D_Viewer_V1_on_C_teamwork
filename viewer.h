#ifndef VIEWER_H
#define VIEWER_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef struct Node {
  double *coords;
  int *nodes;
  int nodeCount;
  struct Node *prev;
  struct Node *next;
} Node;

typedef struct List {
  Node *head;
  Node *tail;
} List;

/**
 * @brief Loads file, fills two lists
 *
 * @param path path to the file
 * @param nodes_ lists of nodes
 * @param surfaces list of surfaces
 * @param nodeAmount amount of loaded nodes
 * @param edgeAmount amount of loaded surfaces
 * @param borderlands - min/max coords for all 3 axis
 */
char loadFromFile(char *path, List *nodes_static, List *nodes_dynamic,
                  List *surfaces, int *nodeAmount, int *edgeAmount,
                  double *borderlands, double *borderlands_2);

/**
 * @brief Checks if the string in obj file fits the template
 *
 * @param objstr string in obj file
 * @return 0-2 - check result (Doesn't fit/Is a node/Is a surface)
 */
int isValidObj(char *objstr);

/**
 * @brief Checks if the entire string is a number
 *
 * @param token string to check
 * @param filter node or surface
 * @return 0/1 - check result (No/Yes)
 */
char isNumber(char *token, char filter);
int countNodes(char *token);
void fillNode(char *tok, double *coordstmp, int *nodestmp, int nodeCounttmp);
void push(List *view_list, Node *view_node);
void pop(List *view_list);

/**
 * @brief Frees the allocated memory
 *
 * @param view_list list to destroy
 */
void destroyList(List *view_list);

/**
 * @brief Moves the object
 * @param nodes_static initial array of coordinates
 * @param nodes_dynamic array of coordinates after translation
 * @param N amount of nodes
 * @param new_ coordinates to translate to
 */
void adjustTranslate(double **nodes_static, double **nodes_dynamic, int N,
                     double newX, double newY, double newZ,
                     double *borderlands_2, double *borderlands);

/**
 * @brief Rotates the object
 * @param nodes_dynamic array of coordinates
 * @param N amount of nodes
 * @param new_ angles of rotation
 */
void adjustRotate(double **nodes_dynamic, int N, double newX, double newY,
                  double newZ);

/**
 * @brief Scales the object
 * @param nodes_dynamic array of coordinates
 * @param N amount of nodes
 * @param ratio scale ratio
 */
void adjustScale(double **nodes_dynamic, int N, double ratio);

#endif  // VIEWER_H
