#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

#include "oclFiltro.hpp"

#define HEIGHT 480
#define WIDTH 640

using namespace std;
using namespace cv;

float filtro1[] = {
    0, -1, 0,
    -1, 5, -1,
    0, -1, 0
};

float filtro2[] = {
    0, 1, 0,
    1, -4, 1,
    0, 1, 0
};

int main( int argc, char** argv )
{
    oclFiltro *filtro;
    VideoCapture cap(0);

    namedWindow("OpenCL0",1);
    namedWindow("OpenCV",1);
    namedWindow("OpenCL1",1);

    if(!cap.isOpened())
        return EXIT_FAILURE;

    cap.set(CV_CAP_PROP_FRAME_HEIGHT, HEIGHT);
    cap.set(CV_CAP_PROP_FRAME_WIDTH, WIDTH);

    Mat frame, frameGray;
    uchar* data;
    uchar data2[WIDTH*HEIGHT];

    uchar data3[WIDTH*HEIGHT];

   filtro = new oclFiltro(1, WIDTH, HEIGHT);

   for (;;) {
       clock_t inicio;
       double duracion;
       inicio = clock();

       cap >> frame;
       cvtColor(frame, frameGray, CV_RGB2GRAY);
       data = frameGray.data;
//        cvtColor(frame, edges, COLOR_BGR2GRAY);
//        GaussianBlur(edges, edges, Size(15, 15), 2, 2);
//        Canny(edges, edges, 0, 30, 3);
       cvtColor(frame, frameGray, CV_RGB2GRAY);
       filtro->compute(data, data2, filtro1);
       filtro->compute(data2, data3, filtro2);

       Mat edges(frameGray.rows, frameGray.cols, CV_8UC1, data3);
       //Mat edges2(frameGray.rows, frameGray.cols, CV_8UC1, data3);

       duracion = ( clock() - inicio ) / (double) CLOCKS_PER_SEC;
       //cout << "time: " << duracion;

       imshow("OpenCL0", edges);
//       imshow("OpenCL1", edges2);
       imshow("OpenCV", frameGray);
       if(waitKey(30) >= 0) break;
    }
    return EXIT_SUCCESS;
}

