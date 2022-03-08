#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/video.hpp>

using namespace std;
using namespace cv;

int main()
{
	cout << "CV_VERSION : " << CV_VERSION << endl;

	cv::VideoCapture cap("C:\\video.mp4");

	if (!cap.isOpened())
	{
		cout << "mp4 open failed\n";
	}
	else
	{
		cout << "wid : " << cap.get(CAP_PROP_FRAME_WIDTH) << endl;
		cout << "hei : " << cap.get(CAP_PROP_FRAME_HEIGHT) << endl;

		Mat frame;

		while (true)
		{
			cap >> frame;

			if (frame.empty())
				break;

			imshow("frame", frame);

			if (waitKey(10) == 'q')
				break;

		}

	}

	destroyAllWindows();


	//Mat mat1;

	//mat1 = imread("C:\\Users\\HYEON-desk\\Pictures\\nucleo_f302r8_morpho_right_2017_3_23.png");

	//if (mat1.empty())
	//{
	//	cout << "image load failed\n";
	//}
	//else
	//{
	//	imshow("image", mat1);
	//	waitKey();
	//}



	system("pause");
	return 0;
}