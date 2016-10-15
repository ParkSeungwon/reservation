#pragma once
#include<gtkmm.h>

class ResButton : public Gtk::Button
{
public:
	ResButton(std::string name, std::string tel, int from, int until);

protected:
	std::string name;
	std::string tel;
	int from;
	int until;
};

class Facility : public Gtk::HBox
{
public:
	Facility(std::string facility);

protected:
	std::vector<ResButton> v;
};

class Win : public Gtk::Window
{
public:
	Win();

protected:
	Gtk::VBox vb;
	std::vector<Facility> v;
};
