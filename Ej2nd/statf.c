#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/statvfs.h>

/*
	Agregar que muestre el tipo del sistema de archivo
	tambien, darle formato
*/

int main(){
	struct statvfs vfs;
	char *ruta = "/";

	if (statvfs(ruta, &vfs) != 0){
		perror("statvfs");
		exit(EXIT_FAILURE);
	}

	printf("\tArchivo: %s\n", ruta);
	printf("\tTamaño de bloques: %ld\n", (long)vfs.f_bsize);
	printf("\tTamaño de fragmento: %ld\n", (long)vfs.f_frsize);
	printf("\tTamaño en unidades: %ld\n", (unsigned long)vfs.f_blocks);
	printf("\tBloques libres: %lu\n", (unsigned long)vfs.f_bfree);
	printf("\tBloques disponibles: %lu\n", (unsigned long)vfs.f_bavail);
	printf("\tNúmero de inodos: %lu\n", (unsigned long)vfs.f_files);
	printf("\tNúmero de inodos libres: %lu\n", (unsigned long)vfs.f_ffree);
	printf("\tNúmero de inodos disponibles: %lu\n", (unsigned long)vfs.f_favail);
	printf("\tID del S.A.: %#lx\n", (unsigned long)vfs.f_fsid);
	printf("\tBandera:	");

	if (vfs.f_flag == 0){
		printf("(Ninguna)\n");
	} else {
		if ((vfs.f_flag & ST_RDONLY) != 0)
			printf("ST_RDONLY");
		if ((vfs.f_flag & ST_NOSUID) != 0)
			printf("ST_NOSUID");
		printf("\n");
	}

	printf("\tLongitud max para archivos: %ld\n", (long)vfs.f_namemax);
	return EXIT_SUCCESS;
}