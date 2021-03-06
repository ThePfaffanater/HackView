// HackView.cpp : Defines the entry point for the console application.
//uhhh look here? https://github.com/bb20basketball/Virtual-Birds-Eye-View/blob/master/webcamGet.py idk what is going on here, seems to be some pleb language I can read it but it is clearly inferior to my non functioning code.
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <thread>

using namespace cv;
using namespace std;


#define Cam0 0 //positions of cameras (USB)
#define Cam1 1
#define Cam2 2
#define Cam3 3

cv::Mat BirdsEye(cv::Mat& frame0, cv::Mat& frame1, cv::Mat& frame2, cv::Mat& frame3, cv::Mat& errorFrame);
void ShowManyImages(string title, int nArgs, ...);
void checkFrame(cv::Mat& frame, cv::Mat& backUp);

int main()
{
	cv::Mat backUpFrame = cv::imread("noCam.png");
	cv::Mat errorFrame = cv::imread("test.jpg");

	cv::VideoCapture vid0;
	cv::VideoCapture vid1;
	cv::VideoCapture vid2;
	cv::VideoCapture vid3;

	cv::Mat vid0Frame;
	cv::Mat vid1Frame;
	cv::Mat vid2Frame;
	cv::Mat vid3Frame;

	vid0.open(Cam0);//Start VideoCapture
	vid1.open(Cam1);
	vid2.open(Cam2);
	vid3.open(Cam3);

	while (1) //shhhhhhh, no complaining about infinite loops
	{

		//Getting camera feeds on different threads
		thread t0{ [&]()
		{
			vid0 >> vid0Frame;
			checkFrame(vid0Frame, backUpFrame);
		} };
		thread t1{ [&]()
		{
		vid1 >> vid1Frame;
		checkFrame(vid1Frame, backUpFrame);
		} };
		thread t2{ [&]()
		{
		vid2 >> vid2Frame;
		checkFrame(vid2Frame, backUpFrame);
		} };
		thread t3{ [&]()
		{
		vid3 >> vid3Frame;
		checkFrame(vid3Frame, backUpFrame);
		} };
		
		t0.join();
		t1.join();
		t2.join();
		t3.join();

		ShowManyImages("Video Feeds (DEBUG VIEW)", 4, vid0Frame, vid1Frame, vid2Frame, vid3Frame);
		
		if (cv::waitKey(10) == 27) return 1; // stop capturing by pressing ESC 
	}
		
    return -1;
}


cv::Mat BirdsEye(cv::Mat& frame0, cv::Mat& frame1, cv::Mat& frame2, cv::Mat& frame3, cv::Mat& errorFrame)
{
	cv::Mat final = errorFrame;


//	cv::resize()
//	cv::resize(vid4.read(), (400, 300), dst = preImg1)
//	cv.resize(vid3.read(), (400, 300), dst = preImg2)
//	cv.resize(img1, (400, 300), dst = preImg3)
//	cv.resize(vid2.read(), (400, 300), dst = preImg4)
//
//	cv.flip(preImg1, -1, dst = preImg1)
//	cv.flip(preImg2, -1, dst = preImg2)
//	cv.flip(preImg3, -1, dst = preImg3)
//	cv.flip(preImg4, -1, dst = preImg4)
	



	return final;
}

void ShowManyImages(string title, int nArgs, ...) //For debug display purposes
{
	int size;
	int i;
	int m, n;
	int x, y;

	// w - Maximum number of images in a row
	// h - Maximum number of images in a column
	int w, h;

	// scale - How much we have to resize the image
	float scale;
	int max;

	// If the number of arguments is lesser than 0 or greater than 12
	// return without displaying
	if (nArgs <= 0) {
		printf("Number of arguments too small....\n");
		return;
	}
	else if (nArgs > 14) {
		printf("Number of arguments too large, can only handle maximally 12 images at a time ...\n");
		return;
	}
	// Determine the size of the image,
	// and the number of rows/cols
	// from number of arguments
	else if (nArgs == 1) {
		w = h = 1;
		size = 300;
	}
	else if (nArgs == 2) {
		w = 2; h = 1;
		size = 300;
	}
	else if (nArgs == 3 || nArgs == 4) {
		w = 2; h = 2;
		size = 300;
	}
	else if (nArgs == 5 || nArgs == 6) {
		w = 3; h = 2;
		size = 200;
	}
	else if (nArgs == 7 || nArgs == 8) {
		w = 4; h = 2;
		size = 200;
	}
	else {
		w = 4; h = 3;
		size = 150;
	}

	// Create a new 3 channel image
	Mat DispImage = Mat::zeros(Size(50 + size*w, 40 + size*h), CV_8UC3);

	// Used to get the arguments passed
	va_list args;
	va_start(args, nArgs);

	// Loop for nArgs number of arguments
	for (i = 0, m = 20, n = 20; i < nArgs; i++, m += (20 + size)) {
		// Get the Pointer to the IplImage
		Mat img = va_arg(args, Mat);

		// Check whether it is NULL or not
		// If it is NULL, release the image, and return
		if (img.empty()) {
			printf("Invalid arguments");
			return;
		}

		// Find the width and height of the image
		x = img.cols;
		y = img.rows;

		// Find whether height or width is greater in order to resize the image
		max = (x > y) ? x : y;

		// Find the scaling factor to resize the image
		scale = (float)((float)max / size);

		// Used to Align the images
		if (i % w == 0 && m != 20) {
			m = 20;
			n += 20 + size;
		}

		// Set the image ROI to display the current image
		// Resize the input image and copy the it to the Single Big Image
		Rect ROI(m, n, (int)(x / scale), (int)(y / scale));
		Mat temp; resize(img, temp, Size(ROI.width, ROI.height));
		temp.copyTo(DispImage(ROI));
	}

	// Create a new window, and show the Single Big Image
	namedWindow(title, 1);
	imshow(title, DispImage);

	// End the number of arguments
	va_end(args);
}//for debug Display porp

void checkFrame(cv::Mat& frame, cv::Mat& backUp)//checks to see if camera is getting everything. If not replace with backUpFrame
{
	if(frame.empty())
	{
		frame = backUp;
	}
}