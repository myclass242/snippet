#include "Buffer.h"

#include <sys/uio.h>

using namespace muduo;


ssize_t Buffer::readFd(int fd, int* savedErrno)
{
    char extra[65536];
    struct iovec iov[2];
    iov[0].iov_base = beginWrite();
    iov[0].iov_len = writableBytes();
    iov[1].iov_base = extra;
    iov[1].iov_len = sizeof(extra);

    ssize_t n = ::readv(fd, iov, 2);
    if (n == -1) {
        *savedErrno = errno;
    } else if (n <= writableBytes()) {
        hasWritten(n);
    } else {
        hasWritten(writableBytes());
        append(iov[1].iov_base, n - iov[0].iov_len);
    }
    return n;
}