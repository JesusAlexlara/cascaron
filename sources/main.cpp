#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

void convolucion(Mat &a, Mat &b, float ma[3][3]);

int main( int argc, char** argv )
{
    Mat imagen;
    Mat salida;

    imagen = imread("pp.jpeg", IMREAD_GRAYSCALE);

    salida = imagen.clone();

    if(! imagen.data ) {
        cout <<  "No se pudo abrir la imagen.." << endl ;
        return -1;
    }

    namedWindow("Imagen" , WINDOW_AUTOSIZE);
    imshow("Imagen", imagen);

    float ma[3][3] = {
        {1/-1.f, 1/-1.f, 1/-1.f},
        {1/-1.f, 1/8.f, 1/-1.f},
        {1/-1.f, 1/-1.f, 1/-1.f}
    };

    convolucion(imagen, salida, ma);

    namedWindow("Imagen salida" , WINDOW_AUTOSIZE);
    imshow("Imagen salida",salida);

    waitKey(0);

    return 0;


}

void convolucion(Mat &a, Mat &b, float ma[3][3]) {
    float sum;

    for (int x = 1; x < a.rows - 1; x++) {
        for (int y = 1 ; y < a.cols - 1; y++) {
            sum = 0.0f;

            for (int j = -1; j <= 1; j++) {
                for (int k = -1; k <= 1; k++) {
                    sum += ma[k+1][j+1] *
                            a.at<uchar>(x - k, y - j);
                }
            }
            b.at<uchar>(x,y) = sum;
        }
    }
    return;
}
