/****************************************************
Author     : Tong ZHAO
Date       : 2016-12-02
Description: The cpp file of my functions for operating quadtrees with files
****************************************************/

#include <Imagine/Graphics.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <math.h>
#include "../quadtree/quadtree.h"
#include "TreeCompress.h"

using namespace std;
using namespace Imagine;

// the function to save a quadtree to a txt file
void saveTree(QuadTree<byte>* img_tree, int img_size, const char* name)
{
  freopen(name, "w", stdout);
  cout << "img_size:" << img_size << endl;
  cout << "tree_height:" << getTreeHeight(img_tree) << endl;
  display(img_tree);
  fclose(stdout);    
}

// the function to load a quadtree from a txt file
QuadTree<byte>* loadTree(const char* name)
{
  string line;
  ifstream infile(name);
   
  int img_size = 0, tree_height = 0;

  // get image size
  getline(infile,line);
  int i = 0;
  while(i < line.length() && line[i] != ':') i++;
  while(i < line.length() - 1) img_size = img_size * 10 + line[++i] - '0';

  // get tree height
  getline(infile,line);
  i = 0;
  while(i < line.length() && line[i] != ':') i++;
  while(i < line.length() - 1) tree_height = tree_height * 10 + line[++i] - '0';

  // cout << img_size << "   " << tree_height << endl;
 
  // load tree nodes
  vector<QuadTree<byte>*> tree_heap; 
  char direction[2] = {'-', '-'};

  if(infile.is_open()){
    while(getline(infile, line)){
      vector<int> path;  // a local variable to store the path for each line
      for(int j = 1; j < line.length(); j++){
        if(line[j] == 'N' || line[j] == 'S') direction[0] = line[j];
        else if(line[j] == 'W' || line[j] == 'E') direction[1] = line[j];
        // calculate leaf value
        else if(line[j] == ' '){
          int leaf_value = 0;
          for(j = j + 3; j < line.length(); j++) leaf_value = leaf_value * 10 + line[j] - '0';
          tree_heap.push_back(new QuadLeaf<byte>(leaf_value));
          // the last son of its parent
          if(direction[0] == 'S' && direction[1] == 'W'){
            int m = path.size();
            do{
              m--;
              int tree_size = tree_heap.size();
              QuadTree<byte>* new_leaf = new QuadNode<byte>(tree_heap[tree_size - 4],
                                                            tree_heap[tree_size - 3],
                                                            tree_heap[tree_size - 2],
                                                            tree_heap[tree_size - 1]);
              for(int n = 0; n < 4; n++) tree_heap.pop_back();
              tree_heap.push_back(new_leaf);
            } while(m > 0 && path[m] == 3);
          }
        }
        // save path
        else if(line[j] == '-'){
          if(direction[0] == 'N' && direction[1] == 'W')      path.push_back(0);
          else if(direction[0] == 'N' && direction[1] == 'E') path.push_back(1);
          else if(direction[0] == 'S' && direction[1] == 'E') path.push_back(2);
          else if(direction[0] == 'S' && direction[1] == 'W') path.push_back(3);
        }
      }
    }
  }
  // check tree
  if(tree_heap.size() == 1) return tree_heap[0];
  else return nullptr; 
}
