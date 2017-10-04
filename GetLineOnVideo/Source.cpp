
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#define PI 3.1416

using namespace cv;
using namespace std;

void calcLinesP(const Mat &input, std::vector<Vec4i> &lines);//圖片, int[4]
void drawLinesP(Mat &input, const std::vector<Vec4i> &lines);
//void drawRectangleP(Mat &input, const std::vector<Vec4i> &lines);9/29註解掉

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
	//--------------------------------------------------------------------------
	VideoCapture cap(0); // open the video camera no. 0
						 //VideoCapture cap("Car2.avi");
	if (!cap.isOpened())  // if not success, exit program
	{
		cout << "Cannot open the video cam" << endl;
		return -1;
	}
	//抓取影像寬高
	double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH); 
	double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

	cout << "Frame size : " << dWidth << " x " << dHeight << endl;

	namedWindow("RealVideo", CV_WINDOW_AUTOSIZE); //create a window 
	namedWindow("CannyVideo", CV_WINDOW_AUTOSIZE);
	//Mat zone(dHeight, dWidth, 0);

	vector<Vec4i> linesP;//線段座標
	int counTs = 0;//延遲計數

	while (1)
	{
		Mat frame;  //儲存讀取到的影格

		bool bSuccess = cap.read(frame); //讀取影格
		waitKey(300); //延遲
		if (counTs<10)
		{
			Mat RealFram;
			frame.copyTo(RealFram);
			drawLinesP(RealFram, linesP);//
			imshow("RealVideo", RealFram);
			counTs += 1;
		}
		else if (counTs >= 10)
		{
			counTs = 0;
			if (!bSuccess) //讀取失敗
			{
				cout << "Cannot read a frame from video stream" << endl;
				break;
			}
			Mat RealFram, HiGrayFram, GrayFram, CannyFram;
			frame.copyTo(RealFram);
			cvtColor(RealFram, GrayFram, CV_RGB2GRAY);//轉灰階
			GrayFram.copyTo(HiGrayFram);
			int widthLimit = GrayFram.channels() * GrayFram.cols;
			for (int height = 0; height < GrayFram.rows; height++)
			{
				uchar *HiGrayData = HiGrayFram.ptr<uchar>(height);
				for (int width = 0; width < widthLimit; width++)
				{
					HiGrayData[width] /= 96;
					HiGrayData[width] *= 96;
				}
			}



			//imshow("RealVideo", RealVideo);
			//imshow("zone", zone);




			vector<Vec2f> lines;//
			Canny(GrayFram, CannyFram, 60, 175); //取 input 邊緣 至 contours
			calcLinesP(GrayFram, linesP);//
			drawLinesP(RealFram, linesP);//

			imshow("RealVideo", RealFram);
			imshow("CannyVideo", CannyFram);

			if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
			{
				cout << "esc key is pressed by user" << endl;
				break;
			}
		}
	}
	return 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//抓線
void calcLinesP(const Mat &input, std::vector<Vec4i> &lines) {
	Mat contours;
	Canny(input, contours, 60, 175); //取 input 邊緣 至 contours
	lines.clear();
	HoughLinesP(contours, lines, 1, CV_PI / 360, 170, 30, 15);
	//lines是一個向量，將存儲（x，y，x，y）總之是測量直線
}
//畫線
void drawLinesP(Mat &input, const std::vector<Vec4i> &lines) {
	for (int i = 0; i<lines.size(); i++) {
		int rng = 10;
		if ((lines[i][1] - rng < lines[i][3]) && (lines[i][1] + rng > lines[i][3]))
		{
			line(input, Point(lines[i][0], lines[i][1]), Point(lines[i][2], lines[i][3]), Scalar(0, 255, 0), 2);
		}
		//void line(Mat& img, Point pt1, Point pt2, const Scalar& color, int thickness=1, int lineType=8, int shift=0)
	}
}
//繪製矩形
void drawRectangleP(Mat &input, const std::vector<Vec4i> &lines) {
	for (int i = 0; i<lines.size(); i++) {
		rectangle(input, Point(lines[i][0], lines[i][1]), Point(lines[i][2], lines[i][3]), Scalar(0, 255, 0), 2);
	}
}
/////////