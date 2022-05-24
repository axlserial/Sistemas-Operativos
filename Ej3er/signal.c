#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void control_c(int signal);
void signalkill(int signal);

int main(){
	signal(2, control_c);
	signal(9, signalkill);
	while (1){
		pause();
	}
	return 0;
}

void control_c(int signal){
	printf("No quiero terminar\n");
}

void signalkill(int signal){
	printf("No me mates\n");
}