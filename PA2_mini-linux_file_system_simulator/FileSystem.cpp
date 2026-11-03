#include "FileSystem.h"
#include "FileEntry.h"
#include "Directory.h"
#include "Diagnose.h"
#include <iostream>

FileSystem::~FileSystem(){
    delete root;
}

FileEntry* FileSystem::resolve_path(const User& user, const path_t& path,
                                    const std::string& cmd) const {
    if (path.empty()) {
        return cwd;
    }

    FileEntry* curr = nullptr;
    size_t start = 0;

    if (path.front().empty()) {   // absolute path
        curr = root;
        start = 1;
    } else {
        curr = cwd;
    }

    for (size_t i = start; i < path.size(); i++) {
        if (curr->get_type() != 'd') {
            diagnose::report(cmd, curr->get_name(), diagnose::ErrorType::NotADirectory);
            return nullptr;
        }
        Directory* dir = static_cast<Directory*>(curr);
        curr = dir->search(user, path[i], cmd);
        if (curr == nullptr) {
            return nullptr;
        }
    }
    return curr;
}
Directory* FileSystem::resolve_dir(const User& user, const path_t& path, const std::string& cmd) const {
    FileEntry* entry = resolve_path(user, path, cmd);
    if (entry == nullptr) {
        return nullptr;                 // error already reported
    }
    Directory* dir = dynamic_cast<Directory*>(entry);
    if (dir == nullptr) {
        diagnose::report(cmd, entry->get_name(), diagnose::ErrorType::NotADirectory);
        return nullptr;
    }
    return dir;
}

void FileSystem::add_user_dir(const User& root, const User& user, const std::string& cmd){
    Directory* home_dir = resolve_dir(root, path_t{"", "home"}, cmd);
    if (home_dir == nullptr)
        return;

    Directory* user_dir = new Directory{user.get_name(), user, user.get_personal_group(), home_dir};
    home_dir->insert_entry(root, user_dir, cmd);
}

void FileSystem::cat(const User& user, const std::string& target) const{
    path_t path = path::split_path_string(target);
    FileEntry* entry = resolve_path(user, path, "cat");

    if(entry == nullptr)
        return;
    entry->cat(user);
}

void FileSystem::ls(const User& user, const path_t& targets, bool all, bool long_format, bool recursive) const{
path_t targets_path = targets;
    if (targets_path.empty()) targets_path.push_back(".");

    for (const std::string& target_str : targets_path) {   // <-- was `targets`
        path_t path = path::split_path_string(target_str);
        FileEntry* entry = resolve_path(user, path, "ls");
        if (entry == nullptr) continue;                    // <-- avoid null deref (error already reported)

        FormatData format_data{0, 0, 0};
        if (long_format) {
            if (entry->get_type() == 'd') {
                format_data = static_cast<Directory*>(entry)->compute_ls_widths(all, recursive);
            } else {
                format_data = FormatData{
                    entry->get_owner()->get_name().size(),
                    entry->get_group()->get_name().size(),
                    std::to_string(entry->get_size()).size()};
            }
        }

    if (targets.size() > 1) {
    std::string header = entry->get_name().empty() ? "/" : entry->get_name();
    std::cout << header << ":" << std::endl;
    }        
    if (long_format) entry->ls(user, all, recursive, format_data);
        else             entry->ls(user, all, recursive, std::nullopt);
        if (targets.size() > 1) std::cout << std::endl;
    }
}

void FileSystem::mkdir(const User& user, const path_t& targets, bool create_parents) const{
    for (const std::string& target_str : targets) {
        std::vector<std::string> comps = path::split_path_string(target_str);
        if (comps.empty()) continue;

        const std::string last = comps.back();

        // 1. Find (or, with -p, create) the parent directory
        Directory* parent = nullptr;
        if (!create_parents) {
            std::vector<std::string> parent_comps(comps.begin(), comps.end() - 1);
            parent = resolve_dir(user, parent_comps, "mkdir");
            if (!parent) return;
        } else {
            Directory* curr = comps.front().empty() ? root : cwd;
            size_t start = comps.front().empty() ? 1 : 0;
            for (size_t i = start; i + 1 < comps.size(); ++i) {
                if (comps[i].empty()) continue;
                FileEntry* next = curr->search(user, comps[i], "mkdir", CreateDirIfNotExists);
                if (!next) return;
                if (next->get_type() != 'd') {
                    diagnose::report("mkdir", next->get_name(), diagnose::NotADirectory);
                    return;
                }
                curr = static_cast<Directory*>(next);
            }
            parent = curr;
        }

        // 2. "/" (empty name), "." and ".." always exist
        if (last.empty() || last == "." || last == "..") {
            diagnose::report("mkdir", last, diagnose::FileOrDirectoryExists); // "" prints as '/'
            return;
        }

        // 3. Create the last component; if the entry count didn't change, it already existed
        unsigned int before = parent->get_entry_count();
        FileEntry* entry = parent->search(user, last, "mkdir", CreateDirIfNotExists);
        if (!entry) return;   // permission error already reported
        if (parent->get_entry_count() == before) {
            diagnose::report("mkdir", last, diagnose::FileOrDirectoryExists);
            return;
        }
    }
}

void FileSystem::touch(const User& user, const path_t& targets) const{
    for (const std::string& target_path : targets) {
        std::vector<std::string> comps = path::split_path_string(target_path);
        if (comps.empty()) continue;

        // 提取父目录路径
        std::vector<std::string> parent_comps(comps.begin(), comps.end() - 1);
        
        // 查找父目录。如果父目录不存在或路径上有文件，内部会报错，返回 nullptr
        Directory* parent_dir = resolve_dir(user, parent_comps, "touch");
        if (!parent_dir) {
            return; // 遇到错误，立即停止执行并跳过 targets 中的后续目标
        }

        const std::string& new_file_name = comps.back();
        
        // 如果目标是 "." 或 ".."，它们必定存在，直接根据要求 "do nothing"
        if (new_file_name == "." || new_file_name == "..") {
            continue; 
        }

        // 核心逻辑：利用 Directory.cpp 中写好的 search 功能
        // - 如果文件/目录已存在，它返回现有指针（什么都不做）。
        // - 如果不存在，它负责 new File(...) 并校验 Write 权限进行 insert_entry。
        FileEntry* entry = parent_dir->search(user, new_file_name, "touch", CreateFileIfNotExists);
        
        if (!entry) {
            // 如果 search 返回 nullptr，说明发生了错误（例如当前用户对父目录没有 Write 或 Exe 权限）
            // 错误信息在 has_permissions 中已经打印过了，这里只需中断退出
            return; // Fail-fast: 中断退出
        }
    }
}


void FileSystem::chmod(const User& user, const std::string& target, const std::string& new_perm, bool recursive) const{
    // 1. 将目标路径字符串切分为 vector 格式[cite: 16]
    path_t target_path = path::split_path_string(target);

    // 2. 利用你封装好的 resolve_path 找到对应的 FileEntry
    // 如果路径不存在或者路径上的目录没有 Exe 权限，resolve_path 内部会报错并返回 nullptr
    FileEntry* entry = resolve_path(user, target_path, "chmod");
    
    if (!entry) {
        return; // Fail-fast: 遇到错误立刻停止并退出
    }

    // 3. 多态调用目标对象 (File 或 Directory) 的 chmod 方法[cite: 36, 37]
    // 文件所有权 (is_owned_by) 检查和后续的报错，均已在 File::chmod 或 Directory::chmod 内部完成[cite: 31, 36, 37]
    entry->chmod(user, new_perm, recursive);
}

void FileSystem::cp(const User& user, const std::string& src_path, const std::string& dest_path, bool recursive, bool preserve_perms) const{
    // 1. 解析并获取源文件 (Source Entry)
    path_t src_comps = path::split_path_string(src_path);
    FileEntry* src_entry = resolve_path(user, src_comps, "cp");
    if (!src_entry) {
        return; // 若路径无效或无执行权限，resolve_path 已报错，直接中断[cite: 19]
    }

    // 2. 检查源文件是否为目录，并且是否漏加了递归参数
    if (src_entry->get_type() == 'd' && !recursive) {
        // 如果是目录但没有 -r 或 -R，报 NoRecursionSpecified 错误并终止
        diagnose::report("cp", src_entry->get_name(), diagnose::NoRecursionSpecified);
        return; 
    }

path_t dest_comps = path::split_path_string(dest_path);
    FileEntry* dest_entry = nullptr;

    if (dest_comps.empty() || dest_comps.back().empty() ||
        dest_comps.back() == "." || dest_comps.back() == "..") {
        // "/", ".", ".." (or a trailing-slash path) must already exist
        dest_entry = resolve_path(user, dest_comps, "cp");
        if (!dest_entry) return;
    } else {
        std::vector<std::string> parent_comps(dest_comps.begin(), dest_comps.end() - 1);
        Directory* dest_parent = resolve_dir(user, parent_comps, "cp");
        if (!dest_parent) return;

        // search() returns the existing entry if present, otherwise creates one
        // of the matching type, with no "No such file" message in either case.
        SearchOption opt = (src_entry->get_type() == 'd') ? CreateDirIfNotExists
                                                          : CreateFileIfNotExists;
        dest_entry = dest_parent->search(user, dest_comps.back(), "cp", opt);
        if (!dest_entry) return;
    }

    // 4. Delegate
    src_entry->cp(user, *dest_entry, preserve_perms);
}

void FileSystem::rm(const User& user, const path_t& targets, bool recursive) const{
    for (const std::string& target_path : targets) {
        std::vector<std::string> comps = path::split_path_string(target_path);
        if (comps.empty()) continue;

        // 1. 解析路徑找到對應的 FileEntry
        // 如果路徑不存在或無權限存取，resolve_path 內部會報錯並回傳 nullptr
        FileEntry* entry = resolve_path(user, comps, "rm");
        if (!entry) {
            return; // Fail-fast: 遇錯立刻中斷並跳過剩餘目標
        }

        // 2. 針對目錄型態的特例攔截
        if (entry->get_type() == 'd') {
            // 條件 1: 若為目錄但 recursive 為 false，報錯並中斷[cite: 38]
            if (!recursive) {
                diagnose::report("rm", entry->get_name(), diagnose::ErrorType::NoRecursionSpecified);
                return;
            }

            Directory* target_dir = static_cast<Directory*>(entry);
            
            // 條件 2: 檢查是否會連帶刪除 cwd[cite: 38]
            // 利用 is_ancestor_of 判斷：若 target_dir 等於 cwd 或為 cwd 的上層祖先，則拒絕操作
            if (target_dir->is_ancestor_of(*cwd)) {
                diagnose::report("rm", entry->get_name(), diagnose::ErrorType::PermissionDenied);
                return;
            }
        }

        // 3. 執行實際的刪除操作 (多態分發至 File::rm 或 Directory::rm)[cite: 36, 37]
        // 文件與目錄的 Write 權限檢查，以及錯誤訊息輸出，已在底層實作中處理完畢[cite: 49]
        if (!entry->rm(user)) {
            return; // Fail-fast: 若刪除失敗 (如無 Write 權限)，立刻停止後續目標的刪除[cite: 19]
        }
    }
}

void FileSystem::cd(const User& user, const std::string& target) {
    // 1. 将输入的字符串路径切分为 vector[cite: 16]
    path_t comps = path::split_path_string(target);

    // 2. 利用 resolve_dir 寻找目标目录
    // 路径解析、权限校验 (Exe 权限)、以及“如果不是目录则报错 NotADirectory”的逻辑
    // 已经全部在 resolve_dir 及其底层的 search 函数中完美处理
    Directory* new_dir = resolve_dir(user, comps, "cd");

    // 3. 如果成功找到且中途没有任何错误，更新当前工作目录
    if (new_dir != nullptr) {
        cwd = new_dir;
    }
}