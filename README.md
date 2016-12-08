# Zipimage

This is a library for image compression. 

## Requirement

* cmake ( >= 2.6)
* g++
* Imagine++

## Run Exemple

```
$sh zipimage/run.sh
```

## Function

#### TreeCompress

* __encodeImage__ : the function to encode a grey image to a quadtree
* __quaddag__: the function to encode a grey image to quaddag (a quadtree sharing its leaves)
* __restoreImage__: the function to restore agrey image from a quadtree
* __restoreImageWithSquare__: the function to restore a grey image from a quadtree and draw bounding squares for its leaves
* __getCompressionRate__: the function to print a quadtree's compression rate
* __encodeRectImage__: the function to encode a rectangle grey image to a quadtree
* __compressImage__:  the function to encode a grey image to a lossy quadtree with a threshold
* __encoderColorImage__:  the function to load a color image

#### FileCompress

* __saveTree__: the function to save a quadtree to a txt file
* __loadTree__: the function to load a quadtree from a txt file
