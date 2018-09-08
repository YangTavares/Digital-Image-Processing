#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int, char** argv){
	Mat image,copy;
	Vec3b val;


	image = imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
	copy = image.clone();
	if(!image.data)
		cout << "nao abriu bolhas.png" << endl;

	int rows = image.rows;
	int cols = image.cols;

	namedWindow("janela",WINDOW_AUTOSIZE);

	for(int i=0;i<rows/2;i++){
		for(int j=0;j<cols/2;j++){
			image.at<uchar>(i,j)= copy.at<uchar>(i+(rows/2),j+(cols/2));
		}
	}

	for(int i=rows/2;i<rows;i++){
		for(int j=cols/2;j<cols;j++){
			image.at<uchar>(i,j)= copy.at<uchar>(i-(rows/2),j-(cols/2));
		}
	}

	for(int i=rows/2;i<rows;i++){
		for(int j=0;j<cols/2;j++){
			image.at<uchar>(i,j)= copy.at<uchar>(i-(rows/2),j+(cols/2));
		}
	}

	for(int i=0;i<rows/2;i++){
		for(int j=cols/2;j<cols;j++){
			image.at<uchar>(i,j)= copy.at<uchar>(i+(rows/2),j-(cols/2));
		}
	}

	imshow("janela", image);  
	waitKey();

	return 0;
}
