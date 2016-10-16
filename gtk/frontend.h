#pragma once
#include<gtkmm.h>

class ResButton : public Gtk::Button
{//테이블 셀에 해당하는 버튼, 예약 길이에 따라 크기가 달라짐.
public:
	ResButton(std::string name, std::string tel, int from, int until, float scale = 1);

protected:
	std::string name;
	std::string tel;
	int from;
	int until;
};

class Facility : public Gtk::HBox
{//각 시설당 한 줄로 위의 테이블 셀을 담는 컨테이너
public:
	Facility(std::string facility, int from, int end, float scale = 1);

protected:
	std::vector<ResButton> v;
	void on_click(std::string fac, std::string tel, int from, int to);
};

class Win : public Gtk::Dialog
{//메인 윈도우
public:
	Win(int, int, float);

protected:
	Gtk::ScrolledWindow scwin, scr;
	Gtk::VBox vb, fac_label_box;
	Gtk::HBox hb;
	std::vector<Facility> v;
	std::vector<Gtk::Button> vl;
	std::vector<ResButton> vm, vd, vh;
	Gtk::Button mon, day, hr;
	Gtk::HBox mon_box, day_box, hour_box;

private:
	void pack_all();
};

class ResDialog : public Gtk::Dialog
{//셀을 클릭했을 때 나오는 예약 입력 윈도우
public:
	ResDialog(int from, int to);
	Gtk::SpinButton spf[5], spt[5];
	Gtk::Entry name, tel;

protected:
	Gtk::HBox hb1, hb2;
};
