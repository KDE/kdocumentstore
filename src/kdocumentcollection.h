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

#ifndef _KVARIANT_COLLECTION_H
#define _KVARIANT_COLLECTION_H

#include "kdocumentstore_export.h"
#include <QVariantMap>
#include <tcejdb/ejdb.h>

class KDocumentStore;
class KDocumentQuery;

class KDOCUMENTSTORE_EXPORT KDocumentCollection
{
public:
    KDocumentCollection();
    ~KDocumentCollection();

    QString collectionName() const;

    QString insert(const QVariantMap& map);
    QVariantMap fetch(const QString& id) const;
    bool remove(const QString& id);

    KDocumentQuery find(const QVariantMap& query, const QVariantMap& hints = QVariantMap()) const;
    int count(const QVariantMap& query) const;
    QVariantMap findOne(const QVariantMap& query, const QVariantMap& hints = QVariantMap()) const;

    bool ensureIndex(const QString& propertyName);

private:
    KDocumentCollection(EJDB* db, const QString& name);

    EJDB* m_db;
    EJCOLL* m_coll;
    QString m_collectionName;

    friend class KDocumentStore;
};

#endif
