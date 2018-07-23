#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <strings.h>
#include <stdlib.h>
#include <string>


using namespace std;

void taskContainer();

static int connFd;

int main(int argc, char* argv[])
{
  int pID, portNo, listenFd;
  socklen_t len; //store size of the address
  bool loop = false;
  struct sockaddr_in svrAdd, clntAdd;

  pthread_t threadA[3];

  if (argc < 2)
  {
    cerr << "Syntam : ./server <port>" << endl;
    return 0;
  }

  portNo = atoi(argv[1]);

  if((portNo > 65535) || (portNo < 2000))
  {
    cerr << "Please enter a port number between 2000 - 65535" << endl;
    return 0;
  }

  //create socklen
  listenFd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

  if(listenFd < 0)
  {
    cerr << "Cannot open socket" << endl;
    return 0;
  }

  bzero((char*) &svrAdd, sizeof(svrAdd));

  svrAdd.sin_family = AF_INET;
  svrAdd.sin_addr.s_addr = INADDR_ANY;
  svrAdd.sin_port = htons(portNo);

  //bind socket
  if(bind(listenFd, (struct sockaddr *)&svrAdd, sizeof(svrAdd)) < 0)
  {
    cerr << "Cannot bind" << endl;
    return 0;
  }

  listen(listenFd, 5);

  int noContnr = 0;

  while (noContnr < 3)
  {
    len = sizeof(clntAdd);
    cout << "listening" << endl;

    //this is where client connects. svr will hang in this mode until client connects
    connFd = accept(listenFd, (struct sockaddr *)&clntAdd, &len);

    if (connFd < 0)
    {
      cerr << "Cannot accept connection" << endl;
      return 0;
    }
    else
    {
      cout << "Connection successful" << endl;
    }

    taskContainer();

    noContnr++;
  }

  cout << "\nClosing thread and conn" << endl;
  close(connFd);
}

void taskContainer ()
{
  system("docker run hello-world");
}
