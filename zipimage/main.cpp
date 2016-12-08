#include <Imagine/Graphics.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <math.h>
#include "../quadtree/quadtree.h"
#include "TreeCompress.h"
#include "FileCompress.h"

using namespace Imagine;
using namespace std;

#ifndef srcPath
#define srcPath(s) ("." s)
#endif

int main()
{
  /*-----------------------------------------------------------*/
  /*  Test function: encodeImage(first version), restoreImage  */
  // if you want to test the next function, please commend all these codes.

  const char *image_file = (argc > 1) ? argv[1]: srcPath("/running-horse-square.png");
  byte* image;
  int width, height;
  point start_point(0, 0);

  // load image
  loadGreyImage(image_file, image, width, height);
  QuadTree<byte> *quad_image; 

  // encode image
  quad_image = encodeImage(image, start_point, width, width);

  // restore image
  int tree_height = getTreeHeight(quad_image);
  int img_size = (int)pow(2, tree_height - 1);
  byte* re_img = new byte[img_size * img_size];
  restoreImage(quad_image, re_img, start_point, tree_height, img_size);

  delete image;
  delete re_img;
  delete quad_image;

  /*--------------------------------------------------*/
  /*  Test function: quaddag, restoreImageWithSquare  

  const char *image_file = (argc > 1) ? argv[1]: srcPath("/running-horse-square.png");
  byte* image;
  int width, height;
  point start_point(0, 0);

  // load image
  loadGreyImage(image_file, image, width, height);
  QuadTree<byte> *quad_image; 

  // encode image
  quad_image = quaddag(image, start_point, width, width);

  // restore image
  int tree_height = getTreeHeight(quad_image);
  int img_size = (int)pow(2, tree_height - 1);
  byte* re_img = new byte[img_size * img_size];
  restoreImageWithSquare(quad_image, re_img, start_point, tree_height, img_size);

  delete image;
  delete re_img;*/
  
  /*-------------------------------------------------------*/
  /*          Test function: encodeRectImage 

  const char *image_file = (argc > 1) ? argv[1]: srcPath("/running-horse-rect.png");
  byte* image;
  int width, height;
  point start_point(0, 0);

  // load image
  loadGreyImage(image_file, image, width, height);
  QuadTree<byte> *quad_image; 

  // encode image
  quad_image = encodeRectImage(image, start_point, 0, width, height);

  // restore image
  int tree_height = getTreeHeight(quad_image);
  int img_size = (int)pow(2, tree_height - 1);
  byte* re_img = new byte[img_size * img_size];
  restoreImage(quad_image, re_img, start_point, tree_height, img_size);

  delete image;
  delete re_img;
  delete quad_image; */

  /*-------------------------------------------------------*/
  /*   Test function: compressImage, getCompressionRate  

  const char *image_file = (argc > 1) ? argv[1]: srcPath("/lenna_grey.png");
  byte* image;
  int width, height;
  point start_point(0, 0);

  // load image
  loadGreyImage(image_file, image, width, height);
  QuadTree<byte> *quad_image; 

  // encode image
  quad_image = encodeImage(image, start_point, width, width);

  // restore image
  int tree_height = getTreeHeight(quad_image);
  int img_size = (int)pow(2, tree_height - 1);
  byte* re_img = new byte[img_size * img_size];
  restoreImage(quad_image, re_img, start_point, tree_height, img_size);

  // calculate compression rate
  double cmp_rate = getCompressionRate(quad_image, width, height);
  cout << "The compression rate is " << cmp_rate << endl;

  delete image;
  delete re_img;
  delete quad_image;*/

  /*-------------------------------------------------------*/
  /*         Test function: encoderColorImage  

  const char *image_file = (argc > 1) ? argv[1]: srcPath("/lenna.png");
  byte** color_img = new byte* [3];
  int width, height;
  point start_point(0, 0);

  // load image
  loadColorImage(image_file, color_img[0], color_img[1], color_img[2], width, height);
  QuadTree<byte>** color_tree = new QuadTree<byte>* [3]; 

  // encode image
  color_tree = encoderColorImage(color_img, width, height);

  for(int i = 0; i < 3; i++){
    delete color_img[i];
    delete color_tree[i];
  }

  delete color_img;
  delete color_tree;*/

  /*-------------------------------------------------------*/
  /*          Test function: saveTree, loadTree  

  const char *image_file = (argc > 1) ? argv[1]: srcPath("/lenna_grey.png");
  byte* image;
  int width, height;
  point start_point(0, 0);

  // load image
  loadGreyImage(image_file, image, width, height);
  QuadTree<byte> *quad_image; 

  // encode image
  quad_image = encodeImage(image, start_point, width, width);

  // save file
  saveTree(quad_image, width, "lenna_tree.txt");

  // load file
  QuadTree<byte>* file_lena = loadTree("lenna_tree.txt");
  int img_size = (int)pow(2, getTreeHeight(quad_image) - 1);

  byte* re_img = new byte[img_size * img_size];
  restoreImage(quad_image, re_img, start_point, getTreeHeight(quad_image), img_size);


  delete image;
  delete re_img;
  delete quad_image;
  delete file_lena;*/


  return 0;
}
