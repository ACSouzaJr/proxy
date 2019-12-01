// #include "socket.hpp"
#include <string>
#include <queue>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <regex>
#include <set>
#include <unistd.h>

#define PORT 80

using namespace std;

string download_html(string url, string host)
{
  int sock = 0, valread;
  struct sockaddr_in serv_addr;
  struct hostent *host_entry; /* server host name information        */
  std::stringstream buffer;
  char cur;
  char *IPbuffer;

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    printf("\n Socket creation error \n");
    exit(1);
  }

  memset(&serv_addr, '0', sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);

  host_entry = gethostbyname(host.c_str());
  if (host_entry == (struct hostent *)0)
  {
    printf("Gethostbyname failed\n");
    exit(1);
  }

  IPbuffer = inet_ntoa(*((struct in_addr *)host_entry->h_addr_list[0]));
  printf("%s\n", IPbuffer);

  // Convert IPv4 and IPv6 addresses from text to binary form
  if (inet_pton(AF_INET, IPbuffer, &serv_addr.sin_addr) <= 0)
  {
    printf("\nInvalid address/ Address not supported \n");
    exit(1);
  }

  // serv_addr.sin_addr.s_addr = *(long*)host_entry->h_addr_list[0];

  cout << "connecting to servern\n";

  // printf("Press any key to connect to the server\n");
  // getchar();
  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
  {
    printf("\nConnection Failed \n");
    exit(1);
  }
  // printf("Successfully connected to server. Press any key to send a message\n");
  // getchar();

  string request = "GET " + url + " HTTP/1.1\r\nHost:" + host + "\r\nConnection: close\r\n\r\n";

  cout << request;
  send(sock, request.c_str(), request.size(), 0);

  printf("Receiving response from server\n");

  while (recv(sock, &cur, 1, 0) > 0)
  {
    buffer << cur;
  }

  return buffer.str();
}

bool existsInVector(vector<string> v, string elem)
{
  if (v.empty())
    return false;

  return std::find(v.begin(), v.end(), elem) != v.end();
}

int main(int argc, char const *argv[])
{
  // Socket* sock = new Socket(PORT);
  // sock->create();
  // sock->readRequest();
  // sock->createClient();
  // sock->receiveResponse();
  // sock->sendRequest();

  queue<string> hostQueue;
  hostQueue.push("http://www.sitepx.com/");
  vector<string> accessed_links;
  string url;
  string host = "www.sitepx.com";

  do
  {
    do
    {  
      url = hostQueue.front();
      hostQueue.pop();
    } while (existsInVector(accessed_links, url));
    

    accessed_links.push_back(url);

    // Filtra todos os links que fazem sentido pra aplicacao
    usleep(1000000);
    string response(download_html(url, host));
    std::regex hl_regex("<\\s*A\\s+[^>]*href\\s*=\\s*\"([^\"]*)\"", std::regex::icase);

    std::set<std::string> html_links(std::sregex_token_iterator(response.begin(), response.end(), hl_regex, 1),
                                     std::sregex_token_iterator());

    for (auto it = html_links.begin(); it != html_links.end();)
    {
      // se encontrar o link com o hotname
      if (it->find(host) != std::string::npos)
      {
        // Colocar todos os links dentro de um vetor
        hostQueue.push(*it);
        ++it;
      }
      else
      {
        it = html_links.erase(it);
      }
    }

    std::copy(html_links.begin(),
              html_links.end(),
              std::ostream_iterator<std::string>(std::cout, "\n"));

  } while (!hostQueue.empty());

  return 0;
}
