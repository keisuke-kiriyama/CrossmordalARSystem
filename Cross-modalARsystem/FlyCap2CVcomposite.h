#pragma once

#ifdef _WIN32
#define WIN32
#endif
#ifdef _WIN64
#define WIN64
#endif

#define SQUAREVERTICAL 2	//チェスボードのマスの数縦
#define SQUAREHORIZONTAL 3	//チェスボードのマスの数横
#define PARTITION 7		//widthの分割個数
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

//enumdisplay.exeで確認
const Point2f projectorCoordinate = Point2f(1920, 0);//プロジェクターウインドウの左上の座標 
const int projectorWindowWidth = 1280;	//プロジェクターウインドウのwidth
const int projectorWindowHeight = 1024;	//プロジェクターウインドウのheight
const int numOfSquarecols = 9;	//チェスボードの行方向のコーナーの個数
const int numOfSquarerows = 7;		//チェスボードの列方向のコーナーの個数
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
	void projectChessBoard();	//チェスボードを投影する関数
	void getChessCorner(Mat chessImage);	//チェスボードのコーナー座標を求める関数　引数にチェスボード画像
	void check();
	Mat evaluateHomography();	//ホモグラフィ行列を求める関数

};