//
// Created by ASUS on 6/3/2025.
//

#ifndef GET_ECXEPTION_H
#define GET_ECXEPTION_H

#include <exception>
#include<string>

class Get_Exception {
public:
    class Element_NameException : public std::exception {
        std::string element;
        std::string message;
    public:
        explicit Element_NameException(const std::string& element)
            : element(element), message("Error: Element " + element + " not found in library") {}

        const char* what() const noexcept override {
            return message.c_str();
        }
    };
    class Invalid_Syntax_Exception : public std::exception {
    public:
        const char* what() const noexcept override {
            return "Error: Syntax error";
        }
    };
    class Invalid_Value_Exception : public std::exception {
    public:
        const char* what() const noexcept override {
            return "Error: Resistance cannot be zero or negative";
        }
    };
    class Duplicate_Name_Exception : public std::exception {
        std::string element;
        std::string message;
    public:
        explicit Duplicate_Name_Exception(const std::string& element)
            : element(element), message("Error: Element " + element + " already exists in the circuit") {}

        const char* what() const noexcept override {
            return message.c_str();
        }
    };
    class Invalid_DiodModel_Exception : public std::exception {
        std::string element;
        std::string message;
    public:
        explicit Invalid_DiodModel_Exception(const std::string& element)
            : element(element), message("Error: Model " + element + "  not found in library") {}

        const char* what() const noexcept override {
            return message.c_str();
        }
    };
    class Delete_Exception : public std::exception {
    public:
        char type;
        explicit Delete_Exception(const char& type) {
            this->type = type;
        }
        const char* what() const noexcept override {
            switch (type) {
                case 'R':
                    return "Error: Cannot delete resistor; component not found";
                case 'L':
                    return "Error: Cannot delete inductor; component not found";
                case 'C':
                    return "Error: Cannot delete capacitor; component not found";
                case 'D':
                    return "Error: Cannot delete diode; component not found";
            }

        }
    };
};



#endif //GET_ECXEPTION_H
