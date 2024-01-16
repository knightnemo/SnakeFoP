
#ifndef BOOTLOADER_H
#include <iostream>
#include<ctime>
#define BOOTLOADER_H
using namespace std;
extern char landscape[30][30];
void menu();
extern void Game();

void menu() {
	clearScreen();
	string menu[15];
	for(int i=0; i<15; i++) {
		menu[i]="|               |";
	}
	menu[0]="-----------------";
	menu[14]="-----------------";
	menu[2]="|   SNAKE_FoP   |";
	menu[4]="| 开始游戏请按g |";
	menu[5]="| 回放游戏请按r |";
	menu[6]="| 自动游戏请按v |";
	menu[7]="| 新建配置请按i |";
	menu[8]="| 配置载入请按u |";
	menu[9]="| 新建地图请按n |";
	menu[10]="| 地图载入请按m |";
	menu[11]="| 新建主题请按s |";
	menu[12]="| 主题载入请按x |";
	menu[13]="| 退出游戏请按q |";
	for(int i=0; i<15; i++) {
		cout<<menu[i]<<endl;
	}
	return;
}
#endif