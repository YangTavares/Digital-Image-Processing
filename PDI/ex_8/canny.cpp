#include <iostream>
#include "opencv2/opencv.hpp"
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ctime>
#include <cstdlib>

using namespace std;
using namespace cv;

#define STEP 3
#define JITTER 2
#define RAIO 3

int top_slider = 57;
int top_slider_max = 200;

char TrackbarName[50];

Mat image, border;

void on_trackbar_canny(int, void*){
  Canny(image, border, top_slider, 3*top_slider);
  imshow("canny", border);
  vector<int> yrange;
  vector<int> xrange;

  Mat frame, points;

  int width, height, gray;
  int x, y;

  srand(time(0));

  width=image.size().width;
  height=image.size().height;


  points = Mat(height, width, CV_8U, Scalar(255));

  xrange.resize(height/STEP);
  yrange.resize(width/STEP);

  iota(xrange.begin(), xrange.end(), 0);
  iota(yrange.begin(), yrange.end(), 0);

  for(uint i=0; i<xrange.size(); i++){
    xrange[i]= xrange[i]*STEP+STEP/2;
  }

  for(uint i=0; i<yrange.size(); i++){
    yrange[i]= yrange[i]*STEP+STEP/2;
  }

  random_shuffle(xrange.begin(), xrange.end());


  for(auto i : xrange){
    random_shuffle(yrange.begin(), yrange.end());
    for(auto j : yrange){
			//if(border.at<uchar>(i,j)!=0){
      x = i+rand()%(2*JITTER)-JITTER+1;
      y = j+rand()%(2*JITTER)-JITTER+1;
      gray = image.at<uchar>(x,y);
      circle(points,
             cv::Point(y,x),
             RAIO,
             CV_RGB(gray,gray,gray),
             -1,
             CV_AA);
    }
  }

  imwrite("pontos_bef.jpg", points);

	int NSTEP = 1;
	int NJITTER = 1;
	int NRAIO = 1;

  xrange.resize(height/NSTEP);
  yrange.resize(width/NSTEP);

  iota(xrange.begin(), xrange.end(), 0);
  iota(yrange.begin(), yrange.end(), 0);

  for(uint i=0; i<xrange.size(); i++){
    xrange[i]= xrange[i]*NSTEP+NSTEP/2;
  }

  for(uint i=0; i<yrange.size(); i++){
    yrange[i]= yrange[i]*NSTEP+NSTEP/2;
  }

  random_shuffle(xrange.begin(), xrange.end());


  for(auto i : xrange){
    random_shuffle(yrange.begin(), yrange.end());
    for(auto j : yrange){
			if(border.at<uchar>(i,j)!=0){
				x = i+rand()%(2*NJITTER)-NJITTER+1;
				y = j+rand()%(2*NJITTER)-NJITTER+1;
				gray = image.at<uchar>(x,y);
				circle(points,
							 cv::Point(y,x),
							 NRAIO,
							 CV_RGB(gray,gray,gray),
							 -1,
							 CV_AA);
			}
    }
  }
  imwrite("pontos.jpg", points);
}

int main(int argc, char**argv){
  int width, height;

  image= imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);

  width=image.size().width;
  height=image.size().height;

  sprintf( TrackbarName, "Threshold inferior", top_slider_max );

  namedWindow("canny",1);
  createTrackbar( TrackbarName, "canny",
                &top_slider,
                top_slider_max,
                on_trackbar_canny );

  on_trackbar_canny(top_slider, 0 );

  waitKey();
  imwrite("cannyborders.png", border);
  return 0;
}
