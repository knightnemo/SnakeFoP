#include<iostream>
#include<vector>
using namespace std;
int manhatton(int row,int col,int rowEnd,int colEnd){
    return sqrt(pow((row-rowEnd),2))+sqrt(pow((col-colEnd),2));
}
bool found=false;
char landscape[17][17];
void Initializer(){
    for(int i=0;i<17;i++){
        for(int j=0;j<17;j++){
            landscape[i][j]=' ';
        }
    }
    for(int i=0;i<17;i++){
    landscape[0][i]='x';
    landscape[16][i]='x';}
    for(int j=0;j<17;j++){
    landscape[j][0]='x';
    landscape[j][16]='x';
    }
    landscape[8][8]='S';
}
void MapPrint(){
    for(int i=0;i<17;i++){
        for(int j=0;j<17;j++){
            cout<<landscape[i][j];
        }
        cout<<endl;
    }
}
//用pos来描述一个地图上的位置的指标
struct pos{
    int row,col;//节点的行与列
    int f,g,h;//f是总距离,g是距离起点曼哈顿距离,h是距终点
    //A*算法是用的总距离来启发式寻路
};
struct treeNode{
    pos thisPoint;//当前节点
    vector<treeNode*> child;//存子节点的地址（不唯一）
    treeNode* parent;//存parent的地址（唯一）
};//一个四叉树上的节点的范式
treeNode* createTreeNode(int row,int col){
    treeNode* pNew= new treeNode;
    memset(pNew, 0, sizeof(treeNode));
    pNew->thisPoint.row=row;
    pNew->thisPoint.col=col;
    return pNew;
}//创建树节点
enum direc{up,down,lhs,rhs};
int main(){
    Initializer();
    int n;
    cout<<"请输入障碍物个数"<<endl;
    cin>>n;
    for(int i=0;i<n;i++){
        MapPrint();
        cout<<"请输入第"<<i+1<<"个障碍物的坐标"<<endl;
        int row,col;
        cin>>row>>col;
        landscape[row][col]='x';
    }
    MapPrint();
    cout<<"请输入终点坐标"<<endl;
    int destRow,destCol;
    cin>>destRow>>destCol;
    landscape[destRow][destCol]='F';
    MapPrint();
    //创建一个标记节点是否已经被遍历的地图，0代表还没,1代表去过了
    bool hasBeen[17][17]={0};
    //创建一颗树
    treeNode* root=createTreeNode(8,8);
    hasBeen[8][8]=1;
    //创建一个数组
    vector<treeNode*> buff;
    vector<treeNode*>::iterator iter1;//变化
    vector<treeNode*>::iterator iter2;//记录f最小
    //开始树的生长
    treeNode* cur=root;
    treeNode* curChild= NULL;
    while(true){
        //遍历当前节点周围的点
        for(int i=0;i<4;i++){
            curChild=createTreeNode(cur->thisPoint.row,cur->thisPoint.col);
        switch (i){
            case up:
                curChild->thisPoint.row--;
                break;
            case down:
                curChild->thisPoint.row++;
                break;
            case lhs:
                curChild->thisPoint.col--;
                break;
            case rhs:
                curChild->thisPoint.col++;
                break;
            default:
                cout<<"Error"<<endl;
                break;
        }
        curChild->thisPoint.g++;
        //检查这个新节点能不能走
        int tmp1=curChild->thisPoint.row;
        int tmp2=curChild->thisPoint.col;
            if((landscape[tmp1][tmp2]!='x')&&(!hasBeen[tmp1][tmp2])){
                curChild->thisPoint.h=manhatton(tmp1,tmp2,destRow,destCol);
                curChild->thisPoint.f=curChild->thisPoint.g+curChild->thisPoint.h;
                //把节点放进树里
                cur->child.push_back(curChild);
                curChild->parent=cur;
                //入数组
                buff.push_back(curChild);
            }
            else{
                delete curChild;
            }
        }//以上找到了当前节点周围所有能走的点
        //下面开始挑最小的点：
        iter1=buff.begin();
        iter2=buff.begin();
        while(iter1!=buff.end()){
            if((*iter2)->thisPoint.f>=(*iter1)->thisPoint.f){
                iter2=iter1;
            }
            iter1++;
        }
        //将这个最小的f所在坐标设置为新的当前节点
        cur=*iter2;
        hasBeen[cur->thisPoint.row][cur->thisPoint.col]=true;
        landscape[cur->thisPoint.row][cur->thisPoint.col]='A';
        MapPrint();
        //判断终点
        if((destRow==cur->thisPoint.row)&&(destCol==cur->thisPoint.col)){
            found=true;
            break;
        }
        //判断没找到
        if(buff.size()==0){
            cout<<"404 Not Found"<<endl;
            break;
        }
        buff.erase(iter2);
    }
    if(found){
        while(cur){
            landscape[cur->thisPoint.row][cur->thisPoint.col]='T';
            cur=cur->parent;
        }
    }
    MapPrint();
}