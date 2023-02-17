#include "viewer.h"

char loadFromFile(char *path, List *nodes_static, List *nodes_dynamic,
                  List *surfaces, int *nodeAmount, int *edgeamount,
                  double *borderlands, double *borderlands_2) {
  FILE *obj;
  char tok[255];
  Node *nElem;
  if (!(obj = fopen(path, "r+"))) {
    return -1;
  }

  for (int i = 0; i < 6; i++) {
    borderlands[i] = NAN;
  }

  int objCheck = 0;

  while (fgets(tok, 254, obj)) {
    tok[strcspn(tok, "\n")] = 0;
    tok[strcspn(tok, "\r")] = 0;

    objCheck = isValidObj(tok);

    if (objCheck == 1) {
      nElem = malloc(sizeof(Node));
      nElem->coords = malloc(3 * sizeof(double));
      nElem->nodes = NULL;
      nElem->nodeCount = 0;
      fillNode(tok + 2, nElem->coords, nElem->nodes, nElem->nodeCount);
      push(nodes_static, nElem);
      nElem = malloc(sizeof(Node));
      nElem->coords = malloc(3 * sizeof(double));
      nElem->nodes = NULL;
      nElem->nodeCount = 0;
      fillNode(tok + 2, nElem->coords, nElem->nodes, nElem->nodeCount);
      push(nodes_dynamic, nElem);
      (*nodeAmount)++;
      if (borderlands[0] != borderlands[0]) {
        for (int i = 0, j = 0; i < 6; i++, j = i / 2) {
          borderlands[i] = nElem->coords[j];
          borderlands_2[i] = nElem->coords[j];
        }
      } else {
        if (nElem->coords[0] > borderlands[0]) {
          borderlands[0] = nElem->coords[0];
          borderlands_2[0] = nElem->coords[0];
        } else if (nElem->coords[0] < borderlands[1]) {
          borderlands[1] = nElem->coords[0];
          borderlands_2[1] = nElem->coords[0];
        }
        if (nElem->coords[1] > borderlands[2]) {
          borderlands[2] = nElem->coords[1];
          borderlands_2[2] = nElem->coords[1];
        } else if (nElem->coords[1] < borderlands[3]) {
          borderlands[3] = nElem->coords[1];
          borderlands_2[3] = nElem->coords[1];
        }
        if (nElem->coords[2] > borderlands[4]) {
          borderlands[4] = nElem->coords[2];
          borderlands_2[4] = nElem->coords[2];
        } else if (nElem->coords[2] < borderlands[5]) {
          borderlands[5] = nElem->coords[2];
          borderlands_2[5] = nElem->coords[2];
        }
      }
    }
    if (objCheck > 1) {
      Node *nElem = malloc(sizeof(Node));
      nElem->coords = NULL;
      nElem->nodes = malloc(objCheck * sizeof(int));
      nElem->nodeCount = objCheck;
      fillNode(tok + 2, nElem->coords, nElem->nodes, nElem->nodeCount);
      push(surfaces, nElem);
      (*edgeamount) += objCheck;
    }
  }
  fclose(obj);
  return 0;
}

void adjustTranslate(double **nodes_static, double **nodes_dynamic, int N,
                     double newX, double newY, double newZ,
                     double *borderlands_2, double *borderlands) {
  for (int i = 0; i < N; i++) {
    nodes_dynamic[i][0] = nodes_static[i][0] + newX;
    nodes_dynamic[i][1] = nodes_static[i][1] + newY;
    nodes_dynamic[i][2] = nodes_static[i][2] + newZ;
  }
  borderlands[0] = borderlands_2[0] + newX;
  borderlands[1] = borderlands_2[1] + newX;
  borderlands[2] = borderlands_2[2] + newY;
  borderlands[3] = borderlands_2[3] + newY;
  borderlands[4] = borderlands_2[4] + newZ;
  borderlands[5] = borderlands_2[5] + newZ;
}

void adjustRotate(double **nodes_dynamic, int N, double newX, double newY,
                  double newZ) {
  newX *= M_PI / 180; /*rotation relative to X*/
  newY *= M_PI / 180; /*rotation relative to Y*/
  newZ *= M_PI / 180; /*rotation relative to Z*/
  double newX_coord, newY_coord, newZ_coord;
  double *trig = malloc(6 * sizeof(double));
  trig[0] = sin(newX);
  trig[1] = cos(newX);
  trig[2] = sin(newY);
  trig[3] = cos(newY);
  trig[4] = sin(newZ);
  trig[5] = cos(newZ);
  for (int i = 0; i < N; i++) {
    newX_coord = nodes_dynamic[i][0] * trig[3] * trig[5] +
                 nodes_dynamic[i][1] * trig[3] * -trig[4] +
                 nodes_dynamic[i][2] * trig[2];
    newY_coord = nodes_dynamic[i][0] *
                     (-trig[0] * -trig[2] * trig[5] + trig[1] * trig[4]) +
                 nodes_dynamic[i][1] *
                     (-trig[0] * -trig[2] * -trig[4] + trig[1] * trig[5]) +
                 nodes_dynamic[i][2] * -trig[0] * trig[3];
    newZ_coord = nodes_dynamic[i][0] *
                     (trig[1] * -trig[2] * trig[5] + trig[0] * trig[4]) +
                 nodes_dynamic[i][1] *
                     (trig[1] * -trig[2] * -trig[4] + trig[0] * trig[5]) +
                 nodes_dynamic[i][2] * trig[1] * trig[3];
    nodes_dynamic[i][0] = newX_coord;
    nodes_dynamic[i][1] = newY_coord;
    nodes_dynamic[i][2] = newZ_coord;
  }

  free(trig);
}

void adjustScale(double **nodes_dynamic, int N, double ratio) {
  if (ratio <= 0) {
    return;
  }
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < 3; j++) {
      nodes_dynamic[i][j] *= ratio;
    }
  }
}

int isValidObj(char *objstr) {
  if ((*objstr != 'v') && (*objstr != 'f')) {
    return 0;
  }
  int truth = -1;
  int nodeCount = 0;
  char *objstr_tmp = malloc(strlen(objstr) + 1);
  strcpy(objstr_tmp, objstr);
  char *token = strtok(objstr_tmp, " ");
  if (!strcmp(token, "v")) {
    truth = 1;
  } else if ((!strcmp(token, "f")) && (nodeCount = countNodes(objstr + 2))) {
    truth = nodeCount;
  } else {
    truth = 0;
  }

  if (truth == 1) {
    for (int i = 0; (truth) && (i < 3); i++) {
      token = strtok(NULL, " ");
      truth *= isNumber(token, 'v');
    }

    if (truth) {
      token = strtok(NULL, " ");
      if (token) {
        truth = 0;
      }
    }
  }

  free(objstr_tmp);
  return truth;
}

int countNodes(char *f_string) {
  int count = 0;
  char truth = 1;
  char *f_string_tmp = malloc(strlen(f_string) + 1);
  strcpy(f_string_tmp, f_string);
  char *token = strtok(f_string_tmp, " ");
  while (token && truth) {
    truth = isNumber(token, 'f');
    count = (count + truth) * truth;
    token = strtok(NULL, " ");
  }
  if (count < 2) {
    count = 0;
  }
  free(f_string_tmp);
  return count;
}

char isNumber(char *token, char filter) {
  if (!token) {
    return 0;
  }
  char truth = 1;
  switch (filter) {
    case 'v': {
      if (*token == '-') {
        token++;
      }
      if (!(*token)) {
        return 0;
      }
      char hasDot = 0;
      char hasE = 0;
      for (char *tmp = token; *tmp && truth; tmp++) {
        if ((!hasDot) && (!hasE) && (*tmp == '.')) {
          hasDot++;
        } else if ((!hasE) && ((*tmp == 'E') || (*tmp == 'e')) &&
                   ((*(tmp + 1) == '+') || (*(tmp + 1) == '-'))) {
          hasE++;
          tmp++;
        } else if ((*tmp < '0') || (*tmp > '9')) {
          truth = 0;
        }
      }
      break;
    }
    case 'f': {
      if ((*token > '0') && (*token <= '9')) {
        token++;
      } else {
        truth = 0;
      }
      for (char *tmp = token; (*tmp) && (*tmp != '/') && (truth); tmp++) {
        if ((*tmp < '0') || (*tmp > '9')) {
          truth = 0;
        }
      }
      break;
    }
    default:
      truth = 0;
  }

  return truth;
}

void fillNode(char *tok, double *coordstmp, int *nodestmp, int nodeCounttmp) {
  if (coordstmp) {
    for (int i = 0; i < 2; i++) {
      coordstmp[i] = atof(tok);
      while ((*tok) && (*tok != ' ')) {
        tok++;
      }
      if (*tok == ' ') {
        tok++;
      }
    }
    coordstmp[2] = atof(tok);
  }
  if (nodestmp) {
    for (int i = 0; i < nodeCounttmp - 1; i++) {
      nodestmp[i] = atoi(tok);
      while ((*tok) && (*tok != ' ')) {
        tok++;
      }
      if (*tok == ' ') {
        tok++;
      }
    }
    nodestmp[nodeCounttmp - 1] = atoi(tok);
  }
}

void push(List *view_list, Node *view_node) {
  if (!(view_list->head)) {
    view_list->head = view_node;
    view_list->tail = view_node;
    view_node->prev = NULL;
  } else {
    view_list->tail->next = view_node;
    view_node->prev = view_list->tail;
    view_list->tail = view_node;
  }
  view_node->next = NULL;
}

void pop(List *view_list) {
  if (view_list->head) {
    Node *tmp = view_list->tail;
    if (tmp->nodes) {
      free(tmp->nodes);
      tmp->nodes = NULL;
    }
    if (view_list->head != view_list->tail) {
      view_list->tail = tmp->prev;
      view_list->tail->next = NULL;
    } else {
      view_list->head = NULL;
      view_list->tail = NULL;
    }
    tmp->prev = NULL;
    tmp->next = NULL;
    free(tmp);
    tmp = NULL;
  }
}

void destroyList(List *view_list) {
  while (view_list->head) {
    pop(view_list);
  }
  free(view_list);
}
