#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

#include "json.hpp"
#include <curl/curl.h>

using Jason = nlohmann::json;

const char *GEMINI_KEY_CSTR = std::getenv("GEMINI_API_KEY");
const std::string GEMINI_API_KEY = GEMINI_KEY_CSTR ? GEMINI_KEY_CSTR : "";

static size_t WriteCallbackForCurl(void *Contents, size_t SizeOfMember, size_t NumberofMembers, std::string& Output) {
    size_t TotalSize = SizeOfMember * NumberofMembers;
    Output.append((char *)Contents, TotalSize);
    return TotalSize;
}

std::string Translate(const std::string& Input) {
    CURL* curl = curl_easy_init();
    std::string translated_response;
    
    if (curl) {
        std::string prompt = Input;
        std::string translation_instructions = "Translate the name of each file/folder into English. Respond only with the translated name.";


        Jason payload = {
            {"system_instruction", {
                {"parts", {{
                    {"text", translation_instructions}
                }}}
            }},
            {"contents", {{
                {"parts", {{
                    {"text", prompt}
                }}}
            }}},
            {"generationConfig", {
                {"thinkingConfig", {
                    {"thinkingBudget", 0}
                }}
            }}
        };
        std::string payload_string = payload.dump();
        
        struct curl_slist *curl_headers = nullptr;
        curl_headers = curl_slist_append(curl_headers, "Content-Type: application/json");
        curl_headers = curl_slist_append(curl_headers, ("X-goog-api-key: " + GEMINI_API_KEY).c_str());
 
        const std::string GEMINI_URL = "https://generativelanguage.googleapis.com/v1beta/models/gemini-2.0-flash:generateContent?key=";
        
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, curl_headers);
        curl_easy_setopt(curl, CURLOPT_URL, GEMINI_URL.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload_string.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallbackForCurl);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &translated_response);
        curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_1);
        
        CURLcode reponse_code = curl_easy_perform(curl);
        if (reponse_code != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(reponse_code) << "\n";
        }
        
        curl_slist_free_all(curl_headers);
        curl_easy_cleanup(curl);
    }
    
    std::cerr << "Raw Response:\n" << translated_response << '\n';
    
    try {
        auto json_response = Jason::parse(translated_response);
        std::string TranslatedInput = json_response["candidates"][0]["content"]["parts"][0]["text"];
        return TranslatedInput;
    }
    catch (...) {
        std::cerr << "Translator function error parsing json response";
        return Input;
    }
}

bool RenameFolder(const std::filesystem::path& PathToFile, const std::string& NewName) {
    std::filesystem::path new_path_to_file = PathToFile.parent_path() / NewName;
    try {
        std::filesystem::rename(PathToFile, new_path_to_file);
        std::cout << "Renamed: " << PathToFile << " >> " << new_path_to_file << '\n';
        return true;
    }
    catch (const std::exception& exception) {
        std::cerr << "Rename failed: " << exception.what() << '\n';
        return false;
    }
}

void RunPiecemeal(std::filesystem::path TargetDirectory, bool IsTestRun) {
    for (const auto& file : std::filesystem::directory_iterator(TargetDirectory)) {
        const auto& path = file.path();
        std::string file_name = path.stem().string();
        std::string file_extension = path.extension().string();
        
        std::string translated_name = Translate(file_name);
        translated_name.erase(std::remove(translated_name.begin(), translated_name.end(), '\n'), translated_name.end());
        
        std::string new_name = translated_name + file_extension;
        
        if (IsTestRun) {
            std::cout << "[Test Run] Would rename \"" << file_name << file_extension << "\" to \n" << translated_name << file_extension << '\n';
        } else {
            RenameFolder(file.path(), new_name);
        }
    }
}

bool CreateTestFiles(const std::filesystem::path& TestFilesDirectory, int NumberOfTestFiles, bool UsedMixedLanguage) {
    const std::vector<std::string> randomFileNames = UsedMixedLanguage ?
        std::vector<std::string>{
        "El_sol_brilla", "L'eau est_froide", "Die Katze schläft",
        "Le chat est 5", "123-おはよう、元気???", "La vie en rose",
        "C'est_la_vie", "Ein Bier, bitte!", "Dos cervezas por_favor",
        "Comment_allez-vous?", "Que_hora_es?", "Wie geht's?",
        "Alles_gute!", "Buona giornata!", "Ciao a tutti!",
        "Grazie 1000", "Arrivederci, Roma", "Où est la gare?",
        "S'il vous plaît", "あれ_は_誰？"
    } :
        std::vector<std::string>{
        "こんにちは_世界", "おはよう、元気？", "お疲れ様_です！",
        "日本語が_好き", "ご飯_食べ_た", "ありがとう、ね",
        "はい、そうです", "いいえ、違います", "ごめんなさい",
        "すみません_100", "これ_は_何？", "あれ_は_誰？",
        "また_会いましょう", "頑張って！", "おやすみ_なさい",
        "楽しかった_ね", "トイレは_どこ？", "いくら_ですか",
        "道に迷った_5", "もう_一度"
    };

    if (static_cast<size_t>(NumberOfTestFiles > randomFileNames.size())) {
        std::cout << "i only got about " << randomFileNames.size() << "random names man";
        NumberOfTestFiles = randomFileNames.size();
    }
    
    std::cout << "[Creating test files]\n";
    for (int i = 0; i < NumberOfTestFiles; i++) {
        std::filesystem::path test_file_path = TestFilesDirectory / randomFileNames[i];
        std::ofstream ofs_temp(test_file_path);
        if (!ofs_temp) {
            std::cerr << "  Error creating test file: " << test_file_path << '\n';
            return false;
        }
        std::cout << "  Created file \"" << std::filesystem::absolute(test_file_path) << "\"\n";
    }
    return true;
}

int main(int argc, const char * argv[]) {
    bool test_run = false;
    std::filesystem::path target_directory = ".";
    std::cout << "Target directory: " << std::filesystem::absolute(target_directory).lexically_normal() << '\n';

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        
        if (arg == "-d" && i + 1 < argc) {
            target_directory = argv[++i];
        }
        else if (arg == "--test") {
            test_run = true;
        }
        else {
            std::cerr << "Unknown argument " << arg << '\n';
        }
    }

    CreateTestFiles(target_directory, 20, true);
    RunPiecemeal(target_directory, test_run);
    return 0;
}
