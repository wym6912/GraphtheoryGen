#include <iostream>
#include <string>

const int SUPPORTED_MODE = 3;
const int MODE_NODE = 1, MODE_EDGE = 2, MODE_DIRECTED = 3;
int mode = 0;

template <typename T>
inline void __assert__(const T &condition, const std::string &message)
{
    if (! condition)
    {
        std :: cerr << message << std :: endl;
        exit(2);
    }
}

bool DispatchCharacters()
{
	char ch;
	Read:
	while(std :: cin >> ch && (ch == 'N' || ch == 'Y' || ch == 'y' || ch == 'n'))
	{
		if(ch == 'N' || ch == 'n') return false;
		if(ch == 'Y' || ch == 'y') return true;
	}
	goto Read;
}

int main()
{
	std :: ios :: sync_with_stdio(false);
	std :: cout << "We support " << SUPPORTED_MODE << " modes.\n";
	std :: cout << "This program can tell you the mode number.\n";
	std :: cout << "Mode 1: Do you want to display information on nodes? (Y/N)\n";
	if(DispatchCharacters()) ++ mode;
	std :: cout << "Mode 2: Do you want to display information on edges? (Y/N)\n";
	if(DispatchCharacters()) mode += 2;
	std :: cout << "Mode 3: Do you want to view the Undirected edges? (Y/N)\n";
	if(DispatchCharacters()) mode += 4;
	std :: cout << "Your mode is " << mode << '\n';
	return 0;
}
