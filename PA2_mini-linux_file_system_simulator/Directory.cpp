#include "Directory.h"
#include "Diagnose.h"
#include "File.h"
#include <iostream>

Directory::Directory(const std::string& name, const User& owner, const Group& group, Directory* parent):
    FileEntry(name, owner, group, parent){}

Directory::~Directory(){
    for(FileEntry* file_entry : file_entries)
        delete file_entry;
}

bool Directory::insert_entry(const User& user, FileEntry* entry, const std::string& cmd){
    if(FileEntry::has_permissions(user, Write, cmd)){
        for (FileEntry* file_entry : file_entries){
            if (file_entry->get_name() == entry->get_name()){
                diagnose::report(cmd, entry->get_name(), diagnose::FileOrDirectoryExists);
                delete entry;
                return false;
            }
        }
        // fulfil condition, inserts in ascending lexicographical order
        for(std::vector<FileEntry*>::iterator curr = file_entries.begin(); curr != file_entries.end(); curr++){
            if (entry->get_name() < (*curr)->get_name()){
                file_entries.insert(curr, entry);
                return true;
            }
        }
        file_entries.emplace_back(entry);
        return true;
    }
    delete entry;
    return false;
}

bool Directory::remove_entry(const User& user, const FileEntry& entry, const std::string& cmd){
    if(FileEntry::has_permissions(user, Write, cmd)){
        for(std::vector<FileEntry*>::iterator curr = file_entries.begin(); curr != file_entries.end(); curr++){
            if((*curr) == &entry){
                file_entries.erase(curr);
                return true; 
            }
        }
        //return false; guarantee the given entry is always in the directory
    }
    return false;
}

FileEntry* Directory::search(const User& user, const std::string& target, const std::string& cmd, SearchOption option){
    if(!FileEntry::has_permissions(user, Exe, cmd)){
        return nullptr;
    }    

    if(target == ".")
            return this;
    if(target == ".."){
        if (get_parent() == nullptr)
            return this;
        else
            return get_parent();
        }
    // search

    for(FileEntry* file_entry : file_entries){
        if(file_entry->get_name() == target)
            return file_entry;
    }
    
    if(option == Error){
        diagnose::report(cmd, target, diagnose::NoFileOrDirectory);
        return nullptr;
    }

    if(option == CreateFileIfNotExists){
        File* new_file = new File(target, user, user.get_personal_group(), "", this); 
        if(!insert_entry(user, new_file, cmd))
            return nullptr;
        return new_file;
    }

    if(option == CreateDirIfNotExists){
        Directory* new_directory = new Directory(target, user, user.get_personal_group(), this); 
        if(!insert_entry(user, new_directory, cmd))
            return nullptr;
        return new_directory;
    }

    return nullptr; //useless, prevent warning
}

void Directory::cat([[maybe_unused]]const User& user) const{
    diagnose::report("cat", get_name(), diagnose::NoAtFile);
} //warning: unused parameters

bool Directory::ls(const User& user, bool all, bool recursive, const std::optional<FormatData>& long_format, const std::string& display_name) const{
    if (!has_permissions(user, Permission::Read, "ls")) return false;
    if (!has_permissions(user, Permission::Exe, "ls")) return false;

    if (all) {
        FileEntry::ls(user, all, recursive, long_format, ".");
        if (get_parent() != nullptr) {
            get_parent()->FileEntry::ls(user, all, recursive, long_format, "..");
        } else {
            FileEntry::ls(user, all, recursive, long_format, "..");
        }
    }

    for (FileEntry* file_entry : file_entries) {
        file_entry->FileEntry::ls(user, all, recursive, long_format, file_entry->get_name());

        if (recursive && file_entry->get_type() == 'd') {
            file_entry->ls(user, all, recursive, long_format, "");
        }
    }
    return true;
}

unsigned int Directory::get_size() const{
    return file_entries.size() * 32;
}

char Directory::get_type() const{
    return 'd';
}

bool Directory::chmod(const User& user, const std::string& new_perm, bool recursive) {
    if (!is_owned_by(user, "chmod")) {
        return false; 
    }

    if (!FileEntry::chmod(user, new_perm, recursive)) {
        return false;
    }

    if (recursive) {
        for (FileEntry* entry : file_entries) {
            entry->chmod(user, new_perm, recursive);
        }
    }

    return true; 
}

bool Directory::cp(const User& user, FileEntry& dest, bool preserve_perms) {
    if (dest.get_type() != 'd') {
        diagnose::report("cp", dest.get_name(), diagnose::ErrorType::NotADirectory);
        return false;
    }
    
    Directory* dest_dir = dynamic_cast<Directory*>(&dest);

    if (this->is_ancestor_of(*dest_dir)) {
        diagnose::report("cp", this->get_name(), diagnose::ErrorType::PermissionDenied);
        return false;
    }

    for (FileEntry* entry : file_entries) {
        if (entry->get_type() == 'd') {
            FileEntry* next_dest = dest_dir->search(user, entry->get_name(), "cp", CreateDirIfNotExists); 
            entry->cp(user, *next_dest, preserve_perms);
        } else {
            entry->cp(user, *dest_dir, preserve_perms);
        }
    }

    return true; // 命令成功执行完毕[cite: 37]
}
bool Directory::rm(const User& user) {
    // 1. 如果此目錄是 root 目錄，報錯 Permission denied
    // 判斷 root 的標準：沒有父目錄 (get_parent() == nullptr)
    if (get_parent() == nullptr) {
        // 根據文件第19頁說明：如果是 root，其名稱為空，為了避免混淆需映射為 "/"
        std::string err_name = get_name().empty() ? "/" : get_name();
        diagnose::report("rm", err_name, diagnose::ErrorType::PermissionDenied);
        return false;
    }

    // 2. 透過遞迴呼叫 rm 來移除所有子條目，並釋放未使用的記憶體
    // 由於成功的 child->rm(user) 會觸發 remove_entry 從 file_entries 抹除指標，
    // 我們可以安全地用 while 迴圈不斷獲取最後一個元素進行移除。
    while (!file_entries.empty()) {
        FileEntry* child = file_entries.back();
        
        // 遞迴呼叫子條目的 rm。若遇到權限不足等錯誤，立即中斷並回傳 false (Fail-fast)[cite: 19]
        if (!child->rm(user)) {
            return false; 
        }
        
        // 【記憶體釋放說明】：
        // 作業要求 "release unused memory"[cite: 37]。
        // 因為我們在上一環節的 File::rm 以及接下來的第 3 步中，
        // 都使用了 `delete this;` 來自我銷毀，所以這裡 child 的記憶體已經被徹底釋放，無需再次 delete。
    }

    // 3. 將此目錄從其父目錄中移除[cite: 37]
    // 呼叫父目錄的 remove_entry，這會自動檢查父目錄的 Write 權限[cite: 49]
    if (get_parent()->remove_entry(user, *this, "rm")) {
        
        // 解除綁定成功後，釋放自身的記憶體，完美達成防範 Memory Leak 的要求[cite: 37]
        delete this; 
        
        return true; // 指令成功執行
    }

    return false;
}