#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <algorithm>
#include <iostream>
#include <vector>

#include "Winsock2.h"
#pragma comment(lib, "WS2_32.lib")

using namespace std;

bool GetServer(char* name, short port, sockaddr* from, int* flen);


std::string GetErrorMsgText(int code) {
    std::string msgText;
    switch (code) {
    case WSAEINTR: msgText = "Interrupted function call";
        break;
    case WSAEACCES: msgText = "Permission denied";
        break;
    case WSAEFAULT: msgText = "Bad address";
        break;
    case WSAEINVAL: msgText = "Invalid argument";
        break;
    case WSAEMFILE: msgText = "Too many open files";
        break;
    case WSAEWOULDBLOCK: msgText = "Resource temporarily unavailable";
        break;
    case WSAEINPROGRESS: msgText = "Operation now in progress";
        break;
    case WSAEALREADY: msgText = "Operation already in progress";
        break;
    case WSAENOTSOCK: msgText = "Socket operation on non-socket";
        break;
    case WSAEDESTADDRREQ: msgText = "Destination address required";
        break;
    case WSAEMSGSIZE: msgText = "Message too long";
        break;
    case WSAEPROTOTYPE: msgText = "Protocol wrong type for socket";
        break;
    case WSAENOPROTOOPT: msgText = "Bad protocol option";
        break;
    case WSAEPROTONOSUPPORT: msgText = "Protocol not supported";
        break;
    case WSAESOCKTNOSUPPORT: msgText = "Socket type not supported";
        break;
    case WSAEOPNOTSUPP: msgText = "Operation not supported";
        break;
    case WSAEPFNOSUPPORT: msgText = "Protocol family not supported";
        break;
    case WSAEAFNOSUPPORT: msgText = "Address family not supported by protocol family";
        break;
    case WSAEADDRINUSE: msgText = "Address already in use";
        break;
    case WSAEADDRNOTAVAIL: msgText = "Cannot assign requested address";
        break;
    case WSAENETDOWN: msgText = "Network is down";
        break;
    case WSAENETUNREACH: msgText = "Network is unreachable";
        break;
    case WSAENETRESET: msgText = "Network dropped connection on reset";
        break;
    case WSAECONNABORTED: msgText = "Software caused connection abort";
        break;
    case WSAECONNRESET: msgText = "Connection reset by peer";
        break;
    case WSAENOBUFS: msgText = "No buffer space available";
        break;
    case WSAEISCONN: msgText = "Socket is already connected";
        break;
    case WSAENOTCONN: msgText = "Socket is not connected";
        break;
    case WSAESHUTDOWN: msgText = "Cannot send after socket shutdown";
        break;
    case WSAETIMEDOUT: msgText = "Connection timed out";
        break;
    case WSAECONNREFUSED: msgText = "Connection refused";
        break;
    case WSAEHOSTDOWN: msgText = "Host is down";
        break;
    case WSAEHOSTUNREACH: msgText = "No route to host";
        break;
    case WSAEPROCLIM: msgText = "Too many processes";
        break;
    case WSASYSNOTREADY: msgText = "Network subsystem is unavailable";
        break;
    case WSAVERNOTSUPPORTED: msgText = "Winsock.dll version out of range";
        break;
    case WSANOTINITIALISED: msgText = "Successful WSAStartup not yet performed";
        break;
    case WSAEDISCON: msgText = "Graceful shutdown in progress";
        break;
    case WSATYPE_NOT_FOUND: msgText = "Class type not found";
        break;
    case WSAHOST_NOT_FOUND: msgText = "Host not found";
        break;
    case WSATRY_AGAIN: msgText = "Non-authoritative host not found";
        break;
    case WSANO_RECOVERY: msgText = "Non-recoverable error";
        break;
    case WSANO_DATA: msgText = "Valid name, no data record of requested type";
        break;
    case WSA_INVALID_HANDLE: msgText = "Specified event object handle is invalid";
        break;
    case WSA_INVALID_PARAMETER: msgText = "One or more parameters are invalid";
        break;
    case WSA_IO_INCOMPLETE: msgText = "Overlapped I/O event object not in signaled state";
        break;
    case WSA_IO_PENDING: msgText = "Overlapped operations will complete later";
        break;
    case WSA_NOT_ENOUGH_MEMORY: msgText = "Not enough memory available";
        break;
    case WSA_OPERATION_ABORTED: msgText = "Overlapped operation aborted";
        break;
    case WSASYSCALLFAILURE: msgText = "System call failure";
        break;
    default: break;
    }
    return msgText;
};

std::string SetErrorMsgText(std::string msgText, int code) {
    return msgText + GetErrorMsgText(code);
};

SOCKET sock;

int main() {
    WSADATA wsaData;
    SOCKADDR_IN from;
    char name[] = "Hello";
    memset(&from, 0, sizeof(from));
    int lfrom = sizeof(from);
    int optval = 1;
    vector<string> explored;
    try {
        if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
            throw SetErrorMsgText("Startup: ", WSAGetLastError());
        cout << "CLIENT" << endl;
        if ((sock = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
            throw SetErrorMsgText("socket: ", WSAGetLastError());
        bool flag = false;
        while (true) {
            if (GetServer(name, 2000, (sockaddr*)&from, &lfrom)) {
                for (auto ip : explored) {
                    if (strcmp(ip.c_str(), inet_ntoa(from.sin_addr)) == 0) {
                        flag = true;
                    }
                }
                if (!flag) {
                    cout << "Connected server: " << endl;
                    cout << "IP: " << inet_ntoa(from.sin_addr) << endl;
                    cout << "Port: " << htons(from.sin_port) << endl;
                    explored.push_back(inet_ntoa(from.sin_addr));
                }
                else {
                    flag = false;
                }
            }
        }

        if (closesocket(sock) == SOCKET_ERROR)
            throw SetErrorMsgText("closesocket: ", WSAGetLastError());
        if (WSACleanup() == SOCKET_ERROR)
            throw SetErrorMsgText("Cleanup: ", WSAGetLastError());
    }
    catch (string errorMsgText) {
        cout << "WSAGetLastError: " << errorMsgText << endl;
    }
}

bool GetServer(char* name, short port, sockaddr* from, int* flen) {
    ((SOCKADDR_IN*)from)->sin_family = AF_INET;
    ((SOCKADDR_IN*)from)->sin_port = htons(port);
    ((SOCKADDR_IN*)from)->sin_addr.s_addr = inet_addr("172.20.10.15");
    int lbuf;
    char ibuf[50];
    int optval = 1;


    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char*)&optval, sizeof(optval)) == SOCKET_ERROR)
        throw SetErrorMsgText("opt: ", WSAGetLastError());

    if (lbuf = sendto(sock, name, strlen(name) + 1, NULL, from, *flen) == SOCKET_ERROR)
        throw SetErrorMsgText("sendto: ", WSAGetLastError());

    if (lbuf = recvfrom(sock, ibuf, sizeof(ibuf), NULL, from, flen) == SOCKET_ERROR) {
        if (WSAGetLastError() == WSAETIMEDOUT)
            return false;
        else
            throw SetErrorMsgText("recvfrom: ", WSAGetLastError());
    }
    return true;
}