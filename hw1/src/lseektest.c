#include "types.h"
#include "stat.h"
#include "user.h"

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

int main(int argc, char *argv[]) {

	// printf(1, "%s %s %s %s %d\n", argv[0], argv[1], argv[2], argv[3], argc);

	if (argc < 4) {
		printf(1, "usage : lseektest <filename> <offset> <string>\n");
		exit();
	}

	int fd = open(argv[1], 2); //2: can read and write

	if (fd < 0) {
		printf(1, "lseek: cannot open %s\n", argv[1]);
		exit();
	}

	char buf[20];
	int n;

	lseek(fd, 0, SEEK_SET);
	
	printf(1, "Before : ");
	
	while ((n = read(fd, buf, sizeof(buf) - 1)) > 0) { // read returns size
		buf[n] = '\0';
		printf(1, "%s", buf);
	}
	printf(1, "\n");

	n = atoi(argv[2]);

	lseek(fd, n, SEEK_SET);

	write(fd, argv[3], strlen(argv[3]));

	lseek(fd, 0, SEEK_SET);

	printf(1, "After : ");
	while ((n = read(fd, buf, sizeof(buf) - 1)) > 0) {
        buf[n] = '\0';
        printf(1, "%s", buf);
    }
	printf(1, "\n");

	close(fd);

	exit();
}