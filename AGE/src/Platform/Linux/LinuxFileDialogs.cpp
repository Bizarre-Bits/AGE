//
// Created by alex on 2/22/23.
//

#include "agepch.h"

#include "Age/Utils/FileDialogs.h"

namespace AGE {
  age_string_t FileDialogs::SaveFile(const char* filter) {
    char filename[1024];
    memset(filename, 0, 1024);
    std::stringstream command;
    command << "zenity --file-selection --save --confirm-overwrite";
    if (strlen(filter)) {
      command << " --file-filter=" << '\"' << filter << '\"';
    }
    FILE* f = popen(command.str().c_str(), "r");
    fgets(filename, 1024, f);

    age_string_t filenameStr{filename};
    filenameStr.erase(std::remove(filenameStr.begin(), filenameStr.end(), '\n'), filenameStr.cend());
    return filenameStr;
  }

  age_string_t FileDialogs::OpenFile(const char* filter) {
    char filename[1024];
    memset(filename, 0, 1024);
    std::stringstream command;
    command << "zenity --file-selection";
    if (strlen(filter)) {
      command << " --file-filter=" << '\"' << filter << '\"';
    }
    FILE* f = popen(command.str().c_str(), "r");
    fgets(filename, 1024, f);

    age_string_t filenameStr{filename};
    filenameStr.erase(std::remove(filenameStr.begin(), filenameStr.end(), '\n'), filenameStr.cend());
    return filenameStr;
  }
}// namespace AGE