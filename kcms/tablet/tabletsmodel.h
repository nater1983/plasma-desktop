/*
    SPDX-FileCopyrightText: 2021 Aleix Pol Gonzalez <aleixpol@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QAbstractListModel>

#include <memory>
#include <vector>

#include "inputdevice.h"

class QDBusInterface;
// class InputDevice;

struct TabletDevice {
    QString deviceGroup;
    std::unique_ptr<InputDevice> padDevice;
    std::unique_ptr<InputDevice> penDevice;
};

class TabletsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit TabletsModel(QObject *parent = nullptr);

    QHash<int, QByteArray> roleNames() const override;
    QVariant data(const QModelIndex &index, int role) const override;
    int rowCount(const QModelIndex &parent) const override;
    // Q_SCRIPTABLE InputDevice *deviceAt(int row) const;

    // void load();
    // void save();
    // void defaults();
    // bool isSaveNeeded() const;
    // bool isDefaults() const;

private Q_SLOTS:
    void onDeviceAdded(const QString &sysName);
    void onDeviceRemoved(const QString &sysName);

Q_SIGNALS:
    void needsSaveChanged();

private:
    void addDevice(const QString &sysname, bool tellModel);
    void resetModel();

    std::vector<TabletDevice> m_devices;
    QDBusInterface *m_deviceManager;
};
