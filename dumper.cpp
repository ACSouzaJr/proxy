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
#include <iostream>
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

  cout << "connecting to server...\n";

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

string extractHost(string header)
{
    std::regex r( R"rgx((?:\w+\.)+\w+)rgx" );

    std::smatch m;
    if (std::regex_search(header, m, r))
    {
//      for (auto v : m)
//      std::cout << v << std::endl;
        return m.str(0);
    }

    // qDebug() << "Erro ao encontrar host\n";
    cout << "Erro ao encontrar host\n";
    return "";
}

bool existsInVector(vector<string> v, string elem)
{
  if (v.empty())
    return false;

  return std::find(v.begin(), v.end(), elem) != v.end();
}

int main(int argc, char const *argv[])
{
  vector<string> accessed_links;
  accessed_links.push_back("http://www.sitepx.com/");
  accessed_links.push_back("http://www.sitepx.com/about.html");
  accessed_links.push_back("http://www.sitepx.com/ajuda.html");

  string host = "www.sitepx.com";

  for(auto link = 0; link < accessed_links.size(); link++) {
    size_t pos = accessed_links[link].find_last_of("/") + 1;
    string file_name = accessed_links[link].substr(pos);
    
    if(file_name.empty()){
      file_name = "index.html";
    }

    std::ofstream outfile (file_name);
    
    string payload = download_html(accessed_links[link], host);
    size_t html_tag = payload.find("<!DOCTYPE html");
    outfile << payload.substr(html_tag) << std::endl;
    outfile.close();
  }
  
  // intera pelo vetor de urls
  // criar um arquivo com o nome da url
  // Fazer download do site
  // substituir referencias <a />
  // escrever no arquivo o conteudo do site
  //
  

  return 0;
}
