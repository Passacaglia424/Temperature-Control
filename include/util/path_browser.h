/**
   *******************************************
   * @file      path_browser.h
   * @author    yifan
   * @brief     None
   * @attention None
   * @date      2023/9/23
   ********************************************
   */


#ifndef PATH_BROWSER_H
#define PATH_BROWSER_H

#include "QWidget"
#include "QDir"
#include "QLineEdit"

class PathBrowser : public QWidget {
Q_OBJECT
public:
    explicit PathBrowser(QWidget *parent = nullptr);

    ~PathBrowser() override = default;

    inline void setPath(const QString &path) {
        QDir dir(path);
        m_path_browser->setText(dir.absolutePath());
        m_path_browser->setToolTip(dir.absolutePath());
    }

    inline QString getPath() {
        return m_path_browser->text();
    }

signals:

    void sigPathChanged(const QString &path);

private:
    QLineEdit *m_path_browser;

    void setUpUi();
};

#endif //PATH_BROWSER_H
