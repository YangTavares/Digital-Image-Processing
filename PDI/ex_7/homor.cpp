#include <iostream>
#include <opencv2/opencv.hpp>
#include <math.h>

using namespace cv;
using namespace std;

double alfa;
int alfa_slider = 0;
int alfa_slider_max = 100;

int top_slider = 0;
int top_slider_max = 100;

int heigh_slider = 0;
int heigh_slider_max = 100;

Mat image1, image2, blended;
Mat temp1,temp2;
Mat frame32f, frameFiltered;
Mat kernel;

char TrackbarName[50];


void on_trackbar_blend(int, void*){

	image2 = image1.clone();

  flip(image2, image2, 1);


	int kernel_size = 20;

	kernel = Mat::ones( kernel_size, kernel_size, CV_32F )/ (float)(kernel_size*kernel_size);
  filter2D(image2, frameFiltered,
		image2.depth(), kernel, Point(1,1), 0);

	image2 = frameFiltered.clone();

  int h = image1.size().height;
	int w = image1.size().width*3; //RGB
	temp1 = image1.clone();
	temp2 = image2.clone();
  for(int i=0; i<h; i++){
		double d = (double) heigh_slider+1;
		double l1 = (double) top_slider;
		double c = (double) alfa_slider*255.0/100.0;

		double res = ((tanh((i-c+l1)/d)-tanh((i-c-l1)/d))/2.0);
		printf("res %f\n",res);
		int counter = 0;
    for(int j=0; j<w; j++){
			counter++;
			temp1.at<uchar>(i,j) = (unsigned char)((res)*image1.at<uchar>(i,j));
			temp2.at<uchar>(i,j) = (unsigned char)((1.0-res)*image2.at<uchar>(i,j));
		}
		printf("counter: %d\n",counter);
	}
	printf("w: %d\n",w);
	printf("h: %d\n",h);
	
 addWeighted( temp1, 1.0, temp2, 1.0, 0.0, blended);
 imshow("addweighted", blended);
}

void on_trackbar_line(int, void*){
  on_trackbar_blend(alfa_slider,0);
}
void on_trackbar_heigh(int, void*){
  on_trackbar_blend(alfa_slider,0);
}

int main(int argvc, char** argv){
  image1 = imread("blend1.jpg");
  //image2 = imread("blend2.jpg");
  namedWindow("addweighted", 1);

  sprintf( TrackbarName, "Center y %d", alfa_slider_max );
  createTrackbar( TrackbarName, "addweighted",
				  &alfa_slider,
				  alfa_slider_max,
				  on_trackbar_blend );
  on_trackbar_blend(alfa_slider, 0 );

  sprintf( TrackbarName, "Height y %d", top_slider_max );
  createTrackbar( TrackbarName, "addweighted",
				  &top_slider,
				  top_slider_max,
				  on_trackbar_line );
  on_trackbar_line(top_slider, 0 );

  sprintf( TrackbarName, "Decay y %d", heigh_slider_max );
  createTrackbar( TrackbarName, "addweighted",
				  &heigh_slider,
				  heigh_slider_max,
				  on_trackbar_heigh );
  on_trackbar_heigh(heigh_slider, 0 );

  waitKey(0);
  return 0;
}
