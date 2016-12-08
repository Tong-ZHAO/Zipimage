/****************************************************
Author     : Tong ZHAO
Date       : 2016-12-02
Description: The cpp file of my functions for compressing images 
****************************************************/

#include <Imagine/Graphics.h>
#include <algorithm>
#include <iostream>
#include <math.h>
#include "../quadtree/quadtree.h"
#include "TreeCompress.h"

using namespace Imagine;
using namespace std;

QuadLeaf<byte>* leaf_white = new QuadLeaf<byte>((byte)255);
QuadLeaf<byte>* leaf_black = new QuadLeaf<byte>((byte)0);


// the function to encode an image to a quadtree
QuadTree<byte>* encodeImage(byte* img, point p_nw, int window_size, int img_size)
{
  // arrive at a leaf
  if(window_size == 1)
    return new QuadLeaf<byte>(img[p_nw.px * img_size + p_nw.py]);
  else{
    QuadTree<byte> *q_nw, *q_ne, *q_se, *q_sw;
    // divide into four parts and calculate their start points
    point p_ne(p_nw.px, p_nw.py + window_size / 2), 
          p_se(p_nw.px + window_size / 2, p_nw.py + window_size / 2), 
          p_sw(p_nw.px + window_size / 2, p_nw.py);

    // call the function recursively
    q_nw = encodeImage(img, p_nw, window_size / 2, img_size);
    q_ne = encodeImage(img, p_ne, window_size / 2, img_size);
    q_se = encodeImage(img, p_se, window_size / 2, img_size);
    q_sw = encodeImage(img, p_sw, window_size / 2, img_size);

    // if all its sons are leaves
    if(q_nw->isLeaf() && q_ne->isLeaf() && q_sw->isLeaf() && q_se->isLeaf()){
      bool q_eqal = true;
      int q_value[nbQuadDir] = {q_nw->value(),
                                q_ne->value(),
                                q_se->value(),
                                q_sw->value()};
      // check value
      for(int j = 1; j < nbQuadDir; j++)
        if(q_value[j] != q_value[0]){
          q_eqal = false;
          break;
        }

      // merge nodes
      if(q_eqal == true){
        delete q_ne, q_se, q_sw;
        return q_nw;
      }
      else return new QuadNode<byte>(q_nw, q_ne, q_se, q_sw);
    }
    else return new QuadNode<byte>(q_nw, q_ne, q_se, q_sw);
  }
}

// the function to encode an image to quaddag
QuadTree<byte>* quaddag(byte* img, point p_nw, int window_size, int img_size)
{
  // return a leaf according to its value
  if(window_size == 1)
    return ((int)img[p_nw.px * img_size + p_nw.py] == 0) ? leaf_black : leaf_white;
  else{
    QuadTree<byte> *q_nw, *q_ne, *q_se, *q_sw;
    point p_ne(p_nw.px, p_nw.py + window_size / 2), 
          p_se(p_nw.px + window_size / 2, p_nw.py + window_size / 2), 
          p_sw(p_nw.px + window_size / 2, p_nw.py);

    q_nw = quaddag(img, p_nw, window_size / 2, img_size);
    q_ne = quaddag(img, p_ne, window_size / 2, img_size);
    q_se = quaddag(img, p_se, window_size / 2, img_size);
    q_sw = quaddag(img, p_sw, window_size / 2, img_size);

    if(q_nw->isLeaf() && q_ne->isLeaf() && q_sw->isLeaf() && q_se->isLeaf()){
      bool q_eqal = true;
      int q_value[nbQuadDir] = {q_nw->value(),
                                q_ne->value(),
                                q_se->value(),
                                q_sw->value()};
      for(int j = 1; j < nbQuadDir; j++){
        if(q_value[j] != q_value[0]){
          q_eqal = false;
          break;
        }
      }

      if(q_eqal == true) return q_nw;
      else return new QuadNode<byte>(q_nw, q_ne, q_se, q_sw);
    }
    else return new QuadNode<byte>(q_nw, q_ne, q_se, q_sw);
  }
}

// the function to get a tree's height
int getTreeHeight(QuadTree<byte>* img_tree)
{
  if(img_tree->isLeaf()) return 1;
  else 
    return max(getTreeHeight(img_tree->son(NW)),
             max(getTreeHeight(img_tree->son(NE)),
               max(getTreeHeight(img_tree->son(SW)),
                   getTreeHeight(img_tree->son(SE))))) + 1;
}

// the function to restore an image from a quadtree
void restoreImage(QuadTree<byte>* img_tree, byte* img, point p_nw, int my_h, int wid)
{
  if(img_tree->isLeaf())
    for(int i = 0; i < (int)pow(2, my_h - 1); i++)
      for(int j = 0; j < (int)pow(2, my_h - 1); j++)
        img[(p_nw.px + i) * wid + p_nw.py + j] = img_tree->value();
  else{
    int window_size = (int)pow(2, my_h - 2);
    point p_ne(p_nw.px, p_nw.py + window_size),
          p_se(p_nw.px + window_size, p_nw.py + window_size),
          p_sw(p_nw.px + window_size, p_nw.py);

    restoreImage(img_tree->son(0), img, p_nw, my_h - 1, wid);
    restoreImage(img_tree->son(1), img, p_ne, my_h - 1, wid);
    restoreImage(img_tree->son(2), img, p_se, my_h - 1, wid);
    restoreImage(img_tree->son(3), img, p_sw, my_h - 1, wid);
  }

  // show the image at the end
  if((int)pow(2, my_h - 1) == wid){
    Window window = openWindow(wid, wid);
    putGreyImage(IntPoint2(0,0), img, wid, wid);
    click();
  }
}

// the function to restore an image from a quadtree and draw bounding squares for its leaves
void restoreImageWithSquare(QuadTree<byte>* img_tree, byte* img, point p_nw, int my_h, int wid)
{
  // calculate the window's edge length
  int my_size = (int)pow(2, my_h - 1);
  if(img_tree->isLeaf()){
    if(my_h == 1) img[p_nw.px * wid + p_nw.py] = (byte)img_tree->value();
    else{
      // draw bounding squares
      for(int m = 0; m < my_size; m++) {
        img[p_nw.px * wid + p_nw.py + m] = (byte)128;
        img[(p_nw.px + my_size - 1) * wid + p_nw.py + m] = (byte)128;
      }
      for(int n = 0; n < my_size; n++) {
        img[(p_nw.px + n) * wid + p_nw.py] = (byte)128;
        img[(p_nw.px + n) * wid + p_nw.py + my_size - 1] = (byte)128;
      }
      // fill inside
      if(my_size > 2)
        for(int i = 1; i < (int)pow(2, my_h - 1) - 1; i++)
          for(int j = 1; j < (int)pow(2, my_h - 1) - 1; j++)
            img[(p_nw.px + i) * wid + p_nw.py + j] = img_tree->value();
    }
  }
  else{
    int window_size = my_size / 2;
    point p_ne(p_nw.px, p_nw.py + window_size),
          p_se(p_nw.px + window_size, p_nw.py + window_size),
          p_sw(p_nw.px + window_size, p_nw.py);

    restoreImageWithSquare(img_tree->son(0), img, p_nw, my_h - 1, wid);
    restoreImageWithSquare(img_tree->son(1), img, p_ne, my_h - 1, wid);
    restoreImageWithSquare(img_tree->son(2), img, p_se, my_h - 1, wid);
    restoreImageWithSquare(img_tree->son(3), img, p_sw, my_h - 1, wid);
  }

  // show image
  if((int)pow(2, my_h - 1) == wid){
    Window window = openWindow(wid, wid);
    putGreyImage(IntPoint2(0,0), img, wid, wid);
    click();
  }
}

// the function to print a quadtree's compression rate
double getCompressionRate(QuadTree<byte>* img_tree, int wid, int hei)
{
  int nb_nodes = img_tree->nbNodes();
  cout << "Leaves: " << img_tree->nbLeaves() << endl;
  cout << "Nodes: " << nb_nodes << endl;
  return nb_nodes / (double)(wid * hei);
}

// the function to encode a rectangle image to a quadtree
QuadTree<byte>* encodeRectImage(byte* img, point p_nw, int window_size, int img_wid, int img_hei)
{
  if(window_size == 0){
    int min_size = 1;
    int max_edge = max(img_wid, img_hei);
    for(min_size; min_size < max_edge; min_size *= 2);
    window_size = min_size;
  }
  
  if(window_size == 1){
    if(p_nw.px < img_hei && p_nw.py < img_wid)
      return new QuadLeaf<byte>(img[p_nw.px * img_wid + p_nw.py]);
    else
      return new QuadLeaf<byte>((byte)255);   // define the point out of the image is white
  }
  else{
    QuadTree<byte> *q_nw, *q_ne, *q_se, *q_sw;
    point p_ne(p_nw.px, p_nw.py + window_size / 2), 
          p_se(p_nw.px + window_size / 2, p_nw.py + window_size / 2), 
          p_sw(p_nw.px + window_size / 2, p_nw.py);

    q_nw = encodeRectImage(img, p_nw, window_size / 2, img_wid, img_hei);
    q_ne = encodeRectImage(img, p_ne, window_size / 2, img_wid, img_hei);
    q_se = encodeRectImage(img, p_se, window_size / 2, img_wid, img_hei);
    q_sw = encodeRectImage(img, p_sw, window_size / 2, img_wid, img_hei);

    if(q_nw->isLeaf() && q_ne->isLeaf() && q_sw->isLeaf() && q_se->isLeaf()){
      bool q_eqal = true;
      int q_value[nbQuadDir] = {q_nw->value(),
                                q_ne->value(),
                                q_se->value(),
                                q_sw->value()};
      for(int j = 1; j < nbQuadDir; j++){
        if(q_value[j] != q_value[0]){
          q_eqal = false;
          break;
        }
      }

      if(q_eqal == true){
        delete q_ne, q_se, q_sw;
        return q_nw;
      }
      else return new QuadNode<byte>(q_nw, q_ne, q_se, q_sw);
    }
    else return new QuadNode<byte>(q_nw, q_ne, q_se, q_sw);
  }
}

// the function to get the difference among four integers
int getDiff(int value_1, int value_2, int value_3, int value_4)
{
  int min_value = min(value_1,
                      min(value_2,
                          min(value_3,
                              value_4)));
  int max_value = max(value_1,
                      max(value_2,
                          max(value_3,
                              value_4)));
  return (max_value - min_value);
}

// the function to encode an image to a lossy quadtree with a threshold 
QuadTree<byte>* compressImage(byte* img, point p_nw, int window_size, int img_size, int threshold)
{
  if(window_size == 1)
    return new QuadLeaf<byte>(img[p_nw.px * img_size + p_nw.py]);
  else{
    QuadTree<byte> *q_nw, *q_ne, *q_se, *q_sw;
    point p_ne(p_nw.px, p_nw.py + window_size / 2), 
          p_se(p_nw.px + window_size / 2, p_nw.py + window_size / 2), 
          p_sw(p_nw.px + window_size / 2, p_nw.py);

    q_nw = compressImage(img, p_nw, window_size / 2, img_size, threshold);
    q_ne = compressImage(img, p_ne, window_size / 2, img_size, threshold);
    q_se = compressImage(img, p_se, window_size / 2, img_size, threshold);
    q_sw = compressImage(img, p_sw, window_size / 2, img_size, threshold);

    if(q_nw->isLeaf() && q_ne->isLeaf() && q_sw->isLeaf() && q_se->isLeaf()){
      int q_value[nbQuadDir] = {q_nw->value(),
                                q_ne->value(),
                                q_se->value(),
                                q_sw->value()};
      // calculate difference value
      int diff = getDiff(q_value[0], q_value[1], q_value[2], q_value[3]);
      if(diff < threshold){
         delete q_nw, q_ne, q_se, q_sw;
         return new QuadLeaf<byte>((byte)((q_value[0] + q_value[1] + q_value[2] + q_value[3]) / 4));
      }
      else return new QuadNode<byte>(q_nw, q_ne, q_se, q_sw);
    }
    else return new QuadNode<byte>(q_nw, q_ne, q_se, q_sw);
  }
}

// the function to load a color image
QuadTree<byte>** encoderColorImage(byte** img, int img_wid, int img_hei)
{
  QuadTree<byte>** color_tree = new QuadTree<byte>* [3];
  point start_point(0, 0);
  color_tree[0] = encodeRectImage(img[0], start_point, 0, img_wid, img_hei);
  color_tree[1] = encodeRectImage(img[1], start_point, 0, img_wid, img_hei);
  color_tree[2] = encodeRectImage(img[2], start_point, 0, img_wid, img_hei);
  
  return color_tree;
}
