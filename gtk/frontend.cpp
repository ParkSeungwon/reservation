#include"frontend.h"
using namespace std;

int main(int c, char** v)
{
	auto app = Gtk::Application::create(c, v, "");
	Win win;
	app->run(win);
}
