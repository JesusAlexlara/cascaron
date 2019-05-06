#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main( int argc, char** argv )
{
    //VideoCapture cap(0);
    VideoCapture cap('video.mp4');

    Mat edges;
    namedWindow("edges",1);

    if(!cap.isOpened())
        return EXIT_FAILURE;

    for (;;) {
        Mat frame;

        cap >> frame;
        //cvtColor(frame, edges, COLOR_BGR2GRAY);
        //GaussianBlur(edges, edges, Size(15, 15), 2, 2);
        //Canny(edges, edges, 0, 30, 3);
        imshow("video", edges);
        if(waitKey(30) >= 0) break;
    }
    return EXIT_SUCCESS;
}

