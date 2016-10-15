#include"tcpip.h"
#include<string>
#include<iostream>
using namespace std;

int main()
{
	Client cl;
	string s;
	getline(cin, s);
	cl.send(s);
	cout << cl.recv() << endl;
}
