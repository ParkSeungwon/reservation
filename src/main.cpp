#include"reserv.h"
#include"tcpip.h"
#include<iostream>
using namespace std;

int main()
{
	Reserv rsv;
//	Time f = {2016, 11, 13, 11, 0};
//	Time t = {2016, 11, 13, 12, 30};
//	Reservation r = {"박승원", "031-255-6698", to_minute(&f), to_minute(&t)};
//	rsv.reserv("청실", &r);
//	rsv.cancel("청실", 8871060);
//	rsv.display("청실");
//	rsv.display("홍실");
	
	cout << rsv("reserve 박승원 031-244 2016 11 1 12 0 2016 11 1 13 0 청실") << endl;
	cout << rsv("display 청실") << endl;

	Server sv;
	sv.start(rsv);
}
