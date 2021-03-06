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
  int sock_f_dsc, port_no;
  struct sockaddr_in serv_addr;
  struct hostent *server;
  char buffer[255];

  if(argc < 3)
  {
    cerr << "Syntax : ./client <host name> <port>" << endl;
    return 0;
  }

  // create a socket
  // socket(int domain, int type, int protocol)
  sock_f_dsc = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  if(sock_f_dsc < 0)
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
  port_no = atoi(argv[2]);
  if((port_no > 65535) || (port_no < 2000))
  {
    cerr << "Please enter port number between 2000 - 65535" << endl;
    return 0;
  }
  serv_addr.sin_port = htons(port_no);

  // Connect the socket to the address of the server using the connect() system call.
  int checker = connect(sock_f_dsc, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
  if (checker < 0)
  {
    cerr << "Cannot connect!" << endl;
    return 0;
  }

  bzero(buffer, 256);

  // send ID to server
  send(sock_f_dsc, "9751914896", 10, 0);
  read(sock_f_dsc, buffer, 255);

  cout << buffer << endl;
  system((string("ping ") + buffer).c_str());

}
