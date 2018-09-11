#include <iostream>
#include <opencv2/opencv.hpp>
#include <math.h>

using namespace cv;
using namespace std;

Mat image1, image2, blended;
Mat temp1,temp2;
Mat frame32f, frameFiltered;
Mat kernel;

int main(int argvc, char** argv){
	string filename = "motiondetect.mp4";
	VideoCapture capture(filename);
	int frame_width=   capture.get(CV_CAP_PROP_FRAME_WIDTH);
	int frame_height=   capture.get(CV_CAP_PROP_FRAME_HEIGHT);
	VideoWriter outputVideo("out.avi",CV_FOURCC('M','J','P','G'),10, 
			                        Size(frame_width,frame_height),true);


	while(1){

		capture >> image1;
		capture >> image1;
		capture >> image1;
		capture >> image1;
		capture >> image1;
		if(image1.empty()) break;

		image2 = image1.clone();

		int kernel_size = 12;

		kernel = Mat::ones( kernel_size, kernel_size, CV_32F )/ (float)(kernel_size*kernel_size);
		filter2D(image2, frameFiltered,
			image2.depth(), kernel, Point(1,1), 0);

		image2 = frameFiltered.clone();

		int h = image1.size().height;
		int w = image1.size().width; //RGB
		temp1 = image1.clone();
		temp2 = image2.clone();
		for(int i=0; i<h; i++){
			double d = 6.0;
			double l1 = 20.0;
			double c = 100.0;

			double res = ((tanh((i-c+l1)/d)-tanh((i-c-l1)/d))/2.0);
			for(int j=0; j<w; j++){
				temp1.at<uchar>(i,j) = (unsigned char)((res)*image1.at<uchar>(i,j));
				temp2.at<uchar>(i,j) = (unsigned char)((1.0-res)*image2.at<uchar>(i,j));
			}
		}
		
		addWeighted( temp1, 1.0, temp2, 1.0, 0.0, blended);
		outputVideo << blended;
	}

  return 0;
}
