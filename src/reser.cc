//C언어를 전적으로 사용한 리스트구조와 그 함수
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

typedef struct Time {
	int year, month, day, hour, minute;
} Time;

const char *month_name[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", 
	"Sep", "Oct", "Nov", "Dec"};
int day_of_months[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
int is_special_year(int y) 
{//윤년인지?
	if(y % 400 == 0) return 1;
	if(y % 100 == 0) return 0;
	if(y % 4 == 0) return 1;
	else return 0;
}

Time to_time(int minute)
{//2000년 0시 기준 분값을 시간 구조체로 변환
	int tmp, year, month, day, hour, m;
	for(year=2000; minute > (m = 60*24*(365 + is_special_year(year))); year++) {
		minute -= m;
	}
	for(month=0; minute > (m = 60 * 24 * day_of_months[month]); month++) {
		minute -= m;
	}
	day = minute / (60 * 24);
	minute %= 60 * 24;
	hour = minute / 60;
	minute %= 60;
	Time t = {year, month+1, day+1, hour, minute};
	return t;
}

int to_minute(Time* t) 
{//시간 구조체를 2000년 0시 기준 분값으로 변환
	int r = 0;
	for(int y=2000; y < t->year; y++) r += 60 * 24 * (365 + is_special_year(y));
	for(int m=1; m < t->month; m++) r += 60 * 24 * day_of_months[m-1];
	for(int d=1; d < t->day; d++) r += 60 * 24;
	for(int h=0; h < t->hour; h++) r += 60;
	r += t->minute;
	return r;
}

typedef struct Reservation 
{//예약 리스트 자료구조.
	char name[40];
	char tel[20];
	int from, until;//minute 2000년0시를 0분으로 기준한 분 값.
	struct Reservation* node;
} Reservation;

Reservation* insert(Reservation* p, Reservation* to_insert) 
{//시간에 따라 자동으로 정렬되는 삽입함수
	if(!p || to_insert->until <= p->from) {
		Reservation* newr = (Reservation*)malloc(sizeof(Reservation));
		newr->node = p;
		newr->from = to_insert->from;
		newr->until = to_insert->until;
		strcpy(newr->name, to_insert->name);
		strcpy(newr->tel, to_insert->tel);
		return newr;
	} else if(p->until <= to_insert->from) {
		p->node = insert(p->node, to_insert);
	}
	return p;
}

Reservation* del(Reservation* p, int from) 
{//시작 시간을 매개변수로 예약을 삭제하는 재귀함수.
	if(p == NULL || p->from > from) return p;
	if(p->from == from) {
		Reservation* tmp = p->node;
		free(p);
		return tmp;
	}
	p->node = del(p->node, from);
	return p;
}

void show(Reservation* p) 
{//현재 포인터가 가리키는 시설의 모든 예약 상황을 보여줌
	for(; p; p = p->node) {
		Time f = to_time(p->from);
		Time u = to_time(p->until);
		printf("%s[%s] : %d년 %02d월%02d일 %02d:%02d ~ %d년 %02d월%02d일 %02d:%02d\n", 
			p->name, p->tel, f.year, f.month, f.day, f.hour, f.minute, 
			u.year, u.month, u.day, u.hour, u.minute);
	}
}

void free_mem(Reservation* p) 
{//재귀적으로 리스트의 메모리를 모두 해제한다.
	if(!p) return;
	free_mem(p->node);
	free(p);
}

void enter_time(Time* t) 
{//대화형으로 콘솔로 시간을 입력받는 함수
	time_t now = time(NULL);
	char *tm = ctime(&now);
	t->year = atoi(tm + 20);
	t->month = 0;
	for(int i=0; i<12; i++) if(!strncmp(tm + 4, month_name[i], 3)) t->month = i+1;
	tm[10] = '\0';
	t->day = atoi(tm + 8);

	char buff[10];
	printf("예약할 시간을 입력하세요.\n");
	printf("년도(%d) : ", t->year);
	fgets(buff, 6, stdin);
	if(buff[0] != '\n') t->year = atoi(buff);
	printf("월(%d) : ", t->month);
	fgets(buff, 4, stdin);
	if(buff[0] != '\n') t->month = atoi(buff);
	printf("일(%d) : ", t->day);
	fgets(buff, 4, stdin);
	if(buff[0] != '\n') t->day = atoi(buff);
	printf("시간(0-23) : ");
	fgets(buff, 4, stdin);
	if(buff[0] != '\n') t->hour = atoi(buff);
	printf("분(0-59) : ");
	fgets(buff, 4, stdin);
	if(buff[0] != '\n') t->minute = atoi(buff);
}
