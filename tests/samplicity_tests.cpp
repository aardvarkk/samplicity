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

    void myFirstTest()
    {
        QVERIFY(1 == 1);
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
