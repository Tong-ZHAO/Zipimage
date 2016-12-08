/****************************************************
Author     : Tong ZHAO
Date       : 2016-12-02
Description: The head file of my functions for compressing images 
****************************************************/

#ifndef TREECOMPRESS_H
#define TREECOMPRESS_H

#include <Imagine/Graphics.h>
#include <algorithm>
#include <iostream>
#include <math.h>
#include "../quadtree/quadtree.h"

using namespace Imagine;

struct point{
  int px;
  int py;
  point(int x_i, int y_i): px(x_i), py(y_i) {}
};

// the function to encode an image to a quadtree
QuadTree<byte>* encodeImage(byte* img, point p_nw, int window_size, int img_size);

// the function to encode an image to quaddag
QuadTree<byte>* quaddag(byte* img, point p_nw, int window_size, int img_size);

// the function to get a tree's height
int getTreeHeight(QuadTree<byte>* img_tree);

// the function to restore an image from a quadtree
void restoreImage(QuadTree<byte>* img_tree, byte* img, point p_nw, int my_h, int wid);

// the function to restore an image from a quadtree and draw bounding squares for its leaves
void restoreImageWithSquare(QuadTree<byte>* img_tree, byte* img, point p_nw, int my_h, int wid);

// the function to print a quadtree's compression rate
double getCompressionRate(QuadTree<byte>* img_tree, int wid, int hei);

// the function to encode a rectangle image to a quadtree
QuadTree<byte>* encodeRectImage(byte* img, point p_nw, int window_size, int img_wid, int img_hei);

// the function to get the difference among four integers
int getDiff(int value_1, int value_2, int value_3, int value_4);

// the function to encode an image to a lossy quadtree with a threshold 
QuadTree<byte>* compressImage(byte* img, point p_nw, int window_size, int img_size, int threshold);

// the function to load a color image
QuadTree<byte>** encoderColorImage(byte** img, int img_wid, int img_hei);

#endif
