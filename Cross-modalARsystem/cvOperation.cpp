#include"FlyCap2CVcomposite.h"


void cvOperation::splitImg(cv::Mat bgrImg)
{
	/*
	zero.create(bgrImg.size(), CV_8UC1);		//create Image filled zero
	zero.setTo(0);
	*/
	cvtColor(bgrImg, hsvImg, CV_BGR2HSV);		//create HSV Image
	cvtColor(bgrImg, ycrcbImg, CV_BGR2YCrCb);		//create YCrCb Image


	vector<Mat> ch(3);

	split(hsvImg, ch);
	//[0].convertTo(hImg, CV_8UC1);
	ch[1].convertTo(sImg, CV_8UC1);
	//ch[2].convertTo(vImg, CV_8UC1);

	//split(ycrcbImg, ch);
	//ch[0].convertTo(yImg, CV_8UC1);
	//ch[1].convertTo(crImg, CV_8UC1);
	ch[2].convertTo(cbImg, CV_8UC1);

	
	return;
}

void cvOperation::thresholdImg()
{
	
	//threshold(hImg, threH, Hthre, 255, THRESH_BINARY_INV);
	threshold(sImg, threS, Sthre, 255, THRESH_BINARY);
	//threshold(vImg, threV, Vthre, 255, THRESH_BINARY);
	//threshold(yImg, threY, Ythre, 255, THRESH_BINARY);
	//threshold(crImg, threCr, Crthre, 255, THRESH_BINARY);
	threshold(cbImg, threCb, Cbthre, 255, THRESH_BINARY);

	
	
	//imshow("Himage", threH);
	imshow("Simage", threS);
	//imshow("Vimage", threV);
	//imshow("Yimage", threY);
	//imshow("Crimage", threCr);
	imshow("Cbimage", threCb);


	return;
}

void cvOperation::closingImg()
{
	//morphologyEx(threH, closeH, MORPH_CLOSE, Mat(), Point(-1, -1), 3, BORDER_REPLICATE);
	//imshow("closingH", closeH);
	morphologyEx(threS, closeS, MORPH_CLOSE, Mat(), Point(-1, -1), 3, BORDER_REPLICATE);
	//imshow("closingS", closeS);
	//(threV, closeV, MORPH_CLOSE, Mat(), Point(-1, -1), 3, BORDER_REPLICATE);
	//imshow("closingV", closeV);
	//morphologyEx(threY, closeY, MORPH_CLOSE, Mat(), Point(-1, -1), 3, BORDER_REPLICATE);
	//imshow("closingY", closeY);
	//morphologyEx(threCr, closeCr, MORPH_CLOSE, Mat(), Point(-1, -1), 3, BORDER_REPLICATE);
	//imshow("closingCr", closeCr);
	morphologyEx(threCb, closeCb, MORPH_CLOSE, Mat(), Point(-1, -1), 3, BORDER_REPLICATE);
	//imshow("closingCb", closeCb);
}

void cvOperation::ANDImg()
{
	closeS.copyTo(maskImg);

	for (int py = 0; py < closeCb.rows; py++)
	{
		for (int px = 0; px < closeCb.cols; px++)
		{
			/*if ((GRAY(closeS, px, py) == 255) && (GRAY(closeCb, px, py) == 255))
			{
				GRAY(maskImg, px, py) = 255;
			}
			else
			{
				GRAY(maskImg, px, py) = 0;
			}*/
			if (GRAY(closeCb, px, py) == 255) {
				GRAY(maskImg, px, py) = 255;
			}
			else {
				GRAY(maskImg, px, py) = 0;
			}
		}
	}

	blur(maskImg, blurImg, Size(15, 15));
	//imshow("GaussianImage", blurImg);
	threshold(blurImg, threblurImg, 100, 255, THRESH_BINARY);
	//imshow("threshold blur Image", threblurImg);
	
	
	showFPS(maskImg, Point(20, 40));

	imshow("MaskImage", maskImg);
}
void cvOperation::cannyImg()
{
	//Canny(closeH, cannyH, 50, 150, 3);
	//imshow("cannyH", cannyH);
	Canny(closeS, cannyS, 50, 150, 3);
	imshow("cannyS", cannyS);
	//Canny(closeV, cannyV, 50, 150, 3);
	//imshow("cannyV", cannyV);
	//Canny(closeY, cannyY, 50, 150, 3);
	//imshow("cannyY", cannyY);
	//Canny(closeCr, cannyCr, 50, 150, 3);
	//imshow("cannyCr", cannyCr);
	Canny(closeCb, cannyCb, 50, 150, 3);
	imshow("cannyCb", cannyCb);
}

void cvOperation::createColorImg(Mat homograhyMatrix)
{
	//colorImg = Mat(1024, 1280, CV_8UC3);

	//for (int py = 0; py < 1024; py++)
	//{
	//	for (int px = 0; px < 1280; px++)
	//	{
	//		if (GRAY(maskImg, px, py) != 0 )
	//		{
	//			RU(colorImg, px, py) = 255;		//Image of projection mapping
	//			GU(colorImg, px, py) = 255;
	//			BU(colorImg, px, py) = 255;
	//		}
	//		else
	//		{
	//			RU(colorImg, px, py) = 0;		
	//			GU(colorImg, px, py) = 0;
	//			BU(colorImg, px, py) = 0;
	//		}
	//	}
	//}

	warpPerspective(maskImg, colorImg, homograhyMatrix, Size(projectorWindowWidth, projectorWindowHeight));

	namedWindow("ColorImage", /*CV_WINDOW_NORMAL || */CV_GUI_NORMAL);
	cvResizeWindow("ColorImage", projectorWindowWidth, projectorWindowHeight);
	cvSetWindowProperty("ColorImage", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
	cvMoveWindow("ColorImage", int(projectorCoordinate.x), int(projectorCoordinate.y));
	imshow("ColorImage", colorImg);

	return;

}

void cvOperation::showFPS(Mat &img, Point &pt)
{
	static int64 startTick, endTick, diffTick;

	endTick = getTickCount();
	diffTick = endTick - startTick;
	startTick = getTickCount();

	double msec = 1000.0 * (diffTick / getTickFrequency());
	double fps = 1000.0 / msec;

	stringstream st;
	st.precision(1);
	st << fixed << msec << "[ms] " << fps << "[fps]";
	putText(img, st.str(), pt, FONT_HERSHEY_SIMPLEX, 1, Scalar(100, 100, 100), 2, LINE_AA);
}

