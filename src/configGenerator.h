#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
#include"BootLoader.h"
using namespace std;
int Speed;
int seed;
int foodNum;
float probability[3];
string cur_config;
extern vector<string> AllConfig;
extern void MapGenerator();

void readFile(string m){
    ifstream fin;
    fin.open(m);
    if(!fin){
        cout<<m<<endl;
        cout<<"error"<<m;
    }
    fin>>Speed;
    fin>>seed;
    fin>>foodNum;
    for(int i=0;i<3;i++){
        fin>>probability[i];
    }
    fin.close();
}
void InvalidInput(){//在非法输入时出现的界面
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
bool checkFileName(const int k,string m){
    for(int i=0;i<k;i++){
        if((m[i]=='_')||((m[i]>=65)&&(m[i]<=90))||((m[i]>=97)&&(m[i]<=122))||((m[i]>=48)&&(m[i]<=57))){
        }
        else{
            return false;//合法的输入只能包括英语字母、下划线和数字
        }        
    }
    return true;
}//检查输入的文件名是否是合法的输入
void creatFile(){
    restoreTerminal();
    cin.clear();
    string m;
    string configUI[13];
    for(int i=0;i<13;i++){
        configUI[i]="|                         |";
    }
    configUI[0]="---------------------------";
    configUI[12]="---------------------------";
    for(int i=0;i<13;i++){
        if(i==4){
            cout<<"|    请输入新建配置名:    |"<<endl;
        }
        else if(i==5){
            cout<<"| (由字母,数字,下划线构成)|"<<endl;
        }
        else if(i==8){
            cout<<"|      退出新建请按q      |"<<endl;
        }
        else{
        cout<<configUI[i]<<endl;
        }
    }//配置界面的ui绘制完成
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
        cout<<"请输入游戏难度(1~10间,数字越大难度越高):"<<endl;
        int level;
        cin>>level;//相当于Speed
        if((level<=10)&&(level>=1)){
            cout<<"请输入随机种子(-1表示不固定随机种子):"<<endl;
            int randominit;
            cin>>randominit;//相当于seed
            cout<<"请输入同一时刻场地中的食物数量(1~5之间)"<<endl;
            int foody;
            cin>>foody;
            if((foody>=1)&&(foody<=5)){
                float prob[3];
                cout<<"请输入1,2,3分食物的生成概率(0~1间的3个浮点数,用空格、换行符或制表符隔开,其和应当为1)"<<endl;
                cin>>prob[0]>>prob[1]>>prob[2];
                if((prob[0]+prob[1]+prob[2]==1)&&(prob[0]>=0)&&(prob[0]<=1)&&(prob[1]>=0)&&(prob[1]<=1)&&(prob[2]>=0)&&(prob[2]<=1)){
                    AllConfig.push_back(m);
                    ofstream fout;
                    fout.open("config/allconfigs.txt", ios::app);
                    fout<<endl<<m;
                    fout.close();
                    m="config/"+m+".config";
                    fout.open(m);
                    fout<<level<<" "<<endl;
                    fout<<randominit<<" "<<endl;
                    fout<<foody<<" "<<endl;//相当于foodNum
                    fout<<prob[0]<<" "<<prob[1]<<" "<<prob[2]<<" "<<endl;
                    fout.close();
                    cout<<"Loading...";//现在可以处理我所能想到的所有非法输入了
                    for(int i=0;i<10;i++){
                        Sleep(10);
                        cout<<"%";
                    }
                    cout<<" 100%"<<endl;
                    cout<<"config文件创建成功,将在1s内自动退出"<<endl;
                    Sleep(1000);
                    clearScreen();
                    menu();
                    MapGenerator();//假如输入都符合要求就会保存到文件中，然后退出
                }
                else{
                    m.clear();
                    InvalidInput();
                }
            }
            else{
                m.clear();
                InvalidInput();
            }
        }
        else{
            m.clear();
            InvalidInput();
        }
    }
    else{
        m.clear();
        InvalidInput();
    }//其他不合法的输入都会导致页面重新启动
}
//查询这个配置文件是否存在
bool checkFileExist(string attempt){
    bool flag=true;
    for(int i=0;i<AllConfig.size();i++){
        flag=true;
        for(int j=0;j<attempt.size();j++){
            if(attempt[j]!=AllConfig[i][j]){
                flag=false;
                break;//attempt不是AllConfig[i]
            }
        }
        if(flag==true){
            return true;
        }
    }
    return false;
}
void ChooseFile(){
    restoreTerminal();
    cin.clear();
    cout<<"当前的配置文件名称为:"<<endl;
    cout<<cur_config<<endl;
    cout<<"当前配置文件包括:"<<endl;
    for(int i=0;i<AllConfig.size();i++){
        cout<<i+1<<". "<<AllConfig[i]<<endl;
    }
    string fileAttempt;
    cout<<"配置载入过程开始(如需退出,请按q)"<<endl;
    cout<<"请输入您想要载入的配置文件的名称(输入文件名即可，不需要.config扩展名)"<<endl;
    getline(cin,fileAttempt);//获得用户希望载入的配置名称
    if(fileAttempt=="q"){
        clearScreen();
        menu();
        MapGenerator();
    }
    else if(checkFileExist(fileAttempt)){
        cur_config=fileAttempt;
        cout<<"已将"<<cur_config<<".config载入为当前配置"<<endl<<"将在一秒后返回主菜单"<<endl;
        Sleep(1000);
        clearScreen();
        menu();
        MapGenerator();//载入配置成功
    }
    else{
        cout<<"配置文件不存在,将在一秒后重启配置载入界面!"<<endl;
        Sleep(1000);
        clearScreen();
        ChooseFile();
    }//配置文件不存在
}
