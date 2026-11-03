#include "Device.h"
#include "Diagnose.h"
#include <iostream>

char Device::get_type() const {
    return 'c';
}

bool DevNull::set_content(const User& user, const std::string& content, const std::string& cmd) {
    if (has_permissions(user, Write, cmd)) {
        return true;
    }
    return false;
}

std::optional<std::string> DevNull::get_content(const User& user, const std::string& cmd) const {
    if (has_permissions(user, Read, cmd)) {
        return std::string("");
    }
    return std::nullopt;
}

bool DevStdIn::set_content(const User& user, const std::string& content, const std::string& cmd) {
    diagnose::report(cmd, get_name(), diagnose::ErrorType::PermissionDenied);
    return false;
}

std::optional<std::string> DevStdIn::get_content(const User& user, const std::string& cmd) const {
    if (has_permissions(user, Permission::Read, cmd)) {
        std::string buffer;
        std::getline(std::cin, buffer);
        
        return buffer;
    }
    
    return std::nullopt;
}

bool DevStdOut::set_content(const User& user, const std::string& content, const std::string& cmd) {
    if (has_permissions(user, Permission::Write, cmd)) {
        std::cout << content << std::endl;      
        return true;
    }
    return false;
}

std::optional<std::string> DevStdOut::get_content(const User& user, const std::string& cmd) const {
    diagnose::report(cmd, get_name(), diagnose::ErrorType::PermissionDenied);   
    return std::nullopt;
}