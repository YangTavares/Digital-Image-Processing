#include <opencv2/opencv.hpp>
#include <bits/stdc++.h>

using namespace cv;
using namespace std;

int main(int argc, char** argv){
  Mat image, mask;
  int width, height;
  int nobjects;
  
  CvPoint p;
  image = imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
  
  if(!image.data){
    std::cout << "imagem nao carregou corretamente\n";
    return(-1);
  }
  width=image.size().width;
  height=image.size().height;

  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
      if(i==height-1 || i==0 || j==0 || j==width-1){
        if(image.at<uchar>(i,j) == 255){
		  // achou um objeto
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

  // busca objetos com buracos presentes
  nobjects=0;
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
      if(image.at<uchar>(i,j) == 255){
		// achou um objeto
		nobjects++;
		p.x=j;
		p.y=i;
		floodFill(image,p,200);
	  }
	}
  }

  Mat canny_output;
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;
  //Canny(image,canny_ouput,100,200,3);
  findContours(image, contours, hierarchy,  RETR_TREE, CHAIN_APPROX_NONE, Point(0, 0)); 
  int num=0;
  for(int i=0;i<contours.size(); i++){
     if(hierarchy[i][1]<0){ 
	   num++;
	   Rect r = boundingRect(contours[i]);
	   rectangle(image,Point(r.x,r.y), Point(r.x+r.width,r.y+r.height), Scalar(0,0,255),1,8,0);
	 }
  }
  //std::cout << num << '\n';

  imshow("image", image);
  imwrite("labeling.png", image);
  waitKey();
  //std::cout << nobjects << '\n';
  std::cout << "Without holes: " << nobjects-num << " With holes: " << num << '\n';
  return 0;
}
