#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <regex>
#include <thread>
#include <chrono>

#include "linwin.hpp"


std::unordered_map<std::string, std::string> stringVariables;
std::unordered_map<std::string, int> intVariables;

void slowPrint(const std::string& text, int delay = 30) {
    for (char c : text) {
        std::cout << c;
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }
    std::cout << std::endl;
}

void fastPrint(const std::string& text) {
    std::cout << text << std::endl;
}

void showAsciiArt() {
    linwin::set_color(ANSI(ANSI_FG, ANSI_CYAN));
    std::cout << R"(
8b    d8    db    88b 88 88   88    db    88               dP""b8 
88b  d88   dPYb   88Yb88 88   88   dPYb   88     ________ dP   `" 
88YbdP88  dP__Yb  88 Y88 Y8   8P  dP__Yb  88  .o """""""" Yb      
88 YY 88 dP""""Yb 88  Y8 `YbodP' dP""""Yb 88ood8           YboodP 
    )" << std::endl;
    linwin::reset_color();
}

void startEditor();
void showFunctional();
void showSyntax();
void showAuthors();

bool isNumber(const std::string& s) {
    return !s.empty() && std::find_if(s.begin(), s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}

std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(' ');
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

void showMainMenu() {
    linwin::clear();
    showAsciiArt();
    slowPrint("\n1. Coding\n2. Functional\n3. Syntax\n4. Author\n5. Quit\n");
    slowPrint("Select an option: ");
}

// Интерпретатор Manual-C
void interpretManualC(const std::string& code) {
    bool useMk = false; 
    [[maybe_unused]] bool useString = false;
    std::vector<std::string> lines;

    
    size_t pos = 0, found;
    while ((found = code.find_first_of("\n", pos)) != std::string::npos) {
        lines.push_back(code.substr(pos, found - pos));
        pos = found + 1;
    }
    lines.push_back(code.substr(pos));

    std::regex variablePattern(R"((\w+)\s*=\s*(.*);)");   
    std::regex mklinePattern(R"(mkline\((.*)\);)");       
    std::smatch matches;

    
    for (const auto& line : lines) {
        std::string trimmedLine = trim(line); 

        
        if (trimmedLine.find("#use-flag <mk>") != std::string::npos) {
            useMk = true;
        }
        if (trimmedLine.find("#use-flag <string>") != std::string::npos) {
            useString = true;
        }

        
        if (std::regex_search(trimmedLine, matches, variablePattern)) {
            std::string varName = matches[1].str();
            std::string varValue = matches[2].str();

            varValue = trim(varValue);

            
            if (isNumber(varValue)) {
                intVariables[varName] = std::stoi(varValue);  
            }
            else if (varValue.front() == '"' && varValue.back() == '"') {
                stringVariables[varName] = varValue.substr(1, varValue.size() - 2);  
            }
            else {
                slowPrint("[Error]: Invalid variable format.");
                return;
            }
        }

        
        if (std::regex_search(trimmedLine, matches, mklinePattern)) {
            if (!useMk) {
                slowPrint("[Error]: Missing required <mk> flag.");
                return;
            }

            std::string argument = trim(matches[1].str());

            
            std::regex concatPattern(R"((.*)\s*\+\s*(.*))");
            if (std::regex_search(argument, matches, concatPattern)) {
                std::string firstArg = trim(matches[1].str());
                std::string secondArg = trim(matches[2].str());

                std::string result;
                if (intVariables.find(firstArg) != intVariables.end()) {
                    result += std::to_string(intVariables[firstArg]);
                }
                else if (stringVariables.find(firstArg) != stringVariables.end()) {
                    result += stringVariables[firstArg];
                }
                else if (firstArg.front() == '"' && firstArg.back() == '"') {
                    result += firstArg.substr(1, firstArg.size() - 2);
                }

                if (intVariables.find(secondArg) != intVariables.end()) {
                    result += std::to_string(intVariables[secondArg]);
                }
                else if (stringVariables.find(secondArg) != stringVariables.end()) {
                    result += stringVariables[secondArg];
                }
                else if (secondArg.front() == '"' && secondArg.back() == '"') {
                    result += secondArg.substr(1, secondArg.size() - 2);
                }

                slowPrint(result);
            }
            else {
                
                if (intVariables.find(argument) != intVariables.end()) {
                    slowPrint(std::to_string(intVariables[argument]));  
                }
                else if (stringVariables.find(argument) != stringVariables.end()) {
                    slowPrint(stringVariables[argument]);  
                }
                else if (argument.front() == '"' && argument.back() == '"') {
                    slowPrint(argument.substr(1, argument.size() - 2));  
                }
                else {
                    slowPrint("[Error]: Unknown variable or invalid argument.");
                    return;
                }
            }
        }


        // Пропускаем комментарии
        if (trimmedLine.find("//") != std::string::npos) {
            continue;
        }
    }
}

void startEditor() {
    linwin::clear();
    slowPrint("mkvim 2.1    press ESC to run code");
    std::string code;
    char ch;
    int lineNumber = 1;

    std::cout << lineNumber << " ";  // Нумерация строк

    // Ввод кода
    linwin::disable_buf();
    while ((ch = linwin::getch()) != 27) {  
        if (ch == '\r') {  // Enter
            code += "\n";
            lineNumber++;
            std::cout << "\n" << lineNumber << " "; 
        }
        else {
            code += ch;
            std::cout << ch;
        }
    }
    linwin::enable_buf();

    linwin::clear();
    slowPrint("code:");
    std::cout << code << "\n\n";
    slowPrint("output:");
    interpretManualC(code);  

    std::this_thread::sleep_for(std::chrono::seconds(10)); 
}

void showFunctional() {
    linwin::clear();
    slowPrint("Available commands:");
    slowPrint("mkline(\"Text\") - Prints the text to the screen.");
    slowPrint("Assign variables with var = value (e.g., p = \"text\", f = 123).");
    slowPrint("Press ESC to return to menu.");
    while (linwin::getch() != 27);  // Ожидание ESC для выхода
}

void showSyntax() {
    linwin::clear();
    slowPrint("== manual-c syntax ==");

    // Цветной вывод синтаксиса
    // HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    // SetConsoleTextAttribute(hConsole, 10);  // Зеленый цвет
    linwin::set_color(ANSI(ANSI_FG, ANSI_GREEN));
    std::cout << "#use-flag <mk>\n#use-flag <string>\n\n";
    linwin::set_color(ANSI(ANSI_FG, ANSI_GREEN));
    // SetConsoleTextAttribute(hConsole, 15);  // Белый цвет
    slowPrint("int-main() {");
    linwin::set_color(ANSI(ANSI_FG, ANSI_YELLOW));
    // SetConsoleTextAttribute(hConsole, 14);  // Желтый цвет
    slowPrint("    f = 101;");
    linwin::set_color(ANSI(ANSI_FG, ANSI_WHITE));
    //SetConsoleTextAttribute(hConsole, 15);  // Белый цвет
    slowPrint("    mkline(f);");
    linwin::set_color(ANSI(ANSI_FG, ANSI_YELLOW));
    //SetConsoleTextAttribute(hConsole, 14);  // Желтый цвет
    slowPrint("    p = \"manual-c\";");
    linwin::set_color(ANSI(ANSI_FG, ANSI_WHITE));
    linwin::reset_color();
    //SetConsoleTextAttribute(hConsole, 15);  // Белый цвет
    slowPrint("    mkline(p);");
    slowPrint("}");
    slowPrint("Press ESC to return to menu.");
    while (linwin::getch() != 27);  // Ожидание ESC для выхода
}

void showAuthors() {
    linwin::clear();
    slowPrint("== author ==\nmkfs.ext4\ncve0000 (ported to linux)");
    slowPrint("Press ESC to return to menu.");
    while (linwin::getch() != 27);  // Ожидание ESC для выхода
}

void quitApplication() {
    linwin::clear();
    slowPrint("Goodbye!");
    std::this_thread::sleep_for(std::chrono::seconds(1));
    exit(0);
}

int main() {
    char choice;

    do {
        showMainMenu();
        choice = linwin::getch();

        switch (choice) {
        case '1':
            startEditor();
            break;
        case '2':
            showFunctional();
            break;
        case '3':
            showSyntax();
            break;
        case '4':
            showAuthors();
            break;
        case '5':
            quitApplication();
            break;
        default:
            slowPrint("Invalid option. Try again.");
        }

    } while (choice != '5');

    return 0;
}

