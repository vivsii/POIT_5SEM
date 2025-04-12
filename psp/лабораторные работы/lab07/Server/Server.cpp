#include <windows.h>
#include <iostream>


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
    char* slotName = "\\\\.\\mailslot\\Box";
    DWORD timeout = 180000;
    try {
        HANDLE hMailslot = CreateMailslot(slotName, 500, timeout, NULL);
        if (hMailslot == INVALID_HANDLE_VALUE) {
            throw SetMailSlotError("Error creating Mailslot", GetLastError());
        }

        std::cout << "The server is waiting for messages..." << std::endl;

        while (true) {
            DWORD messageSize;
            DWORD messageCount;

            BOOL success = GetMailslotInfo(hMailslot, NULL, &messageSize, &messageCount, NULL);

            if (!success) {
                std::cerr << "Error receiving information from Mailslot: " << GetLastError() << std::endl;
                break;
            }

            if (messageSize == MAILSLOT_NO_MESSAGE) {
                char buffer[300];
                DWORD bytesRead;

                success = ReadFile(hMailslot, buffer, sizeof(buffer), &bytesRead, NULL);
                if (!success) {
                    DWORD error = GetLastError();
                    if (error == ERROR_SEM_TIMEOUT) {
                        std::cout << "Timeout: no messages received in the last 3 minutes." << std::endl;
                        break;
                    } else {
                        std::cerr << "Error reading from Mailslot: " << error << std::endl;
                        break;
                    }
                } else {
                    std::cout << "Message received: " << buffer << std::endl;
                }
            }

        }
        CloseHandle(hMailslot);
        return 0;
    }
    catch (std::string ErrorMailSlotText)
    {
        std::cout << std::endl << ErrorMailSlotText;
    }
}
