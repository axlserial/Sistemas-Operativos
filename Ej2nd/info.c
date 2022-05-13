#include <stdio.h>
#include <sys/sysinfo.h>

#define minuto 60
#define hora (minuto * 60)
#define dia (hora * 24)
#define KB 1024

int main(){
	struct sysinfo info;

	sysinfo(&info);
	printf("Tiempo del sistema: %ld días, %ld:%02ld:%02ld\n",
		info.uptime / dia, (info.uptime % dia) / hora, (info.uptime % hora) / minuto, info.uptime % minuto);
	printf("Total RAM: %ld KB\n", info.totalram / KB);
	printf("Libre RAM: %ld KB\n", info.freeram / KB);
	printf("Swap: %ld KB\n", info.totalswap / KB);
	printf("Cantidad de procesos: %d\n", info.procs);

	return 0;
}