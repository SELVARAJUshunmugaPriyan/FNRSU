/* The server address and port number are passed as arguments */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <strings.h>
#include <string>
#include <time.h>
#include <vector>

using namespace std;

int main (int argc, char* argv[])
{
  int sockfd, portno;
  struct sockaddr_in serv_addr;
  struct hostent *server;

  char buffer[256];
  if(argc < 3)
  {
    cerr << "Syntax : ./client <host name> <port>" << endl;
    return 0;
  }

  // create a socket
  // socket(int domain, int type, int protocol)
  sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  if(sockfd < 0)
  {
    cerr << "Cannot open socket" << endl;
    return 0;
  }

  // get server address
  server = gethostbyname(argv[1]);
  if(server == NULL)
  {
    cerr << "Host does not exist" << endl;
    return 0;
  }

  bzero((char *) &serv_addr, sizeof(serv_addr));
  /* setup the host_addr structure for use in bind call */
  // server byte order
  serv_addr.sin_family = AF_INET;

  bcopy((char *) server -> h_addr, (char *) &serv_addr.sin_addr.s_addr, server -> h_length);
  // convert short integer value for port must be converted into network byte order
  portno = atoi(argv[2]);
  if((portno > 65535) || (portno < 2000))
  {
    cerr << "Please enter port number between 2000 - 65535" << endl;
    return 0;
  }
  serv_addr.sin_port = htons(portno);

  // Connect the socket to the address of the server using the connect() system call.
  int checker = connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
  if (checker < 0)
  {
    cerr << "Cannot connect!" << endl;
    return 0;
  }

  // send ID to server
  send(sockfd, "9751914896", 10, 0);
}
