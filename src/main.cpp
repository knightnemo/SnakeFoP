#include<iostream>
#include<string>
#include<vector>
#include<ctime>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include<cmath>
#include<ctime>
#include<random>
#include<cstdlib>
#include<iomanip>
#include<limits.h>
#include"Compatability.h"
#include"BootLoader.h"
#include"configGenerator.h"
#include"skin_editor.h"
#include"mapGenerator.h"
#include"replay.h"
#include"AI_gaming.h"

using namespace std;
struct position {
	int row;
	int col;
};
struct SnakeModel {
	string Snake_Skin;
	vector<position> snake;
	char direction;//朝向的方向
	int length;
};
struct Food {
	int foodlevel;
	position fdpos;
};

//basic data:
char landscape[30][30];
int cur_score=0;
SnakeModel Snakeyyyy;
vector<Food> snakefood;
extern int Speed;
extern int seed;
extern int foodNum;
extern float probability[3];
int food_on_screen=0;
bool isalive=true;
float step=0.0;
//config:
extern string cur_config;
vector<string> AllConfig;
//map:
extern int width;
extern int height;
int obstacle[15][2]= {0};
int IsTrueWall[4]= {0};
int uselessCount;
extern string cur_map;
//Skin
extern vector<string> AllSkin;
Skin cur_skin;

//function:
char getinput(int speed);
void MapGenerator();
void readyToPlay();
void record();
void IsFood(int x,int y);
void GenerateFood();
bool Collideself();
void CollideWall();
int checkFood(int i,int j);
void EndGame();
void Map_Change_Snake();
void MapPrint();
void ActiveMode(char m);
void Game();
void initialization();
void readyToPlayAI();
void AutoMode(char m);

char getinput(int speed) {
	char direc='0';
	float limittime=1000/speed;
	int n=0;
	while(true) {
		Sleep(10);
		while(kbhit()) {
			direc=getch();
		}
		n+=10;
		if(n>=limittime) {
			return direc;
		}
	}
}
void MapGenerator() {
	char p='0';
	do {
		Sleep(1);
	} while(!kbhit());
	while(kbhit()) {
		p=getch();
	}
	if(p=='g') {
		readyToPlay();
	} else if(p=='q') {
		clearScreen();
		abort;
	} else if(p=='i') {
		clearScreen();
		creatFile();
	} else if(p=='u') {
		clearScreen();
		ChooseFile();
	} else if(p=='m') {
		clearScreen();
		loadOldMap();
	} else if(p=='n') {
		clearScreen();
		CreateNewMap();
	} else if(p=='r') {
		readSkin("skin/"+cur_skinname+".skin");
		clearScreen();
		replayMenu();
	} else if(p=='s') {
		clearScreen();
		writeSkinFile();//新建skin配置
	} else if(p=='x') {
		clearScreen();
		ChooseSkinFile();//选择skin配置
	}else if(p=='v'){
		clearScreen();
		readyToPlayAI();
	} 
	else {
		MapGenerator();
		return;
	}
}


void readyToPlay() {
	clearScreen();
	ifstream fingame;
	//map
	fingame.open("map/"+cur_map+".map");
	fingame>>width;
	fingame>>height;
	fingame>>uselessCount;
	for(int i=0; i<15; i++) {
		obstacle[i][0]=0;
		obstacle[i][1]=0;
	}
	for(int temp=0; temp<uselessCount; temp++) {
		fingame>>obstacle[temp][0];
		fingame>>obstacle[temp][1];
	}
	for(int i=0; i<4; i++) {
		fingame>>IsTrueWall[i];
	}
	fingame.close();
	//config
	readFile("config/"+cur_config+".config");
	ofstream fout;
	fout.open("config/curConfig.txt");
	fout<<cur_config<<endl;
	fout.close();
	//skin
	readSkin("skin/"+cur_skinname+".skin");//读入主题
	fout.open("skin/curSkinName.txt");
	fout<<cur_skinname<<endl;
	fout.close();//写入当前主题
	//landscape change:
	mapInitialization();
	//game
	cout<<"正在为您粉刷控制台..."<<endl;
	Sleep(1000);
	for(int p=0; p<=height+1; p++) {
		for(int i=0; i<=width+1; i++) {
			cout<<landscape[p][i];
		}
		cout<<endl;
	}
	Sleep(300);
	cout<<"正在抓来一条赛博蛇..."<<endl;
	cout<<'@';
	for(int i=0; i<=5; i++) {
		cout<<'#';
		Sleep(100);
	}
	cout<<endl<<"正在把它饿瘦好让它变得贪吃..."<<endl<<'@';
	for(int i=0; i<=5; i++) {
		cout<<'*';
		Sleep(100);
	}
	cout<<endl;
	fout.open("record/tempRecord.txt");//start to record
	fout<<width<<' '<<height<<' ';
	fout<<uselessCount<<' ';
	for(int i=0; i<uselessCount; i++) {
		fout<<obstacle[i][0]<<' '<<obstacle[i][1]<<' ';
	}
	fout<<IsTrueWall[0]<<' '<<IsTrueWall[1]<<' '<<IsTrueWall[2]<<' '<<IsTrueWall[3]<<' ';
	fout<<Speed<<' ';
	fout.close();
	Sleep(1000);
	Game();
}

void record() {
	ofstream fout;
	fout.open("record/tempRecord.txt",ios::app);
	for(int y=1; y<=height; y++) {
		for(int x=1; x<=width; x++) {
			if(landscape[y][x]=='#') {
				fout<<3<<' '<<y<<' '<<x<<' ';
			} else if(landscape[y][x]=='*') {
				fout<<8<<' '<<y<<' '<<x<<' ';
			} else if(landscape[y][x]=='@') {
				fout<<2<<' '<<y<<' '<<x<<' ';
				for(int i=0; i<foodNum; i++) {
					if((y==snakefood[i].fdpos.col)&&(x==snakefood[i].fdpos.row)) {
						fout<<snakefood[i].foodlevel<<' ';
						break;
					}
				}
			}
		}
	}
	fout<<0<<' '<<cur_score<<' '<<0<<' ';
	fout.close();
}



void GenerateFood() {
	while(food_on_screen<foodNum) { //无论行、列还是食物种类都是伪随机生成的(而不是random device生成的)
		int rd2=rand();
		bool flag=0;
		int row_food= 1+(rd2%width);//食物的行(1,15)之间
		int rd3=rand();
		int col_food= 1+(rd3%height);//食物的列(1，15)之间
		if ((landscape[col_food][row_food]!='#')&&(landscape[col_food][row_food]!='*')&&(landscape[col_food][row_food]!='@')&&(landscape[col_food][row_food]!='|')) {
			snakefood[food_on_screen].fdpos.row=row_food;
			snakefood[food_on_screen].fdpos.col=col_food;
			landscape[col_food][row_food]='@';
			int rdfake=rand();
			float rank_food=rdfake/float(RAND_MAX);
			if(rank_food<probability[0]) {
				snakefood[food_on_screen].foodlevel=1;//1分食物：蓝色
			} else if(rank_food<(probability[0]+probability[1])) {
				snakefood[food_on_screen].foodlevel=2;//2分食物：紫色
			} else {
				snakefood[food_on_screen].foodlevel=3;//3分食物：黄色
			}
			food_on_screen++;
		}
	}
}
bool Collideself() {
	for(int i=1; i<Snakeyyyy.length; i++) {
		if((Snakeyyyy.snake[0].col==Snakeyyyy.snake[i].col)&&(Snakeyyyy.snake[0].row==Snakeyyyy.snake[i].row)) {
			return true;
		}
	}
	return false;
}

void CollideWall() {
	for(int i=0; i<uselessCount; i++) {
		if((Snakeyyyy.snake[0].col==obstacle[i][1])&&(Snakeyyyy.snake[0].row==obstacle[i][0])) {
			isalive=false;
		}
	}//障碍物判断
	if((Snakeyyyy.snake[0].col==0&&IsTrueWall[2]==1)||(Snakeyyyy.snake[0].row==0&&IsTrueWall[0]==1)||(Snakeyyyy.snake[0].col==height+1&&IsTrueWall[1]==1)||(Snakeyyyy.snake[0].row==width+1&&IsTrueWall[3]==1)) {
		isalive=false;
	} else if(Collideself()) {
		isalive=false;
	}
}

int checkFood(int i,int j) {
	for(int k=0; k<snakefood.size(); k++) {
		if((snakefood[k].fdpos.col==i)&&(snakefood[k].fdpos.row==j)) {
			return k;
		}
	}
	cout<<"error: checkfood func.";
	return -1;
}
void EndGame() {
	clearScreen();
	for(int i=0; i<=height+1; i++) {
		for(int j=0; j<=width+1; j++) {
			if(landscape[i][j]=='#') {
				cout<<"\033[37;41m"<<cur_skin.SnakeHead<<"\033[0m";
			} else if(landscape[i][j]=='*') {
				cout<<"\033[37;41m"<<cur_skin.SnakeBody<<"\033[0m";
			} else if(landscape[i][j]=='@') {
				int m=checkFood(i,j);
				if(snakefood[m].foodlevel==1) {
					cout<<"\033[37;44m"<<cur_skin.OneFood<<"\033[0m";//1分食物：蓝色
				} else if(snakefood[m].foodlevel==2) {
					cout<<"\033[37;45m"<<cur_skin.TwoFood<<"\033[0m";//2分食物：紫色
				} else {
					cout<<"\033[37;43m"<<cur_skin.ThrFood<<"\033[0m";//3分食物：黄色
				}
			} else {
				BasicMapPrinter(i,j);//wall and tap
			}
		}
		if(i==0) {
			cout<<"     current score: "<<cur_score;
		}
		if(i==1) {
			cout<<"     config:        "<<cur_config;
		}
		if(i==2) {
			cout<<"     map:           "<<cur_map;
		}
		if(i==3) {
			cout<<"     theme:         "<<cur_skinname;
		}
		cout<<endl;
	}
	Sleep(1000);
	clearScreen();
	string checkout[13];
	for(int i=0; i<13; i++) {
		checkout[i]="|               |";
	}
	checkout[0]="-----------------";
	checkout[12]="-----------------";
	checkout[4]="|  游·戏·结·束  |";
	checkout[5]="|您的最终分数是:|";
	checkout[7]="|按b保存本局回放|";
	checkout[8]="| 若按其他键返回|";
	for(int i=0; i<13; i++) {
		if(i!=6) {
			cout<<checkout[i]<<endl;
		} else {
			cout<<"|    "<<setw(4)<<cur_score<<"分"<<"     |"<<endl;
		}
	}
	char tempchar='0';
	tempchar=getch();
	if(tempchar=='b') {
		reserveRecord();
	} else {
		remove("record/tempRecord.txt");
	}
	snakefood.clear();
}

void Map_Add_Snake(string skin) {
	Snakeyyyy.snake.clear();
	position a;
	int w=width/2;
	int h=height/2;
	for(int i=0; i<4; i++) {
		Snakeyyyy.snake.push_back(a);
	}
	int snake_len=skin.length();
	landscape[h][w]=skin[0];
	Snakeyyyy.snake[0].col=h;
	Snakeyyyy.snake[0].row=w;
	for(int i=1; i<snake_len; i++) {
		landscape[h][w+i]=skin[i];
		Snakeyyyy.snake[i].col=h;
		Snakeyyyy.snake[i].row=w+i;
	}
}

void Collide(position tail) {
	for(int k=0; k<foodNum; k++) {
		if((Snakeyyyy.snake[0].col==snakefood[k].fdpos.col)&&(Snakeyyyy.snake[0].row==snakefood[k].fdpos.row)) {
			food_on_screen-=1;
			int tmpC=snakefood[k].fdpos.col;
			int tmpR=snakefood[k].fdpos.row;
			int tmpL=snakefood[k].foodlevel;
			landscape[tmpC][tmpR]='#';
			if(tmpL==1) {
				cur_score+=1;
			} else if(tmpL==2) {
				cur_score+=2;
			} else {
				cur_score+=3;
			}
			for(int i=k; i<foodNum-1; i++) {
				snakefood[i].foodlevel=snakefood[i+1].foodlevel;
				snakefood[i].fdpos.col=snakefood[i+1].fdpos.col;
				snakefood[i].fdpos.row=snakefood[i+1].fdpos.row;
			}
			position tmp;
			tmp.col=tail.col;
			tmp.row=tail.row;
			Snakeyyyy.snake.push_back(tmp);
			Snakeyyyy.length++;
		}
	}
}
void SnakeMovement() {
	position tail;
	tail.col=Snakeyyyy.snake[Snakeyyyy.length-1].col;
	tail.row=Snakeyyyy.snake[Snakeyyyy.length-1].row;
	for(int i=Snakeyyyy.length-1; i>0; i--) {
		int tmp1=Snakeyyyy.snake[i-1].col;
		int tmp2=Snakeyyyy.snake[i-1].row;
		Snakeyyyy.snake[i].row=tmp2;
		Snakeyyyy.snake[i].col=tmp1;
	}
	if(Snakeyyyy.direction=='a') {
		if(Snakeyyyy.snake[0].row!=1) {
			Snakeyyyy.snake[0].row-=1;
		} else if(Snakeyyyy.snake[0].row==1&&IsTrueWall[2]==0) {
			Snakeyyyy.snake[0].row=width;
		}
	}
	if(Snakeyyyy.direction=='w') {
		if(Snakeyyyy.snake[0].col!=1) {
			Snakeyyyy.snake[0].col-=1;
		} else if(Snakeyyyy.snake[0].col==1&&IsTrueWall[0]==0) {
			Snakeyyyy.snake[0].col=height;
		}
	}
	if(Snakeyyyy.direction=='s') {
		if(Snakeyyyy.snake[0].col!=height) {
			Snakeyyyy.snake[0].col+=1;
		} else if(Snakeyyyy.snake[0].col==height&&IsTrueWall[1]==0) {
			Snakeyyyy.snake[0].col=1;
		}
	}
	if(Snakeyyyy.direction=='d') {
		if(Snakeyyyy.snake[0].row!=width) {
			Snakeyyyy.snake[0].row+=1;
		} else if(Snakeyyyy.snake[0].row==width&&IsTrueWall[3]==0) {
			Snakeyyyy.snake[0].row=1;
		}
	}
	Collide(tail);
	CollideWall();
	GenerateFood();
}
void Map_Change_Snake() {
	for(int i=0; i<=height+1; i++) {
		landscape[i][0]='|';
		landscape[i][width+1]='|';
		for(int j=1; j<width+1; j++) {
			landscape[i][j]=' ';
		}
	}
	for(int j=0; j<=width+1; j++) {
		landscape[0][j]='-';
		landscape[height+1][j]='-';
	}
	for(int i=1; i<=height+1; i++) {
		for(int p=1; p<=width+1; p++) {
			if(ifBar(p,i,obstacle)) {
				landscape[i][p]='|';
			}
		}
	}
	for(int i=1; i<Snakeyyyy.length; i++) {
		int tmp1=Snakeyyyy.snake[i].col;
		int tmp2=Snakeyyyy.snake[i].row;
		landscape[tmp1][tmp2]='*';
	}
	int tmp3=Snakeyyyy.snake[0].col;
	int tmp4=Snakeyyyy.snake[0].row;
	landscape[tmp3][tmp4]='#';
	for(int k=0; k<snakefood.size(); k++) {
		landscape[snakefood[k].fdpos.col][snakefood[k].fdpos.row]='@';
	}
}

void MapPrint() {
	clearScreen();
	for(int i=0; i<=height+1; i++) {
		for(int j=0; j<=width+1; j++) {
			if(landscape[i][j]=='#') {
				cout<<"\033[37;42m"<<cur_skin.SnakeHead<<"\033[0m";
			} else if(landscape[i][j]=='*') {
				cout<<"\033[37;42m"<<cur_skin.SnakeBody<<"\033[0m";
			} else if(landscape[i][j]=='@') {
				//食物的等级
				int m=checkFood(i,j);
				if(snakefood[m].foodlevel==1) {
					cout<<"\033[37;44m"<<cur_skin.OneFood<<"\033[0m";//1分食物：蓝色
				} else if(snakefood[m].foodlevel==2) {
					cout<<"\033[37;45m"<<cur_skin.TwoFood<<"\033[0m";//2分食物：紫色
				} else {
					cout<<"\033[37;43m"<<cur_skin.ThrFood<<"\033[0m";//3分食物：黄色
				}
			} else {
				BasicMapPrinter(i,j);//wall and tap
			}
		}
		if(i==0) {
			cout<<"     current score: "<<cur_score;
		}
		if(i==1) {
			cout<<"     config:        "<<cur_config;
		}
		if(i==2) {
			cout<<"     map:           "<<cur_map<<" Map";
		}
		if(i==3) {
			cout<<"     theme:         "<<cur_skinname;
		}
		cout<<endl;
	}
}
void ActiveMode(char m) {
	while((m!=' ')&&(isalive==true)) {
		char p=Snakeyyyy.direction;
		m=getinput(Speed);
		if((m=='w')&&(Snakeyyyy.direction!='s')) {
			Snakeyyyy.direction='w';
		} else if((m=='a')&&(Snakeyyyy.direction!='d')) {
			Snakeyyyy.direction='a';
		} else if((m=='s')&&(Snakeyyyy.direction!='w')) {
			Snakeyyyy.direction='s';
		} else if((m=='d')&&(Snakeyyyy.direction!='a')) {
			Snakeyyyy.direction='d';
		} else {
			Snakeyyyy.direction=p;
		}
		SnakeMovement();
		if(isalive==true){
			Map_Change_Snake();
		}
		record();
		if(isalive==false) {
		ofstream fout;
		fout.open("record/tempRecord.txt",ios::app);
		fout<<-1<<' ';
		fout.close();
		EndGame();
		menu();
		MapGenerator();
		return;
	} 
	MapPrint();
	}
	m='\0';
	while((m!=' ')&&(m!='q')) {
		m=getch();
	}
	if(m==' ') {
		m='a';
		ActiveMode(m);
	} 
	else if(m=='q') {
		EndGame();
		menu();
		MapGenerator();
		return;
	}
}
extern void Game() {
	Snakeyyyy.Snake_Skin="#***";
	Snakeyyyy.length=4;
	Snakeyyyy.direction='a';
	food_on_screen=0;
	isalive=true;
	cur_score=0;
	if(seed==-1) {
		srand(time(NULL));
	} else {
		srand(seed);
	}
	Map_Add_Snake(Snakeyyyy.Snake_Skin);
	MapPrint();
	char m='a';
	Food a;
	for(int k=0; k<foodNum; k++) {
		snakefood.push_back(a);
	}
	cout<<"|按任意键开始游戏|";
	getch();
	//初始化结束，开始游戏
	ActiveMode(m);
}



void initialization() {
	setsize();
	ifstream fin;
	fin.open("config/allconfigs.txt");
	string m;
	while(getline(fin,m)) { //当能读入的时候；也就是依次读入了这个文件的所有行
		AllConfig.push_back(m);
		m.clear();
	}
	fin.close();
	fin.open("config/curConfig.txt");//记录着上一次游戏的config
	if(!fin) {
		cout<<"error"<<endl;
	}
	getline(fin,cur_config);
	fin.close();//当前的配置文件；新建的合理配置文件会被加入到AllConfig中并保存到allconfigs.txt
	fin.open("map/curmap.txt");
	if(!fin) {
		cout<<"检测到尚无已有的自定义配置：若无特别设置，将以默认配置开始游戏";
		ofstream ofs;
		ofs.open("map/Original.map");
		cur_map="Original";
		ofs<<15<<' '<<15;
		ofs.close();
	}
	getline(fin,cur_map);
	fin.close();
	fin.open("skin/allskins.txt");
	string n;
	while(getline(fin,n)) { //当能读入的时候；也就是依次读入了这个文件的所有行
		AllSkin.push_back(n);
		n.clear();
	}
	fin.close();
	fin.open("skin/curSkinName.txt");//记录着上一次游戏的skin
	getline(fin,cur_skinname);
	fin.close();
	menu();
	//下面先绘制地图：
	MapGenerator();
}
//AI相关

void AutoMode(char m){
	GenerateFood();
	while((m!=' ')) {
		int foodcol;
		int foodrow;
		int headrow;
		int headcol;
		for(int i=0;i<17;i++){
			for(int j=0;j<17;j++){
				if(landscape[i][j]=='@'){
					foodrow=i;
					foodcol=j;
				}
				if(landscape[i][j]=='#'){
					headrow=i;
					headcol=j;
				}
			}
		}
		float limittime=1000/Speed;
		int n=0;
		while(true) {
			Sleep(1);
			n++;
			if(n==limittime) {
				break;
			}
		}
		while(kbhit()) {
			char hit;
			hit=getch();
			if(hit=='q') {
				step=0.0;
				EndGame();
				menu();
				MapGenerator();
				return;
			} else if(hit==' ') {
				hit='\0';
				while((hit!=' ')&&(hit!='q')) {
					hit=getch();
				}
				if(hit==' ') {
					hit=aiDirection(headrow,headcol,foodrow,foodcol);
					AutoMode(hit);
					return;
				} else if(hit=='q') {
					step=0.0;
					EndGame();
					menu();
					MapGenerator();
					return;
				}
			}
		}
		Snakeyyyy.direction=aiDirection(headrow,headcol,foodrow,foodcol);
		SnakeMovement();
		if(isalive==false) {
		ofstream fout_ai;
		fout_ai.open("ai_History.txt",ios::app);
		fout_ai<<cur_score<<endl;
		ifstream fin_ai;
		fin_ai.open("ai_History.txt");
		int sum=0;
		int turns=0;
		while(fin_ai){
			int tmp;
			fin_ai>>tmp;
			sum+=tmp;
			turns++;
		}
		EndGame();
		cout<<"平均分数(Score)"<<setprecision(2)<<sum/turns<<endl;
		step=0.0;
		Sleep(1000);
		menu();
		MapGenerator();
		return;
	}
		Map_Change_Snake();
		MapPrint();
		step+=1.0;
		cout<<"获得每分的平均步数(Steps per score): "<<setprecision(2)<<step/cur_score<<endl;
}
}

void readyToPlayAI(){
	clearScreen();
	ifstream fingame;
	//map
	cur_map="hahha";
	fingame.open("map/"+cur_map+".map");
	fingame>>width;
	fingame>>height;
	fingame>>uselessCount;
	for(int i=0; i<15; i++) {
		obstacle[i][0]=0;
		obstacle[i][1]=0;
	}
	for(int temp=0; temp<uselessCount; temp++) {
		fingame>>obstacle[temp][0];
		fingame>>obstacle[temp][1];
	}
	for(int i=0; i<4; i++) {
		fingame>>IsTrueWall[i];
	}
	fingame.close();
    ofstream fout;
	fout.open("map/curmap.txt");
	fout<<cur_map<<endl;
	fout.close();
	//config
    cur_config="test";
	readFile("config/"+cur_config+".config");
	fout.open("config/curConfig.txt");
	fout<<cur_config<<endl;
	fout.close();
	//skin
	readSkin("skin/"+cur_skinname+".skin");//读入主题
	fout.open("skin/curSkinName.txt");
	fout<<cur_skinname<<endl;
	fout.close();//写入当前主题
	//landscape change:
	mapInitialization();
	//game
	cout<<"正在为您粉刷控制台..."<<endl;
	Sleep(1000);
	for(int p=0; p<=height+1; p++) {
		for(int i=0; i<=width+1; i++) {
			cout<<landscape[p][i];
		}
		cout<<endl;
	}
	Sleep(300);
	cout<<"正在抓来一条赛博蛇..."<<endl;
	cout<<'@';
	for(int i=0; i<=5; i++) {
		cout<<'#';
		Sleep(100);
	}
	cout<<endl<<"正在把它饿瘦好让它变得贪吃..."<<endl<<'@';
	for(int i=0; i<=5; i++) {
		cout<<'*';
		Sleep(100);
	}
	cout<<endl;
	fout.open("record/tempRecord.txt");//start to record
	fout<<width<<' '<<height<<' ';
	fout<<uselessCount<<' ';
	for(int i=0; i<uselessCount; i++) {
		fout<<obstacle[i][0]<<' '<<obstacle[i][1]<<' ';
	}
	fout<<IsTrueWall[0]<<' '<<IsTrueWall[1]<<' '<<IsTrueWall[2]<<' '<<IsTrueWall[3]<<' ';
	fout<<Speed<<' ';
	fout.close();
	Sleep(1000);
		Snakeyyyy.Snake_Skin="#***";
	Snakeyyyy.length=4;
	Snakeyyyy.direction='a';
	food_on_screen=0;
	isalive=true;
	cur_score=0;
	if(seed==-1) {
		srand(time(NULL));
	} else {
		srand(seed);
	}
	Map_Add_Snake(Snakeyyyy.Snake_Skin);
	MapPrint();
	Food a;
	for(int k=0; k<foodNum; k++) {
		snakefood.push_back(a);
	}
	cout<<"|按任意键开始游戏|";
	getch();
	AutoMode('a');
}


int main(){
	initialization();
	return 0;
}



