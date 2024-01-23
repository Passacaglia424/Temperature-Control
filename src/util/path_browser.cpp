/**
   *******************************************
   * @file      path_browser.cpp
   * @author    yifan
   * @brief     None
   * @attention None
   * @date      2023/9/23
   ********************************************
   */

#include "QPushButton"
#include "QHBoxLayout"
#include "QFileDialog"
#include "util/path_browser.h"

#define BROWSER_BUTTON_ICON ":/common/Resources/common/browse.png"

PathBrowser::PathBrowser(QWidget *parent) : QWidget(parent), m_path_browser(new QLineEdit) {
    setUpUi();
    setContentsMargins(0, 0, 0, 0);
}

void PathBrowser::setUpUi() {
    auto layout = new QHBoxLayout;
    layout->setContentsMargins(0,0,0,0);
    m_path_browser->setReadOnly(true);
    auto browser_button = new QPushButton;
    //browser_button->setIcon(QIcon(BROWSER_BUTTON_ICON));

    layout->addWidget(m_path_browser);
    layout->addWidget(browser_button);

    connect(browser_button, &QPushButton::clicked, this, [this]() {
        auto current_dir = QDir(m_path_browser->text()).absolutePath();
        auto new_dir = QFileDialog::getExistingDirectory(this, "Save Path", current_dir);
        if (!new_dir.isEmpty()) {
            m_path_browser->setText(new_dir);
        }
        if (!new_dir.isEmpty() && new_dir != current_dir) {
            emit sigPathChanged(new_dir);
        }
    });

    setLayout(layout);
}
