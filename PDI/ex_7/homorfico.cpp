#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define RADIUS 20

using namespace cv;
using namespace std;

float yh=0.0,yl=0.0;
int yh_slider=0,yl_slider=0;
float c=0.0;
int c_slider=0;
float d=0.0;
int d_slider=0;

// troca os quadrantes da imagem da DFT
void deslocaDFT(Mat& image ){
  Mat tmp, A, B, C, D;

  // se a imagem tiver tamanho impar, recorta a regiao para
  // evitar cÃ³pias de tamanho desigual
  image = image(Rect(0, 0, image.cols & -2, image.rows & -2));
  int cx = image.cols/2;
  int cy = image.rows/2;
  
  // reorganiza os quadrantes da transformada
  // A B   ->  D C
  // C D       B A
  A = image(Rect(0, 0, cx, cy));
  B = image(Rect(cx, 0, cx, cy));
  C = image(Rect(0, cy, cx, cy));
  D = image(Rect(cx, cy, cx, cy));

  // A <-> D
  A.copyTo(tmp);  D.copyTo(A);  tmp.copyTo(D);

  // C <-> B
  C.copyTo(tmp);  B.copyTo(C);  tmp.copyTo(B);
}

Mat image;

void on_trackbar_hom(int, void*){
  Mat imaginaryInput, complexImage, multsp;
  Mat padded, filter, mag;
  Mat image, imagegray, tmp; 
  Mat_<float> realInput, zeros;
  vector<Mat> planos;

	image = imread("biel.png");

	yh = (float) yh_slider/100.0;
	yl = (float) yl_slider/100.0;
	d = (float) 10.0*d_slider/100.0;
	c = (float) c_slider/100.0;


  // guarda tecla capturada

  // valores ideais dos tamanhos da imagem
  // para calculo da DFT
  int dft_M, dft_N;


  // identifica os tamanhos otimos para
  // calculo do FFT
  dft_M = getOptimalDFTSize(image.rows);
  dft_N = getOptimalDFTSize(image.cols);

  // realiza o padding da imagem
  copyMakeBorder(image, padded, 0,
                 dft_M - image.rows, 0,
                 dft_N - image.cols,
                 BORDER_CONSTANT, Scalar::all(0));

  // parte imaginaria da matriz complexa (preenchida com zeros)
  zeros = Mat_<float>::zeros(padded.size());

  // prepara a matriz complexa para ser preenchida
  complexImage = Mat(padded.size(), CV_32FC2, Scalar(0));

  // a funÃ§Ã£o de transferÃªncia (filtro frequencial) deve ter o
  // mesmo tamanho e tipo da matriz complexa
  filter = complexImage.clone();

  // cria uma matriz temporÃ¡ria para criar as componentes real
  // e imaginaria do filtro ideal
  tmp = Mat(dft_M, dft_N, CV_32F);

  // prepara o filtro gaussiano
	float d2;
  for(int i=0; i<dft_M; i++){
    for(int j=0; j<dft_N; j++){
				d2 = (i-dft_M/2)*(i-dft_M/2)+(j-dft_N/2)*(j-dft_N/2);
				tmp.at<float>(i,j) = (yh-yl)*(1.0-(float)exp(-(c*d2/(d*d))))+yl;
    }
  }

  // cria a matriz com as componentes do filtro e junta
  // ambas em uma matriz multicanal complexa
  Mat comps[]= {tmp, tmp};
  merge(comps, 2, filter);

  cvtColor(image, imagegray, CV_BGR2GRAY);
  imshow("original", imagegray);

  // realiza o padding da imagem
  copyMakeBorder(imagegray, padded, 0,
                 dft_M - image.rows, 0,
                 dft_N - image.cols,
                 BORDER_CONSTANT, Scalar::all(0));

  // limpa o array de matrizes que vao compor a
  // imagem complexa
  planos.clear();
  // cria a compoente real
  realInput = Mat_<float>(padded);
  // insere as duas componentes no array de matrizes
  planos.push_back(realInput);
  planos.push_back(zeros);

  // combina o array de matrizes em uma unica
  // componente complexa
  merge(planos, complexImage);

  // calcula o dft
  dft(complexImage, complexImage);

  // realiza a troca de quadrantes
  deslocaDFT(complexImage);

  // aplica o filtro frequencial
  mulSpectrums(complexImage,filter,complexImage,0);

  // limpa o array de planos
  planos.clear();
  // separa as partes real e imaginaria para modifica-las
  split(complexImage, planos);
 

  // recompoe os planos em uma unica matriz complexa
  merge(planos, complexImage);

  // troca novamente os quadrantes
  deslocaDFT(complexImage);

  // calcula a DFT inversa
  idft(complexImage, complexImage);

  // limpa o array de planos
  planos.clear();

  // separa as partes real e imaginaria da
  // imagem filtrada
  split(complexImage, planos);

  // normaliza a parte real para exibicao
  normalize(planos[0], planos[0], 0, 1, CV_MINMAX);
  imshow("filtrada", planos[0]);
}

int main(int , char**){

	int max = 100;
	char TrackbarName[50];

  namedWindow("filtrada", 1);
 
  sprintf( TrackbarName, "yh %d", max );
  createTrackbar( TrackbarName, "filtrada",
				  &yh_slider,
				  max,
				  on_trackbar_hom );

  sprintf( TrackbarName, "yl %d", max );
  createTrackbar( TrackbarName, "filtrada",
				  &yl_slider,
				  max,
				  on_trackbar_hom );
  sprintf( TrackbarName, "c %d", max );
  createTrackbar( TrackbarName, "filtrada",
				  &c_slider,
				  max,
				  on_trackbar_hom );
  sprintf( TrackbarName, "d %d", max );
  createTrackbar( TrackbarName, "filtrada",
				  &d_slider,
				  max,
				  on_trackbar_hom );
	on_trackbar_hom(100,0);
  waitKey(0);
}
