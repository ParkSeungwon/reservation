#include"frontend.h"
using namespace std;

int main(int c, char** v)
{
	auto app = Gtk::Application::create(c, v, "");
	Win win{8832000, 8833750, 2};
	app->run(win);
}
