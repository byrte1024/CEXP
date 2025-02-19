#include <string>
#include <Windows.h>

void errorPopup(std::string title, std::string message) {
    MessageBoxA(NULL, message.c_str(), title.c_str(), MB_ICONERROR);
}

void warningPopup(std::string title, std::string message) {
    MessageBoxA(NULL, message.c_str(), title.c_str(), MB_ICONWARNING);
}

void messagePopup(std::string title, std::string message) {
    MessageBoxA(NULL, message.c_str(), title.c_str(), MB_ICONINFORMATION);
}

bool questionPopup(std::string title, std::string message) {
    int result = MessageBoxA(NULL, message.c_str(), title.c_str(), MB_ICONQUESTION | MB_YESNO);
    return result == IDYES;
}