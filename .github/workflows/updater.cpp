#include <windows.h>
#include <wininet.h>
#include <iostream>
#include <string>

#pragma comment(lib, "wininet.lib")

// الإصدار الحالي للبرنامج
const std::string CURRENT_VERSION = "1.0.0";

// رابط الإصدارات على GitHub API (استبدل USERNAME و REPO باسم حسابك ومستودعك)
const std::string GITHUB_API_URL = "https://api.github.com/repos/qusai011/MyProject/releases/latest";

// دالة لجلب البيانات من الرابط
std::string FetchLatestVersion() {
    std::string response = "";
    HINTERNET hInternet = InternetOpenA("GitHubUpdaterApp", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    
    if (hInternet) {
        // حجز اتصال مع GitHub
        HINTERNET hConnect = InternetOpenUrlA(
            hInternet, 
            GITHUB_API_URL.c_str(), 
            "User-Agent: C++App\r\n", 
            -1L, 
            INTERNET_FLAG_RELOAD | INTERNET_FLAG_SECURE, 
            0
        );

        if (hConnect) {
            char buffer[1024];
            DWORD bytesRead = 0;
            
            while (InternetReadFile(hConnect, buffer, sizeof(buffer) - 1, &bytesRead) && bytesRead > 0) {
                buffer[bytesRead] = '\0';
                response += buffer;
            }
            InternetCloseHandle(hConnect);
        }
        InternetCloseHandle(hInternet);
    }
    return response;
}

// دالة استخراج رقم الإصدار من استجابة JSON البسيطة
std::string ExtractTagVersion(const std::string& jsonResponse) {
    std::string key = "\"tag_name\":\"";
    size_t startPos = jsonResponse.find(key);
    
    if (startPos != std::string::npos) {
        startPos += key.length();
        size_t endPos = jsonResponse.find("\"", startPos);
        if (endPos != std::string::npos) {
            return jsonResponse.substr(startPos, endPos - startPos);
        }
    }
    return "";
}

int main() {
    std::cout << "=== App Version: " << CURRENT_VERSION << " ===" << std::endl;
    std::cout << "Checking for updates on GitHub..." << std::endl;

    std::string jsonResponse = FetchLatestVersion();
    std::string latestVersion = ExtractTagVersion(jsonResponse);

    if (latestVersion.empty()) {
        std::cout << "[!] Could not fetch update info. Check URL or internet connection." << std::endl;
    } else {
        std::cout << "Latest Version on GitHub: " << latestVersion << std::endl;

        if (latestVersion != CURRENT_VERSION) {
            std::cout << "[+] New update available! Please download the latest release from GitHub." << std::endl;
        } else {
            std::cout << "[=] You are using the latest version." << std::endl;
        }
    }

    system("pause");
    return 0;
}
