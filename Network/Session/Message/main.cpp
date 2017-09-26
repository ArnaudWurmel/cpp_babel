#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> /* close */
#include <netdb.h> /* gethostbyname */
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;
#include "Message.h"

int	main()
{
  babel::Message	mess(babel::Message::Connect);

  mess.setBody("Arnaud", 7);
  mess.encodeHeader();
  int clientSocket;
  struct sockaddr_in serverAddr;
  socklen_t addr_size;

  clientSocket = socket(PF_INET, SOCK_STREAM, 0);
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(8080);
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  addr_size = sizeof(serverAddr);
  connect(clientSocket, (struct sockaddr *)&serverAddr, addr_size); 
  
  write(clientSocket, mess.data(), babel::Message::headerSize);
  write(clientSocket, mess.getBody(), mess.getBodySize());
}
