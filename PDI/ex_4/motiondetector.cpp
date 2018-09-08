#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv){
	Mat image;
	int width, height;
	VideoCapture cap;
	//vector<Mat> planes;
	Mat histR, histG, histB;
	int nbins = 64;
	float range[] = {0, 256};
	const float *histrange = { range };
	bool uniform = true;
	bool acummulate = false;

	cap.open(0);

	if(!cap.isOpened()){
		cout << "cameras indisponiveis";
		return -1;
	}

	width  = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

	cout << "largura = " << width << endl;
	cout << "altura  = " << height << endl;

	int histw = nbins, histh = nbins/2;
	Mat histImgR(histh, histw, CV_8UC3, Scalar(0,0,0));
	cvtColor(histImgR, histImgR, CV_BGR2GRAY);
	Mat gray;
	Mat detector;
	int dif;
	bool flag = true;

	while(1){
   		cap >> image;
		cvtColor(image, gray, CV_BGR2GRAY);
    	calcHist(&gray, 1, 0, Mat(), histR, 1,
             &nbins, &histrange,
             uniform, acummulate);
		//std::cout << histR.rows << " " << histR.cols << '\n';

    	normalize(histR, histR, 0, histImgR.rows, NORM_MINMAX, -1, Mat());

		dif=0;

		if(flag){
			flag = false;
			detector = histR.clone();
		}

		int max=0;
		for(int i=0;i<histR.rows;i++){
			dif += (int)abs(histR.at<float>(i)-detector.at<float>(i));
			if(dif>max) max=dif;
		}
		
		std::cout << dif << '\n';
		if(dif > 20) std::cout << "Movement!!\n";
		detector = histR.clone();
    	histImgR.setTo(0);
	
    	for(int i=0; i<nbins; i++){
      		line(histImgR,
        	Point(i, histh),
        	Point(i, histh-cvRound(histR.at<float>(i))),
        	150, 1, 8, 0);
    	}
    	histImgR.copyTo(gray(Rect(0, 0       ,nbins, histh)));
		equalizeHist( gray, gray );
    	imshow("image", gray);
    	if(waitKey(30) >= 0) break;
  	}
  	return 0;
}
