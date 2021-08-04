#pragma once

#include <string_view>

#include <Windows.h>

#undef ERROR

namespace Near::dialog{

  enum class DialogIcon{
    NONE,
    INFORMATION,
    WARNING,
    ERROR,
  };
  
  void alert(std::string_view title, std::string_view message, DialogIcon icon, std::string_view button);
  
  bool confirm(std::string_view title, std::string_view message, DialogIcon icon, std::string_view buttonConfirm, bool focusConfirm = true);
  
}
