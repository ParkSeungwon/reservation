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
		if(start < from) {
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
		Gtk::MessageDialog cancel("예약을 취소하시겠습니까?", 
				false, Gtk::MESSAGE_WARNING, Gtk::BUTTONS_YES_NO);
		cancel.set_secondary_text("연락처 : " + tel);
		if(cancel.run() == Gtk::RESPONSE_YES) {
			Time t = to_time(from);
			string command = "./client.x cancel " + to_string(t.year) + ' ' 
				+ to_string(t.month) + ' ' + to_string(t.day) + ' '
				+ to_string(t.hour) + ' ' + to_string(t.minute) + ' ' + fac;
			cout << command << endl;
			cout << psstm(command.data());
		}
	} else {
		ResDialog respin(from, until);
		if(respin.run() == 1) {
			Time f = {respin.spf[0].get_value(), respin.spf[1].get_value(), 
				respin.spf[2].get_value(), respin.spf[3].get_value(), 
				respin.spf[4].get_value()};
			Time t = {respin.spt[0].get_value(), respin.spt[1].get_value(), 
				respin.spt[2].get_value(), respin.spt[3].get_value(), 
				respin.spt[4].get_value()};
			int mf = to_minute(&f);
			int mt = to_minute(&t);
			if(from <= mf && mf < mt && mt <= until) {
				string command = "./client.x reserve " + respin.name.get_text() 
					+ ' ' + respin.tel.get_text() + ' ' + to_string(f.year) + ' '
					+ to_string(f.month) + ' ' + to_string(f.day) + ' ' +
					to_string(f.hour) + ' ' + to_string(f.minute) + ' ' +
					to_string(t.year) + ' ' + to_string(t.month) + ' ' +
					to_string(t.day) + ' ' + to_string(t.hour) + ' ' + 
					to_string(t.minute) + ' ' + fac;
				cout << command << endl;
				cout << psstm(command.data());
			}
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

ResDialog::ResDialog(int from, int until)
{
	set_title("예약 시간 입력창");
	Time f = to_time(from);
	Time t = to_time(until);

	name.set_placeholder_text("성함을 입력하세요.");
	name.set_input_purpose(Gtk::INPUT_PURPOSE_ALPHA);
	tel.set_placeholder_text("연락처를 입력하세요.");
	tel.set_input_purpose(Gtk::INPUT_PURPOSE_DIGITS);
	Gtk::Box* box = get_content_area();
	box->pack_start(name);
	box->pack_start(tel);
	box->pack_start(hb1);
	box->pack_start(hb2);
	add_button("취소", 0);
	add_button("예약", 1);
	for(int i=0; i<5; i++) {
		hb1.pack_start(spf[i]);
		hb2.pack_start(spt[i]);
		spf[i].set_increments(1,10);
		spt[i].set_increments(1,10);
		spf[i].set_numeric();
		spt[i].set_numeric();
	}

	spf[0].set_range(f.year, t.year);
	spt[0].set_range(f.year, t.year);
	spf[1].set_range(1, 12);
	spt[1].set_range(1, 12);
	spf[2].set_range(1, 31);
	spt[2].set_range(1, 31);
	spf[3].set_range(0, 23);
	spt[3].set_range(0, 23);
	spf[4].set_range(0, 59);
	spt[4].set_range(0, 59);
	if(f.year == t.year) {
		spf[1].set_range(f.month, t.month);
		spt[1].set_range(f.month, t.month);
		if(f.month == t.month) {
			spf[2].set_range(f.day, t.day);
			spt[2].set_range(f.day, t.day);
			if(f.day == t.day) {
				spf[3].set_range(f.hour, t.hour);
				spt[3].set_range(f.hour, t.hour);
				if(f.hour == t.hour) {
					spf[4].set_range(f.minute, t.minute);
					spt[4].set_range(f.minute, t.minute);
				} 
			}
		}
	}
	spf[0].set_value(f.year);
	spt[0].set_value(t.year);
	spf[1].set_value(f.month);
	spt[1].set_value(t.month);
	spf[2].set_value(f.day);
	spt[2].set_value(t.day);
	spf[3].set_value(f.hour);
	spt[3].set_value(t.hour);
	spf[4].set_value(f.minute);
	spt[4].set_value(t.minute);
	show_all_children();
}

