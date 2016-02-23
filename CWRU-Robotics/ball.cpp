#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

// namespace is the same as local function
namespace video
{
	int process(VideoCapture& capture)
	{
		int n = 0;
		char filename[200];
		string window_name = "video";  // create the name for the window
		namedWindow(window_name, WINDOW_AUTOSIZE); // create the window 
		Mat frame1; // original frame
		Mat frame2; // gray frame
		Mat frame3; // HSV frame, for detecting color 

		while (true)    // forever loop
		{
			capture >> frame1;  // read the data from capture in store in frame1
			if (frame1.empty())
				break;
			
			frame1.convertTo(frame2, CV_8U);  // convert frame to 8 bit single channel
			medianBlur(frame2, frame2, 5);    // blur the frame
			cvtColor(frame2, frame2, COLOR_BGR2GRAY);
			vector<Vec3f> circles;  // what is vector <vec3f>
			HoughCircles(frame2, circles, HOUGH_GRADIENT, 
					1,          // inverse ratio fo the accumulator resolution
					10,          // minimum distance between circle center 
					100,	    // higher threshold value for Canny 
					50,         // accumulator threshold for the circle centers 
					10,	    // minimum radius 
					250);        // maximum radius
			// the trickiest value is the threshold for the circle centers, the smaller the value,
			// the more circle the program will detect, thus we will have a lot of false circle. But
			// if this value is big, say 500, we cannot detect anycircle. 50 is a reasonable value for 
			// our ball

			// change the original frame to HSV
			cvtColor(frame1, frame3, COLOR_BGR2HSV);

			for (size_t i = 0; i < circles.size(); i++)
			{
				Vec3i c = circles[i];
				circle(frame1, Point(c[0], c[1]), c[2], Scalar(0, 255, 0), 3, LINE_AA);
				// find the HSV value of the center (c[0], c[1]) 
				Vec3b pixel = frame3.at<Vec3b>(c[0], c[1]);
				int H = pixel.val[0];
				int S = pixel.val[1];
				int V = pixel.val[2];
				cout << "H: " << H << " " 
				     << "S: " << S << " "
				     << "V: " << V << endl;
			}

			imshow(window_name, frame1);

			char key = (char)waitKey(30);

			switch(key)
			{
				case 'q':
				case 'Q':
				case 27 :
					return 0;
				case ' ': // save image
					sprintf(filename, "filename%.3d.jpg", n++);
					imwrite(filename, frame1);
					cout << "Saved " << filename << endl;
					break;
				default:
					break;
			}
		}
		return 0;
	}
}

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		cout << "Enter the address of camera: 0" << endl;
		return 1;
	}

	std::string cap = argv[1];
	VideoCapture capture(cap);

	if (!capture.isOpened())
		capture.open(atoi(cap.c_str()));
	if (!capture.isOpened())
	{
		cout << "Failed to open camera" << endl;
		return 1;
	}
	return video::process(capture);
}
