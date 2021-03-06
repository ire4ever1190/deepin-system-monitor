/* -*- Mode: C++; indent-tabs-mode: nil; tab-width: 4 -*-
 * -*- coding: utf-8 -*-
 *
 * Copyright (C) 2011 ~ 2017 Deepin, Inc.
 *               2011 ~ 2017 Wang Yong
 *
 * Author:     Wang Yong <wangyong@deepin.com>
 * Maintainer: Wang Yong <wangyong@deepin.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QApplication>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFontMetrics>
#include <QLayout>
#include <QPainter>
#include <QStandardPaths>
#include <QString>
#include <QWidget>
#include <QtMath>

#include "utils.h"

Utils::Utils(QObject *parent) : QObject(parent)
{
    
}

QString Utils::getImagePath(QString imageName)
{
    QDir dir(qApp->applicationDirPath());
    dir.cdUp();

    return QDir(dir.filePath("image")).filePath(imageName);
}

QString Utils::getQssPath(QString qssName)
{
    return QString(":/qss/%1").arg(qssName);
}

void Utils::applyQss(QWidget *widget, QString qssName)
{
    QFile file(Utils::getQssPath(qssName));
    file.open(QFile::ReadOnly);
    QTextStream filetext(&file);
    QString stylesheet = filetext.readAll();
    widget->setStyleSheet(stylesheet);
    file.close();
}

QString Utils::getQrcPath(QString imageName)
{
    return QString(":/image/%1").arg(imageName);
}

QSize Utils::getRenderSize(int fontSize, QString string)
{
    QFont font;
    font.setPointSize(fontSize);
    QFontMetrics fm(font);

    int width = 0;
    int height = 0;
    foreach (auto line, string.split("\n")) {
        int lineWidth = fm.width(line);
        int lineHeight = fm.height();

        if (lineWidth > width) {
            width = lineWidth;
        }
        height += lineHeight;
    }

    return QSize(width, height);
}

void Utils::setFontSize(QPainter &painter, int textSize)
{
    QFont font = painter.font() ;
    font.setPointSize(textSize);
    painter.setFont(font);
}

void Utils::removeChildren(QWidget *widget)
{
    qDeleteAll(widget->children());
}

void Utils::removeLayoutChild(QLayout *layout, int index)
{
    QLayoutItem *item = layout->itemAt(index);
    if (item != 0) {
        QWidget *widget = item->widget();
        if (widget != NULL) {
            widget->hide();
            widget->setParent(NULL);
            layout->removeWidget(widget);
        }
    }
}

void Utils::addLayoutWidget(QLayout *layout, QWidget *widget)
{
    layout->addWidget(widget);
    widget->show();
}

QString Utils::formatMillisecond(int millisecond)
{
    if (millisecond / 1000 < 3600) {
        // At least need return 1 seconds.
        return QDateTime::fromTime_t(std::max(1, millisecond / 1000)).toUTC().toString("mm:ss");
    } else {
        return QDateTime::fromTime_t(millisecond / 1000).toUTC().toString("hh:mm:ss");
    }
}

QString Utils::getRecordingSaveDirectory()
{
    QDir musicDirectory = QDir(QStandardPaths::standardLocations(QStandardPaths::MusicLocation).first());
    QString subDirectory = tr("Recording");
    musicDirectory.mkdir(subDirectory);
    
    return musicDirectory.filePath(subDirectory);
}

QFileInfoList Utils::getRecordingFileinfos()
{
    QStringList filters;
    filters << "*.wav";
    return QDir(Utils::getRecordingSaveDirectory()).entryInfoList(filters, QDir::Files|QDir::NoDotAndDotDot);
}

bool Utils::fileExists(QString path) 
{
    QFileInfo check_file(path);
    
    // check if file exists and if yes: Is it really a file and no directory?
    return check_file.exists() && check_file.isFile();
}

qreal Utils::easeInOut(qreal x)
{
    return (1 - qCos(M_PI * x)) / 2;
}

qreal Utils::easeInQuad(qreal x)
{
    return qPow(x, 2);
}

qreal Utils::easeOutQuad(qreal x)
{
    return -1 * qPow(x - 1, 2) + 1;
}

qreal Utils::easeInQuint(qreal x)
{
    return qPow(x, 5);
}

qreal Utils::easeOutQuint(qreal x)
{
    return qPow(x - 1, 5) + 1;
}

QString Utils::convertSizeUnit(int bytes) 
{
    if (bytes < qPow(2, 10)) {
        return QString("%1 B").arg(bytes);
    } else if (bytes < qPow(2, 20)) {
        return QString("%1 KiB").arg(QString::number(qreal(bytes) / qPow(2, 10), 'f', 1));
    } else if (bytes < qPow(2, 30)) {
        return QString("%1 MiB").arg(QString::number(qreal(bytes) / qPow(2, 20), 'f', 1));
    } else if (bytes < qPow(2, 40)) {
        return QString("%1 GiB").arg(QString::number(qreal(bytes) / qPow(2, 30), 'f', 1));
    } else if (bytes < qPow(2, 50)) {
        return QString("%1 TiB").arg(QString::number(qreal(bytes) / qPow(2, 40), 'f', 1));
    }
    
    return QString::number(bytes);
}
