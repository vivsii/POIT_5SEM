#include <windows.h>
#include <iostream>
#include <string>

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
    char* slotName = "\\\\.\\mailslot\\Box2";
    HANDLE hMailslot;

    try {
        hMailslot = CreateMailslot(slotName, 0, MAILSLOT_WAIT_FOREVER, NULL);
        if (hMailslot == INVALID_HANDLE_VALUE) {
            throw SetMailSlotError("Error creating Mailslot", GetLastError());
        }

        std::cout << "Server is waiting for messages..." << std::endl;

        while (true) {
            DWORD messageSize;
            DWORD messageCount;
            BOOL success = GetMailslotInfo(hMailslot, NULL, &messageSize, &messageCount, NULL);

            if (messageSize == MAILSLOT_NO_MESSAGE) {
                Sleep(50);
                continue;
            }

            if (!success) {
                throw SetMailSlotError("Error getting Mailslot info", GetLastError());
            }

            char* buffer = new char[messageSize];
            DWORD bytesRead;

            success = ReadFile(hMailslot, buffer, messageSize, &bytesRead, NULL);
            if (!success || bytesRead == 0) {
                delete[] buffer;
                continue;
            }

            std::string message = buffer;
            delete[] buffer;

            std::cout << "Received message: " << message << std::endl;

            HANDLE hClientMailslot = CreateFile("\\\\.\\mailslot\\ClientBox", GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
            if (hClientMailslot == INVALID_HANDLE_VALUE) {
                std::cerr << "Error opening client Mailslot: " << GetMailslotErrorMsgText(GetLastError()) << std::endl;
                continue;
            }

            DWORD bytesWritten;
            success = WriteFile(hClientMailslot, message.c_str(), message.size() + 1, &bytesWritten, NULL);
            if (!success) {
                std::cerr << "Error sending message back to client: " << GetMailslotErrorMsgText(GetLastError()) << std::endl;
            }

            CloseHandle(hClientMailslot);
        }
        CloseHandle(hMailslot);
    }
    catch (std::string& errMsg) {
        std::cerr << errMsg << std::endl;
    }

    return 0;
}

