/*
 * Copyright (C) 2013 Alexander Mezin <mezin.alexander@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef TESTBUTTON_H
#define TESTBUTTON_H

#include <QPushButton>

class TestButton : public QPushButton
{
    Q_OBJECT

public:
    explicit TestButton(QWidget *);

protected:
    void mousePressEvent(QMouseEvent *) Q_DECL_OVERRIDE;

private Q_SLOTS:
    void resetText();

private:
    QString m_originalText;
    bool m_firstClick;
};

#endif // TESTBUTTON_H
