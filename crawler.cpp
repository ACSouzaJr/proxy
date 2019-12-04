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

  string request = "GET " + url + " HTTP/1.1\r\nHost: " + host + "\r\nConnection: close\r\n" + "Accept-Encoding: identity\r\n\r\n";

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
        return m.str(0);
    }

//    cout << "Erro ao encontrar host\n";
    return "";
}

void addExtension(string &file_name)
{
    // se nao encontrar a extensao adiciona
    if(file_name.find(".html") == std::string::npos)
    {
        file_name.append(".html");
    }
}

void replaceLinkRefereces(string &payload, string filename)
{
  string r("<\\s*A\\s+[^>]*href\\s*=\\s*\"(" + payload + "[^\"]*)\"");
  std::regex hl_regex(r, std::regex::icase);
  std::regex_replace(payload, hl_regex, "$0");
}
// intera pelo vetor de urls
// criar um arquivo com o nome da url
// Fazer download do site
// substituir referencias <a />
// escrever no arquivo o conteudo do site
void dumper(vector<string> accessed_links, string host) {

    cout << "dump started\n";

  std::ofstream outfile;

  for(auto link = 0; link < accessed_links.size(); link++) {
    size_t pos = accessed_links[link].find_first_of("/", 7) + 1;
    string file_name = accessed_links[link].substr(pos);

    if(file_name.empty()){
      file_name = "index.html";
    }
    addExtension(file_name);

    string payload = download_html(accessed_links[link], host);
    replaceLinkRefereces(payload, file_name);

    size_t html_tag = payload.find("<!DOCTYPE html");

    outfile.open(file_name);
    outfile << payload.substr(html_tag) << std::endl;
    outfile.close();
  }
  return;
}

bool existsInVector(vector<string> v, string elem)
{
    if(elem.find(".jpg") != std::string::npos || elem.find(".png") != std::string::npos)
      return true;

    return std::find(v.begin(), v.end(), elem) != v.end();
}

vector<string> crawling_in_my_skin(string host){
  queue<string> hostQueue;
  vector<string> accessed_links;
  string url;

  string root = "http://" + host + "/";
  hostQueue.push(root);
  accessed_links.push_back(root);

  // Begin with a base URL that you select, and place it on the top of your queue
  // Pop the URL at the top of the queue and download it
  // Parse the downloaded HTML file and extract all links
  // Insert each extracted link into the queue
  // Goto step 2, or stop once you reach some specified limit

  do {

    url = hostQueue.front();
    hostQueue.pop();

    // Filtra todos os links que fazem sentido pra aplicacao
    usleep(1000000);
    string response(download_html(url, host));
    std::regex hl_regex("<\\s*A\\s+[^>]*href\\s*=\\s*\"([^\"]*)\"", std::regex::icase);

    std::set<std::string> html_links(std::sregex_token_iterator(response.begin(), response.end(), hl_regex, 1),
                                     std::sregex_token_iterator());

    string str;

//    std::copy(html_links.begin(),
//              html_links.end(),
//              std::ostream_iterator<std::string>(std::cout, "\n"));

    for (auto it = html_links.begin(); it != html_links.end();)
    {
        if(!existsInVector(accessed_links, *it) && extractHost(*it) == host)
        {
            hostQueue.push(*it);
            accessed_links.push_back(*it);
            ++it;
        }
        else if(!existsInVector(accessed_links, *it) && extractHost(*it) == "")
        {
            hostQueue.push("http://" + host + *it);
            accessed_links.push_back(*it);
            cout << "host: http://" << host << *it << endl;
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

  cout << "fim de operacao links acessados \n";
  std::copy(accessed_links.begin(),
              accessed_links.end(),
              std::ostream_iterator<std::string>(std::cout, "\n"));
  cout << "end" << endl;

  return accessed_links;
}

int main(int argc, char const *argv[])
{

  string host = "samotabr.com";
  dumper(crawling_in_my_skin(host), host);
}
