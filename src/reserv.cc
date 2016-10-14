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

void Reserv::display(string facility) 
{
	cout << facility << " 예약상황입니다." << endl;
	show(facilities[facility]);
}

