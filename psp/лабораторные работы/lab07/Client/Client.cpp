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
    char* slotName = "\\\\WIN-OQKPG23JS0I\\mailslot\\Box2";

    try {
        HANDLE hMailslot = CreateFile(slotName, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hMailslot == INVALID_HANDLE_VALUE) {
            throw SetMailSlotError("Error creating Mailslot", GetLastError());
        }
        for(int i = 0;i < 1000; i++) {
            std::string message = "Hello from Maislot-client" + i % 5;
            DWORD bytesWritten;
            BOOL success = WriteFile(hMailslot, message.c_str(), message.size() + 1, &bytesWritten, NULL);

            if (success) {
                std::cout << "Message sent: " << message << std::endl;
            } else {
                throw SetMailSlotError("Message sendindg: ", GetLastError());
            }
            Sleep(1);
        }

        CloseHandle(hMailslot);
    }
    catch (std::string ErrorMailSlotText)
    {
        std::cout << std::endl << ErrorMailSlotText;
    }

    return 0;
}
