#include<fstream>
#include<iostream>
#include<string>
#include"tcpip.h"
#include"reserv.h"
using namespace std;

Reserv::Reserv() 
{
	string s;
	int i=0;
	for(ifstream f("facility.txt"); f >> s; i++) facilities[s] = NULL;
	for(ifstream f("reservation.txt"); f >> s;) {
		Reservation tmp;
		f >> tmp.name >> tmp.tel >> tmp.from >> tmp.until;
		tmp.node = NULL;
		facilities[s] = insert(facilities[s], &tmp);//insert easily
	}
}

Reserv::~Reserv()
{
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
	show(facilities[facility], buff);
	return buff;
}

string Reserv::operator()(string s) 
{
	if(s == "청실") return display("청실");
	if(s == "예약") {
		Time f = {2016, 11, 13, 11, 0};
		Time t = {2016, 11, 13, 12, 30};
		Reservation r = {"박승원", "031-255-6698", to_minute(&f), to_minute(&t)};
		reserv("청실", &r);
	}
	else return "";
}

