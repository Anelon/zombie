#include <iostream>
using namespace std;

void Oi() {
	cout << "Hello\n";
}

int main() {//basic program using function pointers
	void (*ptr)();
	ptr = &Oi;
	ptr();
}
