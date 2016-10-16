#include"frontend.h"
#include"src/reserv.h"
#include<time.h>
#include<iostream>
using namespace std;

int diag(int s, int e, float scale)
{
	Win win{s, e, scale};
	return win.run();
}

extern const char* month_name[];

int main(int c, char** v)
{
	//현재 시간을 구하여 표시 시작시간으로 삼음.
	Time t;
	time_t now = time(NULL);
	char *tm = ctime(&now);
	t.year = atoi(tm + 20);
	t.month = 0;
	for(int i=0; i<12; i++) if(!strncmp(tm + 4, month_name[i], 3)) t.month = i+1;
	tm[10] = '\0';
	t.day = atoi(tm + 8);
	t.hour = 0;
	t.minute = 0;
	
	//명령어 변수로 확대 스케일을 정함. 100->100%, 50->50%
	int k = 1;
	auto app = Gtk::Application::create(k, v, "");
	float f = 1;
	if(c > 1) f = (float)atoi(v[1])/100;

	//scale에 따라 예약 테이블 표시범위를 변화시킴
	int start = to_minute(&t);
	int end;
	if(f >= 1) end = start + 24 * 60 * 7;
	else if(f >= 0.05) end = start + 24 * 60 * 30;
	else end = start + 60 * 24 * 365 * 5;
	
	//예약 시스템 가동
	while(diag(start, end, f) == Gtk::RESPONSE_ACCEPT);
}
