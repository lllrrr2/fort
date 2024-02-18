#include "iniuser.h"

#include <conf/appgroup.h>
#include <form/tray/trayicon.h>

IniUser::IniUser(Settings *settings) : MapSettings(settings) { }

void IniUser::saveDefaultIni()
{
    resetLanguage();

    saveAndClear();
}

void IniUser::resetLanguage()
{
    setLanguage(defaultLanguage());
}

void IniUser::resetTrayActions()
{
    for (int i = 0; i < TrayIcon::ClickTypeCount; ++i) {
        const auto clickType = TrayIcon::ClickType(i);
        const QString eventName = TrayIcon::clickNameByType(clickType);

        const TrayIcon::ActionType actionType = TrayIcon::defaultActionTypeByClick(clickType);
        const QString actionName = TrayIcon::actionNameByType(actionType);

        setTrayAction(eventName, actionName);
    }
}

void IniUser::resetToDefault()
{
    resetLanguage();

    setUseSystemLocale(true);
    setHotKeyEnabled(false);
    setHotKeyGlobal(true);

    setSplashWindowVisible(true);
    setHomeAutoShowMenu(false);
    setProgNotifyMessage(true);
    setTrayShowIcon(true);
    setTrayAnimateAlert(true);
    setTrayMaxGroups(MAX_APP_GROUP_COUNT);

    resetTrayActions();

    setConfirmTrayFlags(false);
    setConfirmQuit(true);

    setProgAlertWindowAutoShow(true);
    setProgAlertWindowAlwaysOnTop(false);
    setGraphWindowHideOnClose(false);
}
