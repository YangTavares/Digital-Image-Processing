#include <opencv2/opencv.hpp>
#include <cstdlib>

using namespace cv;

int main( int argc, char** argv ){
  int nClusters = 6;
  Mat rotulos;
  int nRodadas = 1;
  Mat centros;

  if(argc!=3){
	exit(0);
  }
  
  Mat img = imread( argv[1], CV_32F);
	Mat H1 = img.clone(), H2 = img.clon:();
	for(int it=0;it<10;it++){
	  Mat samples(img.rows * img.cols, 3, CV_32F);
		Mat rotulada( img.size(), img.type() );

		for( int y = 0; y < img.rows; y++ ){
			for( int x = 0; x < img.cols; x++ ){
				for( int z = 0; z < 3; z++){
					samples.at<float>(y + x*img.rows, z) = img.at<Vec3b>(y,x)[z];
				}
			}
		}
		
		kmeans(samples,
			 nClusters,
			 rotulos,
			 TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 10000, 0.0001),
			 nRodadas,
			 KMEANS_PP_CENTERS,
			 centros );


		for( int y = 0; y < img.rows; y++ ){
			for( int x = 0; x < img.cols; x++ ){ 
			 int indice = rotulos.at<int>(y + x*img.rows,0);
			 rotulada.at<Vec3b>(y,x)[0] = (uchar) centros.at<float>(indice, 0);
			 rotulada.at<Vec3b>(y,x)[1] = (uchar) centros.at<float>(indice, 1);
			 rotulada.at<Vec3b>(y,x)[2] = (uchar) centros.at<float>(indice, 2);
			}
		}
		if(it<5)	vconcat(rotulada,H1,H1);
		else			vconcat(rotulada,H2,H2);
	}
	Mat F1;
	hconcat(H1,H2,F1);
	
	namedWindow("img",WINDOW_NORMAL);
	//resizeWindow("img", 600,600);


  imshow( "img", F1 );
  imwrite(argv[2], F1);
  waitKey( 0 );
}
