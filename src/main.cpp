#include"reserv.h"
#include"tcpip.h"

int main()
{
	Reserv rsv;
	Time f = {2016, 11, 13, 11, 0};
	Time t = {2016, 11, 13, 12, 30};
	Reservation r = {"박승원", "031-255-6698", to_minute(&f), to_minute(&t)};
	rsv.reserv("청실", &r);
	rsv.cancel("청실", 8871060);
	rsv.display("청실");
	rsv.display("홍실");

	Server sv;
	sv.start(rsv);
}
