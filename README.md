# Image Processing
An application that places different filters and functions on any image. There are 2 functions(copy, scale) and 3 filters(greyscale, edge detection, gaussian blur). The sample image, dog.bmp, is what we will use to apply the filters and functions, and the new transformed image will be created in the images folder.

## Functions
 * Copy: Creates an exact copy of the sample image.
 * Scale: Scales the image dimensions by a specified scale factor.

## Filters
 * greyscale: Creates a black and white image.
  > ![splashpage](dog.bmp) 
  > ![splashpage](/images/dog_grey.bmp)
 * edge detection: Finds the boundaries of objects within the image.
  > ![splashpage](dog.bmp)
  > ![splashpage](/images/dog_edge.bmp)
 * gaussian blur: Blurs the image by a Gaussian function. 
  >![splashpage](dog.bmp)
  >![splashpage](/images/dog_blur.bmp)

## How to use:
Download the repo in your local directory, and run the Makefile with following commands:
```
make clean
```
Always run make clean first to clean images folder and executables. 
```
make all
```
Make all will create the executables of each filter/function.
```
make test
```
Make test will create a folder named images, and create new images(as specified in the Makefile):
* Create a copy of dog.bmp, called dog_copy.bmp
* Apply greyscale to dog.bmp, called dog_grey.bmp
* Scale dog.bmp by a factor of 2, called dog_scale.bmp
* Apply Gaussian blur to dog.bmp, called dog_blur.bmp
* Apply edge detection to dog.bmp, called dog_edge.bmp
	
