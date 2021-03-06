#include <QString>
#include <QtTest>

#include "../database.h"

class SamplicityTests : public QObject
{
    Q_OBJECT

private slots:

    void makeFakeFile(QString const& name)
    {
        QFile fake(name);
        fake.open(QFile::ReadWrite);
        fake.write(name.toLocal8Bit());
        fake.close();
    }

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

        QVERIFY(db->getTagChildren(0).length() == 1);
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
        auto children = db->getTagChildren(parent.id);
        QVERIFY(children.length() > 0);
        auto child = children.first();
        QVERIFY(db->reparentTag(child, 0));
        QVERIFY(child.parent_id == 0);
        QVERIFY(db->reparentTag(parent, child.id));
        QVERIFY(parent.parent_id == child.id);

        // Can't reparent to self
        auto tag = db->addTag("hipster");
        QVERIFY(!db->reparentTag(tag, tag.id));
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
        makeFakeFile("fake.wav");
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
        makeFakeFile("tagged.wav");
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
        makeFakeFile("s1.wav");
        QFile f1("s1.wav");
        makeFakeFile("s2.wav");
        QFile f2("s2.wav");

        db->addFile(f1);
        db->addFile(f2);
        auto s1 = db->getSample(f1);
        auto s2 = db->getSample(f2);

        auto t0 = db->addTag("granny");
        auto t1 = db->addTag("mommy", t0.id);
        auto t2 = db->addTag("baby", t1.id);

        QVERIFY(db->getTagChildren(t0.id).length() == 1);
        QVERIFY(db->getTagDescendants(t0).length() == 2);
        QVERIFY(db->getTagChildren(t1.id).length() == 1);

        db->addSampleTag(s1, t1);
        db->addSampleTag(s2, t2);

        // Should remove the tag, all of its children, and untag all samples
        auto pre = db->getTags().length();
        db->removeTag(t1);
        QVERIFY(db->getSampleTags(s1).length() == 0);
        QVERIFY(db->getSampleTags(s2).length() == 0);
        QVERIFY(db->getTags().length() == pre - 2);
        QVERIFY(db->getTagChildren(t0.id).length() == 0);
        QVERIFY(db->getTagDescendants(t0).length() == 0);
    }

    void tagChild()
    {
        auto daddy = db->addTag("daddy");
        auto k1 = db->addTag("kiddy1", daddy.id);
        auto k2 = db->addTag("kiddy2", daddy.id);
        QVERIFY(db->getTagChild(daddy, 0).id == k1.id);
        QVERIFY(db->getTagChild(daddy, 1).id == k2.id);

        QVERIFY(db->getTagChildIndex(k1) == 0);
        QVERIFY(db->getTagChildIndex(k2) == 1);
    }

    void addRating()
    {
        QVariant rating;

        // Should remove the tag from the given sample
        makeFakeFile("toRate.wav");
        QFile file("toRate.wav");

        QVERIFY(db->addFile(file));
        auto sample = db->getSample(file);
        QVERIFY(db->getRating(sample, rating));
        QVERIFY(rating.isNull());
        QVERIFY(db->addRating(sample, 55));
        QVERIFY(db->getRating(sample, rating));
        QVERIFY(rating == 55);
        QVERIFY(db->addRating(sample, QVariant(QVariant::Int)));
        QVERIFY(db->getRating(sample, rating));
        QVERIFY(rating.isNull());
        QVERIFY(db->addRating(sample, 105));
        QVERIFY(db->getRating(sample, rating));
        QVERIFY(rating.toInt() == 100);
        QVERIFY(db->addRating(sample, -10));
        QVERIFY(db->getRating(sample, rating));
        QVERIFY(rating.toInt() == 0);
    }

    void moveSample()
    {
        QDir dir;

        // Clean up from the last test if we failed
        if (QFile::exists("subfolder/newname.wav")) {
            QVERIFY(dir.remove("subfolder/newname.wav"));
        }

        makeFakeFile("original.wav");
        QFile file("original.wav");

        QVERIFY(db->addFile(file));
        auto sample = db->getSample(file);
        QVERIFY(sample.name == "original.wav");
        QVERIFY(sample.filename == "original.wav");
        QVERIFY(sample.fullPath() == dir.absolutePath() + "/original.wav");

        QVERIFY(dir.mkpath("subfolder"));
        file.close();

        QVERIFY(dir.rename("original.wav", "subfolder/newname.wav"));

        QFile newname("subfolder/newname.wav");
        auto pre = db->getFilteredSamples(QList<QDir>(), QList<Tag>());

        QVERIFY(db->addFile(newname));
        sample = db->getSample(newname);

        // Sample will keep its original name, but be in a new location
        QVERIFY(sample.name == "original.wav");
        QVERIFY(sample.filename == "newname.wav");
        QVERIFY(sample.fullPath() == dir.absolutePath() + "/subfolder/newname.wav");

        auto pst = db->getFilteredSamples(QList<QDir>(), QList<Tag>());
        QVERIFY(pst.count() == pre.count());

        newname.close();
        QVERIFY(dir.remove("subfolder/newname.wav"));
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
