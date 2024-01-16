#ifndef MAPGENERATOR_H
#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
#include"BootLoader.h"
#define MAPGENERATOR_H
using namespace std;

string cur_map;
int height;
int width;
extern char landscape[30][30];
extern int obstacle[15][2];

void creatFileOfMap(string na,int nu,int barr[15][2],int a,int b);
void loadMap();
bool ifBar(int x1,int y1,int b[15][2]);
void printNewMap(string m);
void InvalidInput1();
bool checkFileName1(int k,string m);
void loadOldMap();
void mapPrint(int x,int y,int bar[15][2],int wall[4]);
void mapInitialization();
void CreateNewMap();

void mapInitialization() {
	for(int p=0; p<=height+1; p++) {//行,y
		if(p==0||p==height+1) {
			for(int i=0; i<=width+1; i++) {//列,x
				landscape[p][i]='-';
			}
		} else {
			for(int i=1; i<=width; i++) {
				landscape[p][i]=' ';
			}
			landscape[p][0]='|';
			landscape[p][width+1]='|';
			for(int i=1; i<width+1; i++) {
				if(ifBar(i,p,obstacle)) {
					landscape[p][i]='|';
				}
			}
		}
	}
}
void creatFileOfMap(string na,int nu,int barr[15][2],int a,int b,int Iswall[4]) {
	ofstream ofs;
	ofs.open("map/allmaps.txt",ios::app);
	ofs<<na<<endl;
	ofs.close();
	ofs.open("map/curmap.txt");
	ofs<<na<<endl;
	ofs.close();
	na="map/"+na+".map";
	ofs.open(na);
	ofs<<a<<" "<<b<<" ";
	ofs<<nu<<" ";
	for(int i=0; i<nu; i++) {
		ofs<<barr[i][0]<<" "<<barr[i][1]<<" ";
	}
	for(int i=0; i<4; i++) {
		ofs<<Iswall[i]<<" ";
	}
	ofs.close();
}

void loadMap() {
	cout<<"---若想载入已有地图则按F---"<<endl<<"---创建新地图则按N---"<<endl<<"---返回菜单按Q---";
	char a='0';
	a=getch();
	if(a=='n') {
		CreateNewMap();
	} else if(a=='f') {
		loadOldMap();
	} else if(a=='q') {
		menu();
		MapGenerator();
	} else {
		clearScreen();
		cout<<"无效输入，1s后返回主菜单！"<<endl;
		menu();
		MapGenerator();
	}
}

extern bool ifBar(int x1,int y1,int b[15][2]) {
	for(int i=0; i<15; i++) {
		if(x1==b[i][0]&&y1==b[i][1]) {
			return true;
		}
	}
	return false;
}

void printNewMap(string m) {
	int x=0,y=0;
	int n=0;
	int Bar[15][2]= {0};
	int Wall[4]= {0};

	clearScreen();
	m="map/"+m+".map";
	ifstream ifs;
	ifs.open(m);
	if(!ifs) {
		cout<<"An error occurred when opening map "<<m<<" !"<<endl;
		cout<<"Please check if it`s spelled correctly or try again!";
	} else {
		ifs>>x;
		ifs>>y;
		ifs>>n;
		for(int i=0; i<n; i++) {
			ifs>>Bar[i][0];
			ifs>>Bar[i][1];
		}
		ifs>>Wall[0]>>Wall[1]>>Wall[2]>>Wall[3];
	}
	mapPrint(x,y,Bar,Wall);
	ifs.close();
}


void InvalidInput1() { //在非法输入时出现的界面
	string configUI[13];
	for(int i=0; i<13; i++) {
		configUI[i]="|                         |";
	}
	configUI[0]="---------------------------";
	configUI[12]="---------------------------";
	clearScreen();
	for(int i=0; i<13; i++) {
		if(i==5) {
			cout<<"| 先前输入不符合输入规范  |"<<endl;
		} else if(i==6) {
			cout<<"|  将在一秒后返回主菜单   |"<<endl;
		} else {
			cout<<configUI[i]<<endl;
		}
	}
	Sleep(2000);
	menu();
	MapGenerator();
}
bool checkFileName1(int k,string m) {
	for(int i=0; i<k; i++) {
		if((m[i]=='_')||((m[i]>=65)&&(m[i]<=90))||((m[i]>=97)&&(m[i]<=122))||((m[i]>=48)&&(m[i]<=57))) {
		} else {
			return false;//合法的输入只能包括英语字母、下划线和数字
		}
	}
	return true;
}

void loadOldMap() {
	restoreTerminal();
	clearScreen();
	string input;
	string names;
	ifstream fin1;
	char a='0';

	fin1.open("map/allmaps.txt");
	cout<<"当前地图为："<<cur_map<<endl;
	cout<<"已创建的地图如下，请输入地图名字以载入："<<endl;
	while(getline(fin1,names)) {
		cout<<"#MAP NAME# "<<names<<endl;
	}
	fin1.close();
	getline(cin,input);
	fin1.open("map/"+input+".map");
	if(fin1) {
		fin1.close();
		ofstream fout;
		fout.open("map/curmap.txt");
		fout<<input;
		fout.close();//change curmap
		cur_map=input;
		printNewMap(cur_map);
		cout<<"载入成功！"<<endl;
		cout<<"———————地图预览———————"<<endl<<"————2s后返回主菜单————"<<endl;
		Sleep(2000);
		menu();
		MapGenerator();
	} else {
		cout<<"无效输入"<<endl<<"你可以选择：1.按下N重新选择"<<endl<<"2.按下其他任意键返回主菜单"<<endl;
		a=getch();
		if(a=='n') {
			loadOldMap();
		} else {
			menu();
			MapGenerator();
		}
	}
}


void mapPrint(int x,int y,int bar[15][2],int wall[4]) {
	for(int p=0; p<y+2; p++) {
		if(p==0) {
			if(wall[0]==1) {
				for(int i=1; i<=x+2; i++) {
					cout<<"\033[37;41m"<<'_'<<"\033[0m";
				}
				cout<<endl;
			} else if(wall[0]==0) {
				for(int i=1; i<=x+2; i++) {
					cout<<"\033[37;42m"<<'_'<<"\033[0m";
				}
				cout<<endl;
			}
		} else if(p==y+1) {
			if(wall[1]==1) {
				for(int i=1; i<=x+2; i++) {
					cout<<"\033[37;41m"<<'_'<<"\033[0m";
				}
				cout<<endl;
			} else if(wall[1]==0) {
				for(int i=1; i<=x+2; i++) {
					cout<<"\033[37;42m"<<'_'<<"\033[0m";
				}
				cout<<endl;
			}
		} else {
			if((wall[2]==1)&&(wall[3]==1)) {
				cout<<"\033[37;41m"<<'|'<<"\033[0m";
				for(int i=1; i<x+1; i++) {
					if(ifBar(i,p,bar)) {
						cout<<"\033[37;41m"<<'|'<<"\033[0m";
					} else {
						cout<<' ';
					}
				}
				cout<<"\033[37;41m"<<'|'<<"\033[0m"<<endl;
			} else if((wall[2]==1)&&(wall[3]==0)) {
				cout<<"\033[37;41m"<<'|'<<"\033[0m";
				for(int i=1; i<x+1; i++) {
					if(ifBar(i,p,bar)) {
						cout<<"\033[37;41m"<<'|'<<"\033[0m";
					} else {
						cout<<' ';
					}
				}
				cout<<"\033[37;42m"<<'|'<<"\033[0m"<<endl;
			} else if((wall[2]==0)&&(wall[3]==0)) {
				cout<<"\033[37;42m"<<'|'<<"\033[0m";
				for(int i=1; i<x+1; i++) {
					if(ifBar(i,p,bar)) {
						cout<<"\033[37;41m"<<'|'<<"\033[0m";
					} else {
						cout<<' ';
					}
				}
				cout<<"\033[37;42m"<<'|'<<"\033[0m"<<endl;
			} else if((wall[2]==0)&&(wall[3]==1)) {
				cout<<"\033[37;42m"<<'|'<<"\033[0m";
				for(int i=1; i<x+1; i++) {
					if(ifBar(i,p,bar)) {
						cout<<"\033[37;41m"<<'|'<<"\033[0m";
					} else {
						cout<<' ';
					}
				}
				cout<<"\033[37;41m"<<'|'<<"\033[0m"<<endl;
			}
		}
	}
}
void CreateNewMap() {
	restoreTerminal();
	clearScreen();
	int len=0;
	cin.clear();
	string name;
	int x0=0;
	int y0=0;
	int bar=0;
	int isWall[4]= {0};
	int barrier[15][2]= {0};
	cout<<"输入地图名称（包含字母，数字或下划线）"<<endl;
	cin>>name;
	len=name.length();
	if(!checkFileName1(len,name)) {
		clearScreen();
		InvalidInput1();
	} else {
		cout<<"输入地图宽度和高度（以空格隔开，且8<=input<=25）"<<endl;
		cin>>x0>>y0;
		if(x0<8||x0>25||y0<8||y0>25) {
			InvalidInput1();
		} else {
			clearScreen();
			mapPrint(x0,y0,barrier,isWall);
			cout<<"请输入四面墙的虚实情况（顺序为：上下左右，0表示虚，1表示实）"<<endl;
			cin>>isWall[0]>>isWall[1]>>isWall[2]>>isWall[3];
			for(int i=0; i<4; i++) {
				if(isWall[i]!=0&&isWall[i]!=1) {
					InvalidInput1();
				}
			}
			clearScreen();
			mapPrint(x0,y0,barrier,isWall);
			cout<<"请输入障碍物个数，0<=N<=15"<<endl;
			cin>>bar;
			if(bar>=0&&bar<=15) {
				for(int q=0; q<bar; q++) {
					cout<<"输入第 "<<q+1<<" 个障碍的位置"<<endl;
					cin>>barrier[q][0]>>barrier[q][1];
				}
				clearScreen();
				mapPrint(x0,y0,barrier,isWall);
				creatFileOfMap(name,bar,barrier,x0,y0,isWall);
				cout<<"自定义地图“ "<<name<<" ”创建成功！游戏即将载入"<<endl<<"Loading...";
				cur_map=name;
				ofstream ofs;
				ofs.open("map/curmap.txt");
				ofs<<cur_map<<endl;
				ofs.close();
				cout<<"——————地图预览——————按任意键返回主菜单"<<endl;
				getch();
				menu();
				MapGenerator();
			} else {
				InvalidInput1();
			}
		}
	}

}
#endif