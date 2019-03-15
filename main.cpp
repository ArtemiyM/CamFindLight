#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <stdio.h>
using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
    VideoCapture cap(0); //capture the video from webcam

    if ( !cap.isOpened() )  // if not success, exit program
    {
	cout << "Cannot open the web cam" << endl;
	return -1;
    }
    
    //Capture a temporary image from the camera
    Mat imgTmp;
    cap.read(imgTmp);

	namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"

    int iLowH = 0;
    int iHighH = 180;

    int iLowS = 0;
    int iHighS = 255;

    int iLowV = 250;
    int iHighV = 255;

    //Create trackbars in "Control" window
    createTrackbar("LowH", "Control", &iLowH, 255); //Hue (0 - 179)
    createTrackbar("HighH", "Control", &iHighH, 255);

    createTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
    createTrackbar("HighS", "Control", &iHighS, 255);

    createTrackbar("LowV", "Control", &iLowV, 255);//Value (0 - 255)
    createTrackbar("HighV", "Control", &iHighV, 255);

    while (true){
        Mat imgOriginal;

        // read a new frame from video
		if (!cap.read(imgOriginal)){ //if not success, break loop
		    cout << "Cannot read a frame from video stream" << endl;
		    break;
	        }
		Mat imgHSV;
		cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

		Mat imgThresholded;

		inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image

		//morphological opening (removes small objects from the foreground)
		erode(imgThresholded, imgThresholded, getStructuringElement(CV_SHAPE_ELLIPSE, Size(3, 3) ));
		dilate( imgThresholded, imgThresholded, getStructuringElement(CV_SHAPE_ELLIPSE, Size(3, 3)) );

		//morphological closing (removes small holes from the foreground)
		dilate( imgThresholded, imgThresholded, getStructuringElement(CV_SHAPE_ELLIPSE, Size(3, 3)) );
		erode(imgThresholded, imgThresholded, getStructuringElement(CV_SHAPE_ELLIPSE, Size(3, 3)) );

		//Calculate the moments of the thresholded image
		Moments oMoments = moments(imgThresholded);

		double dM01 = oMoments.m01;
		double dM10 = oMoments.m10;
		double dArea = oMoments.m00;

		// if the area <= 5000, I consider that the there are no object in the image and it's because of the noise, the area is not zero
		if (dArea > 500000){
		    //calculate the position of the ball
		    int posX = dM10 / dArea;
		    int posY = dM01 / dArea;

			printf ("DArea Now: %f, posX: %d posY: %d\n",abs(dArea), posX, posY);
			circle(imgOriginal,Point(posX, posY),10,Scalar(0,255,0),-1,8,0);
		}

		imshow("Original", imgOriginal); //show the original image
		imshow("imgThresholded", imgThresholded); //show the original image

		//wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
	    if (waitKey(30) == 27){
	            cout << "esc key is pressed by user" << endl;
	            break;
		}
    }

    return 0;
}
