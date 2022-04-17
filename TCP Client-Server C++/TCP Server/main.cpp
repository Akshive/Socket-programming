#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>

using namespace std;

int main()
{
    // Create a socket
    int listening = socket(AF_INET /* IPV4 */, SOCK_STREAM /* TCP */, 0);
    if (listening < 0)
    {
        cerr << "can't craete a socket!" << endl;
        return -1;
    }

    // Bind an ip address to the socket
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    hint.sin_addr.s_addr = INADDR_ANY;

    bind(listening, (sockaddr *)&hint, sizeof(hint));

    listen(listening, SOMAXCONN);

    // wait for the connection
    sockaddr_in client;
    int clientSize = sizeof(client);

    int clientSocket = accept(listening, (sockaddr *)&client, (socklen_t *)&clientSize);

    char host[1025];
    char service[1025];

    memset(host, 0, 1025);
    memset(service, 0, 1025);

    if (getnameinfo((sockaddr *)&client, sizeof(client), host, 1025, service, 1025, 0) == 0)
    {
        cout << host << " connected on port " << service << endl;
    }
    else
    {
        inet_ntop(AF_INET, &client.sin_addr, host, 1025);
        cout << host << " connected on port " << ntohs(client.sin_port) << endl;
    }

    // close listening socket
    close(listening);

    // while loop: accept and echo message back to client
    char buf[4096];
    while (true)
    {
        memset(buf, 0, 4096);

        int bytesReceived = recv(clientSocket, buf, 4096, 0);
        if (bytesReceived < 0)
        {
            cerr << "Error in recv() " << endl;
            break;
        }
        if (bytesReceived == 0)
        {
            cout << "client disconnected" << endl;
            break;
        }

        send(clientSocket, buf, bytesReceived + 1, 0);
    }

    // Close the socket
    close(clientSocket);
}