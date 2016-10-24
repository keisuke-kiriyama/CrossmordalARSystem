//	�}���`���j�^�[�̏����擾�v���O����
//	Visual C++ 2013 32/64bit

#include <windows.h>
#include <stdio.h>
#include <tchar.h>

//	�ڑ�����Ă��郂�j�^�̏������i�[
struct MONITORS{
	int max;	//	���j�^�[��
	RECT* rect;	//	�e���j�^�[�̍��W
	MONITORS(){
		max = 0;
		rect = 0;
	}
};

TCHAR szClassNme[] = TEXT("EnumDisplay");

//	�E�B���h�E�v���V�[�W���[
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
//	�N���C�A���g�T�C�Y�̕ύX
void SetClientWindowSize(HWND hWnd, int cx, int cy);
//	EnumDisplayMonitors�֐����烂�j�^�[���ɌĂяo�����֐�
BOOL CALLBACK myinfoenumproc(HMONITOR hMon, HDC hdcMon, LPRECT lpMon, LPARAM dwDate);


int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPreInst,
					 TCHAR* lpszCmdLine, int nCmdShow){
	HWND hWnd;
	MSG lpMsg;
	WNDCLASS wc;

	if (!hPreInst) {
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInstance;
		wc.hIcon = NULL;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH__ *)GetStockObject(WHITE_BRUSH);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = szClassNme;
		if (!RegisterClass(&wc))
			return FALSE;
	}
	hWnd = CreateWindow(szClassNme,
		TEXT("EnumDisplay"),
		WS_OVERLAPPED	//	�I�[�o�[���b�v�E�B���h�E �^�C�g���o�[�Ƙg
		| WS_CAPTION	//	�^�C�g���o�[�Ƙg
		| WS_SYSMENU	//	�^�C�g���o�[��ɃE�B���h�E���j���[
		| WS_MINIMIZEBOX,	//	�ŏ����{�^��
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	while (GetMessage(&lpMsg, NULL, 0, 0)) {
		TranslateMessage(&lpMsg);
		DispatchMessage(&lpMsg);
	}
	return int(lpMsg.wParam);
}

//	�E�B���h�E�v���V�[�W���[

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
    HDC hdc;
    PAINTSTRUCT ps;
    int n;
	TCHAR buf[128];
	static MONITORS mon;	//	�e���j�^�[�̃T�C�Y���i�[
	static RECT vs;		//	���z�X�N���[���T�C�Y
	static double sx = 0.15;
	static double sy = 0.15;
	static int cx = 16;
	static int cy = 16;
    switch (msg) {
		case WM_CREATE:{
			n = GetSystemMetrics(SM_CMONITORS);	//	���j�^�[���擾
			mon.rect = new RECT[n];	//	���j�^�[�����̍��W�i�[�ϐ����m��
			//	�e���j�^�[�̍��W���擾
			EnumDisplayMonitors(NULL, NULL, (MONITORENUMPROC)myinfoenumproc, (LPARAM)&mon);
			//	���z�X�N���[���T�C�Y���擾
			vs.left = GetSystemMetrics(SM_XVIRTUALSCREEN);
			vs.top = GetSystemMetrics(SM_YVIRTUALSCREEN);
			vs.right = vs.left + GetSystemMetrics(SM_CXVIRTUALSCREEN);
			vs.bottom = vs.top + GetSystemMetrics(SM_CYVIRTUALSCREEN);
			int x = vs.right - vs.left;
			int y = vs.bottom - vs.top;
			//	�N���C�A���g�E�B���h�E�T�C�Y��ύX
			SetClientWindowSize(hWnd, cx * 2 + int(double(x*sx)), cy * 2 + int(double(y*sy)));
			break; 
		}
		case WM_PAINT:{
			HBRUSH hBrush, hOldBrush;
			hdc = BeginPaint(hWnd, &ps);
			int x=vs.right-vs.left;
			int y=vs.bottom-vs.top;

			Rectangle(hdc , cx , cy , int(double(x)*sx+cx) , int(double(y)*sy+cy));

			hBrush=CreateSolidBrush(RGB(92, 115, 169));
			hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

			SetBkMode(hdc, TRANSPARENT);	//	���ߏ���
			SetTextColor(hdc, RGB(255, 255, 255));

			for (n = 0; n < mon.max; n++) {
				int x1=int(double(mon.rect[n].left-vs.left)*sx)+cx;
				int y1 = int(double(mon.rect[n].top - vs.top)*sy) + cy;
				int x2 = int(double(mon.rect[n].right - vs.left)*sx) - 1 + cx;
				int y2 = int(double(mon.rect[n].bottom - vs.top)*sy) - 1 + cy;
				RECT rect;

				Rectangle(hdc , x1 , y1 ,	x2 , y2);

				_stprintf_s(buf, sizeof(buf) / sizeof(TCHAR), TEXT("���j�^�[%i"), n);
				rect.left = x1;	rect.right = x2;	rect.top = y1 -12; rect.bottom = y2 + 4;
				DrawText(hdc, buf, -1, &rect, DT_CENTER + DT_VCENTER + DT_SINGLELINE);

				_stprintf_s(buf, sizeof(buf) / sizeof(TCHAR), TEXT("%i*%i"), mon.rect[n].right - mon.rect[n].left, mon.rect[n].bottom - mon.rect[n].top);
				rect.left=x1;	rect.right=x2;	rect.top=y1+8; rect.bottom=y2+24;
				DrawText(hdc,buf,-1,&rect,DT_CENTER + DT_VCENTER + DT_SINGLELINE);
				_stprintf_s(buf, sizeof(buf) / sizeof(TCHAR), TEXT("%i,%i"), mon.rect[n].left, mon.rect[n].top);
				rect.left=x1+4;	rect.right=x2;	rect.top=y1+4; rect.bottom=y2;
				DrawText(hdc,buf,-1,&rect,DT_LEFT + DT_TOP + DT_SINGLELINE);

				_stprintf_s(buf, sizeof(buf) / sizeof(TCHAR), TEXT("%i,%i"), mon.rect[n].right, mon.rect[n].bottom);
				rect.left=x1;	rect.right=x2-4;	rect.top=y1; rect.bottom=y2-4;
				DrawText(hdc,buf,-1,&rect,DT_RIGHT + DT_BOTTOM + DT_SINGLELINE);
			}
			DeleteObject(hBrush);
			EndPaint(hWnd, &ps);
			break; 
		}
        case WM_DESTROY:
			delete [] mon.rect;
            PostQuitMessage(0);
            break;
        default:
            return(DefWindowProc(hWnd, msg, wParam, lParam));
    }
    return (0L);
}

//	�N���C�A���g�T�C�Y�̕ύX

void SetClientWindowSize(HWND hWnd, int cx, int cy){
	RECT wsize, csize;
	GetWindowRect(hWnd, &wsize);
	GetClientRect(hWnd, &csize);
	int nx, ny;
	nx = (wsize.right - wsize.left) - csize.right + cx;
	ny = (wsize.bottom - wsize.top) - csize.bottom + cy;
	SetWindowPos(hWnd, NULL, 0, 0, nx, ny, SWP_NOMOVE | SWP_NOZORDER);
}

//	EnumDisplayMonitors�֐����烂�j�^�[���ɌĂяo�����֐�

BOOL CALLBACK myinfoenumproc(HMONITOR hMon, HDC hdcMon, LPRECT lpMon, LPARAM dwDate){
	MONITORS* mon = (MONITORS*)dwDate;
	mon->rect[mon->max].bottom = lpMon->bottom;
	mon->rect[mon->max].left = lpMon->left;
	mon->rect[mon->max].top = lpMon->top;
	mon->rect[mon->max].right = lpMon->right;
	++mon->max;
	return TRUE;
}
