#include"frontend.h"
#include<fstream>
#include<string>
#include<iostream>
using namespace std;

string cut(string&);

string psstm(string command)
{//return system call output as string
	char buf[10000];
	FILE* f = popen(command.c_str(), "r");
	fgets(buf, sizeof(buf), f);
	pclose(f);
	return buf;
}

Facility::Facility(string s)
{
	string command = "./client.x display " + s;
	s = psstm(command);
	s.pop_back();//remove '\n'!! 
	while(!s.empty())
		v.push_back(ResButton{cut(s), cut(s), stoi(cut(s)), stoi(cut(s))});
	for(auto& a : v) pack_start(a);
	show_all_children();
}

ResButton::ResButton(string name, string tel, int from, int until)
{
	this->name = name;
	this->tel = tel;
	this->from = from;
	this->until = until;
	set_label(name + ' ' + tel);
	set_size_request(until - from, -1);
}

Win::Win()
{
	string s;
	for(ifstream f("facility.txt"); f >> s;) v.push_back(Facility(s));
	add(vb);
	for(auto& a : v) vb.pack_start(a);
	show_all_children();
}
