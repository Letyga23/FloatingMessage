#ifndef FLOATINGMESSAGE_H
#define FLOATINGMESSAGE_H

#include <QTimer>
#include <QWidget>
#include <QLabel>
#include <QProgressBar>
#include <QApplication>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDesktopWidget>
#include <QScreen>
#include <QPainter>
#include <QStyle>

enum class TypeMessage
{
    Information,
    Warning,
    Error
};

class FloatingMessage : public QWidget {
    Q_OBJECT

    QLabel *titleLabel;
    QLabel *textLabel;
    QLabel *iconLabel;
    QProgressBar *progressBar;
    QTimer timer;
    QWidget* _parentForm;

public:
    FloatingMessage();
    void setAlertProperties(TypeMessage typeMessage, QString text, QWidget* parentForm = nullptr);


private:
    void setupUI();
    void setupConnections();
    void positionAlertBox();
    void showNotification();
    void paintEvent(QPaintEvent *event) override;

private slots:
    void updateProgressBar();
};


#endif // FLOATINGMESSAGE_H
