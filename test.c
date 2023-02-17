#include <check.h>

#include "viewer.h"

List *nodes_static_list;
List *nodes_dynamic_list;
List *surfaces;
int nodeAmount;
int edgeAmount;
double *borderlands, *borderlands_2, **nodes_static, **nodes_dynamic;

START_TEST(test_loading_gone_wrong) {
  char res = loadFromFile("./", NULL, NULL, NULL, NULL, NULL, NULL, NULL);
  ck_assert_int_eq(res, -1);
}
END_TEST

START_TEST(test_loading_gone_right) {
  char res = loadFromFile("./test.obj", nodes_static_list, nodes_dynamic_list,
                          surfaces, &nodeAmount, &edgeAmount, borderlands_2,
                          borderlands);
  ck_assert_int_eq(res, 0);

  destroyList(surfaces);

  nodes_static = malloc(nodeAmount * sizeof(double *));
  nodes_dynamic = malloc(nodeAmount * sizeof(double *));

  int j = 0;

  for (Node *tmp = nodes_static_list->head, *tmp2 = nodes_dynamic_list->head;
       tmp; tmp = tmp->next, tmp2 = tmp2->next, j++) {
    nodes_static[j] = tmp->coords;
    nodes_dynamic[j] = tmp2->coords;
  }

  destroyList(nodes_static_list);
  destroyList(nodes_dynamic_list);

  adjustTranslate(nodes_static, nodes_dynamic, nodeAmount, 5.0, -2.0, 0.0,
                  borderlands_2, borderlands);
  adjustRotate(nodes_dynamic, nodeAmount, 5.0, -2.0, 0.0);
  adjustScale(nodes_dynamic, nodeAmount, -6.6);
  adjustScale(nodes_dynamic, nodeAmount, 6.6);

  for (int i = 0; i < nodeAmount; i++) {
    free(nodes_static[i]);
    free(nodes_dynamic[i]);
  }
  free(nodes_static);
  free(nodes_dynamic);
  free(borderlands);
  free(borderlands_2);
}
END_TEST

Suite *view_test_suite(void) {
  Suite *s;
  TCase *tc_view;
  s = suite_create("Testing in progress");
  tc_view = tcase_create("Computing...");
  tcase_add_test(tc_view, test_loading_gone_wrong);
  tcase_add_test(tc_view, test_loading_gone_right);
  suite_add_tcase(s, tc_view);
  return s;
}

int main(void) {
  nodes_static_list = malloc(sizeof(List));
  nodes_dynamic_list = malloc(sizeof(List));
  surfaces = malloc(sizeof(List));
  nodes_static_list->head = NULL;
  nodes_static_list->tail = NULL;
  nodes_dynamic_list->head = NULL;
  nodes_dynamic_list->tail = NULL;
  surfaces->head = NULL;
  surfaces->tail = NULL;
  borderlands = malloc(6 * sizeof(double));
  borderlands_2 = malloc(6 * sizeof(double));

  int number_failed;
  Suite *s = view_test_suite();
  SRunner *sr = srunner_create(s);
  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
