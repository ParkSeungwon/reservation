#pragma once
#include<map>
#include<string>
#include<vector>

typedef struct Time {
	int year, month, day, hour, minute;
} Time;

typedef struct Reservation 
{//예약 리스트 자료구조.
	char name[40];
	char tel[20];
	int from, until;//minute 2000년0시를 0분으로 기준한 분 값.
	struct Reservation* node;
} Reservation;

void free_mem(Reservation* p);
Reservation* insert(Reservation* p, Reservation* to_insert);
Reservation* del(Reservation* p, int from);
void show(Reservation* p);
Time to_time(int);
int to_minute(Time*);
void enter_time(Time*);

class Reserv
{//tcpip모듈에 펑크터로 넘길 클래스. 
public:
	Reserv();
	virtual ~Reserv();
	std::string operator()(std::string s);

protected:
	void reserv(std::string facility, Reservation* p);//insert의 wrapper함수
	void cancel(std::string facility, int from);//del의 wrapper함수
	std::string display(std::string facility);//show의 wrapper함수
	std::map<std::string, Reservation*> facilities;//시설들의 포인터 맵
};

