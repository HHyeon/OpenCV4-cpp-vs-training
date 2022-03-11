#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <sstream>
#include <opencv2/opencv.hpp>
#include <opencv2/video.hpp>

using namespace std;
using namespace cv;

void on_trackbar_action1(int pos, void* userdata);
void on_trackbar_action2(int pos, void* userdata);

int edge_param1 = 100;
int edge_param2 = 200;
int trackbar_pos_scale = 20;

int main()
{
	Mat src;
	src = imread("C:\\Users\\HYEON-desk\\Desktop\\ABX00051_01.iso_1000x750.jpg");

	if (src.empty())
	{
		cerr << "imread failed" << endl;
		return 1;
	}

	Mat edge;
	Canny(src, edge, edge_param1, edge_param2);

	namedWindow("window");
	createTrackbar("edge_param1", "window", 0, 50, on_trackbar_action1, (void*)&src);
	createTrackbar("edge_param2", "window", 0, 50, on_trackbar_action2, (void*)&src);

	setTrackbarPos("edge_param1", "window", edge_param1 / trackbar_pos_scale);
	setTrackbarPos("edge_param2", "window", edge_param2 / trackbar_pos_scale);
	
	Mat src_clone;
	src.copyTo(src_clone);
	putText(src_clone, "src_clone", Point(10, 30), 2, 1.0, Scalar::all(0));
	imshow("src_clone", src_clone);

	putText(edge, "edge", Point(10, 30), 2, 1.0, Scalar::all(255));
	imshow("edge", edge);

	waitKey();
	destroyAllWindows();
	//system("pause");
	return 0;
}

void apply_edge_window(Mat *src)
{
	stringstream ss1;
	ss1 << "param1 - ";
	ss1 << edge_param1;

	stringstream ss2;
	ss2 << "param2 - ";
	ss2 << edge_param2;

	Mat edge;
	Canny(*src, edge, (0 + edge_param1), (0 + edge_param2));
	putText(edge, ss1.str(), Point(10, 30), 2, 1.0, Scalar::all(255));
	putText(edge, ss2.str(), Point(10, 60), 2, 1.0, Scalar::all(255));
	imshow("edge", edge);
}

void on_trackbar_action1(int pos, void* userdata)
{
	Mat src = *(Mat*)userdata;
	edge_param1 = pos * trackbar_pos_scale;
	apply_edge_window(&src);
}

void on_trackbar_action2(int pos, void* userdata)
{
	Mat src = *(Mat*)userdata;
	edge_param2 = pos * trackbar_pos_scale;
	apply_edge_window(&src);
}