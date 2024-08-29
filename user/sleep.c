#include "kernel/types.h"
#include "user/user.h"

int main(int args, char *argv[]){
	if(args != 2){
		fprintf(2, "must 1 argument for sleep\n");
		exit(1);
	}
	int sleepNum = atoi(argv[1]);
	printf("(nothing happens for a little while)\n");
	sleep(sleepNum);
	exit(0);
}
