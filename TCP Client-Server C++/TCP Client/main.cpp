#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

using namespace std;

int main()
{
    string ipAddress = "127.0.0.1"; // ip address of the server
    int port = 54000;               // listening port # on the server

    // create socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        cerr << "Error in opening a socket" << endl;
    }

    // fill in a hint structure
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr); // put byte version of ipAddress in hint.sin_addr

    // connect to server
    int connResult = connect(sockfd, (sockaddr *)&hint, sizeof(hint));
    if (connResult < 0)
    {
        cerr << "can't connect to server" << endl;
    }

    // Do-while loop to send and retrive data
    char buf[4096];
    string userInput;
    do
    {
        cout << ">";
        getline(cin, userInput);

        if (userInput.size() > 0)
        {
            int sendResult = send(sockfd, userInput.c_str(), userInput.size() + 1 /* in c string every string is terminated with a 0 character*/, 0);
            if (sendResult > 0)
            {
                memset(buf, 0, 4096);
                int bytesRecieved = recv(sockfd, buf, 4096, 0);
                if (bytesRecieved > 0)
                {
                    cout << "server reponse: " << string(buf, 0, bytesRecieved) << endl;
                }
            }
        }

    } while (userInput.size() > 0);

    // gracefully close
    close(sockfd);
}