#ifndef OPTMAINPAGE_H
#define OPTMAINPAGE_H

#include "optbasepage.h"

QT_FORWARD_DECLARE_CLASS(QTabWidget)

class OptMainPage : public OptBasePage
{
    Q_OBJECT

public:
    explicit OptMainPage(OptionsController *ctrl = nullptr, QWidget *parent = nullptr);

    void selectTab(int index);

protected slots:
    void onRetranslateUi() override;

private:
    void setupUi();
    void setupTabBar();
    QLayout *setupDialogButtons();
    void setupBackup();
    void setupApplyCancelButtons();

private:
    QTabWidget *m_tabWidget = nullptr;

    QPushButton *m_btMenu = nullptr;

    QPushButton *m_btBackup = nullptr;
    QAction *m_actExport = nullptr;
    QAction *m_actImport = nullptr;
    QPushButton *m_btDefault = nullptr;
    QPushButton *m_btOk = nullptr;
    QPushButton *m_btApply = nullptr;
    QPushButton *m_btCancel = nullptr;

    QVector<OptBasePage *> m_pages;
};

#endif // OPTMAINPAGE_H
