// DetectorDemo.cpp : �������̨Ӧ�ó������ڵ㡣
/***********************************************/
//updated at 2017-3-12
//
//version 1.2
// - �õ��������˵���ɫֱ��ͼ
//
//version 1.1
// - ������˲���ʾ���Ŷ�
// - �ü�������
//
/***********************************************/

#include "stdafx.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

string DoubleToString(double d)
{
	//Need #include <sstream> 
	using namespace std;
	string str;
	stringstream ss;
	ss << d;
	ss >> str;
	return str;
}

string size_tToString(size_t d)
{
	//Need #include <sstream> 
	using namespace std;
	string str;
	stringstream ss;
	ss << d;
	ss >> str;
	return str;
}


void getHist(const vector<Mat> srcImage,  vector<Mat> &srcHist) {
	vector<Mat> srcHsvImage;
	srcHsvImage.resize(srcImage.size());
	vector<Mat> hsv_planes;
	vector<Mat> hPlane;
	hPlane.resize(srcHsvImage.size());

	for (int i = 0; i < srcImage.size(); i++) {
		cvtColor(srcImage[i], srcHsvImage[i], CV_RGB2HSV);
		split(srcHsvImage[i], hsv_planes);
		hPlane[i] = hsv_planes[0];
	}
	
	//ֱ��ͼ����
	int hbins = 18;
	float hRange[] = { 0,180 };
	const float* phranges = hRange;


	for (int i = 0; i < srcHsvImage.size(); i++) {
		calcHist(&hPlane[i], 1, 0, Mat(), srcHist[i], 1, &hbins, &phranges, true, false);
		normalize(srcHist[i], srcHist[i], 0, 1, NORM_MINMAX, -1, Mat());
	}

}


int main(int argc, char** argv)
{
	//�����ļ�·��
	vector<string> srcPath;
	srcPath.push_back("./src/0002002.jpg");
	srcPath.push_back("./src/0001001.jpg");
	srcPath.push_back("./src/0005001.jpg");
	srcPath.push_back("./src/0006002.jpg");
	srcPath.push_back("./src/0036005.jpg");
	srcPath.push_back("./src/0002003.jpg");

	//�����������ļ�
	vector<Mat> srcImage;
	srcImage.resize(srcPath.size());
	for (int i = 0; i < srcImage.size(); i++) {
		srcImage[i] = imread(srcPath[i]);
		if (srcImage.empty())
		{
			std::cout << "read image failed" << std::endl;
		}
	}
	//����
	vector<Mat> srcHist;
	srcHist.resize(srcImage.size());

	getHist(srcImage, srcHist);


	vector<double> compareResult;
	compareResult.resize(srcHist.size());

	int winCols = 0;

	for (int i = 0; i < srcHist.size(); i++) {
		compareResult[i] = compareHist(srcHist[srcHist.size()-1], srcHist[i], 0);
		std::cout << i << "  " << compareResult[i] << "\n" << std::endl;
		cv::imshow(size_tToString(i), srcImage[i]);
		moveWindow(size_tToString(i), winCols, 0);
		winCols += srcImage[i].cols;
	}
	//compareHist����������
	//0: CV_COMP_CORREL�����ƥ��,����ֵԽ��ƥ��̶�Խ�ߣ�
	//1: CV_COMP_CHISQ����,����ֵԽСƥ���Խ�ߣ�
	//2: CV_COMP_INTERSECT ֱ��ͼ�ཻ,����ֵԽ��ƥ���Խ�ߣ�
	//3: CV_COMP_BHATTACHARYYA����ֵԽСƥ���Խ�ߡ�

	cv::waitKey(0);

	return 0;
}