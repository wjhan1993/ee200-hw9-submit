
#include "mon.h"
#include <stdlib.h>
#include <string.h>
int main(int argc, char* argv[])
{
  // create 5 tree elements
  Element z = { .name = "zebra", .left_child = NULL, .right_child = NULL };
  Element y = { .name = "yak", .left_child = NULL, .right_child = NULL };
  Element x = { .name = "xylophone", .left_child = NULL, .right_child = NULL };
  Element b = { .name = "baboon", .left_child = &x, .right_child = &y };
  Element a = { .name = "aardvark", .left_child = &b, .right_child = &z };
 
  // test the function: write_tree
  // test.mon should be the same as sample.mon  
  FILE* wf = fopen("test.mon", "w");
  write_tree(wf, &a);
  fclose(wf);
  
  // test the function load_tree and get the root pointer
  FILE * rf = fopen("test.mon", "r");
  Element* root = load_tree(rf);
  
  // Use the root pointer returned from load_tree to generate .mon file again
  // test_copy.mon should be exactly the same as test.mon and
  // load_tree works well
  FILE * wf_copy = fopen("test_copy.mon", "w");
  write_tree(wf_copy,root);
  
  // free_tree: free the tree memory 
  free_tree(root);
  fclose(rf);
  fclose(wf_copy);
  
  // Use create_left and create_right to build a same tree as before
  root = (struct element_t *)malloc(sizeof(struct element_t));
  root->left_child = NULL;
  root->right_child = NULL;
  root->name = (char*)malloc(500);
  strcpy(root->name,"aardvark");
  Element * B = create_left(root,"baboon");
  Element * X = create_left(B,"xylophone");
  Element * Y = create_right(B,"yak");
  Element * Z = create_right(root,"zebra");

  // test write_tree: test_recons.mon should be same as sample.mon
  FILE * wf_recons = fopen("test_recons.mon","w");
  write_tree(wf_recons,root);
  
  // free tree memory
  free_tree(root);
  fclose(wf_recons);
}

