#include "stdafx.h"
#include "YAPP!.h"
#include "gamedef.h"

void menu(int *menu_arrow, Player *player, int reset, int *stage, int menu_select){
	
	switch(menu_select){
		case BEGIN:
			for(int i=0; i<2; i++){
				player[i].left = 130;
				player[i].right = 130+BOXSIZE;
				player[i].top = 100;
				player[i].bottom = 100+BOXSIZE;
				player[i].life = 1;
			}
			stage[0] = TUTORIAL1;
			break;
		case SAVE:
			savePoint(0, 0, 0, 0, player, stage, RESET);
			break;
		case EXIT:
			exit(0);
	}
}
void DrawMenu(HDC hdc, HDC backDC, HINSTANCE hInst, int *menu_arrow){
	HDC BackgroundDC, BeginDC, SaveDC, ExitDC, PlayerDC;
	HBITMAP Backgroundbit, Beginbit, Savebit, Exitbit, Playerbit;
	BackgroundDC = CreateCompatibleDC(hdc);
	BeginDC = CreateCompatibleDC(hdc);
	SaveDC = CreateCompatibleDC(hdc);
	ExitDC = CreateCompatibleDC(hdc);
	PlayerDC = CreateCompatibleDC(hdc);
	Backgroundbit = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP14));
	Beginbit = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP16));
	Savebit = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP17));
	Exitbit = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP18));
	Playerbit = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
	SelectObject(BackgroundDC,Backgroundbit);
	SelectObject(BeginDC, Beginbit);
	SelectObject(SaveDC, Savebit);
	SelectObject(ExitDC, Exitbit);
	SelectObject(PlayerDC, Playerbit);
	BitBlt(backDC, 0, 0, 900, 600, BackgroundDC, 0, 0, SRCCOPY);
	TransparentBlt(backDC, 80, 230, 200, 200, BeginDC, 0, 0, 200, 200, RGB(255,255,255));
	TransparentBlt(backDC, 340, 230, 200, 200, SaveDC, 0, 0, 200, 200, RGB(255,255,255));
	TransparentBlt(backDC, 600, 230, 200, 200, ExitDC, 0, 0, 200, 200, RGB(255,255,255));
	switch(menu_arrow[0]){
		case BEGIN:
			TransparentBlt(backDC, 170, 470, PLAYERSIZE, PLAYERSIZE, PlayerDC, 0, 0, PLAYERSIZE, PLAYERSIZE, RGB(255,255,255));
			break;
		case SAVE:
			TransparentBlt(backDC, 430, 470, PLAYERSIZE, PLAYERSIZE, PlayerDC, 0, 0, PLAYERSIZE, PLAYERSIZE, RGB(255,255,255));
			break;
		case EXIT:
			TransparentBlt(backDC, 690, 470, PLAYERSIZE, PLAYERSIZE, PlayerDC, 0, 0, PLAYERSIZE, PLAYERSIZE, RGB(255,255,255));
			break;
	}
	DeleteObject(Backgroundbit);
	DeleteObject(Beginbit);
	DeleteObject(Savebit);
	DeleteObject(Exitbit);
	DeleteObject(Playerbit);
	DeleteDC(BackgroundDC);
	DeleteDC(BeginDC);
	DeleteDC(SaveDC);
	DeleteDC(ExitDC);
	DeleteDC(PlayerDC);
}