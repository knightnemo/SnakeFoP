#ifndef SKIN_EDITOR_H
#include<fstream>
#include<iostream>
#include<string.h>
#include"BootLoader.h"
#define SKIN_EDITOR_H
using namespace std;
bool checkFileName(const int k,string m);
void MapGenerator();
struct Skin{
     bool Wide;//是否是宽地图
     string RWall;//真的墙
     string IWall;//假的墙
     string SnakeHead;//蛇头
     string SnakeBody;//蛇身
     string OneFood;//1分食物
     string TwoFood;//2分食物
     string ThrFood;//3分食物
};
ofstream fout;
string cur_skinname;
vector<string> AllSkin;
extern Skin cur_skin;
void readSkin(string m){
    ifstream fin;
    fin.open(m);
    if(!fin){
        cout<<m<<endl;
        cout<<"error";
    }
    fin>>cur_skin.Wide;
    fin>>cur_skin.RWall;
    fin>>cur_skin.IWall;
    fin>>cur_skin.SnakeHead;
    fin>>cur_skin.SnakeBody;
    fin>>cur_skin.OneFood;
    fin>>cur_skin.TwoFood;
    fin>>cur_skin.ThrFood;
    fin.close();
}
void _InvalidInput(){//在非法输入时出现的界面
    string configUI[13];
    for(int i=0;i<13;i++){
        configUI[i]="|                         |";
    }
    configUI[0]="---------------------------";
    configUI[12]="---------------------------";
    clearScreen();
    for(int i=0;i<13;i++){
        if(i==5){
            cout<<"| 先前输入不符合输入规范  |"<<endl;
        }
        else if(i==6){
            cout<<"|  将在一秒后返回主菜单   |"<<endl;
        }
        else{
            cout<<configUI[i]<<endl;
        }
        }
        Sleep(2000);
        menu();
        MapGenerator();
}
void getX(string X,bool tmp){
    string k="GREATNEMO";
    cout<<"请输入代表"<<X<<"的全角字符:"<<endl;
    cin>>k;
    if(tmp){
    while(k.length()!=4){
            cout<<"不合法的输入"<<endl;
            Sleep(1000);
            cout<<"请输入代表"<<X<<"的全角字符:"<<endl;
            cin>>k;
    }
    }
    else{
        while(k.length()!=1){
            cout<<"不合法的输入"<<endl;
            Sleep(1000);
            cout<<"请输入代表"<<X<<"的全角字符:"<<endl;
            cin>>k;
    }
    }
    fout<<k<<endl;
}
void writeSkinFile(){
    restoreTerminal();
    cin.clear();
    string m;
    string skinUI[13];
    for(int i=0;i<13;i++){
        skinUI[i]="|                         |";
    }
    skinUI[0]="---------------------------";
    skinUI[12]="---------------------------";
    for(int i=0;i<13;i++){
        if(i==4){
            cout<<"|    请输入新建皮肤名:    |"<<endl;
        }
        else if(i==5){
            cout<<"| (由字母,数字,下划线构成)|"<<endl;
        }
        else if(i==8){
            cout<<"|      退出新建请按q      |"<<endl;
        }
        else{
        cout<<skinUI[i]<<endl;
        }
    }
    cin>>m;
    const int k=m.length();
    if(m=="q"){
        clearScreen();
        menu();
        MapGenerator();
    }
    else if(checkFileName(k,m)){
        clearScreen();
        cout<<"有效的输入"<<endl;
        cur_skinname=m;
        AllSkin.push_back(m);
        fout.open("skin/allskins.txt",ios::app);
        fout<<endl<<m;
        fout.close();
        m="skin/"+m+".skin";
        fout.open(m);
        cout<<"是否为宽地图(是请填1,否请填0)";
        bool tmp;
        cin>>tmp;
        fout<<tmp<<endl;
        getX("实墙(蛇不能穿入的墙)",tmp);
        getX("虚墙(蛇可以穿入的墙)",tmp);
        getX("蛇头",tmp);
        getX("蛇身",tmp);
        getX("1分食物",tmp);
        getX("2分食物",tmp);
        getX("3分食物",tmp);
        fout.close();
        fout.open("skin/curSkinName.txt");
        fout<<cur_skinname;
        fout.close();
        cout<<"创建成功>>>将在1s后返回主菜单"<<endl;
        Sleep(1000);
        menu();
        MapGenerator();
    }
    else{
        _InvalidInput();
    }
}
bool checkSkinExist(string attempt){
    bool flag=true;
    for(int i=0;i<AllSkin.size();i++){
        flag=true;
        if(attempt.size()!=AllSkin[i].size()){
            flag=false;
        }
        for(int j=0;j<attempt.size();j++){
        if(attempt[j]!=AllSkin[i][j]){
            flag=false;
            break;
        }
        if(flag==true){
            return true;
        }
        }
    }
    return false;
}
void ChooseSkinFile(){
    restoreTerminal();
    cin.clear();
    cout<<"当前的主题文件名称为:"<<endl;
    cout<<cur_skinname<<endl;
    cout<<"当前主题文件包括:"<<endl;
    for(int i=0;i<AllSkin.size();i++){
        cout<<i+1<<". "<<AllSkin[i]<<endl;
    }
    string fileAttempt;
    cout<<"载入过程开始(如需退出,请按q)"<<endl;
    cout<<"请输入您想要载入的主题文件的名称(输入文件名即可，不需要.skin扩展名)"<<endl;
    cin>>fileAttempt;
    if(fileAttempt=="q"){
        clearScreen();
        menu();
        MapGenerator();
    }
    else if(checkSkinExist(fileAttempt)){
        cur_skinname=fileAttempt;
        cout<<"已将"<<cur_skinname<<".skin载入为当前配置"<<endl<<"将在一秒后返回主菜单"<<endl;
        Sleep(1000);
        clearScreen();
        menu();
        MapGenerator();
    }
    else{
        cout<<"配置文件不存在,将在一秒后重启配置载入界面!"<<endl;
        Sleep(1000);
        clearScreen();
        ChooseSkinFile();
    }
}
#endif