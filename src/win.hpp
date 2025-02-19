#include <string>


//Create an error popup
void errorPopup(std::string title, std::string message);

//Create a warning popup
void warningPopup(std::string title, std::string message);

//Create a message popup
void messagePopup(std::string title, std::string message);

//Create a question popup
bool questionPopup(std::string title, std::string message);