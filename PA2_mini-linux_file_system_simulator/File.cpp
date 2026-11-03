#include "File.h"
#include "Directory.h"   // 【關鍵】必須引入，否則 static_cast<Directory*> 會報錯
#include "Diagnose.h"
#include <iostream>


File::File(const std::string& name, const User& owner, const Group& group, const std::string& content, Directory* parent):
    FileEntry(name, owner, group, parent), content(content){}

std::optional<std::string> File::get_content(const User& user, const std::string& cmd) const{
    if(has_permissions(user, Read, cmd))
        return std::optional<std::string>(content);
    return std::nullopt;
}

bool File::set_content(const User& user, const std::string& content, const std::string& cmd){
    if(has_permissions(user, Write, cmd)){
        this->content = content;
        return true;
    }
    return false;
}

unsigned int File::get_size() const{
    return content.size();
}

char File::get_type() const{
    return '-';
}

void File::cat(const User& user) const{
    std::optional<std::string> c = get_content(user, "cat");
    if (c.has_value())
        std::cout << c.value() << std::endl;
}

bool File::ls(const User& user, bool all, bool recursive, const std::optional<FormatData>& long_format, const std::string& display_name) const{
    if(has_permissions(user, Read, "ls")){
        FileEntry::ls(user, all, recursive, long_format, display_name);
        return true;
    }
    return false;
}

bool File::chmod(const User& user, const std::string &new_perm, bool recursive){ // chmod [-R] PERMISSION FILE
    if (!is_owned_by(user, "chmod")) {
        return false;
    }
    return FileEntry::chmod(user, new_perm, recursive);
}

bool File::cp(const User& user, FileEntry& dest, bool preserve_perms){ // cp [-p] [-r|R] SRC DEST
    FileEntry* actual_dest = &dest;

    if (dest.get_type() == 'd') {
        Directory* dest_dir = dynamic_cast<Directory*>(&dest);
        
        actual_dest = dest_dir->search(user, this->get_name(), "cp", CreateFileIfNotExists);
        if(actual_dest == nullptr)
            return false;
        if (actual_dest->get_type() == 'd') {
            diagnose::report("cp", actual_dest->get_name(), diagnose::ErrorType::NoAtFile);
            return false; 
        }
    }

    // 2. 讀取當前檔案內容[cite: 36]
    // 依據 File.h 宣告，呼叫 get_content 需傳入 user 與 cmd
    std::optional<std::string> content_opt = this->get_content(user, "cp");
    if (!content_opt.has_value()) {
        return false; 
    }

    // 寫入目標檔案[cite: 36]
    // 由於 actual_dest 已確認不是 'd'，可安全轉型為 File* 以呼叫 set_content
    File* dest_file = dynamic_cast<File*>(actual_dest);
    if(!dest_file->set_content(user, content_opt.value(), "cp"))
        return false;

    // 3. 若 preserve_perms 為 true，複製權限[cite: 36]
    if (preserve_perms) {
        actual_dest->copy_permission(*this);
    }

    return true;
}

bool File::rm(const User& user){// rm [-r|R] FILE...
    if (!has_permissions(user, Permission::Write, "rm")) {
        return false;
    }

    if (get_parent()->remove_entry(user, *this, "rm")) {
        delete this;   // entry is now unreachable from the tree, so free it
        return true;
    }
    return false;
}