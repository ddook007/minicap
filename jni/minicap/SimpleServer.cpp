#include "SimpleServer.hpp"

#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>

SimpleServer::SimpleServer(): mFd(0) {
}

SimpleServer::~SimpleServer() {
  if (mFd > 0) {
    ::close(mFd);
  }
}

int
SimpleServer::start(int sockport) {
  // int sfd = socket(AF_UNIX, SOCK_STREAM, 0);

  // if (sfd < 0) {
  //   return sfd;
  // }

  // struct sockaddr_un addr;
  // memset(&addr, 0, sizeof(addr));
  // addr.sun_family = AF_UNIX;
  // strncpy(&addr.sun_path[1], sockname, strlen(sockname));

  // if (::bind(sfd, (struct sockaddr*) &addr,
  //     sizeof(sa_family_t) + strlen(sockname) + 1) < 0) {
  //   ::close(sfd);
  //   return -1;
  // }

  // 不使用unix abstract socket，便于app内socket 连接到minicap不会被拒绝
  int sfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sfd < 0) {
    return sfd;
  }
  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY; 
  addr.sin_port = htons( sockport );
  if (::bind(sfd, (struct sockaddr*) &addr,
      sizeof(addr)) < 0) {
    ::close(sfd);
    return -1;
  }

  ::listen(sfd, 1);

  mFd = sfd;

  return mFd;
}

int
SimpleServer::accept() {
  // struct sockaddr_un addr;
  // socklen_t addr_len = sizeof(addr);
  // return ::accept(mFd, (struct sockaddr *) &addr, &addr_len);
  struct sockaddr_in addr;
  socklen_t addr_len = sizeof(addr);
  return ::accept(mFd, (struct sockaddr *) &addr, &addr_len);
}
