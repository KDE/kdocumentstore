/*
 * Copyright (C) 2014  Vishesh Handa <vhanda@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "kdocumentstore.h"
#include "kdocumentcollection.h"

#include <QDateTime>
#include <QFile>
#include <QDebug>

KDocumentStore::KDocumentStore()
{
    m_jdb = ejdbnew();
}

KDocumentStore::~KDocumentStore()
{
    ejdbclose(m_jdb);
    ejdbdel(m_jdb);
}

QString KDocumentStore::filePath() const
{
    return m_filePath;
}

void KDocumentStore::setPath(const QString& filePath)
{
    m_filePath = filePath;
}

bool KDocumentStore::open()
{
    //TODO: Make sure it does not already exist and that it is not a directory!!
    QByteArray path = QFile::encodeName(m_filePath);
    if (!ejdbopen(m_jdb, path.constData(), JBOWRITER | JBOCREAT)) {
        qDebug() << "Could not open db" << m_filePath;
        return false;
    }

    return true;
}

KDocumentCollection KDocumentStore::collection(const QString& name)
{
    return KDocumentCollection(m_jdb, name);
}
