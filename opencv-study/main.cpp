#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <sstream>
#include <opencv2/opencv.hpp>
#include <opencv2/video.hpp>
#include <Windows.h>

using namespace std;
using namespace cv;

void on_trackbar_action1(int pos, void* userdata);
void on_trackbar_action2(int pos, void* userdata);

void apply_houghlines(Mat* src);

int edge_param1 = 500;
int edge_param2 = 700;
int trackbar_pos_scale = 20;

#define window_name_edge "edge"
#define window_name_src "src_clone"

std::string GetCurrentDirectory()
{
	char buffer[360];
	GetModuleFileNameA(NULL, buffer, 360);
	//std::string::size_type pos = std::string(buffer).find_last_of("\\/");
	return std::string(buffer)/*.substr(0, pos)*/;
}

int main()
{
	string image_path = GetCurrentDirectory();
	cout << image_path << endl;
	
	for(int i=0;i<3;i++) // back 3 times
		image_path = image_path.substr(0, image_path.find_last_of("\\/"));

	image_path += "\\ABX00051_03.front_1000x750.jpg";

	Mat src;
	src = imread(image_path);

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
	putText(src_clone, window_name_src, Point(10, 30), 2, 1.0, Scalar::all(0));
	imshow(window_name_src, src_clone);

	putText(edge, window_name_edge, Point(10, 30), 2, 1.0, Scalar::all(255));
	imshow(window_name_edge, edge);

	while (true)
	{
		int ch = waitKey();

		if (ch == 'q')
			break;

		switch (ch)
		{
		case ' ':
			apply_houghlines(&src);
			break;
		}
	}

	destroyAllWindows();
	//system("pause");
	return 0;
}

void apply_houghlines(Mat *src)
{
	Mat edge;
	Canny((*src), edge, edge_param1, edge_param2);

	vector<Vec2f> lines;
	HoughLines(edge, lines, 1, CV_PI / 180, 250);


	for (size_t i = 0; i < lines.size(); i++)
	{
		float r = lines[i][0];
		float t = lines[i][1];
		cout << "r : " << r << endl;
		cout << "t : " << t << endl;
		double cos_t = cos(t);
		double sin_t = sin(t);
		double x0 = r * cos_t;
		double y0 = r * sin_t;
		double alpha = 1000;

		Point pt1(cvRound(x0 + alpha * (-sin_t)), cvRound(y0 + alpha * cos_t));
		Point pt2(cvRound(x0 - alpha * (-sin_t)), cvRound(y0 - alpha * cos_t));
		cout << "(" << pt1.x << ", " << pt1.y << "), (" << pt2.x << ", " << pt2.y << ")";
		line(edge, pt1, pt2, Scalar::all(255), 2, LINE_AA);
	}

	//line(edge, Point(10, 10), Point(100, 100), Scalar(255, 255, 255), 2, LINE_AA);

	stringstream ss;
	ss << "lines.size() : " << lines.size();

	putText(edge, ss.str(), Point(10, 30), 2, 1.0, Scalar::all(255));
	imshow(window_name_edge, edge);
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
	Canny(*src, edge, edge_param1, edge_param2);
	putText(edge, ss1.str(), Point(10, 30), 2, 1.0, Scalar::all(255));
	putText(edge, ss2.str(), Point(10, 60), 2, 1.0, Scalar::all(255));
	imshow(window_name_edge, edge);
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