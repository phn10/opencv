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
		Mat frame1; // original fram
		Mat frame2; // gray frame

		while (true)    // forever loop
		{
			capture >> frame1;  // read the data from capture in store in frame1
			if (frame1.empty())
				break;
			
			frame1.convertTo(frame2, CV_8U);  // convert frame to 8 bit single channel
			medianBlur(frame2, frame2, 5);    // blur the frame
			cvtColor(frame2, frame2, COLOR_BGR2GRAY);
			vector<Vec3f> circles;  // what is vector <vec3f>
			HoughCircles(frame2, circles, HOUGH_GRADIENT, 2, 32.0, 30, 550); // not sure about the input

			for (size_t i = 0; i < circles.size(); i++)
			{
				Vec3i c = circles[i];
				circle(frame1, Point(c[0], c[1]), c[2], Scalar(0, 2555555, 0), 3, LINE_AA);
				circle(frame1, Point(c[0], c[1]), 2, Scalar(0, 255, 0), 3, LINE_AA);
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
