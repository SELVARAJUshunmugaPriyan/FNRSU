/* The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <netdb.h>
#include <iostream>
#include <fstream>
#include <strings.h>
#include <string>

using namespace std;

void taskContainer();

void error(const char *msg)
{
  perror(msg);
  exit(1);
}

int main(int argc, char* argv[])
{
  int sockfd, newsockfd, portno; // pID, portNo, sockfd;
  socklen_t clilen; //store size of the address
  char buffer[256];
  struct sockaddr_in svr_addr, cli_addr;
  int n;

  pthread_t threadA[3];

  if (argc < 2)
  {
    cerr << "Syntam : ./server <port>" << endl;
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

  bzero((char*) &svr_addr, sizeof(svr_addr));
  /* setup the host_addr structure for use in bind call */
  // server byte order
  svr_addr.sin_family = AF_INET;

  // automatically be filled with current host's IP address
  svr_addr.sin_addr.s_addr = INADDR_ANY;

  // convert short integer value for port must be converted into network byte order
  portno = atoi(argv[1]);
  if((portno > 65535) || (portno < 2000))
  {
    cerr << "Please enter a port number between 2000 - 65535" << endl;
    return 0;
  }
  svr_addr.sin_port = htons(portno);

  // bind(int fd, struct sockaddr *local_addr, socklen_t addr_length)
  // bind() passes file descriptor, the address structure,
  // and the length of the address structure
  // This bind() call will bind the socket to the current IP address on port, portno
  if(bind(sockfd, (struct sockaddr *) &svr_addr, sizeof(svr_addr)) < 0)
  {
    cerr << "Cannot bind" << endl;
    return 0;
  }

  // This length() call tells the socket to listen to the incoming connections.
  // The listen() function places all incoming connection into a backlog queue
  // until accept() call accepts the connection.
  // Here, we set the maximum size for the backlog queue to 5.
  listen(sockfd, 5);

  int noContnr = 0;

  while (noContnr < 3)
  {
    clilen = sizeof(cli_addr);
    cout << "listening" << endl;

    // This accept() function will write the connecting client's address info
    // into the address structure and the size of that structure is clilen.
    // The accept() returns a new socket file descriptor for the accepted connection.
    // So, the original socket file descriptor can continue to be used
    // for accepting new connections while the new sokcet file descriptor is used for
    // communication with the connected client.
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

    if (newsockfd < 0)
    {
      cerr << "Cannot accept connection" << endl;
      return 0;
    }
    else
    {
      cout << "server: got connection from " << inet_ntoa(cli_addr.sin_addr) <<  " port " <<  ntohs(cli_addr.sin_port) << endl;
    }

    taskContainer();

    noContnr++;
  }

  cout << "\nClosing thread and conn" << endl;
  close(newsockfd);
}

void taskContainer ()
{
  system("docker run hello-world");
}
