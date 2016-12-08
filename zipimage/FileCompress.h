/****************************************************
Author     : Tong ZHAO
Date       : 2016-12-02
Description: The head file of my functions for operating quadtrees with files
****************************************************/

#ifndef FILECOMPRESS_H
#define FILECOMPRESS_H

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
void saveTree(QuadTree<byte>* img_tree, int img_size, const char* name);

// the function to load a quadtree from a txt file
QuadTree<byte>* loadTree(const char* name);

#endif
