#include"frontend.h"
using namespace std;

int main(int c, char** v)
{
	int k = 1;
	auto app = Gtk::Application::create(k, v, "");
	float f = 1;
	if(c > 1) f = (float)atoi(v[1])/100;
	Win win{8832000, 8833750, f};
	app->run(win);
}
