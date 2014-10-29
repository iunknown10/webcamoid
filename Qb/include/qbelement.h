/* Webcamoid, webcam capture application.
 * Copyright (C) 2011-2014  Gonzalo Exequiel Pedone
 *
 * Webcamod is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Webcamod is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Webcamod. If not, see <http://www.gnu.org/licenses/>.
 *
 * Email     : hipersayan DOT x AT gmail DOT com
 * Web-Site 1: http://github.com/hipersayanX/Webcamoid
 * Web-Site 2: http://opendesktop.org/content/show.php/Webcamoid?content=144796
 */

#ifndef QBELEMENT_H
#define QBELEMENT_H

#include <QStringList>
#include <QQmlEngine>

#include "qbpacket.h"

#define qbSend(packet) { \
    if (packet) \
        emit this->oStream(packet); \
    \
    return packet; \
}

class QbElement;

typedef QSharedPointer<QbElement> QbElementPtr;

/// Plugin template.
class QbElement: public QObject
{
    Q_OBJECT
    Q_ENUMS(ElementState)
    Q_PROPERTY(QbElement::ElementState state
               READ state
               WRITE setState
               RESET resetState
               NOTIFY stateChanged)

    public:
        enum ElementState
        {
            ElementStateNull,
            ElementStatePaused,
            ElementStatePlaying
        };

        explicit QbElement(QObject *parent=NULL);
        virtual ~QbElement();

        Q_INVOKABLE virtual QbElement::ElementState state() const;
        Q_INVOKABLE virtual QObject *controlInterface(QQmlEngine *engine,
                                                      const QString &controlId) const;

        Q_INVOKABLE virtual bool link(const QObject *dstElement,
                                      Qt::ConnectionType connectionType=Qt::AutoConnection) const;

        Q_INVOKABLE virtual bool link(const QbElementPtr &dstElement,
                                      Qt::ConnectionType connectionType=Qt::AutoConnection) const;

        Q_INVOKABLE virtual bool unlink(const QObject *dstElement) const;
        Q_INVOKABLE virtual bool unlink(const QbElementPtr &dstElement) const;

        Q_INVOKABLE static bool link(const QbElementPtr &srcElement, const QObject *dstElement,
                                     Qt::ConnectionType connectionType=Qt::AutoConnection);

        Q_INVOKABLE static bool link(const QbElementPtr &srcElement, const QbElementPtr &dstElement,
                                     Qt::ConnectionType connectionType=Qt::AutoConnection);

        Q_INVOKABLE static bool unlink(const QbElementPtr &srcElement, const QObject *dstElement);
        Q_INVOKABLE static bool unlink(const QbElementPtr &srcElement, const QbElementPtr &dstElement);
        Q_INVOKABLE static QbElementPtr create(const QString &pluginId, const QString &elementName="");
        Q_INVOKABLE static QStringList searchPaths();
        Q_INVOKABLE static void addSearchPath(const QString &path);
        Q_INVOKABLE static void setSearchPaths(const QStringList &searchPaths);
        Q_INVOKABLE static void resetSearchPaths();
        Q_INVOKABLE static QStringList listPlugins(const QString &type="");
        Q_INVOKABLE static QVariantMap pluginInfo(const QString &pluginId);

    private:
        static QList<QMetaMethod> methodsByName(const QObject *object, const QString &methodName);
        static bool methodCompat(const QMetaMethod &method1, const QMetaMethod &method2);

    protected:
        QbElement::ElementState m_state;

        virtual void stateChange(QbElement::ElementState from, QbElement::ElementState to);

    signals:
        void stateChanged(QbElement::ElementState state);
        void oStream(const QbPacket &packet);

    public slots:
        virtual QbPacket iStream(const QbPacket &packet);
        virtual void setState(QbElement::ElementState state);
        virtual void resetState();
};

Q_DECLARE_METATYPE(QbElement::ElementState)

#endif // QBELEMENT_H
