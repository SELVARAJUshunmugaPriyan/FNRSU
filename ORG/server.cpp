/* The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <netdb.h>
#include <iostream>
#include <fstream>
#include <strings.h>
#include <string>
#include <ctime>
#include <time.h>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>

using namespace std;

void tsk_cntr();

// system command for deploying container. xxxxxxxxxxxx -> container name
string CMD("docker run --name xxxxxxxxxxxx testRun --network fn-cntr-int-net -dit alpine sh > /dev/null && docker inspect --format='{{range .NetworkSettings.Networks}}{{.IPAddress}}{{end}}' xxxxxxxxxxxx");

int main(int argc, char* argv[])
{
  int sock_fl_desc, nov_sock_fl_desc, port_no;
  socklen_t clnt_len; //store size of the address
  char buffer[256];
  int no_cntr = 0;
  struct sockaddr_in svr_addr, cli_addr;
  int no_rcvd_bts;

  // acquire time
  time_t t;
  tm* now;
  char tme [20];

  if (argc < 2)
  {
    cerr << "Syntam : ./server <port>" << endl;
    return 0;
  }

  // create a socket
  // socket(int domain, int type, int protocol)
  sock_fl_desc = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  if(sock_fl_desc < 0)
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
  port_no = atoi(argv[1]);
  if((port_no > 65535) || (port_no < 2000))
  {
    cerr << "Please enter a port number between 2000 - 65535" << endl;
    return 0;
  }
  svr_addr.sin_port = htons(port_no);

  // bind(int fd, struct sockaddr *local_addr, socklen_t addr_length)
  // bind() passes file descriptor, the address structure,
  // and the length of the address structure
  // This bind() call will bind the socket to the current IP address on port, port_no
  if(bind(sock_fl_desc, (struct sockaddr *) &svr_addr, sizeof(svr_addr)) < 0)
  {
    cerr << "Cannot bind" << endl;
    return 0;
  }

  // This length() call tells the socket to listen to the incoming connections.
  // The listen() function places all incoming connection into a backlog queue
  // until accept() call accepts the connection.
  // Here, we set the maximum size for the backlog queue to 5.
  listen(sock_fl_desc, 5);

  while (no_cntr < 1)
  {
    clnt_len = sizeof(cli_addr);
    cout << "listening" << endl;

    // This accept() function will write the connecting client's address info
    // into the address structure and the size of that structure is clnt_len.
    // The accept() returns a new socket file descriptor for the accepted connection.
    // So, the original socket file descriptor can continue to be used
    // for accepting new connections while the new sokcet file descriptor is used for
    // communication with the connected client.
    nov_sock_fl_desc = accept(sock_fl_desc, (struct sockaddr *) &cli_addr, &clnt_len);

    if (nov_sock_fl_desc < 0)
    {
      cerr << "Cannot accept connection" << endl;
      return 0;
    }
    else
    {
      cout << "server: got connection from " << inet_ntoa(cli_addr.sin_addr) <<  " port " <<  ntohs(cli_addr.sin_port) << endl;
    }

    bzero(buffer, 256);

    no_rcvd_bts = read(nov_sock_fl_desc, buffer, 255);
    if (no_rcvd_bts < 0)
    {
      cerr << "Cannot read from socket" << endl;
    }
    cout << "[ ID ] " << buffer << endl;
    // memcmp does not depend on the received data being null terminated, on comapred to strcmp
    if ( memcmp(buffer, "9751914896", strlen("9751914896")) == 0)
    {
      t = time(0);
      now = localtime(&t);
      strftime (tme, 20, "%g%m%d%H%M%S", now);
      // place container name
      CMD.replace(18, 12, tme);
      CMD.replace(178, 12, tme);
      tsk_cntr();

    }
    else
    {
      cout << "UNAUTHORIZED ONBOARD REQUEST" << endl;
    }
    no_cntr++;
  }

  cout << "\nClosing thread and conn" << endl;
  close(nov_sock_fl_desc);
}

void tsk_cntr ()
{
  system(CMD.c_str());
}
