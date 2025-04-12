#include <windows.h>
#include <iostream>
#include <string>
#include <sstream>

std::string GetMailslotErrorMsgText(int code) {
    std::string msgText;
    switch (code) {
        case ERROR_FILE_NOT_FOUND:
            msgText = "Mailslot not found";
        break;
        case ERROR_ACCESS_DENIED:
            msgText = "Access to Mailslot denied";
        break;
        case ERROR_INVALID_HANDLE:
            msgText = "Invalid handle for Mailslot";
        break;
        case ERROR_ALREADY_EXISTS:
            msgText = "Mailslot already exists";
        break;
        case ERROR_INVALID_PARAMETER:
            msgText = "Invalid parameter";
        break;
        case ERROR_IO_PENDING:
            msgText = "I/O operation is in progress";
        break;
        case ERROR_INSUFFICIENT_BUFFER:
            msgText = "Insufficient buffer for Mailslot message";
        break;
        case ERROR_MORE_DATA:
            msgText = "More data available for Mailslot message";
        break;
        case ERROR_BAD_NETPATH:
            msgText = "Bad network path for Mailslot";
        break;
        case ERROR_NETNAME_DELETED:
            msgText = "Network name for Mailslot deleted";
        break;
        case ERROR_SEM_TIMEOUT:
            msgText = "Mailslot timed out waiting for response";
        break;
        case ERROR_OPERATION_ABORTED:
            msgText = "Mailslot operation aborted";
        break;
        case ERROR_NO_SYSTEM_RESOURCES:
            msgText = "No system resources available for Mailslot";
        break;
        case ERROR_NOT_CONNECTED:
            msgText = "Mailslot server is not connected";
        break;
        case ERROR_DISK_FULL:
            msgText = "Disk is full, cannot write to Mailslot";
        break;
        default:
            msgText = "Unknown Mailslot error";
        break;
    }
    return msgText;
}

std::string SetMailSlotError(std::string msgText, int code) {
    return msgText + ": " + GetMailslotErrorMsgText(code);
}

int main() {
    char* serverSlotName = "\\\\WIN-OQKPG23JS0I\\mailslot\\Box2";
    char* clientSlotName = "\\\\.\\mailslot\\Box2";
    HANDLE hClientMailslot;

    hClientMailslot = CreateMailslot(clientSlotName, 0, MAILSLOT_WAIT_FOREVER, NULL);
    if (hClientMailslot == INVALID_HANDLE_VALUE) {
        std::cerr << SetMailSlotError("Error creating client Mailslot", GetLastError()) << std::endl;
        return 1;
    }

    try {
        HANDLE hServerMailslot = CreateFile(serverSlotName, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hServerMailslot == INVALID_HANDLE_VALUE) {
            throw SetMailSlotError("Error opening server Mailslot", GetLastError());
        }

        int counter = 1;

        while (true) {
            std::ostringstream messageStream;
            messageStream << "Hello from Mailslot-client " << counter;
            std::string message = messageStream.str();

            DWORD bytesWritten;
            BOOL success = WriteFile(hServerMailslot, message.c_str(), message.size() + 1, &bytesWritten, NULL);
            if (!success) {
                throw SetMailSlotError("Error sending message", GetLastError());
            }

            std::cout << "Sent message: " << message << std::endl;

            DWORD messageSize;
            DWORD messageCount;

            while (true) {
                BOOL success = GetMailslotInfo(hClientMailslot, NULL, &messageSize, &messageCount, NULL);
                if (!success) {
                    throw SetMailSlotError("Error getting client Mailslot info", GetLastError());
                }

                if (messageSize == MAILSLOT_NO_MESSAGE) {
                    Sleep(50); // Ждем появления сообщения
                    continue;
                }

                char* buffer = new char[messageSize];
                DWORD bytesRead;

                success = ReadFile(hClientMailslot, buffer, messageSize, &bytesRead, NULL);
                if (!success || bytesRead == 0) {
                    delete[] buffer;
                    throw SetMailSlotError("Error reading from client Mailslot", GetLastError());
                }

                std::string receivedMessage = buffer;
                delete[] buffer;

                std::cout << "Received message from server: " << receivedMessage << std::endl;

                counter++;
                break;
            }

            if (counter > 1000) {
                break;
            }

            Sleep(1);
        }

        CloseHandle(hServerMailslot);
    }
    catch (std::string& errMsg) {
        std::cerr << errMsg << std::endl;
    }

    CloseHandle(hClientMailslot);

    return 0;
}
