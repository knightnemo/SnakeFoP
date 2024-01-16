//实现跨系统：通过函数重载调用不同系统的库实现相同或相似的功能
    #include<iostream>
    using namespace std;
#ifndef COMPATABILITY_H
    #ifdef _WIN64
       #define HEIGHT 32
        #define WIDTH 125
        #include<windows.h>
        #include<conio.h>
        void setsize(){
            char cmd[128];
    sprintf_s(cmd, "mode con cols=%d lines=%d", WIDTH, HEIGHT);
    system(cmd);
        }
        void restoreTerminal(){return;}//Win无事发生，但linux系统会重置缓冲区回显
        void clearScreen(){
            system("cls");
        }
    #elif _WIN32
        #define HEIGHT 32
        #define WIDTH 125
        #include<windows.h>
        #include<conio.h>
        void setsize(){
            char cmd[128];
    sprintf_s(cmd, "mode con cols=%d lines=%d", WIDTH, HEIGHT);
    system(cmd);
        }
        void restoreTerminal(){return;}//Win无事发生，但linux系统会重置缓冲区回显
        void clearScreen(){
            system("cls");
        }
    #elif __APPLE__
        #include<ncurses.h>
        void clearScreen(){
            system("clear");
        }
        void setsize(){
        system("resize -s 32 125");
        system("clear");}
        #include <stdio.h>
        #include <unistd.h>
        #include <termios.h>
        #include <sys/ioctl.h>
        #define getch ___gtch
        static const int STDIN = 0;
        static bool initialized = false;
        static termios originalTerm;
        char ___gtch(){
            termios tmp = originalTerm;
            tmp.c_lflag &= ~ICANON;
            tmp.c_lflag &= ~ECHO;
            tcsetattr(STDIN, TCSANOW, &tmp);
            char ch;
            ch=getchar();
            return ch;
            setbuf(stdin, NULL);
        }
        int kbhit(){
            if (!initialized){
            tcgetattr(STDIN, &originalTerm);
            initialized = true;
            }
            termios term = originalTerm;
            term.c_lflag &= ~ICANON;
            term.c_lflag &= ~ECHO;
            tcsetattr(STDIN, TCSANOW, &term);
            setbuf(stdin, NULL);
            int bytesWaiting;
            ioctl(STDIN, FIONREAD, &bytesWaiting);
            return bytesWaiting;
        }
        void restoreTerminal() {
        tcsetattr(STDIN, TCSANOW, &originalTerm);
        setbuf(stdin, NULL);
        }
        void Sleep(int i){
            usleep(i*1000);
        }
    #elif __linux__
        #include<ncurses.h>
        void clearScreen(){
            system("clear");
        }
        void setsize(){
        system("resize -s 32 125");
        system("clear");}
        #include <stdio.h>
        #include <unistd.h>
        #include <termios.h>
        #include <sys/ioctl.h>
        #define getch ___gtch
        static const int STDIN = 0;
        static bool initialized = false;
        static termios originalTerm;
        char ___gtch(){
            termios tmp = originalTerm;
            tmp.c_lflag &= ~ICANON;
            tmp.c_lflag &= ~ECHO;
            tcsetattr(STDIN, TCSANOW, &tmp);
            char ch;
            ch=getchar();
            return ch;
            setbuf(stdin, NULL);
        }
        int kbhit(){
            if (!initialized){
            tcgetattr(STDIN, &originalTerm);
            initialized = true;
            }
            termios term = originalTerm;
            term.c_lflag &= ~ICANON;
            term.c_lflag &= ~ECHO;
            tcsetattr(STDIN, TCSANOW, &term);
            setbuf(stdin, NULL);
            int bytesWaiting;
            ioctl(STDIN, FIONREAD, &bytesWaiting);
            return bytesWaiting;
        }
        void restoreTerminal() {
        tcsetattr(STDIN, TCSANOW, &originalTerm);
        setbuf(stdin, NULL);
        }
        void Sleep(int i){
            usleep(i*1000);
        }
    #else
        cout << "暂时不支持此操作系统,为获得更佳体验,请更换至以MacOS/Windows/Linux为操作系统的电脑" << endl;
    #endif
#endif