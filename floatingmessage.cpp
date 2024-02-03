#include "floatingmessage.h"
#include <QDebug>

FloatingMessage::FloatingMessage()
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground); //для скругления

    setupUI();
    setupConnections();
}

void FloatingMessage::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.setBrush(palette().window());
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(contentsRect(), 10, 10); //радиус скругления
}

void FloatingMessage::setAlertProperties(TypeMessage typeMessage, QString text, QColor progressBarColor, QWidget* parentForm)
{
    _parentForm = parentForm;

    QString title;
    QString backgroundColor;
    QIcon icon;

    switch (typeMessage)
    {
    case TypeMessage::Information :
        title = "Информация";
        backgroundColor = "#9bf29e";
        icon = style()->standardIcon(QStyle::SP_MessageBoxInformation);
        break;
    case TypeMessage::Warning :
        title = "Внимание";
        backgroundColor = "#fcb786";
        icon = style()->standardIcon(QStyle::SP_MessageBoxWarning);
        break;
    case TypeMessage::Error :
        title = "Ошибка";
        backgroundColor = "#ff8585";
        icon = style()->standardIcon(QStyle::SP_MessageBoxCritical);
        break;
    }

    titleLabel->setText(title);
    textLabel->setText(text);
    setStyleSheet(QString("background-color: %1;").arg(backgroundColor));
    progressBar->setStyleSheet(QString("QProgressBar::chunk { background-color: %1; }").arg(progressBarColor.name()));
    iconLabel->setPixmap(icon.pixmap(iconLabel->size()));
    showNotification();
}

void FloatingMessage::showNotification() {
    positionAlertBox();
    show();
    timer.start(1);
}

void FloatingMessage::updateProgressBar()
{
    progressBar->setValue(progressBar->value() + 1);
    if (progressBar->value() == progressBar->maximum())
        close();
}

void FloatingMessage::setupUI()
{
    setFixedSize(300, 100);

    QGridLayout* mainLayout = new QGridLayout(this);

    iconLabel = new QLabel(this);
    iconLabel->setFixedSize(32, 32);

    titleLabel = new QLabel(this);
    textLabel = new QLabel(this);
    progressBar = new QProgressBar(this);
    progressBar->setTextVisible(false);

    mainLayout->addWidget(iconLabel, 1, 0, 2, 1);
    mainLayout->addWidget(titleLabel, 1, 1);
    mainLayout->addWidget(textLabel, 2, 1);
    mainLayout->addWidget(progressBar, 3, 0, 1, 2);

    int time = 1500;

    progressBar->setRange(0, time);
}

void FloatingMessage::setupConnections()
{
    connect(&timer, &QTimer::timeout, this, &FloatingMessage::updateProgressBar);
}

void FloatingMessage::positionAlertBox()
{
    if (_parentForm) {
        QPoint parentPos = _parentForm->mapToGlobal(QPoint(0, 0));
        int x = parentPos.x() + _parentForm->width() - width() - 15;
        int y = parentPos.y() + _parentForm->height() - height() - 15;
        move(x, y);
    }
    else
    {
        QRect screenGeometry = QGuiApplication::primaryScreen()->availableGeometry();
        int x = screenGeometry.width() - width() - 15;
        int y = screenGeometry.height() - height() - 15;
        move(x, y);
    }
}
