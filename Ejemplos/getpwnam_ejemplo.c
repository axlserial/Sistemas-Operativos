#include <stdio.h>
#include <sys/types.h>
#include <pwd.h>

int main(){
	struct passwd *p;
	// p = getpwnam("root");

	// printf("Name: %s\n", p->pw_name);
	// printf("Uid: %ld\n", (long)p->pw_uid);
	// printf("Gid: %ld\n", (long)p->pw_gid);
	// printf("Dir: %s\n", p->pw_dir);
	// printf("Shell: %s\n", p->pw_shell);

	while (p = getpwent()){
		printf("Name: %s\n", p->pw_name);
	}
}