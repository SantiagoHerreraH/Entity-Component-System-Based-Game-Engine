#include "FileManager.h"
#include "MathTools.h"

namespace dae {
    void FileManager::Init(const std::filesystem::path& data)
    {
        m_dataPath = data;
    }
    void FileManager::EnsureFileExists(const std::string& filename)
    {

        const auto fullPath = m_dataPath / filename;
        const auto fileName = std::filesystem::path(fullPath).filename().string();

        if (!std::filesystem::exists(fileName)) {
            std::ofstream createFile(fileName);
            if (!createFile) {
                throw std::runtime_error("Failed to create file: " + fileName);
            }
            std::cout << "File '" << fileName << "' created (was missing)." << std::endl;
        }
    }
    std::unordered_map<std::string, int> FileManager::ReadNameValueFile(const std::string& fileName)
    {

        const auto fullPath = m_dataPath / fileName;
        const auto filename = std::filesystem::path(fullPath).filename().string();

        EnsureFileExists(filename);
        std::unordered_map<std::string, int> nameToValue;
        std::ifstream file(filename);

        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file: " + filename);
        }

        std::string name;
        int value;
        while (file >> name >> value) {
            nameToValue[name] = value;
        }

        return nameToValue;
    }
    int FileManager::GetHighestValueInFile(const std::string& filename)
    {

        std::unordered_map<std::string, int> map = ReadNameValueFile(filename);
        auto pair = MathTools::GetMaxEntry(map);

        return pair.second;
    }
    std::string FileManager::GetHighestValueNameInFile(const std::string& filename)
    {
        std::unordered_map<std::string, int> map = ReadNameValueFile(filename);
        auto pair = MathTools::GetMaxEntry(map);

        return pair.first;
    }
    std::pair<std::string, int> FileManager::GetHighestValuePairInFile(const std::string& filename)
    {
        std::unordered_map<std::string, int> map = ReadNameValueFile(filename);
        auto pair = MathTools::GetMaxEntry(map);

        return pair;
    }
    void FileManager::AddEntryToFile(const std::string& fileName, const std::string& name, int value)
    {
        const auto fullPath = m_dataPath / fileName;
        const auto filename = std::filesystem::path(fullPath).filename().string();
        std::ofstream file(filename, std::ios::app);  // open in append mode
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file for writing: " + filename);
        }
        file << name << " " << value << '\n';
    }
}