#include"tcpip.h"
#include<string>
#include<iostream>
using namespace std;

int main(int c, char** v)
{
	if(c < 4) {
		cout << v[0] << " [server ip] [port] [commands.....]" << endl;
		return 0;
	}
	Client cl(v[1], atoi(v[2]));
	string s;
	for(int i=3; i<c; i++) {
		s += v[i];
		s += ' ';
	}
	s.back() = '\n';
	s += '\n';
	cl.send(s);
	while(s.size()) cout << (s = cl.recv()) << endl;
}
