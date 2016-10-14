#include"reserv.h"

int main()
{
	Reserv rsv;
	Time f = {2016, 11, 13, 15, 0};
	Time t = {2016, 11, 13, 17, 30};
	Reservation r = {"박승원", "031-255-6698", to_minute(&f), to_minute(&t)};
	//rsv.reserv("홍실", &r);
	rsv.display("청실");
	rsv.display("홍실");
}
