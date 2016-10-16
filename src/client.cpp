#include"tcpip.h"
#include<string>
#include<iostream>
#include<fstream>
using namespace std;

int main(int c, char** v)
{
	string s;
	ifstream f("serverip.cfg");//서버의 ip주소를 저장한 파일.
	f >> s;
	Client cl(s);
	s.clear();
	if(c > 1) {
		for(int i=1; i<c; i++) {
			s += v[i]; 
			s += ' ';
		}
		s.pop_back(); //+= v[c-1];
	} else cout << "usage : " << v[0] << " [명령어]" << endl;
	cl.send(s);
	cout << cl.recv() << endl;
}
