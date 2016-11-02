#include<cstring>
#include<sstream>
#include<fstream>
#include<iostream>
#include<string>
#include"tcpip.h"
#include"reserv.h"
using namespace std;

Reserv::Reserv() 
{//서버 시작시 시설을 초기화하고, 예약상황을 읽어들인다.
	string s;
	int i=0;
	for(ifstream f("facility.txt"); f >> s; i++) facilities[s] = NULL;
	for(ifstream f("reservation.txt"); f >> s;) {
		Reservation tmp;
		f >> tmp.name >> tmp.tel >> tmp.from >> tmp.until;
		tmp.node = NULL;
		facilities[s] = insert(facilities[s], &tmp);//C함수
	}
}

Reserv::~Reserv()
{//서버 종료시 파일로 저장하고 메모리를 해제한다.
	ofstream f("reservation.txt");
	for(auto& a : facilities) {
		for(Reservation* p = a.second; p; p = p->node) { 
			f << a.first << ' ' << p->name << ' ' << p->tel;
			f << ' ' << p->from << ' ' << p->until << endl;
		}
		free_mem(a.second);//C 함수를 불러서 사용함.
	}
}

void Reserv::reserv(string fc, Reservation* p)
{
	facilities[fc] = insert(facilities[fc], p);
}

void Reserv::cancel(string fc, int from)
{
	facilities[fc] = del(facilities[fc], from);
}

string Reserv::display(string facility) 
{
	cout << facility << " 예약상황입니다." << endl;
	show(facilities[facility]);
	string r;
	for(Reservation* p = facilities[facility]; p; p = p->node) {
		r += p->name; r += ' ';
		r += p->tel; r += ' ';
		r += to_string(p->from); r += ' ';
		r += to_string(p->until); r += ' ';
	}
	if(!r.empty()) r.pop_back();
	return r;
}

basic_istream<char>& operator>>(basic_istream<char>& ss, Time& t)
{
	ss >> t.year;
	ss >> t.month;
	ss >> t.day;
	ss >> t.hour;
	ss >> t.minute;
	return ss;
}

string Reserv::operator()(string s) 
{//TCPIP 통신 하는 부분. 스트링 s는 수신이고, 리턴값이 tcpip의 답신.
	stringstream ss;
	ss << s;
	ss >> s;
	try {
		if(s == "display") {
			ss >> s;
			return display(s);
		} else if(s == "reserve") {
			Reservation r;
			Time f, t;
			ss >> r.name >> r.tel >> f >> t >> s;
			r.from = to_minute(&f);
			r.until = to_minute(&t);
			reserv(s, &r);
			return "예약되었습니다.";
		} else if(s == "cancel") {
			Time f;
			ss >> f >> s;
			cancel(s, to_minute(&f));
			return "취소되었습니다.";
		} else return s + " from server";
	} catch(...) {
		return "Syntax error";
	}
}
//예약구문 reserve 박승원 010-3456-5678 2016 10 11 12 30 2016 10 11 14 0 청실
//조회구문 display 청실
//취소구문 cancel 2016 10 11 12 30 청실

