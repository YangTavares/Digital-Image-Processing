#include <opencv2/opencv.hpp>
#include <bits/stdc++.h>

using namespace cv;
using namespace std;

int main(int argc, char** argv){
  Mat image, mask;
  int width, height;
  int nobjects;
  int nholes;
  bool hole_computed;
  
  CvPoint p;
  image = imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
  
  if(!image.data){
    std::cout << "imagem nao carregou corretamente\n";
    return(-1);
  }
  width=image.size().width;
  height=image.size().height;

  // Remove border objects
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
      if(i==height-1 || i==0 || j==0 || j==width-1){
        if(image.at<uchar>(i,j) == 255){
		  nobjects++;
		  p.x=j;
		  p.y=i;
	  	  floodFill(image,p,0);
	    }
	  }
	}
  }

  p.x=0;
  p.y=0;
  // Make holes to be different with image color
  floodFill(image,p,70);

  // Find objects
  nobjects=0;
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
      if(image.at<uchar>(i,j) == 255){
		// object found
		nobjects++;
		p.x=j;
		p.y=i;
		floodFill(image,p,140);
	  }
	}
  }

  // Find objects with holes
  nholes=0;
  hole_computed=false;
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
      if(image.at<uchar>(i,j) == 210){
        hole_computed=true; 
      }
      else if(image.at<uchar>(i,j) == 70){
        hole_computed=false; 
      }
			if(image.at(i,j) == 0 && !hole_computed){
				nholes++;
				while(image.at(i,j) != 140) j++;
				p.x=j;
				p.y=i;
			  floodFill(image,p,210);
			}
		}
  }
  //70 for background
  //0 for holes
  //210 for objects with found holes
  //140 for objects found
  //255 for unidentified objects

  imshow("image", image);
  imwrite("labeling.png", image);
  waitKey();
  //std::cout << nobjects << '\n';
  std::cout << "Without holes: " << nobjects-nholes << " With holes: " << nholes << '\n';

  return 0;
}
