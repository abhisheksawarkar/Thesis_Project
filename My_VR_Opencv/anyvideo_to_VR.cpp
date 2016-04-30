#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
	int vrFrameWidth = 1900, vrFrameHeight = 1000;
	VideoCapture cap;// ("w.wmv"); //
	//int codec = CAP_PROP_FOURCC*('M', 'J', 'P', 'G');
	//VideoWriter video;
	//video.open("out.avi", -1, 30.0, Size(vrFrameWidth, vrFrameHeight));
	cap.open("http://192.168.1.137:8080/video?x.mjpeg");
	if (!cap.isOpened())  // if not success, exit program
	{
		cout << "Cannot open the video cam" << endl;
		system("pause");
		return -1;
	}

	double dWidth = cap.get(CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
	double dHeight = cap.get(CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video

	cout << "Frame size : " << dWidth << " x " << dHeight << endl;

	namedWindow("MyVideo", WINDOW_AUTOSIZE); //create a window called "MyVideo"
	//namedWindow("MyNegativeVideo", WINDOW_AUTOSIZE);
	
	Mat frame, frameLeft, frameRight, vrFrame(Size(vrFrameWidth, vrFrameHeight), CV_8UC3);
	
	
	/*if (!video.isOpened())
	{
		cout << "Could not open the output video for write: " << endl;
		system("pause");
		return -1;
	}*/
	while (1)
	{
		bool bSuccess = cap.read(frame); // read a new frame from video

		if (!bSuccess) //if not success, break loop
		{
			cout << "Cannot read a frame from video stream" << endl;
			break;
		}
		resize(frame, frameLeft, Size((vrFrameWidth / 2)-5, vrFrameHeight), 0, 0, INTER_CUBIC);
		resize(frame, frameRight, Size((vrFrameWidth / 2)-5, vrFrameHeight), 0, 0, INTER_CUBIC);
		frameLeft.copyTo(vrFrame(Rect(0, 0, (vrFrameWidth / 2)-5, vrFrameHeight)));
		frameRight.copyTo(vrFrame(Rect((vrFrameWidth / 2)+5, 0, (vrFrameWidth / 2)-5, vrFrameHeight)));
		//video.write(vrFrame);

		imshow("MyVideo", vrFrame); //show the frame in "MyVideo" window

		if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}
	}
	return 0;
}
