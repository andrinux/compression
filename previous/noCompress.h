#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


#define BUF_SIZE 4096

void *nullOpen(int fd, const char *mode)
{
	return (void *)(intptr_t) fd;
}

static off_t nullCompress(void *cancel_cookie, int fd_source, int fd_dest)
{
	char buf[BUF_SIZE];
	unsigned wr;
	int rd;
	off_t size = 0;

	while ((rd = read(fd_source, buf, sizeof(buf))) > 0)
	{
		size += rd;

		wr = write(fd_dest, buf, rd);
		if (wr == 0)
		{
			return (off_t) FAIL;
		}

		if (compress_testcancel(cancel_cookie))
		{
			break;
		}
	}

	if (rd < 0)
	{
		return (off_t) FAIL;
	}

	return size;
}




