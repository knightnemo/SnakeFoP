#ifndef REPLAY_H
#include<iostream>
#include<fstream>
#include"mapGenerator.h"
#define REPLAY_H
using namespace std;

extern int height;
extern int width;
extern int cur_score;
extern int Speed;
int recordData[100000]= {0};
extern int uselessCount;
extern int obstacle[15][2];
extern int IsTrueWall[4];
extern char landscape[30][30];
extern string cur_skinname;
extern vector<string> AllSkin;
extern Skin cur_skin;

void Replay(string na);
void replayMenu();
void printFood(int h,int l,int tempfood[10][3]);
void reserveRecord();
void BasicMapPrinter(int i,int j);


void Replay(string na) {
	clearScreen();
	ifstream fin;
	fin.open("record/"+na+".txt");
	for(int i=0; i<100000; i++) {
		fin>>recordData[i];
		if(recordData[i]==-1){
			break;
		}
	}
	char monitorQ;
	int clockTime=0;
	int order=3;
	width=recordData[0];
	height=recordData[1];
	uselessCount=recordData[2];
	for(int i=0; i<uselessCount; i++) {
		obstacle[i][0]=recordData[order];
		obstacle[i][1]=recordData[order+1];
		order+=2;
	}
	for(int i=0; i<4; i++) {
		IsTrueWall[i]=recordData[order];
		order++;
	}
	Speed=recordData[order++];
	int LimitTimer=400/Speed;
	while(recordData[order]!=-1) {
		while(1) {
			while(kbhit()) {
				monitorQ=getch();
				if(monitorQ=='q') {
					cout<<"退出回放";
					Sleep(1000);
					menu();
					MapGenerator();
				}
			}
			Sleep(LimitTimer/10);
			clockTime++;
			if(clockTime>=10) {
				break;
			}
		}
		int foodNumber=0;
		int scoreNow=0;
		int tempfood[10][3]= {0};
		//
		int flag=1;
		mapInitialization();
		while(flag) {//each print
			if(recordData[order]==3) {
				landscape[recordData[order+1]][recordData[order+2]]='#';
				order+=3;
			} else if(recordData[order]==8) {
				landscape[recordData[order+1]][recordData[order+2]]='*';
				order+=3;
			} else if(recordData[order]==2) {
				landscape[recordData[order+1]][recordData[order+2]]='@';
				tempfood[foodNumber][0]=recordData[order+1];
				tempfood[foodNumber][1]=recordData[order+2];//先行后列再大小
				tempfood[foodNumber][2]=recordData[order+3];
				foodNumber++;
				order+=4;
			} else if(recordData[order]==0) {
				flag=0;
				scoreNow=recordData[order+1];
				order+=3;
				//
				clearScreen();
				for(int i=0; i<=height+1; i++) {
					for(int j=0; j<=width+1; j++) {
						if(landscape[i][j]=='#') {
							cout<<"\033[37;42m"<<cur_skin.SnakeHead<<"\033[0m";
						} else if(landscape[i][j]=='*') {
							cout<<"\033[37;42m"<<cur_skin.SnakeBody<<"\033[0m";
						} else if(landscape[i][j]=='@') {
							for(int g=0; g<10; g++) {
								if(tempfood[g][0]==i&&tempfood[g][1]==j) {
									if(tempfood[g][2]==1) {
										cout<<"\033[37;44m"<<cur_skin.OneFood<<"\033[0m";
										break;
									} else if(tempfood[g][2]==2) {
										cout<<"\033[37;45m"<<cur_skin.TwoFood<<"\033[0m";
										break;
									} else if(tempfood[g][2]==3) {
										cout<<"\033[37;43m"<<cur_skin.ThrFood<<"\033[0m";
										break;
									}
								}
							}
						} else {
							BasicMapPrinter(i,j);
						}
					}
					if(i==0) {
						cout<<"     current score: "<<scoreNow;
					}
					cout<<endl;
				}
				Sleep(1600/Speed);
			} else {
				cout<<"error!将在1s后结束放映并返回菜单";
				Sleep(1000);
				menu();
				MapGenerator();
			}
		}
	}
	clearScreen();
	cout<<"回放结束，请按任意键返回"<<endl;
	getch();
	menu();
	MapGenerator();
}
void BasicMapPrinter(int i,int j) {
	if(i==0) {
		if(IsTrueWall[0]==0) {
			cout<<"\033[37;42m"<<cur_skin.IWall<<"\033[0m";
		} else {
			cout<<"\033[37;41m"<<cur_skin.RWall<<"\033[0m";
		}
	} else if(i==height+1) {
		if(IsTrueWall[1]==0) {
			cout<<"\033[37;42m"<<cur_skin.IWall<<"\033[0m";
		} else {
			cout<<"\033[37;41m"<<cur_skin.RWall<<"\033[0m";
		}
	} else if(j==0) {
		if(IsTrueWall[2]==0) {
			cout<<"\033[37;42m"<<cur_skin.IWall<<"\033[0m";
		} else {
			cout<<"\033[37;41m"<<cur_skin.RWall<<"\033[0m";
		}
	} else if(j==width+1) {
		if(IsTrueWall[3]==0) {
			cout<<"\033[37;42m"<<cur_skin.IWall<<"\033[0m";
		} else {
			cout<<"\033[37;41m"<<cur_skin.RWall<<"\033[0m";
		}
	} else {
		if(ifBar(j,i,obstacle)) {
			cout<<"\033[37;41m"<<cur_skin.RWall<<"\033[0m";
		} else {
			if(cur_skin.Wide){
			cout<<"  ";}
			else{
				cout<<" ";
			}
		}
	}
}


void replayMenu() {
	restoreTerminal();
	cout<<"当前可回放文件如下："<<endl;
	ifstream fin;
	fin.open("record/allrecords.txt");
	string a;
	while(getline(fin,a)) {
		cout<<a<<endl;
	}
	fin.close();
	cout<<endl<<"请输入想要回放的文件名：  "<<endl;
	cin>>a;
	if(a=="q") {
		menu();
		MapGenerator();
	} else {
		fin.open("record/"+a+".txt");
		if(!fin) {
			cout<<"错误的输入";
			Sleep(1000);
			clearScreen();
			replayMenu();
		} else {
			fin.close();
			cout<<"加载成功！即将播放"<<endl;
			Sleep(1000);
			Replay(a);
		}
	}

}

void reserveRecord() {
	restoreTerminal();
	clearScreen();
	ofstream fout;
	ifstream fin;
	cout<<"为该回放创建一个名字，由下划线、字母和数字组成:"<<endl;
	string a;
	cin>>a;
	int len=a.length();
	if(!checkFileName1(len,a)) {
		cout<<"不合法输入请重试";
		Sleep(1000);
		reserveRecord();
	} else {
		fin.open("record/tempRecord.txt");
		int i=0;
		do {
			fin>>recordData[i];
		}while(recordData[i++]!=-1);
		fin.close();
		fout.open("record/"+a+".txt");
		for(int i=0; i<100000; i++) {
			fout<<recordData[i]<<' ';
			recordData[i]=0;
		}
		fout<<-1;
		fout.close();
		fout.open("record/allrecords.txt",ios::app);
		fout<<a<<' ';
		fout.close();
		cout<<"回放文件："<<a<<" 保存成功！"<<endl;
		Sleep(1000);
	}
}

#endif