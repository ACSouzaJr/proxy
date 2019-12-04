#include "toolkit.h"
#include "htmlutils.h"
#include <fstream>
#include <QtDebug>
#include <queue>
#include <regex>
#include <set>
#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sstream>
#include <QTreeWidgetItem>

Toolkit::Toolkit(QObject *parent) : QObject(parent)
{

}

void Toolkit::spider(const string host)
{
    qDebug() << "spider Started" << host.c_str() << "\n";
    crawler(host);
}

void Toolkit::recursiveClient(const string host)
{
    dumper(crawler(host), host);
}

string download_html(string url, string host)
{
  int sock = 0;
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
  serv_addr.sin_port = htons(80);

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

void addExtension(string &file_name)
{
    // se nao encontrar a extensao adiciona
    if(file_name.find(".html") == std::string::npos)
    {
        file_name.append(".html");
    }
}

void replaceLinkRefereces(string &payload, string host)
{
  string h = "http://" + host;
  // format to regex syntax
  h.replace(5,1,"\\/");
  h.replace(7,1,"\\/");
  string r("(<\\s*A\\s+[^>]*href\\s*=\\s*)\"" + h + "([^\"]*)\"");
//  qDebug() << "regex: " << r.c_str();
  std::regex hl_regex(r, std::regex::icase);
  payload = std::regex_replace(payload, hl_regex, "$1\".$2.html\"");

  // / -> index.html
  string r2("(<\\s*A\\s+[^>]*href\\s*=\\s*)\".\\/*.html\"");
//  qDebug() << "regex: " << r.c_str();
  std::regex h2_regex(r2, std::regex::icase);
  payload = std::regex_replace(payload, h2_regex, "$1\"./index.html\"");

  // remove reference to exit site
  string r3("(<\\s*A\\s+[^>]*href\\s*=\\s*)\"http([^\"]*)\"");
//  qDebug() << "regex: " << r.c_str();
  std::regex h3_regex(r3, std::regex::icase);
  payload = std::regex_replace(payload, h3_regex, "$1\"#\"");
}

void Toolkit::dumper(vector<string> accessed_links, string host) {
    // intera pelo vetor de urls
    // criar um arquivo com o nome da url
    // Fazer download do site
    // substituir referencias <a />
    // escrever no arquivo o conteudo do site

    qDebug() << "dump started\n";

  std::ofstream outfile;

  for(auto link = 0; link < accessed_links.size(); link++) {
    size_t pos = accessed_links[link].find_first_of("/", 7) + 1;
    string file_name = accessed_links[link].substr(pos);

    if(file_name.empty()){
      file_name = "index.html";
    }
    addExtension(file_name);

    string payload = download_html(accessed_links[link], host);
    replaceLinkRefereces(payload, host);

    size_t html_tag = payload.find("<!DOCTYPE html");

    outfile.open(file_name);
    outfile << payload.substr(html_tag) << std::endl;
    outfile.close();
  }
  qDebug() << "dump ended\n";
  return;
}

bool existsInVector(vector<string> v, string elem)
{
    if(elem.find(".jpg") != std::string::npos || elem.find(".png") != std::string::npos)
      return true;

    return std::find(v.begin(), v.end(), elem) != v.end();
}

vector<string> Toolkit::crawler(string host){
  queue<hostTree> hostQueue;
  vector<string> accessed_links;
  hostTree url;

  string root = "http://" + host + "/";
  accessed_links.push_back(root);

  QTreeWidgetItem *rootItem = new QTreeWidgetItem();

  rootItem->setText(0, QString(root.c_str()));
  emit appendRoot(rootItem);
  hostTree rootNode = {.link = root, .parent = nullptr, .linkRef = rootItem};
  hostQueue.push(rootNode);

  // Begin with a base URL that you select, and place it on the top of your queue
  // Pop the URL at the top of the queue and download it
  // Parse the downloaded HTML file and extract all links
  // Insert each extracted link into the queue
  // Goto step 2, or stop once you reach some specified limit
    hostTree childNode;
  do {

    url = hostQueue.front();
    hostQueue.pop();

    // Filtra todos os links que fazem sentido pra aplicacao
    usleep(1000000);
    string response(download_html(url.link, host));
    std::regex hl_regex("<\\s*[a,A]\\s+[^>]*href\\s*=\\s*\"([^\"]*)\"", std::regex::icase);

    std::set<std::string> html_links(std::sregex_token_iterator(response.begin(), response.end(), hl_regex, 1),
                                     std::sregex_token_iterator());

    string str;

//    std::copy(html_links.begin(),
//              html_links.end(),
//              std::ostream_iterator<std::string>(std::cout, "\n"));

    for (auto it = html_links.begin(); it != html_links.end();)
    {
        if(!existsInVector(accessed_links, *it) && HtmlUtils::extractHost(*it) == host)
        {
            QTreeWidgetItem *childItem = new QTreeWidgetItem();
            childItem->setText(0, QString(it->c_str()));

            childNode.link = *it;
            childNode.parent = url.linkRef;
            childNode.linkRef = childItem;

            hostQueue.push(childNode);
            accessed_links.push_back(*it);

            emit newAcessedLink(url.linkRef,childItem);

            ++it;
        }
        else if(!existsInVector(accessed_links, *it) && HtmlUtils::extractHost(*it) == "")
        {
            QTreeWidgetItem *childItem = new QTreeWidgetItem();
            childItem->setText(0, QString(it->c_str()));

            childNode.link = "http://" + host + *it;
            childNode.parent = url.linkRef;
            childNode.linkRef = childItem;

            hostQueue.push(childNode);
            accessed_links.push_back(*it);

            qDebug("host: http://%s%s \n", host.c_str(), it->c_str());
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

  std::cout << "fim de operacao links acessados \n";
  std::copy(accessed_links.begin(),
              accessed_links.end(),
              std::ostream_iterator<std::string>(std::cout, "\n"));
  std::cout << "end" << endl;

  return accessed_links;
}
