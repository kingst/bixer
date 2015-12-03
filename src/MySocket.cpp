#include "MySocket.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string>

using namespace std;

MySocket::MySocket(const char *inetAddr, int port)
{
    struct sockaddr_in server;
    struct addrinfo hints;
    struct addrinfo *res;

    // set up the new socket (TCP/IP)
    sockFd = socket(AF_INET,SOCK_STREAM,0);
    
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    int ret = getaddrinfo(inetAddr, NULL, &hints, &res);
    if(ret != 0) {
        string str;
        str = string("Could not get host ") + string(inetAddr);
        throw MySocketException(str.c_str());
    }
    
    server.sin_addr = ((struct sockaddr_in *) (res->ai_addr))->sin_addr;
    server.sin_port = htons((short) port);
    server.sin_family = AF_INET;
    freeaddrinfo(res);
    
    // conenct to the server
    if( connect(sockFd, (struct sockaddr *) &server,
                sizeof(server)) == -1 ) {
        throw MySocketException("Did not connect to the server");
    }
    
}
MySocket::MySocket(void)
{
    sockFd = -1;
}

MySocket::MySocket(int socketFileDesc)
{
    sockFd = socketFileDesc;
}

MySocket::~MySocket(void)
{
    close();
}


bool MySocket::write_bytes(string buffer)
{
    return write_bytes(buffer.c_str(), buffer.size());
}
bool MySocket::write_bytes(const void *buffer, int len)
{
    const unsigned char *buf = (const unsigned char *) buffer;
    int bytesWritten = 0;

    if(sockFd<0) {
      throw MySocketException("Not connected");
    }

    while(len > 0) {
        bytesWritten = ::write(sockFd, buf, len);
        if(bytesWritten <= 0) {
	  throw MySocketException("could not write to socket");
        }
        buf += bytesWritten;
        len -= bytesWritten;
    }

    return true;

}

int MySocket::read(void *buffer, int len)
{
    if(sockFd<0) {
      throw MySocketException("Not connected");
    }
    
    int ret = ::read(sockFd, buffer, len);
    
    // XXX FIXME should I throw an exception when ret == 0?
    if(ret < 0) {
      throw MySocketException("could not read from socket");
    }
  
    return ret;
}

void MySocket::close(void)
{
    if(sockFd<0) return;
    
    ::close(sockFd);

    sockFd = -1;
}
