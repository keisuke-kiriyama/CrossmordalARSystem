#include "FlyCap2CVcomposite.h"

using namespace cv;
using namespace std;



void cailbrationCameraProjector::projectChessBoard() {

	Scalar black(0, 0, 0), white(255, 255, 255);
	int squareWidth = projectorWindowWidth / PARTITION;	//�`�F�X�{�[�h�̐����`�̕ӂ̒���	

	Mat chessboard(Size(projectorWindowWidth, projectorWindowHeight), CV_8UC1, white);

	//�`�F�X�{�[�h�̕`��
	Point2f upperLeft = Point2f(XUPPERLEFT, YUPPERLEFT);	//�`�F�X�{�[�h�̊J�n�}�X�̍���̍��W
	Point2f bottomRight = Point2f(0, 0);	//�`�F�X�{�[�h�̊J�n�}�X�̉E��̍��W
	bool didDrawCenterSquare = false;

	for (int numOfSquareVertical = 0; numOfSquareVertical != SQUAREVERTICAL; upperLeft.y += (squareWidth * 2)) {
		numOfSquareVertical++;
		upperLeft.x = XUPPERLEFT;
		for (int numOfSquareHorizontal = 0; numOfSquareHorizontal != SQUAREHORIZONTAL; upperLeft.x += (squareWidth * 2)) {
			bottomRight.x = upperLeft.x + squareWidth;
			bottomRight.y = upperLeft.y + squareWidth;

			//�`�F�X�{�[�h�̐����`�̕`��
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

	//			//�`�F�X�{�[�h�̐����`�̕`��
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
	
	//�v���W�F�N�^�[���瓊�e����`�F�X�{�[�h�̍쐬
	//�f�B�X�v���C�̍��W���𓾂�Ƃ���enumdisplay.exe�����s

	cvNamedWindow("chessImage", CV_WINDOW_NORMAL);
	cvResizeWindow("chessImage", projectorWindowWidth, projectorWindowHeight);
	cvSetWindowProperty("chessImage", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
	cvMoveWindow("chessImage", int(projectorCoordinate.x), int(projectorCoordinate.y));

	imshow("chessImage", chessboard);

	waitKey();
	
	//�L�����u���[�V��������

	
}

void cailbrationCameraProjector::getChessCorner(Mat chessImage) {

	//close
	Mat closeChessImage;
	cv::Mat element(0, 0, CV_8U, cv::Scalar(1));
	erode(chessImage, closeChessImage, element, Point(-1, -1), 1);
	//morphologyEx(chessImage, closeChessImage, MORPH_OPEN, element, Point(-1, -1), 1);


	imshow("cheeeeeees", closeChessImage);
	//waitKey();

	//�`�F�X�{�[�h�R�[�i�[�̌��o
	vector<Point2f> corners;	//���o���ꂽ�R�[�i�[���i�[����
	bool patternfound = findChessboardCorners(closeChessImage, patternSize, corners, CV_CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE + CALIB_CB_FAST_CHECK);
	//bool patternfound = false;

	if (patternfound) {
		printf("success to find chessboard corners\n");
		//�R�[�i�[�̍��W�̐��x�����߂�
		cornerSubPix(closeChessImage, corners, Size(11, 11), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
		//�R�[�i�[��`��
		drawChessboardCorners(closeChessImage, patternSize, Mat(corners), patternfound);
		//�R�[�i�[�̍��W���m�F
		const int numOfCorners = (patternSize.width * patternSize.height);
		for (int i = 0; i < numOfCorners; i++) {
			cout << i << " " << (int)(corners[i].x) << " " << (int)(corners[i].y) << endl;
		}

		//beforeConversion�Ɋi�[
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
	//�z���O���t�B�s����v�Z
	//homographyMatrix = getPerspectiveTransform(beforeConversion, afterConversion);
	homographyMatrix = getPerspectiveTransform(beforeConversion, afterConversion);
	//�`�F�X�{�[�h�摜�̔j��
	destroyWindow("chessImage");

	waitKey();

	return homographyMatrix;
}