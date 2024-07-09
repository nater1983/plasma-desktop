/*
    SPDX-FileCopyrightText: 2024 Joshua Goins <joshua.goins@kdab.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "inputsequence.h"

#include <KLocalizedString>

// KWin needs the button defined in linux's input-event-code.h
constexpr int BTN_LEFT = 0x110;
constexpr int BTN_RIGHT = 0x111;
constexpr int BTN_MIDDLE = 0x112;

InputSequence::InputSequence() = default;

InputSequence::InputSequence(const QStringList &config)
{
    if (config.empty()) {
        return;
    }

    const QString &type = config.first();
    if (type == "Disabled") {
        setType(Type::Disabled);
    } else if (type == "Key") {
        setType(Type::Keyboard);

        if (config.size() == 2) {
            keyData() = config.last();
        }
    } else if (type == "MouseButton") {
        setType(Type::Mouse);

        // At least has a valid mouse button
        if (config.size() >= 2) {
            const auto linuxButton = config[1].toInt();
            if (linuxButton == BTN_LEFT) {
                mouseData().button = Qt::LeftButton;
            } else if (linuxButton == BTN_RIGHT) {
                mouseData().button = Qt::RightButton;
            } else if (linuxButton == BTN_MIDDLE) {
                mouseData().button = Qt::MiddleButton;
            }

            // Also has a keyboard modifier at the end
            if (config.size() >= 3) {
                const auto keyboardModsRaw = config[2].toInt();
                mouseData().modifiers = Qt::KeyboardModifiers{keyboardModsRaw};
            }
        }
    } else if (type == "ApplicationDefined") {
        setType(Type::ApplicationDefined);
    }
}

InputSequence::Type InputSequence::type() const
{
    return m_type;
}

void InputSequence::setType(const Type type)
{
    if (m_type != type) {
        m_type = type;

        // Make sure to reset the data when the type changed
        switch (m_type) {
        case Type::Disabled:
        case Type::ApplicationDefined:
            m_data = NoData{};
            break;
        case Type::Keyboard:
            m_data = KeyData{""};
            break;
        case Type::Mouse:
            m_data = MouseData{.button = Qt::LeftButton, .modifiers = {}};
            break;
        default:
            Q_UNREACHABLE();
        }
    }
}

QStringList InputSequence::toConfigFormat() const
{
    switch (m_type) {
    case Type::Disabled:
        return QStringList{"Disabled"};
    case Type::Keyboard: {
        const auto key = keyData().toString(QKeySequence::PortableText);
        return QStringList{"Key", key};
    }
    case Type::Mouse: {
        const auto mouse = mouseData();

        int linuxButton;
        switch (mouse.button) {
        case Qt::LeftButton:
            linuxButton = BTN_LEFT;
            break;
        case Qt::RightButton:
            linuxButton = BTN_RIGHT;
            break;
        case Qt::MiddleButton:
            linuxButton = BTN_MIDDLE;
            break;
        default:
            Q_UNREACHABLE();
        }

        QStringList values{"MouseButton", QString::number(linuxButton)};
        if (mouse.modifiers != Qt::NoModifier) {
            values.append(QString::number(mouse.modifiers.toInt()));
        }
        return values;
    }
    case Type::ApplicationDefined:
        return QStringList{}; // to pass events through, just don't give KWin anything
    default:
        Q_UNREACHABLE();
    }
}

QString InputSequence::toString() const
{
    switch (m_type) {
    case Type::Disabled:
        return i18nc("@action:button This action is disabled", "Disabled");
    case Type::Keyboard: {
        if (keyData().isEmpty()) {
            return i18nc("@action:button There is no keybinding", "None");
        }
        return keyData().toString(QKeySequence::NativeText);
    }
    case Type::Mouse: {
        switch (mouseData().button) {
        case Qt::LeftButton:
            return i18nc("@action:button", "Left mouse button");
        case Qt::RightButton:
            return i18nc("@action:button", "Right mouse button");
        case Qt::MiddleButton:
            return i18nc("@action:button", "Middle mouse button");
        default:
            Q_UNREACHABLE();
        }
    }
    case Type::ApplicationDefined:
        return i18nc("@action:button", "Application-defined");
    default:
        Q_UNREACHABLE();
    }
}

QKeySequence InputSequence::keySequence() const
{
    return keyData();
}

void InputSequence::setKeySequence(const QKeySequence &sequence)
{
    keyData() = sequence;
}

Qt::MouseButton InputSequence::mouseButton() const
{
    return mouseData().button;
}

void InputSequence::setMouseButton(const Qt::MouseButton button)
{
    mouseData().button = button;
}

Qt::KeyboardModifiers InputSequence::keyboardModifiers() const
{
    return mouseData().modifiers;
}

void InputSequence::setKeyboardModifiers(const Qt::KeyboardModifiers modifiers)
{
    mouseData().modifiers = modifiers;
}

InputSequence::KeyData &InputSequence::keyData()
{
    Q_ASSERT(m_type == Type::Keyboard);
    return std::get<KeyData>(m_data);
}

InputSequence::MouseData &InputSequence::mouseData()
{
    Q_ASSERT(m_type == Type::Mouse);
    return std::get<MouseData>(m_data);
}

InputSequence::KeyData InputSequence::keyData() const
{
    Q_ASSERT(m_type == Type::Keyboard);
    return std::get<KeyData>(m_data);
}

InputSequence::MouseData InputSequence::mouseData() const
{
    Q_ASSERT(m_type == Type::Mouse);
    return std::get<MouseData>(m_data);
}