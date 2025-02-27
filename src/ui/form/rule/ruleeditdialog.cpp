#include "ruleeditdialog.h"

#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QVBoxLayout>

#include <form/controls/controlutil.h>
#include <form/controls/plaintextedit.h>
#include <form/controls/zonesselector.h>
#include <manager/windowmanager.h>
#include <util/iconcache.h>
#include <util/net/netutil.h>

#include "rulescontroller.h"

RuleEditDialog::RuleEditDialog(RulesController *ctrl, QWidget *parent) :
    QDialog(parent), m_ctrl(ctrl)
{
    setupUi();
    setupController();
}

RuleListModel *RuleEditDialog::ruleListModel() const
{
    return ctrl()->ruleListModel();
}

void RuleEditDialog::initialize(const RuleRow &ruleRow)
{
    m_ruleRow = ruleRow;

    retranslateUi();

    m_editName->setText(m_ruleRow.ruleName);
    m_editName->setClearButtonEnabled(true);

    m_editNotes->setText(m_ruleRow.notes);
    m_labelEditNotes->setPixmap(IconCache::file(":/icons/script.png"));

    m_cbEnabled->setChecked(ruleRow.enabled);

    m_rbAllow->setChecked(!ruleRow.blocked);
    m_rbBlock->setChecked(ruleRow.blocked);

    m_editRuleText->setText(ruleRow.ruleText);

    m_cbExclusive->setChecked(ruleRow.exclusive);

    m_btZones->setZones(ruleRow.acceptZones);
    m_btZones->setUncheckedZones(ruleRow.rejectZones);

    initializeFocus();
}

void RuleEditDialog::initializeFocus()
{
    m_editName->selectAll();
    m_editName->setFocus();
}

void RuleEditDialog::setupController()
{
    connect(ctrl(), &RulesController::retranslateUi, this, &RuleEditDialog::retranslateUi);
}

void RuleEditDialog::retranslateUi()
{
    this->unsetLocale();

    m_labelEditName->setText(tr("Name:"));
    m_editNotes->setPlaceholderText(tr("Notes"));
    m_cbEnabled->setText(tr("Enabled"));

    m_rbAllow->setText(tr("Allow"));
    m_rbBlock->setText(tr("Block"));

    m_cbExclusive->setText(tr("Exclusive"));
    m_btZones->retranslateUi();

    m_btOptions->setToolTip(tr("Advanced Options"));
    m_btOk->setText(tr("OK"));
    m_btCancel->setText(tr("Cancel"));

    this->setWindowTitle(tr("Edit Rule"));
}

void RuleEditDialog::setupUi()
{
    // Main Layout
    auto layout = setupMainLayout();
    this->setLayout(layout);

    // Font
    this->setFont(WindowManager::defaultFont());

    // Modality
    this->setWindowModality(Qt::WindowModal);

    // Size Grip
    this->setSizeGripEnabled(true);

    // Size
    this->setMinimumSize(500, 300);
}

QLayout *RuleEditDialog::setupMainLayout()
{
    // Form Layout
    auto formLayout = setupFormLayout();

    // Allow/Block Actions Layout
    auto actionsLayout = setupActionsLayout();

    // Rule Text
    m_editRuleText = new PlainTextEdit();

    // Advanced Options
    setupAdvancedOptions();

    // OK/Cancel
    auto buttonsLayout = setupButtons();

    auto layout = new QVBoxLayout();
    layout->addLayout(formLayout);
    layout->addWidget(ControlUtil::createHSeparator());
    layout->addStretch();
    layout->addLayout(actionsLayout);
    layout->addWidget(ControlUtil::createHSeparator());
    layout->addWidget(m_editRuleText);
    layout->addStretch();
    layout->addWidget(ControlUtil::createHSeparator());
    layout->addLayout(buttonsLayout);

    return layout;
}

QLayout *RuleEditDialog::setupFormLayout()
{
    auto layout = new QFormLayout();

    // Name
    m_editName = new QLineEdit();
    m_editName->setMaxLength(1024);

    layout->addRow("Name:", m_editName);
    m_labelEditName = ControlUtil::formRowLabel(layout, m_editName);

    // Notes
    m_editNotes = new PlainTextEdit();
    m_editNotes->setFixedHeight(40);

    layout->addRow("Notes:", m_editNotes);
    m_labelEditNotes = ControlUtil::formRowLabel(layout, m_editNotes);
    m_labelEditNotes->setScaledContents(true);
    m_labelEditNotes->setFixedSize(32, 32);

    // Enabled
    m_cbEnabled = new QCheckBox();

    layout->addRow(QString(), m_cbEnabled);

    return layout;
}

QLayout *RuleEditDialog::setupActionsLayout()
{
    // Allow
    m_rbAllow = new QRadioButton();
    m_rbAllow->setIcon(IconCache::icon(":/icons/accept.png"));
    m_rbAllow->setChecked(true);

    // Block
    m_rbBlock = new QRadioButton();
    m_rbBlock->setIcon(IconCache::icon(":/icons/deny.png"));

    auto layout = ControlUtil::createHLayoutByWidgets(
            { /*stretch*/ nullptr, m_rbAllow, m_rbBlock, /*stretch*/ nullptr });
    layout->setSpacing(20);

    return layout;
}

void RuleEditDialog::setupAdvancedOptions()
{
    // Exclusive
    m_cbExclusive = new QCheckBox();

    // Zones
    m_btZones = new ZonesSelector();
    m_btZones->setIsTristate(true);
    m_btZones->setMaxZoneCount(16); // sync with driver's FORT_APP_ENTRY

    auto layout = new QVBoxLayout();
    layout->addWidget(m_cbExclusive);
    layout->addWidget(ControlUtil::createHSeparator());
    layout->addWidget(m_btZones);

    auto menu = ControlUtil::createMenuByLayout(layout, this);

    m_btOptions = ControlUtil::createButton(":/icons/widgets.png");
    m_btOptions->setShortcut(QKeyCombination(Qt::CTRL, Qt::Key_O));
    m_btOptions->setMenu(menu);
}

QLayout *RuleEditDialog::setupButtons()
{
    // OK
    m_btOk = ControlUtil::createButton(QString(), [&] {
        if (save()) {
            this->close();
        }
    });
    m_btOk->setDefault(true);

    // Cancel
    m_btCancel = new QPushButton();
    connect(m_btCancel, &QAbstractButton::clicked, this, &QWidget::close);

    auto layout = new QHBoxLayout();
    layout->addWidget(m_btOptions);
    layout->addWidget(m_btOk, 1, Qt::AlignRight);
    layout->addWidget(m_btCancel);

    return layout;
}

bool RuleEditDialog::save()
{
    if (!validateFields())
        return false;

    Rule rule;
    fillRule(rule);

    // Add new zone
    if (isEmpty()) {
        return ctrl()->addOrUpdateRule(rule);
    }

    // Edit selected zone
    return saveRule(rule);
}

bool RuleEditDialog::saveRule(Rule &rule)
{
    if (!rule.isOptionsEqual(m_ruleRow)) {
        rule.ruleId = m_ruleRow.ruleId;

        return ctrl()->addOrUpdateRule(rule);
    }

    if (!rule.isNameEqual(m_ruleRow)) {
        return ctrl()->updateRuleName(m_ruleRow.ruleId, rule.ruleName);
    }

    return true;
}

bool RuleEditDialog::validateFields() const
{
    // Name
    if (m_editName->text().isEmpty()) {
        m_editName->setFocus();
        return false;
    }

    return true;
}

void RuleEditDialog::fillRule(Rule &rule) const
{
    rule.enabled = m_cbEnabled->isChecked();
    rule.blocked = !m_rbAllow->isChecked();
    rule.exclusive = m_cbExclusive->isChecked();

    rule.acceptZones = m_btZones->zones();
    rule.rejectZones = m_btZones->uncheckedZones();

    rule.ruleName = m_editName->text();
    rule.notes = m_editNotes->toPlainText();
    rule.ruleText = m_editRuleText->toPlainText();
}
