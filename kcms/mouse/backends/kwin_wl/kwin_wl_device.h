/*
    SPDX-FileCopyrightText: 2018 Roman Gilg <subdiff@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "inputdevice.h"

#include <QObject>
#include <QString>

class QDBusInterface;

class KWinWaylandDevice : public InputDevice
{
    Q_OBJECT

public:
    KWinWaylandDevice(const QString &dbusName);
    ~KWinWaylandDevice() override;

    bool init();

    bool getDefaultConfig();
    bool applyConfig();
    bool isChangedConfig() const;

    //
    // general
    QString name() const override
    {
        return m_name.val;
    }

    QString sysName() const
    {
        return m_sysName.val;
    }

    bool supportsDisableEvents() const override
    {
        return m_supportsDisableEvents.val;
    }

    void setEnabled(bool enabled) override
    {
        m_enabled.set(enabled);
    }

    bool isEnabled() const override
    {
        return m_enabled.val;
    }

    Qt::MouseButtons supportedButtons() const override
    {
        return m_supportedButtons.val;
    }

    //
    // advanced
    bool supportsLeftHanded() const override
    {
        return m_supportsLeftHanded.val;
    }

    bool leftHandedEnabledByDefault() const override
    {
        return m_leftHandedEnabledByDefault.val;
    }

    bool isLeftHanded() const override
    {
        return m_leftHanded.val;
    }

    void setLeftHanded(bool set) override
    {
        m_leftHanded.set(set);
    }

    bool supportsMiddleEmulation() const override
    {
        return m_supportsMiddleEmulation.val;
    }

    bool middleEmulationEnabledByDefault() const override
    {
        return m_middleEmulationEnabledByDefault.val;
    }

    bool isMiddleEmulation() const override
    {
        return m_middleEmulation.val;
    }

    void setMiddleEmulation(bool set) override
    {
        m_middleEmulation.set(set);
    }

    //
    // acceleration speed and profile
    bool supportsPointerAcceleration() const override
    {
        return m_supportsPointerAcceleration.val;
    }

    qreal pointerAcceleration() const override
    {
        return m_pointerAcceleration.val;
    }

    void setPointerAcceleration(qreal acceleration) override
    {
        m_pointerAcceleration.set(acceleration);
    }

    bool supportsPointerAccelerationProfileFlat() const override
    {
        return m_supportsPointerAccelerationProfileFlat.val;
    }

    bool defaultPointerAccelerationProfileFlat() const override
    {
        return m_defaultPointerAccelerationProfileFlat.val;
    }

    bool pointerAccelerationProfileFlat() const override
    {
        return m_pointerAccelerationProfileFlat.val;
    }

    void setPointerAccelerationProfileFlat(bool set) override
    {
        m_pointerAccelerationProfileFlat.set(set);
    }

    bool supportsPointerAccelerationProfileAdaptive() const override
    {
        return m_supportsPointerAccelerationProfileAdaptive.val;
    }

    bool defaultPointerAccelerationProfileAdaptive() const override
    {
        return m_defaultPointerAccelerationProfileAdaptive.val;
    }

    bool pointerAccelerationProfileAdaptive() const override
    {
        return m_pointerAccelerationProfileAdaptive.val;
    }

    void setPointerAccelerationProfileAdaptive(bool set) override
    {
        m_pointerAccelerationProfileAdaptive.set(set);
    }

    //
    // scrolling
    bool supportsNaturalScroll() const override
    {
        return m_supportsNaturalScroll.val;
    }

    bool naturalScrollEnabledByDefault() const override
    {
        return m_naturalScrollEnabledByDefault.val;
    }

    bool isNaturalScroll() const override
    {
        return m_naturalScroll.val;
    }

    void setNaturalScroll(bool set) override
    {
        m_naturalScroll.set(set);
    }

    qreal scrollFactor() const override
    {
        return m_scrollFactor.val;
    }

    void setScrollFactor(qreal set) override
    {
        m_scrollFactor.set(set);
    }

private:
    template<typename T>
    struct Prop {
        using value_type = T;
        using ChangedSignal = void (KWinWaylandDevice::*)();

        explicit Prop(KWinWaylandDevice *device, const char *dbusName, ChangedSignal changedSignal = nullptr)
            : dbus(dbusName)
            , changedSignalFunction(changedSignal)
            , device(device)
        {
        }

        void set(T newVal)
        {
            if (avail && val != newVal) {
                val = newVal;
                if (changedSignalFunction) {
                    (device->*changedSignalFunction)();
                }
            }
        }
        void set(const Prop<T> &p)
        {
            if (avail && val != p.val) {
                val = p.val;
                if (changedSignalFunction) {
                    (device->*changedSignalFunction)();
                }
            }
        }
        bool changed() const
        {
            return avail && (old != val);
        }

        QLatin1String dbus;
        bool avail;
        const ChangedSignal changedSignalFunction;
        KWinWaylandDevice *const device;
        T old;
        T val;
    };

    template<typename T>
    bool valueLoader(Prop<T> &prop);

    template<typename T>
    QString valueWriter(const Prop<T> &prop);

    //
    // general
    Prop<QString> m_name{this, "name"};
    Prop<QString> m_sysName{this, "sysName"};
    Prop<bool> m_supportsDisableEvents{this, "supportsDisableEvents"};
    Prop<bool> m_enabled{this, "enabled", &KWinWaylandDevice::enabledChanged};

    //
    // advanced
    Prop<Qt::MouseButtons> m_supportedButtons{this, "supportedButtons"};

    Prop<bool> m_supportsLeftHanded{this, "supportsLeftHanded"};
    Prop<bool> m_leftHandedEnabledByDefault{this, "leftHandedEnabledByDefault"};
    Prop<bool> m_leftHanded{this, "leftHanded", &KWinWaylandDevice::leftHandedChanged};

    Prop<bool> m_supportsMiddleEmulation{this, "supportsMiddleEmulation"};
    Prop<bool> m_middleEmulationEnabledByDefault{this, "middleEmulationEnabledByDefault"};
    Prop<bool> m_middleEmulation{this, "middleEmulation", &KWinWaylandDevice::middleEmulationChanged};

    //
    // acceleration speed and profile
    Prop<bool> m_supportsPointerAcceleration{this, "supportsPointerAcceleration"};
    Prop<qreal> m_defaultPointerAcceleration{this, "defaultPointerAcceleration"};
    Prop<qreal> m_pointerAcceleration{this, "pointerAcceleration", &KWinWaylandDevice::pointerAccelerationChanged};

    Prop<bool> m_supportsPointerAccelerationProfileFlat{this, "supportsPointerAccelerationProfileFlat"};
    Prop<bool> m_defaultPointerAccelerationProfileFlat{this, "defaultPointerAccelerationProfileFlat"};
    Prop<bool> m_pointerAccelerationProfileFlat{this, "pointerAccelerationProfileFlat", &KWinWaylandDevice::pointerAccelerationProfileFlatChanged};

    Prop<bool> m_supportsPointerAccelerationProfileAdaptive{this, "supportsPointerAccelerationProfileAdaptive"};
    Prop<bool> m_defaultPointerAccelerationProfileAdaptive{this, "defaultPointerAccelerationProfileAdaptive"};
    Prop<bool> m_pointerAccelerationProfileAdaptive{this, "pointerAccelerationProfileAdaptive", &KWinWaylandDevice::pointerAccelerationProfileAdaptiveChanged};

    //
    // scrolling
    Prop<bool> m_supportsNaturalScroll{this, "supportsNaturalScroll"};
    Prop<bool> m_naturalScrollEnabledByDefault{this, "naturalScrollEnabledByDefault"};
    Prop<bool> m_naturalScroll{this, "naturalScroll", &KWinWaylandDevice::naturalScrollChanged};
    Prop<qreal> m_scrollFactor{this, "scrollFactor", &KWinWaylandDevice::scrollFactorChanged};

    QString m_dbusName;
};
