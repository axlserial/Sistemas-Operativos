/*
 *	Documentación del programa 'superbloque.c'
 *
 *	Descripción: 	Programa que muestra las características del superbloque del sistema de archivos
 *					donde se encuentre el archivo dado por argumento en la linea de comandos.
 *
 *	Modo de compilación: gcc -Wall superbloque.c -o superbloque
 *
 *	Modo de ejecución:	./superbloque <archivo>
 *
 *	Elaborado por:
 *		Ayala Ruíz Mario Antonio
 *		Elorza Velásquez Margarita
 *		García González Axel Isaac
 *
 *	Licencia: CC BY-NC-SA 4.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/vfs.h>
#include <sys/statvfs.h>
#include <linux/magic.h>

char *obtenerfs(long);

int main(int argc, char const *argv[]){
	struct statvfs vfs;
	struct statfs fs;
	char *ruta;

	// Verifica paso desde linea de comandos
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

	char *tipo = obtenerfs(fs.f_type);

	printf("\tArchivo: %s\n", ruta);
	printf("\tID del S.A.: %#lx\n", (unsigned long)vfs.f_fsid);
	printf("\tLongitud max para archivos: %ld\n", (long)vfs.f_namemax);
	printf("\tTipo: %s\n", tipo);
	printf("\tTamaño de bloques: %ld\n", (long)vfs.f_bsize);
	printf("\tTamaño de fragmento: %ld\n", (long)vfs.f_frsize);
	printf("\tTamaño en unidades: %ld\n", (unsigned long)vfs.f_blocks);
	printf("\tBloques libres: %lu\n", (unsigned long)vfs.f_bfree);
	printf("\tBloques disponibles: %lu\n", (unsigned long)vfs.f_bavail);
	printf("\tNúmero de inodos: %lu\n", (unsigned long)vfs.f_files);
	printf("\tNúmero de inodos libres: %lu\n", (unsigned long)vfs.f_ffree);
	printf("\tNúmero de inodos disponibles: %lu\n", (unsigned long)vfs.f_favail);
	printf("\tBandera(s): ");

	if (vfs.f_flag == 0){
		printf("(Ninguna)\n");
	} else {

		// ST_MANDLOCK
		if ((vfs.f_flag & 64))
			printf("ST_MANDLOCK ");

		// ST_NOATIME
		if ((vfs.f_flag & 1024))
			printf("ST_NOATIME ");

		// ST_NODEV
		if ((vfs.f_flag & 4))
			printf("ST_NODEV ");

		// ST_NODIRATIME
		if ((vfs.f_flag & 2048))
			printf("ST_NODIRATIME ");

		// ST_NOEXEC
		if ((vfs.f_flag & 8))
			printf("ST_NOEXEC ");

		if ((vfs.f_flag & ST_NOSUID) != 0)
			printf("ST_NOSUID ");

		if ((vfs.f_flag & ST_RDONLY) != 0)
			printf("ST_RDONLY ");

		// ST_RELATIVE
		if ((vfs.f_flag & 4096))
			printf("ST_RELATIME ");

		// ST_SYNCHRONOUS
		if ((vfs.f_flag & 16))
			printf("ST_SYNCHRONOUS ");

		printf("\n");
	}

	free(ruta);

	return EXIT_SUCCESS;
}

/*
*	Función: 		char* obtenerfs(long type)
*	Descripción:	Función que retorna el nombre del sistema de archivos según el
*					tipo dado por parámetro.
*	Parametro de entrada:	long type:	Valor del atributo 'f_type' de la estructura 'statfs' 				
*	Retorno:				char *:		String que contiene el nombre del sistema de archivos
*/
char* obtenerfs(long type){

	switch (type){
	
	/* ext2/ext3/ext4 tiene el mismo magic number */
		case EXT4_SUPER_MAGIC:
			return "ext2/ext3/ext4";

		case MSDOS_SUPER_MAGIC:
			return "ms-dos";

		case EXFAT_SUPER_MAGIC:
			return "exfat";

		case REISERFS_SUPER_MAGIC:
			return "reiserfs";

    	case TMPFS_MAGIC:
    	    return "tmpfs";

    	case NFS_SUPER_MAGIC:
    	    return "nfs";
    
		case V9FS_MAGIC:
    	    return "9p";
    
		case RAMFS_MAGIC:
    	    return "ramfs";
    
		case BTRFS_SUPER_MAGIC:
    	    return "btrfs";
    
		case XFS_SUPER_MAGIC:
    	    return "xfs";
    
		case MINIX_SUPER_MAGIC:
    	case MINIX_SUPER_MAGIC2:
    	case MINIX2_SUPER_MAGIC:
    	case MINIX2_SUPER_MAGIC2:
    	case MINIX3_SUPER_MAGIC:
    	    return "minix";
    
		case UDF_SUPER_MAGIC:
    	    return "udf";
    
		case F2FS_SUPER_MAGIC:
    	    return "f2fs";
    
		case NILFS_SUPER_MAGIC:
    	    return "nilfs";
    
		case OVERLAYFS_SUPER_MAGIC:
    	    return "overlayfs";
    
		case FUSE_SUPER_MAGIC:
    	    return "fuse";	
	
		default:
			return "desconocido";
	}
}