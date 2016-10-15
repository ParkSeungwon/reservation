#include"frontend.h"
#include<fstream>
#include<string>
#include<iostream>
#include"reserv.h"
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

Facility::Facility(string fac, int start, float scale)
{
	string command = "./client.x display " + fac;
	string s = psstm(command);
	s.pop_back();//remove '\n'!! 
	int from, until;
	string tel;
	while(!s.empty()) {
		ResButton bt{cut(s), tel = cut(s), from = stoi(cut(s)), 
			until = stoi(cut(s)), scale};
		bt.signal_clicked().connect(bind(&Facility::on_click, this, fac, tel, from, until));
		if(from > start) {
			ResButton tmp{"", "", start, from, scale};
			tmp.signal_clicked().connect(bind(&Facility::on_click, this, fac, "", start, from));
			v.push_back(move(tmp));
		}
		v.push_back(move(bt));
		start = until;
	}
	for(auto& a : v) pack_start(a, Gtk::PACK_SHRINK);
	show_all_children();
}

void Facility::on_click(string fac, string tel, int from, int until)
{
	if(!tel.empty()) {
		Gtk::MessageDialog cancel("Do you want to CANCEL this reservation?", 
				false, Gtk::MESSAGE_WARNING, Gtk::BUTTONS_YES_NO);
		cancel.set_secondary_text("연락처 : " + tel);
		if(cancel.run() == Gtk::RESPONSE_OK) {
			Time t = to_time(from);
			string command = "./client.x cancel " + to_string(t.year) + ' ' 
				+ to_string(t.month) + ' ' + to_string(t.day) + ' '
				+ to_string(t.hour) + ' ' + to_string(t.minute) + ' ' + fac;
			psstm(command.data());
		}
	} 

}


ResButton::ResButton(string name, string tel, int from, int until, float scale)
{
	this->name = name;
	this->tel = tel;
	this->from = from;
	this->until = until;
	set_label(name);
	set_size_request(scale * (until - from), -1);
}

Win::Win()
{
	string s;
	for(ifstream f("facility.txt"); f >> s;) v.push_back(Facility(s, 8832000, 0.2));
	add(scwin);
	scwin.add(vb);
	for(auto& a : v) vb.pack_start(a, Gtk::PACK_SHRINK);
	set_default_size(1000, 700);
	show_all_children();
}
