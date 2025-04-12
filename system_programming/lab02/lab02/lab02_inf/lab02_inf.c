#include <stdio.h>
#include <windows.h>
int main() {
	while (1) {
		printf("infinite cycle in process...\n");
		Sleep(2000);
	}
	return 0;
}
