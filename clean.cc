#include <iostream>
#include <filesystem>
#include <string>
#include <stack>

using namespace std;
namespace fs = std::filesystem;

static bool is_need_clean(std::string file_name) {
    if (file_name.size() < 3) {
        return false;
    }

    if (file_name == ".gitignore") {
        return false;
    }

    if (file_name.substr(file_name.size() - 3) == ".cc") {
        return false;
    }
    if (file_name.substr(file_name.size() - 2) == ".c") {
        return false;
    }
    if (file_name.substr(file_name.size() - 2) == ".h") {
        return false;
    }

    return true;
}

static void traverse_directory(const fs::path& root_path) {
    stack<fs::path> stack;
    stack.push(root_path);
    while (!stack.empty()) {
        fs::path curr = stack.top();
        stack.pop();
        for (const auto& entry : fs::directory_iterator(curr)) {
            if (entry.is_directory() && !entry.path().filename().string().empty() 
                    && entry.path().filename().string()[0] != '.') {
                stack.push(entry.path());
            } else if (entry.is_regular_file()) {
                fs::file_status file_status = fs::status(entry.path());
                if ((file_status.permissions() & fs::perms::owner_exec) != fs::perms::none) {
                    std::cout << "delete file: " << entry.path().filename() << std::endl;
                    fs::remove(entry.path());
                    cout << entry.path().string() << endl;
                }
            }
        }
    }
}

int main(int argc, char* argv[]) {
    fs::path root_path = "./";
    traverse_directory(root_path);
    return 0;
}
