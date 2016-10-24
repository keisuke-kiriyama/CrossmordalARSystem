#include "FlyCap2CVcomposite.h"

using namespace FlyCapture2;

int main(void)
{
	
	FlyCap2CVcomposite pointGray;
	cvOperation imgOperation;
	cailbrationCameraProjector calibrationOperation;

	//チェスボード画像を投影
	calibrationOperation.projectChessBoard();

	//チェスボード画像をキャプチャ後8bitグレースケール化しキャリブレーション
	Mat chessboardImage = pointGray.readImg();
	Mat grayChessboardImage;
	//Mat threChessboardImage;
	cvtColor(chessboardImage, grayChessboardImage, CV_RGB2GRAY);
	//threshold(grayChessboardImage, threChessboardImage, 200, 255, THRESH_BINARY);
	calibrationOperation.getChessCorner(grayChessboardImage);
	
	calibrationOperation.check();
	//adopt channnel of Cr and V
	Mat homographyMatrix = calibrationOperation.evaluateHomography();

	//namedWindow("Himage");
	//createTrackbar("threshold", "Himage", &imgOperation.Hthre, 255);
	namedWindow("Simage");
	createTrackbar("threshold", "Simage", &imgOperation.Sthre, 255);
	//namedWindow("Vimage");
	//createTrackbar("threshold", "Vimage", &imgOperation.Vthre, 255);
	//namedWindow("Yimage");
	//createTrackbar("threshold", "Yimage", &imgOperation.Ythre, 255);
	//namedWindow("Crimage");
	//createTrackbar("threshold", "Crimage", &imgOperation.Crthre, 255);
	namedWindow("Cbimage");
	createTrackbar("threshold", "Cbimage", &imgOperation.Cbthre, 255);
	//setTrackbarPos("threshold", "Himage", 100);
	setTrackbarPos("threshold", "Simage", 93);
	//setTrackbarPos("threshold", "Vimage", 83);
	//setTrackbarPos("threshold", "Yimage", 67);
	//setTrackbarPos("threshold", "Crimage", 136);
	setTrackbarPos("threshold", "Cbimage", 254);

	char key = '0';
	while (key != 'q')
	{
		Mat img = pointGray.readImg();		//get origiinal image
		imshow("img", img);
		imgOperation.splitImg(img);			//sprit channel HSV and YCrCb
		imgOperation.thresholdImg();		//threshold each channel and show each channel
		imgOperation.closingImg();
		//imgOperation.cannyImg();
		imgOperation.showFPS(imgOperation.maskImg, Point(20, 40));
		imgOperation.ANDImg();
		imgOperation.createColorImg(homographyMatrix);
		//imshow("image", img);
		

		key = cv::waitKey(1);
	}
	return 0;
}


