#pragma once
#include<gtkmm.h>

class ResButton : public Gtk::Button
{
public:
	ResButton(std::string name, std::string tel, int from, int until, float scale = 1);

protected:
	std::string name;
	std::string tel;
	int from;
	int until;
};

class Facility : public Gtk::HBox
{
public:
	Facility(std::string facility, int from, int end, float scale = 1);

protected:
	std::vector<ResButton> v;
	void on_click(std::string fac, std::string tel, int from, int to);
};

class Win : public Gtk::Window
{
public:
	Win(int, int, float);

protected:
	Gtk::ScrolledWindow scwin;
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
{
public:
	ResDialog(int from, int to);
	Gtk::SpinButton spf[5], spt[5];
	Gtk::Entry name, tel;
protected:
	Gtk::HBox hb1, hb2;
};
