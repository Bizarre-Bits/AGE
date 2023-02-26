//
// Created by night on 26/02/2023.
//
#include "agepch.h"

#include "Age/Utils/FileDialogs.h"
#include "Age/Core/Log.h"

#include "Windows.h"

namespace AGE {
  namespace LocalUtils {
    age_string_t ConvertToWindowsFileFilter(const age_string_t& filter) {
      size_t separatorPos = filter.find('|');
      return filter.substr(0, separatorPos) + '\0' + filter.substr(separatorPos + 1);
    }
  }

  age_string_t FileDialogs::SaveFile(const char* filter) {
    age_string_t nativeFilter = LocalUtils::ConvertToWindowsFileFilter(filter);

    OPENFILENAMEA ofn = {sizeof(OPENFILENAMEA)};

    char szFile[_MAX_PATH] = "file";

    ofn.hwndOwner = GetConsoleWindow();
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);
    ofn.lpstrFilter = nativeFilter.c_str();
    ofn.Flags = OFN_OVERWRITEPROMPT;

    if (GetSaveFileName(&ofn)) {
      return ofn.lpstrFile;
    }
    return age_string_t{};
  }

  age_string_t FileDialogs::OpenFile(const char* filter) {
    age_string_t nativeFilter = LocalUtils::ConvertToWindowsFileFilter(filter);
    AGE_CORE_INFO("File filter: {}, Windows file filter {}", filter, nativeFilter);

    OPENFILENAMEA ofn = {sizeof(OPENFILENAMEA)};

    char szFile[_MAX_PATH] = "";

    ofn.hwndOwner = GetConsoleWindow();
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);
    ofn.lpstrFilter = nativeFilter.c_str();
    ofn.Flags = OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn)) {
      return age_string_t {ofn.lpstrFile};
    }
    return age_string_t{};
  }
}// namespace AGE