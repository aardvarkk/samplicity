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

    void tagSample()
    {
        // Create a file to add
        QFile fake("fake.wav");
        db->addFile(fake);
        auto sample = db->getSample(fake);
        QVERIFY(sample.id > 0);
        QVERIFY(sample.filename == "fake.wav");
        QVERIFY(sample.name == "fake.wav");

        auto tag = db->addTag("awesome stuff");
        QVERIFY(db->addSampleTag(sample, tag));
        QVERIFY(db->getSampleTags(sample).first().name == "awesome stuff");

        auto secondTag = db->addTag("seriously awesome stuff", tag.id);
        QVERIFY(db->addSampleTag(sample, secondTag));
        QVERIFY(db->getSampleTags(sample).length() == 2);
    }

    void removeSampleTag()
    {
        // Should remove the tag from the given sample
        QFile file("tagged.wav");
        db->addFile(file);
        auto sample = db->getSample(file);
        auto tag = db->addTag("delete me");
        db->addSampleTag(sample, tag);
        QVERIFY(db->getSampleTags(sample).length() == 1);
        QVERIFY(db->removeSampleTag(sample, tag));
        QVERIFY(db->getSampleTags(sample).length() == 0);
    }

    void removeTag()
    {
        QFile f1("s1.wav");
        QFile f2("s2.wav");
        db->addFile(f1);
        db->addFile(f2);
        auto s1 = db->getSample(f1);
        auto s2 = db->getSample(f2);

        auto t0 = db->addTag("granny");
        auto t1 = db->addTag("mommy", t0.id);
        auto t2 = db->addTag("baby", t1.id);

        QVERIFY(db->getTagChildren(t0).length() == 1);
        QVERIFY(db->getTagDescendants(t0).length() == 2);
        QVERIFY(db->getTagChildren(t1).length() == 1);

        db->addSampleTag(s1, t1);
        db->addSampleTag(s2, t2);

        // Should remove the tag, all of its children, and untag all samples
        auto pre = db->getTags().length();
        db->removeTag(t1);
        QVERIFY(db->getSampleTags(s1).length() == 0);
        QVERIFY(db->getSampleTags(s2).length() == 0);
        QVERIFY(db->getTags().length() == pre - 2);
        QVERIFY(db->getTagChildren(t0).length() == 0);
        QVERIFY(db->getTagDescendants(t0).length() == 0);
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