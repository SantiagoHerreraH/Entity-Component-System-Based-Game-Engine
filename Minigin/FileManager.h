#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <optional>
#include <algorithm>
#include <vector>
#include <filesystem> 
#include "Singleton.h"


namespace dae {

    class FileManager : public Singleton<FileManager>
    {
    public:

        void Init(const std::filesystem::path& data);

        void EnsureFileExists(const std::string& filename);

        std::unordered_map<std::string, int> ReadNameValueFile(const std::string& filename);
        
        void AddEntryToFile(const std::string& filename, const std::string& name, int value);

        int GetHighestValueInFile(const std::string& filename);

        std::string GetHighestValueNameInFile(const std::string& filename);

        std::pair<std::string, int> GetHighestValuePairInFile(const std::string& filename);

    private:

        std::filesystem::path m_dataPath;
    };
    
    
}

