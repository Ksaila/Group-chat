#pragma once

#include <iostream>
#include <string>
#include <ncurses.h>
#include <unistd.h>
#include <cstring>
#include <pthread.h>
class Window{
    public:
	WINDOW *header;
	WINDOW *output;
	WINDOW *online;
	WINDOW *input;
	pthread_mutex_t lock;
    public:
	Window()
	{
	    initscr();
	    curs_set(0);
	    pthread_mutex_init(&lock,NULL);
	}
	void SafeWrefresh(WINDOW *w)
	{
	    pthread_mutex_lock(&lock);
	    wrefresh(w);
	    pthread_mutex_unlock(&lock);
	}
	void DrawHeader()
	{
	    int h = LINES*0.2;
	    int w = COLS;
	    int y = 0;
	    int x = 0;
	    header = newwin(h,w,y,x);
	    box(header,0,0);
	    SafeWrefresh(header);	
	}
	void DrawOutput()
	{
	    int h = LINES*0.6;
	    int w = COLS*0.75;
	    int y = LINES*0.2;
	    int x = 0;
	    output = newwin(h,w,y,x);
	    box(output,0,0);
	    SafeWrefresh(output);	
	}

	void DrawOnline()
	{
	    
	    int h = LINES*0.6;
	    int w = COLS*0.25;
	    int y = LINES*0.2;
	    int x = COLS*0.75;
	    online = newwin(h,w,y,x);
	    box(online,0,0);
	    SafeWrefresh(online);	
	}
	void DrawInput()
	{	    
	    int h = LINES*0.2;
	    int w = COLS;
	    int y = LINES*0.8;
	    int x = 0;
	    input = newwin(h,w,y,x);
	    box(input,0,0);
	    std::string tips = "Please Enter# ";
	    PutStringToWin(input,2,2,tips);
	    SafeWrefresh(input);	
	}
	void GetStringFromInput(std::string &message)
	{
 	    char buffer[1024];
	    memset(buffer,0,sizeof(buffer));
	    wgetnstr(input,buffer,sizeof(buffer));//在input窗口中输入
	    message = buffer;
	    delwin(input);
	    DrawInput();
	}	
	void PutMessageToOutput(const std::string &message)
	{
	    static int line = 1;
	    int y,x;
	    getmaxyx(output,y,x);
	    if(line > y-2){
		delwin(output);
		DrawOutput();
		line = 1;
	    }
	    PutStringToWin(output,line++,2,message);

	}
	void PutStringToWin(WINDOW *w,int y,int x,const std::string &message)
	{
	    mvwaddstr(w,y,x,message.c_str());
	    SafeWrefresh(w);
	}
	void Welcome()
	{
	    const std::string welcome = "welcome to my chat system!";
	    int num = 1;
	    int x,y;
	    int dir = 0;//从左往右 否则等于1
 	    for( ; ; )
	    {
		DrawHeader();
 		getmaxyx(header,y,x);
		PutStringToWin(header,y/2,num,welcome);
		if(num > x - welcome.size() - 3){
		    dir = 1;
		}
		if(num <= 1){
		    dir = 0;
		}
		if(dir == 0){
		    num++;
		}else{
		    num--;
		}
		usleep(100000);
		delwin(header);
	    }
	}
	~Window()
	{
	    endwin();
	    pthread_mutex_destroy(&lock);
	}
};























