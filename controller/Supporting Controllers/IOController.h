#ifndef IOCONTROLLER_H
#define IOCONTROLLER_H

#include <fstream>
#include <iostream>
#include <string>

class IOController {
//     std::string path;
//     std::ofstream file;
//
// public:
//     IOController(const std::string& filePath) : path(filePath) {
//         file.open(path, std::ios::app);
//         if (!file.is_open()) {
//             std::cerr << "Cannot create file at: " << path << std::endl;
//         } else {
//             std::cout << "File created at: " << path << std::endl;
//         }
//     }
//
//     // Move constructor
//     IOController(IOController&& other) noexcept
//         : path(std::move(other.path)), file(std::move(other.file)) {}
//
//     // Move assignment
//     IOController& operator=(IOController&& other) noexcept {
//         if (this != &other) {
//             if (file.is_open()) file.close();
//             path = std::move(other.path);
//             file = std::move(other.file);
//         }
//         return *this;
//     }
//
//     // Delete copy constructor and assignment
//     IOController(const IOController&) = delete;
//     IOController& operator=(const IOController&) = delete;
//
//     // Destructor
//     ~IOController() {
//         if (file.is_open()) {
//             file.close();
//         }
//     }
//
//     void Close_file() {
//         if (file.is_open()) {
//             file.close();
//             std::cout << "last File is saved." << std::endl;
//         }
//     }
//     void write(const std::string& text);
//
//     std::string get_path() const {
//         return path;
//     }
//
//     std::ofstream& get_file() {
//         return file;
//     }
};

#endif // IOCONTROLLER_H
