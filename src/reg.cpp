#include<regex>
#include<string>
#include<iostream>
using namespace std;

string psstm(string command)
{//return system call output as string
	string s;
	char buf[100];
	FILE* f = popen(command.c_str(), "r");
	while(fgets(buf, sizeof(buf), f)) s += buf;
	pclose(f);
	return s;
}

int main(int ac, char** av) 
{
	string s, s2;
	regex e{string("[\\\'\\\"]http://\\S+") + av[2] + "[\\\'\\\"]"};
	smatch m;
	//regex e{R"lit(([\'\"]http://\w*[\"\']))lit"};
	char c;
	while(cin >> noskipws >> c) s += c;
	while(regex_search(s, m, e)) {
		for(auto& a : m) s2 += a;
		s2.erase(0,1);
		s2.pop_back();
		if(*av[1] == 'd') {
			s2.insert(0, "wget -t 0 ");
			system(s2.data());
		} else if(*av[1] == 'b') {
			s2.erase(0, 7);
			s2.insert(0, "./tel.x `addr ");
			s2.append("` 80 GET / HTTP/1.0\n\n");
			system(s2.data());
			cout << s2 << endl;
		} else cout << s2 << endl;
		s = m.suffix().str();
		s2.clear();
	}
}

