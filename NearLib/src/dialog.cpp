#include "pch.h"
#include "dialog.h"

#include <CommCtrl.h>

#include "near.h"
#include "utils.h"

namespace Near::dialog{

  PCWSTR getNativeDialogIcon(DialogIcon icon){
    switch(icon){
      case DialogIcon::INFORMATION: return TD_INFORMATION_ICON;
      case DialogIcon::WARNING: return TD_WARNING_ICON;
      case DialogIcon::ERROR: return TD_ERROR_ICON;
      default: return nullptr;
    }
  }

  HWND getGameWindow(){
    auto* render = renderer();
    return render ? render->getWindow() : NULL;
  }
  
  void alert(std::string_view title, std::string_view message, DialogIcon icon, std::string_view button){
    std::wstring wideTitle = widen(title);
    std::wstring wideMessage = widen(message);
    std::wstring wideButton = widen(button);

    int buttonClicked = 0;
    TASKDIALOG_BUTTON buttons[] = {
      {IDOK, wideButton.c_str()},
    };
    TASKDIALOGCONFIG config = {};
    config.cbSize = sizeof(config);
    config.hInstance = GetModuleHandleW(nullptr);
    config.hwndParent = getGameWindow();
    if(button.empty()){
      config.dwCommonButtons = TDCBF_OK_BUTTON;
    }else{
      config.cButtons = 1;
      config.pButtons = buttons;
    }
    config.pszWindowTitle = L" ";
    config.pszMainIcon = getNativeDialogIcon(icon);
    config.pszMainInstruction = wideTitle.c_str();
    config.pszContent = wideMessage.c_str();
    
    TaskDialogIndirect(&config, &buttonClicked, NULL, NULL);
  }
  
  bool confirm(std::string_view title, std::string_view message, DialogIcon icon, std::string_view buttonConfirm, bool focusConfirm){
    int buttonClicked = 0;
    TASKDIALOG_BUTTON buttons[] = {
      {IDOK, widen(buttonConfirm).c_str()},
    };
    TASKDIALOGCONFIG config = {};
    config.cbSize = sizeof(config);
    config.hInstance = GetModuleHandleW(nullptr);
    config.hwndParent = getGameWindow();
    if(buttonConfirm.empty()){
      config.dwCommonButtons = TDCBF_OK_BUTTON | TDCBF_CANCEL_BUTTON;
    }else{
      config.cButtons = 1;
      config.pButtons = buttons;
      config.dwCommonButtons = TDCBF_CANCEL_BUTTON;
    }
    config.nDefaultButton = focusConfirm ? IDOK : IDCANCEL;
    config.pszWindowTitle = L" ";
    config.pszMainIcon = getNativeDialogIcon(icon);
    config.pszMainInstruction = widen(title).c_str();
    config.pszContent = widen(message).c_str();
    
    TaskDialogIndirect(&config, &buttonClicked, NULL, NULL);
    
    return buttonClicked == IDOK;
  }
  
}
