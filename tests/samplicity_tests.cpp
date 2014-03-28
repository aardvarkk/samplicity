#include <QString>
#include <QtTest>

#include "../database.h"

class SamplicityTests : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase()
    {
        QDir().remove("test.db");
        db = new Database("test.db");
    }

    void addTag()
    {
        auto pre = db->getTags().length();
        db->addTag("chiptune");
        auto pst = db->getTags().length();
        QVERIFY(pst == pre + 1);

        auto tag = db->getTag("chiptune", 0);
        QCOMPARE(tag.name, QString("chiptune"));
        QVERIFY(tag.parent_id == 0);
    }

    void addChildTag()
    {
        auto parent = db->getTag("chiptune", 0);
        QVERIFY(parent.id != 0);

        auto pre = db->getTags().length();
        db->addTag("nuchiptune", parent.id);
        auto pst = db->getTags().length();
        QVERIFY(pst == pre + 1);

        auto tag = db->getTag("nuchiptune", db->getTag("chiptune", 0).id);
        QVERIFY(tag.parent_id == db->getTag("chiptune", 0).id);
        QCOMPARE(tag.name, QString("nuchiptune"));
    }

    void renameTag()
    {
        auto tag = db->getTag(1);
        QVERIFY(tag.name != QString("newName"));
        db->renameTag(tag, "newName");
        QVERIFY(tag.name == QString("newName"));
    }

    void reparentTag()
    {
        // Switch role of child and parent
        auto parent = db->getTag(1);
        auto children = db->getTagChildren(parent);
        QVERIFY(children.length() > 0);
        auto child = children.first();
        QVERIFY(db->reparentTag(child, 0));
        QVERIFY(child.parent_id == 0);
        QVERIFY(db->reparentTag(parent, child.id));
        QVERIFY(parent.parent_id == child.id);
    }

    void invalidReparentTag()
    {
        auto parent = db->addTag("parent");
        auto child = db->addTag("child", parent.id);
        QVERIFY(!db->reparentTag(parent, child.id));
    }

    void mySecondTest()
    {
        QVERIFY(1 != 2);
    }

    void cleanupTestCase()
    {
        delete db;
    }

private:
    Database* db;
};

QTEST_APPLESS_MAIN(SamplicityTests)

#include "samplicity_tests.moc"
