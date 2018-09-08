#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int, char** argv){
	Mat image;
	Vec3b val;


	image= imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
	if(!image.data)
		cout << "nao abriu bolhas.png" << endl;

	int rows = image.rows;
	int cols = image.cols;

	std::cout << "Image limits: ";
	std::cout << "rows: " << rows << " columns: " << cols << '\n';
	std::cout << "Choose retangle corner points <x0> <x1> <y0> <y1>: \n";
	int x0,x1,y0,y1;
	std::cin >> x0 >> x1 >> y0 >> y1;

	namedWindow("janela",WINDOW_AUTOSIZE);

	for(int i=x0;i<y0;i++){
		for(int j=x1;j<y1;j++){
			image.at<uchar>(i,j)= 255 - image.at<uchar>(i,j);
		}
	}

	imshow("janela", image);  
	waitKey();

	return 0;
}
