#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<string>
#include"reserv.h"
using namespace std;
//void psstm(char* buff) {
//	FILE* f = popen(buff, "r");
//	fgets(buff, 10000, f);
//	pclose(f);
//}

string psstm(string command)
{//return system call output as string
	string s;
	char buf[100];
	FILE* f = popen(command.c_str(), "r");
	while(fgets(buf, sizeof(buf), f)) s += buf;
	pclose(f);
	return s;
}
void  console_reserve()
{
	char buff[10000] = {}, name[40], tel[20], facility[20], number[10];
	Time f, t;
	printf("예약 시작 시간을 입력하세요.");
	enter_time(&f);
	printf("예약 종료 시간을 입력하세요.");
	enter_time(&t);
	printf("이름을 입력하세요.");
	scanf("%s", name);
	printf("연락처를 입력하세요.");
	scanf("%s", tel);
	printf("예약할 시설을 입력하세요.");
	scanf("%s", facility);
	strcpy(buff, "./client.x reserve ");
	strcat(buff, name);
	strcat(buff, " ");
	strcat(buff, tel);
	strcat(buff, " ");
	sprintf(number, "%d",  f.year);
	strcat(buff, number);
	strcat(buff, " ");
	sprintf(number, "%d", f.month);
	strcat(buff, number);
	strcat(buff, " ");
	sprintf(number, "%d", f.day);
	strcat(buff, number);
	strcat(buff, " ");
	sprintf(number, "%d", f.hour);
	strcat(buff, number);
	strcat(buff, " ");
	sprintf(number, "%d", f.minute);
	strcat(buff, number);
	strcat(buff, " ");
	sprintf(number, "%d", t.year);
	strcat(buff, number);
	strcat(buff, " ");
	sprintf(number, "%d", t.month);
	strcat(buff, number);
	strcat(buff, " ");
	sprintf(number, "%d", t.day);
	strcat(buff, number);
	strcat(buff, " ");
	sprintf(number, "%d", t.hour);
	strcat(buff, number);
	strcat(buff, " ");
	sprintf(number, "%d", t.minute);
	strcat(buff, number);
	strcat(buff, " ");
	strcat(buff, facility);
	printf("%s\n", buff);
	psstm(buff);
	printf("%s\n", buff);
}

int print_buff(char* buff, int idx) {
	if(buff[0] == '\n') return 1;
	char c[4] = {'[', '\0', '\0', '\0'};
	int num_idx1, num_idx2, i, j=0;
	for(i=idx; buff[i] != '\0'; i++) {
		if(buff[i] == ' ') {
			buff[i] = c[j++];
			if(j == 2) num_idx1 = i + 1;
			else if(j == 3) num_idx2  = i + 1;
			else if(j == 4) break;
		}
	}
	printf("%s] : ", buff + idx);
	Time f = to_time(atoi(buff + num_idx1));
	Time t = to_time(atoi(buff + num_idx2));
	printf("%d년 %d월 %d일 %d시 %d분 ~ %d년 %d월 %d일 %d시 %d분\n",	
			f.year, f.month, f.day, f.hour, f.minute, 
			t.year, t.month, t.day, t.hour, t.minute);
	return i+1;
}

void console_display()
{
	char buff[10000] = {}, facility[20];
	printf("%s", buff);
	printf("표시할 시설을 입력하세요.");
	scanf("%s", facility);
	strcpy(buff, "./client.x display ");
	strcat(buff, facility);
	psstm(buff);
	for(int i = 0; buff[i] != '\0'; i = print_buff(buff, i)) ;
}

void end_server()
{
	char buff[10000] = "./client.x end";
	psstm(buff);
	printf("%s\n", buff);
}

void search()
{
	char person[40];
	printf("찾을 사람을 입력하세요.");
	scanf("%s", person);
	int length = 0;
	for(int i=0; person[i]; i++) length++;
	char facility[100], command[10000];
	FILE* f = fopen("facility.txt", "r");
	while(fgets(facility, 100, f)) {
		strcpy(command, "./client.x display ");
		strcat(command, facility);
		psstm(command);
		for(int i=0; ; i++) if(facility[i] == '\0') {
			facility[i-1] = '\0';
			break;
		}
		if(!strncmp(command, person, length)) {
			printf("%s : " , facility);
			print_buff(command, 0);
		}
		for(int i=0, space=0; command[i] != '\0'; i++) {
			if(command[i] == ' ') {
				space++;
				if(space % 4 == 0 && !strncmp(command+i+1, person, length)) {
					printf("%s : " , facility);
					print_buff(command, i+1);
				}
			}
		}
	}
}

void cancel()
{
	Time t;
	printf("예약 시간을 입력하세요.");
	enter_time(&t);
	printf("예약한 시설을 입력하세요.");
	char facility[40];
	scanf("%s", facility);
	char num[10];
	char buff[1000] = "./client.x cancel ";
	sprintf(num, "%d", t.year);
	strcat(buff, num);
	strcat(buff, " ");
	sprintf(num, "%d", t.month);
	strcat(buff, num);
	strcat(buff, " ");
	sprintf(num, "%d", t.day);
	strcat(buff, num);
	strcat(buff, " ");
	sprintf(num, "%d", t.hour);
	strcat(buff, num);
	strcat(buff, " ");
	sprintf(num, "%d", t.minute);
	strcat(buff, num);
	strcat(buff, " ");
	strcat(buff, facility);
	psstm(buff);
}


int main()
{
	int menu;
	char c;
	int end = 0;
	while(!end) {
		printf("1.예약, 2.표시, 3.취소, 4.검색, 5.서버 종료 및 저장\n입력하세요.");
		scanf("%d", &menu);
		scanf("%c", &c);
		switch(menu) {
			case 1: console_reserve(); break;
			case 2:	console_display(); break;
			case 3: cancel(); break;
			case 4: search(); break;
			case 5: end_server(); end = 1; break;
			default:;
		}
	}
}

