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

#include "../kdocumentstore.h"
#include "../kdocumentcollection.h"
#include "../kdocumentquery.h"

#include <QTest>
#include <QVariantMap>
#include <QDebug>
#include <QTemporaryDir>
#include <QUuid>
#include <QRegularExpression>

class DatabaseTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void init()
    {
        db.reset(new KDocumentStore);
        db->setPath(m_tempDir.path() + QUuid::createUuid().toString());
        QVERIFY(db->open());
    }

    void testInsertAndFetch();
    void testInsertWithId();
    void testInsertArray();
    void testInsertRegex();
    void testInsertMap();
    void testDoubleInsert();
    void testInsertAndQuery();
    void testAndQuery();
    void testQueryNull();
    void testQueryNullValue();
    void testQueryRegexp();
    void testQueryCount();
    void testQueryFindOne();
    void testQuerySort();
private:
    QTemporaryDir m_tempDir;
    QScopedPointer<KDocumentStore> db;
};

void DatabaseTest::testInsertAndFetch()
{
    QVariantMap data;
    data["type"] = "episode";
    data["mimetype"] = "video/mp4";
    data["series"] = "Outlander";
    data["episodeNumber"] = 5;
    data["doubleValue"] = 5.44;
    data["dateValue"] = QDate(2014, 12, 2);
    data["dateTimeValue"] = QDateTime::currentDateTime();

    KDocumentCollection col = db->collection("testCol");
    QString id = col.insert(data);
    QVariantMap output = col.fetch(id);

    data["_id"] = id;
    QCOMPARE(output, data);
}

void DatabaseTest::testInsertWithId()
{
    QString id("50fe106b35cf1e0300000006");
    QVariantMap data;
    data["type"] = "episode";
    data["mimetype"] = "video/mp4";
    data["_id"] = id;

    KDocumentCollection col = db->collection("testCol");
    QString newId = col.insert(data);
    QCOMPARE(newId, id);
    QCOMPARE(col.fetch(id), data);
}

void DatabaseTest::testInsertArray()
{
    QList<QVariant> list = {"alpha", "beta", 46};

    QVariantMap data;
    data["type"] = "episode";
    data["death"] = QVariant(list);
    data["mimetype"] = "video/mp4";

    KDocumentCollection col = db->collection("testCol");
    QString id = col.insert(data);
    QVariantMap output = col.fetch(id);

    data["_id"] = id;

    QCOMPARE(output, data);
    // QVariant comparison does not seem to check the exact types
    for (auto it = output.begin(); it != output.end(); it++) {
        QCOMPARE(it.value().type(), data.value(it.key()).type());
    }
}

void DatabaseTest::testInsertRegex()
{
    QVariantMap data;
    data["type"] = "episode";
    data["mimetype"] = QRegularExpression("video/mp4");

    KDocumentCollection col = db->collection("testCol");
    QString id = col.insert(data);
    data["_id"] = id;

    QVariantMap output = col.fetch(id);
    QCOMPARE(output, data);
}

void DatabaseTest::testInsertMap()
{
    QVariantMap subData = {{"type", "subtype"}};
    QVariantMap data;
    data["type"] = "episode";
    data["data"] = subData;

    KDocumentCollection col = db->collection("testCol");
    QString id = col.insert(data);
    data["_id"] = id;

    QVariantMap output = col.fetch(id);
    QCOMPARE(output, data);
}

void DatabaseTest::testDoubleInsert()
{
    QVariantMap data;
    data["type"] = "episode";
    data["mimetype"] = "video/mp4";

    KDocumentCollection col = db->collection("testCol");
    QString id = col.insert(data);

    data["_id"] = id;
    QCOMPARE(col.fetch(id), data);

    data["type"] = "fire";
    QString id2 = col.insert(data);
    QCOMPARE(id, id2);
    QCOMPARE(col.fetch(id), data);
}

void DatabaseTest::testInsertAndQuery()
{
    QVariantMap data;
    data["type"] = "episode";
    data["mimetype"] = "video/mp4";
    data["series"] = "Outlander";
    data["episodeNumber"] = 5;

    KDocumentCollection col = db->collection("testCol");
    QString id1 = col.insert(data);

    data["episodeNumber"] = 6;
    QString id2 = col.insert(data);

    QVariantMap queryMap = {{"type", "episode"}};
    KDocumentQuery query = col.find(queryMap);

    QCOMPARE(query.totalCount(), 2);
    QVERIFY(query.next());
    data["_id"] = id1;
    data["episodeNumber"] = 5;
    QCOMPARE(query.result(), data);
    QVERIFY(query.next());
    data["_id"] = id2;
    data["episodeNumber"] = 6;
    QCOMPARE(query.result(), data);
    QVERIFY(!query.next());
}

void DatabaseTest::testAndQuery()
{
    QVariantMap data;
    data["type"] = "episode";
    data["mimetype"] = "video/mp4";
    data["series"] = "Outlander";
    data["episodeNumber"] = 5;

    KDocumentCollection col = db->collection("testCol");
    QString id = col.insert(data);

    QVariantMap data2 = data;
    data2["series"] = "Batman";
    col.insert(data2);

    QVariantMap queryMap = {{"type", "episode"},
                            {"series", "Outlander"}};
    KDocumentQuery query = col.find(queryMap);
    QCOMPARE(query.totalCount(), 1);
    QVERIFY(query.next());
    data["_id"] = id;
    QCOMPARE(query.result(), data);
    QVERIFY(!query.next());
}

void DatabaseTest::testQueryNull()
{
    QVariantMap data;
    data["type"] = "episode";
    data["mimetype"] = "video/mp4";
    data["series"] = "Outlander";

    KDocumentCollection col = db->collection("testCol");
    col.insert(data);

    QVariantMap data2 = data;
    data2["series"] = "Batman";
    col.insert(data2);

    QVariantMap queryMap;
    KDocumentQuery query = col.find(queryMap);
    QCOMPARE(query.totalCount(), 2);
}

void DatabaseTest::testQueryNullValue()
{
    QVariantMap data;
    data["type"] = "episode";
    data["mimetype"] = "video/mp4";
    data["series"] = "Outlander";

    KDocumentCollection col = db->collection("testCol");
    QString id = col.insert(data);

    QVariantMap data2 = data;
    data2["series"] = "Batman";
    data2.remove("mimetype");
    QString id2 = col.insert(data2);
    data2["_id"] = id2;

    // Will return all items which do not have a mimetype
    QVariantMap queryMap = {{"type", "episode"},
                            {"mimetype", QVariant()}};
    KDocumentQuery query = col.find(queryMap);
    QCOMPARE(query.totalCount(), 1);
    QVERIFY(query.next());
    QCOMPARE(query.result(), data2);
    QVERIFY(!query.next());
}

void DatabaseTest::testQueryRegexp()
{
    KDocumentCollection col = db->collection("testCol");

    QVariantMap data;
    data["type"] = "episode";
    data["mimetype"] = "video/mp4";
    data["series"] = "Outlander";

    QString id = col.insert(data);
    data["_id"] = id;

    QVariantMap data2;
    data2["type"] = "movie";
    data2["series"] = "Batman";
    col.insert(data2);

    QVariantMap queryMap = {{"type", QRegularExpression("ep.*")}};
    KDocumentQuery query = col.find(queryMap);
    QCOMPARE(query.totalCount(), 1);
    QVERIFY(query.next());
    QCOMPARE(query.result(), data);
    QVERIFY(!query.next());

    QVariantMap queryMap2 = {{"type", QRegularExpression("tv")}};
    KDocumentQuery query2 = col.find(queryMap2);
    QCOMPARE(query2.totalCount(), 0);

    QVariantMap queryMap3 = {{"series", QRegularExpression("outlander")}};
    KDocumentQuery query3 = col.find(queryMap3);
    QCOMPARE(query3.totalCount(), 0);

    QRegularExpression exp("outlander", QRegularExpression::CaseInsensitiveOption);
    QVariantMap queryMap4 = {{"series", exp}};
    KDocumentQuery query4 = col.find(queryMap4);
    QCOMPARE(query4.totalCount(), 1);
    QVERIFY(query4.next());
    QCOMPARE(query4.result(), data);
    QVERIFY(!query4.next());
}

void DatabaseTest::testQueryCount()
{
    QVariantMap data;
    data["type"] = "episode";
    data["mimetype"] = "video/mp4";
    data["series"] = "Outlander";

    KDocumentCollection col = db->collection("testCol");
    col.insert(data);

    QVariantMap data2 = data;
    data2["series"] = "Batman";
    col.insert(data2);

    QVariantMap queryMap = {{"type", "episode"}};
    int count = col.count(queryMap);
    QCOMPARE(count, 2);
}

void DatabaseTest::testQueryFindOne()
{
    QVariantMap data;
    data["type"] = "episode";
    data["mimetype"] = "video/mp4";
    data["series"] = "Outlander";

    KDocumentCollection col = db->collection("testCol");
    QString id = col.insert(data);

    QVariantMap data2 = data;
    data2["series"] = "Batman";
    col.insert(data2);
    data["_id"] = id;

    QVariantMap queryMap = {{"series", "Outlander"}};
    QVariantMap output = col.findOne(queryMap);
    QCOMPARE(output, data);
}

void DatabaseTest::testQuerySort()
{
    QVariantMap a = {{"a", 2}};
    QVariantMap b = {{"a", 1}};
    QVariantMap c = {{"a", 3}};

    KDocumentCollection col = db->collection("testCol");
    col.insert(a);
    QString id = col.insert(b);
    col.insert(c);

    b.insert("_id", id);

    QVariantMap order = {{"a", 1}};
    QVariantMap hint = {{"$orderby", order}};
    QVariantMap output = col.findOne(QVariantMap(), hint);
    QCOMPARE(output, b);
}



QTEST_MAIN(DatabaseTest);

#include "databasetest.moc"
