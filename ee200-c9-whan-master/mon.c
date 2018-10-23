#include "mon.h"
#include <stdlib.h>
#include <string.h>

// Create a new Element with the string in data and add it as the 
// left child to the Element root.
Element* create_left(Element* root, char* data)
{
    struct element_t *left = NULL;
    
    // if root has no left_child, add one element with name = data 
    // as the left child and return a pointer to the new inserion.
    // if root has left child, return NULL.
    if (root->left_child == NULL)
    {
        left = (struct element_t *)malloc(sizeof(struct element_t));
        left->left_child = NULL;
        left->right_child = NULL;
        left->name = (char*)malloc(500);
        strcpy(left->name,data);
	root->left_child = left;
    }
    return left;
}

// Create a new Element with the string in data and add it as the 
// right child to the Element root.
Element* create_right(Element* root, char* data)
{
    struct element_t *right = NULL;

    // if root has no right_child, add one element with name = data 
    // as the right child and return a pointer to the new inserion.
    // if root has right child, return NULL.
    if (root->right_child == NULL)
    {
      right = (struct element_t *)malloc(sizeof(struct element_t));
      right->left_child = NULL;
      right->right_child = NULL;
      right->name = (char*)malloc(500);
      strcpy(right->name,data);
      root->right_child = right;
    }
  return right;
}

// free all of the memory allocated for an element and all of its children
void free_tree(Element* root)
{
    // if the tree is emtpy, do nothing. 
    if (root == NULL)
        return;

    // if the root has no children, free itself
    if (root->left_child == NULL && root->right_child == NULL)
    {   
        free(root->name);
        free(root);
        return;
    }

    // recursion: free the left subtree, right subtree and then the root itself
    free_tree(root->left_child);
    free_tree(root->right_child);
    free(root->name);
    free(root);
}

// Writes a tree to a file
int write_tree(FILE* f, Element* e)
{   
    // if Element is NULL, print NULL to the file
    if (e == NULL)
    {
        int flag = fprintf(f,"%s ","NULL");
        // if the function is successful, return 0 else return a negative number
        if (flag)
	  return 0;
        else 
          return -1;
    }
    
    // if the element has no children, print its name and pointers inside "{ }"
    if (e->left_child == NULL && e->right_child ==  NULL)
    {
	int flag = fprintf(f,"{NULL NULL %s}",e->name);
        
	if (flag)
	    return 0;
	else
	    return -1;
    }

    // recursion: print the left subtree first, then the right subtree 
    // and finally the root itself
    fprintf(f,"%s","{");

    int left = write_tree(f,e->left_child);
    int right = write_tree(f,e->right_child);

    fprintf(f,"%s}",e->name);

    if (left == -1 || right == -1)
      return -1;
    else
      return 0;
}

Element* load_tree(FILE* f)
{
    // This function can be done recursively, while because of the time limit
    // here I give a more complex solution and I hope to submit a different
    // version later.
 
    // read the FILE f into mystring 
    Element * root = NULL;
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);  

    char *mystring = malloc(fsize + 1);
    fread(mystring, fsize, 1, f);

    // Check how many elements in the tree
    int i,j,nodeNum = 0,count = 0;
    char start = '{', end = '}';
    for (i = 0;i < fsize;i++)
    {
      if (mystring[i] == start)
	nodeNum++;
    }
    
    // record the index of mystring where an element starts and ends
    int nodeBreaks[2 * nodeNum];
    for (i = 0;i < fsize;i++)
    {
      // starts index are positive 
      if (mystring[i] == start)
	nodeBreaks[count++] = i + 1;

      // ends index are negative
      if (mystring[i] == end)
	nodeBreaks[count++] = -i - 1;  
    }
    
    // allNodes stores pointers to all elements
    struct element_t *allNodes[nodeNum];
    char elementName[500];

    // memory allocation for all elements
    for (i = 0;i < nodeNum;i++)
        allNodes[i] = (struct element_t *)malloc(sizeof(struct element_t));
    
    // nodeChild stores how many childs the element has 
    // 'n': no children, 'l': only left child, 'r': only right child
    // 'b': both right and left child
    char nodeChild[nodeNum];
    count = 0;
    for(i = 0;i < 2 * nodeNum;i++)
    {
      // when meet a end '{', search forward for the nearest start '}'
      if (nodeBreaks[i] < 0)
      {
	for (j = i;j >=0;j--)
	{
	  if (nodeBreaks[j] > 0)
	    break;
	}

        // nodeStartIndex and nodeEndIndex are the indices of '{' and '}'
        // of the current element in mystring
        int nodeStartIndex = nodeBreaks[j] - 1;
	int nodeEndIndex = -nodeBreaks[i] - 1;
        int n = nodeEndIndex - 1;

	// element's name is just before the end '}'
        while (mystring[n] != ' ' && mystring[n] != '}')
	    n--;

        // nameLength is length of the name 
	int nameLength = nodeEndIndex - n, nameChar = 0;
      
	for (++n;n < nodeEndIndex;n++)
	  elementName[nameChar++] = mystring[n];
         
        elementName[nameChar] = '\0';
        allNodes[count]->name = (char*)malloc(nodeNum);
        strcpy(allNodes[count]->name, elementName);

        // check the children info
	n = n - nameLength;
	int leftChild = 0, rightChild = 0;
        
        // determine whether the element has rightChild or leftChild
	if(mystring[n] == end)
	   rightChild = 1;
	if (mystring[nodeStartIndex + 1] == start)
	   leftChild = 1;
        
	nodeChild[count] = 'n';
	if (rightChild && leftChild)
	  nodeChild[count] = 'b';
	if (rightChild && !leftChild)
	  nodeChild[count] = 'r';
	if (!rightChild && leftChild)
	  nodeChild[count] = 'l';

	count++;
	nodeBreaks[i] = 0;
	nodeBreaks[j] = 0;
      }
    }
    
    // construct the tree
    for (i = nodeNum - 1;i >= 0;i--)
    {
      switch(nodeChild[i])
        {
          // element has two children
	  case 'b':
	    allNodes[i]->left_child = allNodes[i - 2];
	    allNodes[i]->right_child = allNodes[i - 1];
	    break;
	  // element has only left child
	  case 'l':
	    allNodes[i]->left_child = allNodes[i - 1];
	    allNodes[i]->right_child = NULL;
	    break;
	  // element has only right child 
	  case 'r':
	    allNodes[i]->left_child = NULL;
	    allNodes[i]->right_child = allNodes[i - 1];
	    break;
	  // element has no children
	  case 'n':
	    allNodes[i]->left_child = NULL;
	    allNodes[i]->right_child = NULL;
	    break;
	}      
    }

    // root is the last node inside allNodes because of the post-order
    // recursion of write_tree
    root = allNodes[nodeNum - 1];
    free(mystring);
    return root;
}

