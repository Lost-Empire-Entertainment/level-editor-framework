//Copyright (c) <2024> <Lost Empire Entertainment>

#include <iostream>
#include <filesystem>

//level editor
#include "fileutils.hpp"

using std::cout;
using std::exception;
using std::runtime_error;
using std::filesystem::is_directory;
using std::filesystem::is_regular_file;
using std::filesystem::recursive_directory_iterator;
using std::filesystem::copy_options;

namespace Utils
{
    string FileUtils::GetOutputFromBatFile(const char* file)
    {
        char buffer[128];
        string result = "";
        string command = "\"" + string(file) + "\"";
        FILE* pipe = _popen(command.c_str(), "r");

        if (!pipe) throw runtime_error("popen() failed!");

        try
        {
            while (fgets(buffer, sizeof(buffer), pipe) != nullptr)
            {
                result += buffer;
            }
        }
        catch (...)
        {
            _pclose(pipe);
            throw;
        }

        _pclose(pipe);

        return result;
    }

    void FileUtils::MoveOrRenameFileOrFolder(const path& sourcePath, const path& destinationPath, const bool isRenaming)
    {
        string output;

        if (!exists(sourcePath))
        {
            cout << "Error: Source path " << sourcePath.string() << " does not exist!\n\n";
            return;
        }
        if (exists(destinationPath))
        {
            string outputType = isRenaming ? "Cannot rename source " : "Cannot move source ";
            output = outputType + sourcePath.string() + " to destination " + destinationPath.string() + " because destination already exists!\n\n";
            cout << output;
            return;
        }

        try
        {
            rename(sourcePath, destinationPath);

            string outputType = isRenaming ? "Renamed " : "Moved ";
            output = outputType + sourcePath.string() + " to " + destinationPath.string() + ".\n\n";
            cout << output;
        }
        catch (const exception& e)
        {
            output = string(e.what()) + ".\n\n";
            cout << output;
        }
    }

    void FileUtils::CopyFileOrFolder(const path& sourcePath, const path& destinationPath)
    {
        string output;
        string fileOrFolderName = sourcePath.filename().string();
        path fixedTargetPath = destinationPath / fileOrFolderName;

        if (!exists(sourcePath))
        {
            cout << "Error: Source path " << sourcePath.string() << " does not exist!\n\n";
            return;
        }

        try
        {
            if (is_directory(sourcePath))
            {
                create_directories(fixedTargetPath);

                for (const auto& entry : recursive_directory_iterator(sourcePath))
                {
                    path relativePath = relative(entry.path(), sourcePath);
                    path destPath = fixedTargetPath / relativePath;

                    if (is_directory(entry))
                    {
                        create_directory(destPath);
                    }
                    else if (is_regular_file(entry))
                    {
                        copy_file(entry.path(), destPath, copy_options::overwrite_existing);
                    }
                }

                output = "Copied folder " + sourcePath.string() + " to " + fixedTargetPath.string() + ".\n\n";
                cout << output;
            }
            else if (is_regular_file(sourcePath))
            {
                create_directories(destinationPath);

                path destPath = destinationPath / fileOrFolderName;
                copy_file(sourcePath, destPath, copy_options::overwrite_existing);

                output = "Copied file " + sourcePath.string() + " to " + destPath.string() + ".\n\n";
                cout << output;
            }
        }
        catch (const exception& e)
        {
            output = string(e.what()) + ".\n\n";
            cout << output;
        }
    }

    void FileUtils::DeleteFileOrfolder(const path& sourcePath)
    {
        string output;
        if (!exists(sourcePath))
        {
            cout << "Error: " << sourcePath.string() << " does not exist!\n\n";
            return;
        }

        try
        {
            if (is_regular_file(sourcePath)) remove(sourcePath);
            else if (is_directory(sourcePath)) remove_all(sourcePath);

            output = "Deleted " + sourcePath.string() + ".\n\n";
            cout << output;
        }
        catch (const exception& e)
        {
            output = string(e.what()) + ".\n\n";
            cout << output;
        }
    }

    void FileUtils::CreateNewFolder(const path& folderPath)
    {
        string output;
        if (exists(folderPath))
        {
            output = "Error " + folderPath.string() + " already exists!\n\n";
            cout << output;

            return;
        }

        if (is_regular_file(folderPath))
        {
            output = "Error " + folderPath.string() + " must be a folder!\n\n";
            cout << output;

            return;
        }

        try
        {
            create_directory(folderPath);

            output = "Created new folder at " + folderPath.string() + ".\n\n";
            cout << output;
        }
        catch (const exception& e)
        {
            output = string(e.what()) + ".\n\n";
            cout << output;
        }
    }
}