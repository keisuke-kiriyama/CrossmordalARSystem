#include "FlyCap2CVcomposite.h"

using namespace cv;
using namespace std;



void cailbrationCameraProjector::projectChessBoard() {

	Scalar black(0, 0, 0), white(255, 255, 255);
	int squareWidth = projectorWindowWidth / PARTITION;	//チェスボードの正方形の辺の長さ	

	Mat chessboard(Size(projectorWindowWidth, projectorWindowHeight), CV_8UC1, white);

	//チェスボードの描画
	Point2f upperLeft = Point2f(XUPPERLEFT, YUPPERLEFT);	//チェスボードの開始マスの左上の座標
	Point2f bottomRight = Point2f(0, 0);	//チェスボードの開始マスの右上の座標
	bool didDrawCenterSquare = false;

	for (int numOfSquareVertical = 0; numOfSquareVertical != SQUAREVERTICAL; upperLeft.y += (squareWidth * 2)) {
		numOfSquareVertical++;
		upperLeft.x = XUPPERLEFT;
		for (int numOfSquareHorizontal = 0; numOfSquareHorizontal != SQUAREHORIZONTAL; upperLeft.x += (squareWidth * 2)) {
			bottomRight.x = upperLeft.x + squareWidth;
			bottomRight.y = upperLeft.y + squareWidth;

			//チェスボードの正方形の描画
			rectangle(chessboard, upperLeft, bottomRight, black, FILLED);
			Point2f nextSquare = Point2f(bottomRight.x + squareWidth, bottomRight.y + squareWidth);
			rectangle(chessboard, bottomRight, nextSquare, black, FILLED);
			
			if (numOfSquareVertical == 2 && numOfSquareHorizontal == 1) {
				Point2f upperRight;
				Point2f bottomLeft;
				upperRight.x = upperLeft.x + squareWidth;
				upperRight.y = upperLeft.y;
				bottomLeft.x = upperLeft.x;
				bottomLeft.y = upperLeft.y + squareWidth;

				afterConversion[0] = upperLeft;
				afterConversion[1] = bottomLeft;
				afterConversion[2] = bottomRight;
				afterConversion[3] = upperRight;
			}
			
			numOfSquareHorizontal++;
		}
	}

	//for (int numOfSquareVertical = 0; numOfSquareVertical != SQUAREVERTICAL; upperLeft.y += (squareWidth * 2)) {
	//	numOfSquareVertical++;
	//	upperLeft.x = XUPPERLEFT;
	//	for (int numOfSquareHorizontal = 0; numOfSquareHorizontal != SQUAREHORIZONTAL; upperLeft.x += (squareWidth * 2)) {
	//		bottomRight.x = upperLeft.x + squareWidth;
	//		bottomRight.y = upperLeft.y + squareWidth;

	//		
	//		
	//		if (numOfSquareVertical == 2 && numOfSquareHorizontal == 1) {

	//			//チェスボードの正方形の描画
	//			rectangle(chessboard, upperLeft, bottomRight, black, FILLED);
	//			Point2f nextSquare = Point2f(bottomRight.x + squareWidth, bottomRight.y + squareWidth);
	//			rectangle(chessboard, bottomRight, nextSquare, black, FILLED);
	//			Point2f upperRight;
	//			Point2f bottomLeft;
	//			upperRight.x = upperLeft.x + squareWidth;
	//			upperRight.y = upperLeft.y;
	//			bottomLeft.x = upperLeft.x;
	//			bottomLeft.y = upperLeft.y + squareWidth;

	//			afterConversion[0] = upperLeft;
	//			afterConversion[1] = bottomLeft;
	//			afterConversion[2] = bottomRight;
	//			afterConversion[3] = upperRight;
	//		}
	//		
	//		numOfSquareHorizontal++;
	//	}
	//}
	
	//プロジェクターから投影するチェスボードの作成
	//ディスプレイの座標情報を得るときはenumdisplay.exeを実行

	cvNamedWindow("chessImage", CV_WINDOW_NORMAL);
	cvResizeWindow("chessImage", projectorWindowWidth, projectorWindowHeight);
	cvSetWindowProperty("chessImage", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
	cvMoveWindow("chessImage", int(projectorCoordinate.x), int(projectorCoordinate.y));

	imshow("chessImage", chessboard);

	waitKey();
	
	//キャリブレーション処理

	
}

void cailbrationCameraProjector::getChessCorner(Mat chessImage) {

	//close
	Mat closeChessImage;
	cv::Mat element(0, 0, CV_8U, cv::Scalar(1));
	erode(chessImage, closeChessImage, element, Point(-1, -1), 1);
	//morphologyEx(chessImage, closeChessImage, MORPH_OPEN, element, Point(-1, -1), 1);


	imshow("cheeeeeees", closeChessImage);
	//waitKey();

	//チェスボードコーナーの検出
	vector<Point2f> corners;	//検出されたコーナーを格納する
	bool patternfound = findChessboardCorners(closeChessImage, patternSize, corners, CV_CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE + CALIB_CB_FAST_CHECK);
	//bool patternfound = false;

	if (patternfound) {
		printf("success to find chessboard corners\n");
		//コーナーの座標の精度を高める
		cornerSubPix(closeChessImage, corners, Size(11, 11), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
		//コーナーを描画
		drawChessboardCorners(closeChessImage, patternSize, Mat(corners), patternfound);
		//コーナーの座標を確認
		const int numOfCorners = (patternSize.width * patternSize.height);
		for (int i = 0; i < numOfCorners; i++) {
			cout << i << " " << (int)(corners[i].x) << " " << (int)(corners[i].y) << endl;
		}

		//beforeConversionに格納
		beforeConversion[0] = corners[10];
		beforeConversion[1] = corners[11];
		beforeConversion[2] = corners[8];
		beforeConversion[3] = corners[7];

		imshow("findpattern", closeChessImage);
		waitKey();
	}
	else {
		printf("error to find chessboard corners");
		waitKey();
	}

}

void cailbrationCameraProjector::check() {
	for (int i = 0; i < 4; i++) {
		cout << "beforeConversion" << endl;
		cout << i << " " << (int)(beforeConversion[i].x) << " " << (int)(beforeConversion[i].y) << endl;
		cout << "afterConversion" << endl;
		cout << i << " " << (int)(afterConversion[i].x) << " " << (int)(afterConversion[i].y) << endl;
	}

	waitKey();
}

Mat cailbrationCameraProjector::evaluateHomography() {
	//ホモグラフィ行列を計算
	//homographyMatrix = getPerspectiveTransform(beforeConversion, afterConversion);
	homographyMatrix = getPerspectiveTransform(beforeConversion, afterConversion);
	//チェスボード画像の破棄
	destroyWindow("chessImage");

	waitKey();

	return homographyMatrix;
}