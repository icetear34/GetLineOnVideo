
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#define PI 3.1416

using namespace cv;
using namespace std;

void calcLinesP(const Mat &input, std::vector<Vec4i> &lines);//�Ϥ�, int[4]
void drawLinesP(Mat &input, const std::vector<Vec4i> &lines);
//void drawRectangleP(Mat &input, const std::vector<Vec4i> &lines);9/29���ѱ�

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
	//����v���e��
	double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH); 
	double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

	cout << "Frame size : " << dWidth << " x " << dHeight << endl;

	namedWindow("RealVideo", CV_WINDOW_AUTOSIZE); //create a window 
	namedWindow("CannyVideo", CV_WINDOW_AUTOSIZE);
	//Mat zone(dHeight, dWidth, 0);

	vector<Vec4i> linesP;//�u�q�y��
	int counTs = 0;//����p��

	while (1)
	{
		Mat frame;  //�x�sŪ���쪺�v��

		bool bSuccess = cap.read(frame); //Ū���v��
		waitKey(300); //����
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
			if (!bSuccess) //Ū������
			{
				cout << "Cannot read a frame from video stream" << endl;
				break;
			}
			Mat RealFram, HiGrayFram, GrayFram, CannyFram;
			frame.copyTo(RealFram);
			cvtColor(RealFram, GrayFram, CV_RGB2GRAY);//��Ƕ�
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
			Canny(GrayFram, CannyFram, 60, 175); //�� input ��t �� contours
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
//��u
void calcLinesP(const Mat &input, std::vector<Vec4i> &lines) {
	Mat contours;
	Canny(input, contours, 60, 175); //�� input ��t �� contours
	lines.clear();
	HoughLinesP(contours, lines, 1, CV_PI / 360, 170, 30, 15);
	//lines�O�@�ӦV�q�A�N�s�x�]x�Ay�Ax�Ay�^�`���O���q���u
}
//�e�u
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
//ø�s�x��
void drawRectangleP(Mat &input, const std::vector<Vec4i> &lines) {
	for (int i = 0; i<lines.size(); i++) {
		rectangle(input, Point(lines[i][0], lines[i][1]), Point(lines[i][2], lines[i][3]), Scalar(0, 255, 0), 2);
	}
}
/////////