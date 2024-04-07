/*
 * Exercise 8-5. Modify the fsize program to print the other information
 * contained in the inode entry.
*/

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>	/* structure returned by stat */
#include <stdint.h>
#include <time.h>
#include <dirent.h>
#include <sys/sysmacros.h>
#include <pwd.h>
#include <grp.h>

void fstats(char *);

/* print file sizes */
int main(int argc, char *argv[])
{
	if (argc == 1)	/* default: current directory */
		fstats(".");
	else
		while (--argc > 0)
			fstats(*++argv);
	return 0;
}

void dirwalk(char *, void (*fcn)(char *));

/* fstats: print stats of file "name" */
void fstats(char *name)
{
	struct stat stbuf;
	struct passwd *pw;
	struct group *gr;

	if (stat(name, &stbuf) == -1) {
		fprintf(stderr, "fsize: can't access %s\n", name);
		return;
	}
	if ((stbuf.st_mode & S_IFMT) == S_IFDIR)
		dirwalk(name, fstats);

	printf("%s\n", name);
	printf("ID of containing device: [%jx,%jx]\n",
			(uintmax_t) major(stbuf.st_dev),
			(uintmax_t) minor(stbuf.st_dev));
	printf("File type:		");
	switch (stbuf.st_mode & S_IFMT) {
	case S_IFBLK:	printf("block device\n");	break;
	case S_IFCHR:	printf("character device\n");	break;
	case S_IFDIR:	printf("directory\n");		break;
	case S_IFIFO:	printf("FIFO/pipe\n");		break;
	case S_IFLNK:	printf("symlink\n");		break;
	case S_IFREG:	printf("regular file\n");	break;
	#ifdef S_IFSOCK
	case S_IFSOCK:	printf("socket\n");		break;
	#endif
	default:	printf("unknown?\n");		break;
	}

	printf("I-node number:		%ju\n", (uintmax_t) stbuf.st_ino);

	printf("Mode:			%jo (octal)\n",
		       (uintmax_t) stbuf.st_mode);	
	
	printf("User permissions:	");
	putchar((stbuf.st_mode & S_IRUSR) ? 'r' : '-');
	putchar((stbuf.st_mode & S_IWUSR) ? 'w' : '-');
	putchar((stbuf.st_mode & S_IXUSR) ? 'x' : '-');
	printf("\nGroup permission:	");
	putchar((stbuf.st_mode & S_IRGRP) ? 'r' : '-');
	putchar((stbuf.st_mode & S_IWGRP) ? 'w' : '-');
	putchar((stbuf.st_mode & S_IXGRP) ? 'x' : '-');
	printf("\nOthers permissions:	");
	putchar((stbuf.st_mode & S_IROTH) ? 'r' : '-');
	putchar((stbuf.st_mode & S_IWOTH) ? 'w' : '-');
	putchar((stbuf.st_mode & S_IXOTH) ? 'x' : '-');

	printf("\nLink count:		%ju\n", (uintmax_t) stbuf.st_nlink);

	printf("Ownership:		UID=%ju	GID=%ju\n",
			(uintmax_t) stbuf.st_uid, (uintmax_t) stbuf.st_gid);
	if (pw = getpwuid(stbuf.st_uid))
		printf("\tusername:	%s\n", pw->pw_name);
	if (gr = getgrgid(stbuf.st_gid))
		printf("\tgroupname:	%s\n", gr->gr_name);

	printf("Preferred I/O block size: %jd bytes\n",
			(intmax_t) stbuf.st_blksize);
	printf("File size:		%jd bytes\n",
			(intmax_t) stbuf.st_size);
	printf("Blocks allocated:	%d\n",
			(intmax_t) stbuf.st_blocks);

	printf("Last status change:	%s", ctime(&stbuf.st_ctime));
	printf("Last file access:	%s", ctime(&stbuf.st_atime));
	printf("Last file modification:	%s\n", ctime(&stbuf.st_mtime));
}

#define MAX_PATH 1024

/* dirwalk: apply fcn to all files in dir */
void dirwalk(char *dir, void (*fcn)(char *))
{
	char name[NAME_MAX];
	struct dirent *dp;
	DIR *dfd;

	if ((dfd = opendir(dir)) == NULL) {
		fprintf(stderr, "dirwalk: can't open %s\n", dir);
		return;
	}
	while ((dp = readdir(dfd)) != NULL) {
		if (strcmp(dp->d_name, ".") == 0
		 || strcmp(dp->d_name, "..") == 0)
			continue;	/* skip self and parent */
		if (strlen(dir)+strlen(dp->d_name)+2 > sizeof(name))
			fprintf(stderr, "dirwalk: name %s/%s too long\n",
					dir, dp->d_name);
		else {
			sprintf(name, "%s/%s", dir, dp->d_name);
			(*fcn)(name);
		}
	}
	closedir(dfd);
}
