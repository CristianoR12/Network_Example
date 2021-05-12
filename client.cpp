#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <sys/time.h>

using namespace std;

int main()
{
    //Create a socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        return -1;
    } 

    //Create a hint structure for the server we're connecting with
    int port = 54000;
    string ipAddress = "127.0.0.1";
    
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

    // Connect to the server on the socket
    int connectRes = connect(sock, (sockaddr*)&hint, sizeof(hint));
    if (connectRes == -1)
    {
        return 1;
    } 

    //While loop to send name of file to the server
    string filename;  
    int sendRes = 0;

    do {
        //      Enter name of file 
        cout << "> ";
        getline(cin, filename);
        
        //      Send to server
        sendRes = send(sock, filename.c_str(), filename.size() + 1, 0);
        if (sendRes == -1)
        {
            cout << "Could not send to server! \r\n";
            continue;
        }              
    } while(!sendRes);       
    
    //FILE SIZE///////////////////////////////////////
    //Receive the size of the file
    char rec_Siz[10];
    memset(rec_Siz, 0, 10);

    recv(sock, rec_Siz, 10, 0);
    int size = std::stoi(rec_Siz);//convert char to int

    cout << "Size of file: " << size << " bytes." << endl;
    ////////////////////////////////////////////////////
    
    // creating a file to write the received text
    ofstream fout;
    fout.open("sample.txt");
            
    //Reception of the file 
    char rec_buf[100];
    memset(rec_buf, 0, 100);
    
    while(recv(sock, rec_buf, 100, 0)) 
    {
        fout << rec_buf << endl;      
    }

    fout.close();     
    
    //Close the socket
    close(sock);

    return 0;
}

