#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/vfs.h>
#include <sys/statvfs.h>

/*
	Agregar que muestre el tipo del sistema de archivo
	tambien, darle formato
*/

int obtenerfs(long);

int main(int argc, char const *argv[]){
	struct statvfs vfs;
	struct statfs fs;
	char *ruta;

	// Verifica paso desde terminal
	if (argc != 2){
		printf("uso:  ./superbloque <archivo>\n");
		exit(EXIT_FAILURE);
	}

	// Copia ruta al char array
	ruta = malloc((strlen(argv[1]) + 1) + sizeof(char));
	strcpy(ruta, argv[1]);

	// Obtiene info del filesystem del archivo
	if (statvfs(ruta, &vfs) != 0){
		perror("statvfs");
		exit(EXIT_FAILURE);
	}

	if (statfs(ruta, &fs) != 0){
		perror("statfs");
		exit(EXIT_FAILURE);
	}

	printf("\tArchivo: %s\n", ruta);
	printf("\tID del S.A.: %#lx\n", (unsigned long)vfs.f_fsid);
	printf("\tLongitud max para archivos: %ld\n", (long)vfs.f_namemax);
	printf("\tTipo: %s\n", (obtenerfs(fs.f_type) ? "ext2/ext3/ext4": "otros"));
	printf("\tTamaño de bloques: %ld\n", (long)vfs.f_bsize);
	printf("\tTamaño de fragmento: %ld\n", (long)vfs.f_frsize);
	printf("\tTamaño en unidades: %ld\n", (unsigned long)vfs.f_blocks);
	printf("\tBloques libres: %lu\n", (unsigned long)vfs.f_bfree);
	printf("\tBloques disponibles: %lu\n", (unsigned long)vfs.f_bavail);
	printf("\tNúmero de inodos: %lu\n", (unsigned long)vfs.f_files);
	printf("\tNúmero de inodos libres: %lu\n", (unsigned long)vfs.f_ffree);
	printf("\tNúmero de inodos disponibles: %lu\n", (unsigned long)vfs.f_favail);
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

	free(ruta);

	return EXIT_SUCCESS;
}

int obtenerfs(long type){
	int tipo;

	switch (type){
	case 0xEF53:
		tipo = 1;
		// strcpy(tipo, "ext2/ext3/ext4");
		break;
	
	default:
		tipo = 0;
		// strcpy(tipo, "otros");
		break;
	}

	return tipo;
}