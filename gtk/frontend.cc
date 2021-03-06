#include"frontend.h"
#include<fstream>
#include<string>
#include<iostream>
#include"reserv.h"
using namespace std;

string cut(string&);

string psstm(string command)
{//명령을 실행하고 표준출력을 string으로 리턴, 클라이언트를 사용할 함수.
	char buf[10000];
	FILE* f = popen(command.c_str(), "r");
	fgets(buf, sizeof(buf), f);
	pclose(f);
	return buf;
}

Facility::Facility(string fac, int start, int end, float scale)
{
	string command = "./client.x display " + fac;//시설의 예약 상황을 표시하는 구문
	string s = psstm(command);
	s.pop_back();//remove '\n'!! 
	int from, until;
	string tel;
	while(!s.empty()) {
		ResButton bt{cut(s), tel = cut(s), from = stoi(cut(s)), 
			until = stoi(cut(s)), scale};
		bt.signal_clicked().connect(
				bind(&Facility::on_click, this, fac, tel, from, until));
		if(start < from) {
			ResButton tmp{"", "", start, from, scale};
			tmp.signal_clicked().connect(
					bind(&Facility::on_click, this, fac, "", start, from));
			v.push_back(move(tmp));
		}
		v.push_back(move(bt));
		start = until;
	}
	if(start < end) {
		v.push_back(ResButton{"", "", start, end, scale});
		v.back().signal_clicked().connect(
					bind(&Facility::on_click, this, fac, "", start, end));
	}
	for(auto& a : v) pack_start(a, Gtk::PACK_SHRINK);
	show_all_children();
}

Win* p;

void Facility::on_click(string fac, string tel, int from, int until)
{//셀을 클릭했을 때 실행되는 함수.
	if(!tel.empty()) {//예약된 셀일 경우
		Gtk::MessageDialog cancel("예약을 취소하시겠습니까?", 
				false, Gtk::MESSAGE_WARNING, Gtk::BUTTONS_YES_NO);
		cancel.set_secondary_text("연락처 : " + tel);
		if(cancel.run() == Gtk::RESPONSE_YES) {
			Time t = to_time(from);
			string command = "./client.x cancel " + to_string(t.year) + ' ' 
				+ to_string(t.month) + ' ' + to_string(t.day) + ' '
				+ to_string(t.hour) + ' ' + to_string(t.minute) + ' ' + fac;
			cout << psstm(command.data());
		}
	} else {//빈 셀일 경우
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
			if(from <= mf && mf < mt && mt <= until) {//예약을 실행하는 구문
				string command = "./client.x reserve " + respin.name.get_text() 
					+ ' ' + respin.tel.get_text() + ' ' + to_string(f.year) + ' '
					+ to_string(f.month) + ' ' + to_string(f.day) + ' ' +
					to_string(f.hour) + ' ' + to_string(f.minute) + ' ' +
					to_string(t.year) + ' ' + to_string(t.month) + ' ' +
					to_string(t.day) + ' ' + to_string(t.hour) + ' ' + 
					to_string(t.minute) + ' ' + fac;
				cout << psstm(command.data());
			}
		}
	}
	p->response(Gtk::RESPONSE_ACCEPT);//부모 윈도우를 종료시킨다.
}


ResButton::ResButton(string name, string tel, int from, int until, float scale)
{
	this->name = name;
	this->tel = tel;
	this->from = from;
	this->until = until;
	set_label(name);//셀의 라벨을 예약자 이름으로
	set_size_request(scale * (until - from), -1);//셀의 크기를 예약 시간에 맞게
}

Win::Win(int start, int end, float scale) : mon("월"), day("일"), hr("시간")
{
	p = this;
	string s;
	for(ifstream f("facility.txt"); f >> s;) {
		v.push_back(Facility{s, start, end, scale});
		vl.push_back(Gtk::Button(s));
	}

	Time start_time = to_time(start);
	Time end_time = to_time(end);
	Time tmp = start_time;
	tmp.month++;
	tmp.day = 0;
	tmp.hour = 0;
	tmp.minute = 0;
	extern int day_of_months[12];
	for(int t = to_minute(&tmp); t < end; ) {
		vm.push_back(ResButton{to_string(to_time(start).month)+"월", "", 
				start, t, scale});
		start = t;
		tmp = to_time(t);
		tmp.month++;
		if(tmp.month == 13) {
			tmp.year++;
			tmp.month = 1;
		}
		t = to_minute(&tmp);
	}
	vm.push_back(ResButton{to_string(to_time(start).month) + "월", "", 
			start, end, scale});
	for(auto& a : vm) mon_box.pack_start(a, Gtk::PACK_SHRINK);
	fac_label_box.pack_start(mon, Gtk::PACK_SHRINK);
	
	if(scale >= 0.05) {//일 단위의 표시일 경우
		fac_label_box.pack_start(day, Gtk::PACK_SHRINK);
		tmp = start_time;
		start = to_minute(&tmp);
		tmp.minute = 0;
		tmp.hour = 0;
		tmp.day++;

		//날짜 버튼들을 늘어놓는다.
		for(int t = to_minute(&tmp); t < end; start = t, t += 60 * 24) 
			vd.push_back(ResButton{//move symantic을 이용하기 위해
					to_string(to_time(start).day)+"일", "", start, t, scale});
		vd.push_back(ResButton{
				to_string(to_time(start).day)+"일", "", start, end, scale});
		for(auto& a : vd) day_box.pack_start(a, Gtk::PACK_SHRINK);
	}

	if(scale >= 1) {//시간 단위의 표시일 경우
		fac_label_box.pack_start(hr, Gtk::PACK_SHRINK);
		tmp = start_time;
		start = to_minute(&tmp);
		tmp.minute = 0;
		int d = tmp.hour++;

		//시간 버튼들을 늘어놓는다.
		for(int t= to_minute(&tmp); t < end; start = t, t += 60) 
			vh.push_back(ResButton{to_string(d++ % 24)+"시", "", start, t, scale});
		vh.push_back(ResButton{to_string(d % 24)+"시", "", start, end, scale});
		for(auto& a : vh) hour_box.pack_start(a, Gtk::PACK_SHRINK);
	}
	pack_all();
}

void Win::pack_all()
{
	Gtk::Box* box = get_content_area();
	box->pack_start(hb);
	add(hb);
	hb.pack_start(fac_label_box, Gtk::PACK_SHRINK);
	hb.pack_start(scwin);
	scwin.add(vb);

	vb.pack_start(mon_box, Gtk::PACK_SHRINK);
	vb.pack_start(day_box, Gtk::PACK_SHRINK);
	vb.pack_start(hour_box, Gtk::PACK_SHRINK);
	for(auto& a : v) vb.pack_start(a, Gtk::PACK_SHRINK);

	for(auto& a : vl) fac_label_box.pack_start(a, Gtk::PACK_SHRINK);
	
	set_default_size(1000, 700);
	set_title("예약 시스템");
	show_all_children();
}

ResDialog::ResDialog(int from, int until)
{//사용자 입력을 편리하게 하기 위한 스핀버튼과 입력창이다.
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

	//스핀 버튼들을 달력 형식처럼 설정
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

