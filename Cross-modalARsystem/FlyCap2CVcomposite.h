#pragma once

#ifdef _WIN32
#define WIN32
#endif
#ifdef _WIN64
#define WIN64
#endif

#define SQUAREVERTICAL 2	//�`�F�X�{�[�h�̃}�X�̐��c
#define SQUAREHORIZONTAL 3	//�`�F�X�{�[�h�̃}�X�̐���
#define PARTITION 7		//width�̕�����
#define XUPPERLEFT 100
#define YUPPERLEFT 150

#include<opencv2/opencv.hpp>
#include<FlyCapture2.h>
#include<FlyCapture2GUI.h>
#include<Windows.h>
#include<stdio.h>
#include<tchar.h>
#include<vector>


#pragma comment(lib, "opencv_world310d.lib")
#pragma comment(lib, "FlyCapture2.lib")
#pragma comment(lib, "FlyCapture2GUI.lib")

using namespace cv;
using namespace std;

#define RU(IMG, X, Y)((IMG).data[((IMG).step * (Y) + (IMG).channels()*(X)) + 2])
#define GU(IMG, X, Y)((IMG).data[((IMG).step * (Y) + (IMG).channels()*(X)) + 1])
#define BU(IMG, X, Y)((IMG).data[((IMG).step * (Y) + (IMG).channels()*(X)) + 0])
#define GRAY(IMG, X, Y)((IMG).data[(IMG).step * (Y) + X])

//enumdisplay.exe�Ŋm�F
const Point2f projectorCoordinate = Point2f(1920, 0);//�v���W�F�N�^�[�E�C���h�E�̍���̍��W 
const int projectorWindowWidth = 1280;	//�v���W�F�N�^�[�E�C���h�E��width
const int projectorWindowHeight = 1024;	//�v���W�F�N�^�[�E�C���h�E��height
const int numOfSquarecols = 9;	//�`�F�X�{�[�h�̍s�����̃R�[�i�[�̌�
const int numOfSquarerows = 7;		//�`�F�X�{�[�h�̗�����̃R�[�i�[�̌�
const Size patternSize(3, 5);


class FlyCap2CVcomposite
{
protected:
	FlyCapture2::Camera fcCamera;
	FlyCapture2::Error fcError;
	FlyCapture2::Image fcImg, bgrImg;
	FlyCapture2::CameraInfo fcCameraInfo;
	Mat matImg;
	

public:
	FlyCap2CVcomposite();		//constructor
	~FlyCap2CVcomposite();		//destructor
	Mat readImg();
	//Property settings
	void brightnessSetting(bool flag, float percent);		//brightness
	void exposureSetting(bool flag, float ev);			//exposure
	void sharpnessSetting(bool flag, float value);		//sharpness
	void hueSetting(bool flag, float deg);				//hue
	void saturationSetting(bool flag, float percent);		//saturation
	void gammaSetting(bool flag, float value);			//gamma
	void shutterSetting(bool flag, float ms);			//shutter
	void gainSetting(bool flag, float dB);				//gain
	void framerateSetting(bool flag, float fps);		//framerate
	void WBSetting(bool flag, float Rvalue, float Bvalue);		//whitebalance
/*focus, iris, etc  set physical camera*/
	bool checkError();

};

class cvOperation			//Image processing by OpenCV
{
public:
	Mat hsvImg, hImg, sImg, vImg;
	Mat ycrcbImg, yImg, crImg, cbImg;
	Mat threH, threS, threV;
	Mat threY, threCr, threCb;
	Mat zero;
	int Hthre, Sthre, Vthre, Ythre, Crthre, Cbthre;
	Mat closeH, closeS, closeV;
	Mat closeY, closeCr, closeCb;
	Mat cannyH, cannyS, cannyV;
	Mat cannyY, cannyCr, cannyCb;
	Mat maskImg, blurImg, threblurImg;
	Mat colorImg;

public:
	//cvOperation();			//constructor
	//~cvOperation();			//destructor
	void splitImg(Mat bgrImg);
	void thresholdImg();
	void closingImg();
	void ANDImg();
	void cannyImg();
	void createColorImg(Mat homograhyMatrix);
	void showFPS(Mat &img, Point &pt);
	//void settingWindow();

};

class cailbrationCameraProjector
{
private:
	Point2f beforeConversion[4];
	Point2f afterConversion[4];
	Mat homographyMatrix;

public:
	void projectChessBoard();	//�`�F�X�{�[�h�𓊉e����֐�
	void getChessCorner(Mat chessImage);	//�`�F�X�{�[�h�̃R�[�i�[���W�����߂�֐��@�����Ƀ`�F�X�{�[�h�摜
	void check();
	Mat evaluateHomography();	//�z���O���t�B�s������߂�֐�

};