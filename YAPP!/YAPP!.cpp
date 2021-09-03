#include "stdafx.h"
#include "YAPP!.h"
#include "gamedef.h"

#define MAX_LOADSTRING 100
// 전역 변수:
HINSTANCE hInst;	 // 현재 인스턴스입니다.
TCHAR szTitle[MAX_LOADSTRING];	 // 제목 표시줄 텍스트입니다.
TCHAR szWindowClass[MAX_LOADSTRING];	 // 기본 창 클래스 이름입니다.
// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM	 MyRegisterClass(HINSTANCE hInstance);
BOOL	 InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR lpCmdLine,
	_In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

// TODO: 여기에 코드를 입력합니다.
	MSG msg;
	HACCEL hAccelTable;

// 전역 문자열을 초기화합니다.
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_YAPP, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 응용 프로그램 초기화를 수행합니다.
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_YAPP));

	// 기본 메시지 루프입니다.
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
// 함수: MyRegisterClass()
//
// 목적: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style	 = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra	 = 0;
	wcex.cbWndExtra	 = 0;
	wcex.hInstance	 = hInstance;
	wcex.hIcon	 = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_YAPP));
	wcex.hCursor	 = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_YAPP);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm	 = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
// 함수: InitInstance(HINSTANCE, int)
//
// 목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
// 설명:
//
// 이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
// 주 프로그램 창을 만든 다음 표시합니다.
//

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
	CW_USEDEFAULT, 0, 920	, 700, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);

	UpdateWindow(hWnd);

	return TRUE;
}

//
// 함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
// 목적: 주 창의 메시지를 처리합니다.
//
// WM_COMMAND	- 응용 프로그램 메뉴를 처리합니다.
// WM_PAINT	- 주 창을 그립니다.
// WM_DESTROY	- 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int ac=0,j_flag=0,j_not=0;
	static float j_count1=0;
	static Player player[2]; //player[0]는 현재위치 player[1]은 전위치
	PAINTSTRUCT ps;
	static HANDLE hTimer, bTimer;
	static char map[HEIGHT][WIDTH]={};
	static int stage[1]={MENU}, trapKey[10];
	static TRAP trap[10];
	static MapBox mapbox[HEIGHT][WIDTH] = {0};
	int save[3] = {0};	 //save[0] = ac, save[1] = j_count1, save[2] = j_not
	HDC hdc, hBitDC, mapDC, backDC, charDC, BulletDC, LcharDC;
	HBITMAP hBit, mapbit, Bulletbit, Lcharbit;
	HBITMAP backbitmap;	 //기존에 dc에 저장된 BitMap을 다른곳에 보관 해주면서 새 BitMap을 dc에 저장한다.
	RECT rt={0,0,900,700};
	static int player_bullet_direction;
	static Bullet player_bullet[P_BULLET_MAX];
	static Enemy enemy[10];
	static int player_bullet_count[1] = {0};
	static int enemy_count[1] = {0};
	static int reset=0;
	static int menu_select = 0;
	static int menu_arrow[1] = {1}; //1 = 처음하기, 2 = 이어하기, 3 = 끝내기
	static int die_check = 0;
	static int save_stage = MENU;
	static int first[1]={0};

	

	save[0] = ac;
	save[1] = j_count1;
	save[2] = j_not;

	if((stage[0] != save_stage) || player[0].life == 0){
		for(int i=0; i<player_bullet_count[0]; i++){
			player_bullet[i].direction = 0;
			player_bullet[i].left = 0;
			player_bullet[i].right = 0;
			player_bullet[i].top = 0;
			player_bullet[i].bottom = 0;
		}
		for(int i=0; i<enemy_count[0]; i++){
			enemy[i].HP = 0;
			enemy[i].left = 0;
			enemy[i].right = 0;
			enemy[i].top = 0;
			enemy[i].bottom = 0;
		}
		player_bullet_count[0] = 0;
		enemy_count[0] = 0;
		save_stage = stage[0];
		first[0] = 0;
	}

	switch(stage[0])
	{
		case MENU:
			menu(menu_arrow, player, reset, stage, menu_select);
			if(die_check == 1){
			die_check = 0;
			menu_select = 0;
			stage[0] = MENU;
		}
			break;
		
		case TUTORIAL1:
			if(die_check == 1 && GetAsyncKeyState('R')<0)
			{
				die_check = 0;
				menu_arrow[0] = 1;
				menu_select = 0;
				reset = RESET;
				stage[0] = MENU;
			}
			tuto(player, save, map,trap,stage, mapbox,&reset, player_bullet, player_bullet_count, enemy, enemy_count, first);
			break;
		case TUTORIAL2:
			if(die_check == 1 && GetAsyncKeyState('R')<0){
				die_check = 0;
				menu_arrow[0] = 1;
				menu_select = 0;
				stage[0] = MENU;
			}
			tuto2(player,save,map,trap, stage, mapbox, &reset, player_bullet, player_bullet_count, enemy, enemy_count, first);
			break;
		case STAGE1_1:
			if(die_check == 1 && GetAsyncKeyState('R')<0) {
				die_check = 0;
				menu_arrow[0] = 1;
				menu_select = 0;
				stage[0] = MENU;
			}
			stage1(player,save,map,trap, stage, mapbox, &reset, first, player_bullet, player_bullet_count, enemy, enemy_count);
			break;
	}
	

	ac = save[0];
	j_count1 = save[1];
	j_not = save[2];


	if(player[0].life!=0)
	{
		if(GetAsyncKeyState(VK_LEFT) < 0) //왼쪽ㄱㄱ
		{
			if(LR_Crash(map, player, mapbox, LEFT)) // if 문 추가로 입력
			{
				player_bullet_direction = WW;
				player[1].left = player[0].left;
				player[0].left -= 2;
				player[1].right = player[0].right;
				player[0].right -= 2;
			}
		}
		if(GetAsyncKeyState(VK_RIGHT) < 0) //오른쪽 ㄱㄱ
		{
			if(LR_Crash(map, player, mapbox, RIGHT))//if 문 추가로 입력 (이동 불가하게 만듬)
			{
				player_bullet_direction = EE;
				player[1].left = player[0].left;
				player[0].left += 2;
				player[1].right = player[0].right;
				player[0].right += 2;
			}
		}
	}
	
	Bullet_delete(rt, player_bullet, player_bullet_count);
	switch (message)
	{
		
		case WM_CREATE :
			hTimer=(HANDLE)SetTimer(hWnd,AC_TIMER_ID,50,NULL);
			bTimer=(HANDLE)SetTimer(hWnd, BULLET_TIMER_ID, 50, NULL); //총알 타이머
			return 0;


		case WM_KEYDOWN:
			switch(wParam)
			{
				if(stage[0] == MENU){
					case VK_LEFT:
						if(menu_arrow[0] > 1){
							menu_arrow[0]--;
						}
						return FALSE;
					case VK_RIGHT:
						if(menu_arrow[0] < 3){
							menu_arrow[0]++;
						}
						return FALSE;
					case VK_RETURN:
						menu_select = menu_arrow[0];
						return FALSE;
				}
				case 'z': //위누르면 점프 2단까지 허용
				case 'Z':
					if(player[0].life==1 && j_count1<2 && j_not<1.1)
					{
						SetTimer(hWnd,JUMP_TIMER_ID,50,NULL);
						j_not++;
						ac=0;
						j_flag=0;
						j_count1+=1;
					//	InvalidateRect(hWnd,NULL,false);
					}
					return false;
				case 'X':
				case 'x':
					player_bullet[player_bullet_count[0]].direction = player_bullet_direction;
					if(player_bullet[player_bullet_count[0]].direction == WW){
						player_bullet[player_bullet_count[0]].right = player[0].left;
						player_bullet[player_bullet_count[0]].left = player_bullet[player_bullet_count[0]].right - P_BULLETSIZE;
					}
					else if(player_bullet[player_bullet_count[0]].direction == EE){
						player_bullet[player_bullet_count[0]].left = player[0].right;
						player_bullet[player_bullet_count[0]].right = player_bullet[player_bullet_count[0]].left + P_BULLETSIZE;
					}
					player_bullet[player_bullet_count[0]].top = player[0].top + 11;
					player_bullet[player_bullet_count[0]].bottom = player_bullet[player_bullet_count[0]].top + P_BULLETSIZE;
					player_bullet_count[0]++;
					
					return false;

			}

			if(stage[0] != MENU){
				case WM_TIMER:
					switch(wParam)
					{
					case AC_TIMER_ID: //중력처리
						if(ac<17)
							ac+=2;
						player[1].top = player[0].top;
						player[0].top+=ac;
						player[1].bottom = player[0].bottom;
						player[0].bottom+=ac;
						InvalidateRect(hWnd,NULL,FALSE);
						return false;

					case JUMP_TIMER_ID:
						player[1].top = player[0].top;
						player[0].top-=(20-j_flag*3); //점프 올라갈수록 느려짐
						player[1].bottom = player[0].bottom;
						player[0].bottom-=(20-j_flag*3);
						j_flag++;
						if(j_flag==4)
							KillTimer(hWnd,2); 
						return false;
					

					case BULLET_TIMER_ID:
						for(int i=0; i<player_bullet_count[0]; i++){
							if(player_bullet[i].direction == WW){
								player_bullet[i].right -= 15;
								player_bullet[i].left = player_bullet[i].right - P_BULLETSIZE;
							}
							else if(player_bullet[i].direction == EE){
								player_bullet[i].left += 15;
								player_bullet[i].right = player_bullet[i].left + P_BULLETSIZE;
							}
						}
						return false;
					}
			}


		case WM_PAINT:

			hdc = BeginPaint(hWnd, &ps);
			backDC=CreateCompatibleDC(hdc);
			backbitmap = CreateCompatibleBitmap(hdc,rt.right,rt.bottom);
			hBitDC = CreateCompatibleDC(hdc);
			mapDC = CreateCompatibleDC(hdc);
			charDC = CreateCompatibleDC(hdc);
			BulletDC = CreateCompatibleDC(hdc);
			LcharDC = CreateCompatibleDC(hdc);
			if(player[0].life==1){
				mapbit=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP1));
				Lcharbit = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP19));
			}
			else{
				mapbit=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP3));
				Lcharbit = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP3));
				die_check = 1;
			}
			if(stage[0]/10==TUTO)
				hBit = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));
			else if(stage[0]/10==STAGE1)
				hBit = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP7));
			else
				hBit = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP7));
			Bulletbit = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP15));
			SelectObject(backDC, backbitmap);
			SelectObject(mapDC,hBit);
			SelectObject(charDC,mapbit);
			SelectObject(BulletDC, Bulletbit);
			SelectObject(backDC,hBit);
			SelectObject(LcharDC, Lcharbit);
			FillRect(backDC, &rt, (HBRUSH)GetStockObject(WHITE_BRUSH));
			switch(stage[0]/10)
			{
				case MENU:
					DrawMenu(hdc, backDC, hInst, menu_arrow);
					break;
				case TUTO:
					DrawBlockTuto(hdc,backDC,mapDC,trap,stage,hInst,map);
					break;
				case STAGE1:
					DrawBlockStage1(hdc,backDC,mapDC,trap,stage,hInst,map);
					break;
			}
			if(stage[0] != MENU){
			for(int i=0; i<player_bullet_count[0]; i++){
				TransparentBlt(backDC, player_bullet[i].left-BOXSIZE, player_bullet[i].top-BOXSIZE, P_BULLETSIZE, P_BULLETSIZE, BulletDC, 0, 0, P_BULLETSIZE, P_BULLETSIZE, RGB(255,255,255));
				//TextOut(backDC, player_bullet[i].left, player_bullet[i].top, B, strlen(B)); 
			}
				if(player_bullet_direction == EE){
					TransparentBlt(backDC, player[0].left-BOXSIZE, player[0].top-BOXSIZE, PLAYERSIZE, PLAYERSIZE, charDC, 0, 0,PLAYERSIZE,PLAYERSIZE, RGB(255,255,255));
				}
				else{
				TransparentBlt(backDC, player[0].left-BOXSIZE, player[0].top-BOXSIZE, PLAYERSIZE, PLAYERSIZE, LcharDC, 0, 0,PLAYERSIZE,PLAYERSIZE, RGB(255,255,255));
				}
			}

		BitBlt(hdc,0,0,rt.right,rt.bottom,backDC,0,0,SRCCOPY);
// TODO: 여기에 그리기 코드를 추가합니다.
// SelectObject(hBitDC, hOldBit); 
// SelectObject(mapDC, holdmap); 
		DeleteObject(backbitmap);
		DeleteObject(hBit);
		DeleteObject(Bulletbit);
		DeleteObject(mapbit);
		DeleteObject(Lcharbit);
		DeleteDC(hBitDC);
		DeleteDC(backDC);
		DeleteDC(BulletDC);
		DeleteDC(hdc);
		DeleteDC(mapDC);
		DeleteDC(charDC);
		DeleteDC(LcharDC);
		

		EndPaint(hWnd, &ps);
		return FALSE;
	case WM_DESTROY:
		KillTimer(hWnd, AC_TIMER_ID);
		KillTimer(hWnd, BULLET_TIMER_ID);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}