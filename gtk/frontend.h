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
	Facility(std::string facility, int from, float scale = 1);

protected:
	std::vector<ResButton> v;
	void on_click(std::string fac, std::string tel, int from, int to);
};

class Win : public Gtk::Window
{
public:
	Win();

protected:
	Gtk::ScrolledWindow scwin;
	Gtk::VBox vb;
	std::vector<Facility> v;
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
