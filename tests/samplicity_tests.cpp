#include <QString>
#include <QtTest>

class SamplicityTests : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase()
    {
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
    }
};

QTEST_APPLESS_MAIN(SamplicityTests)

#include "samplicity_tests.moc"
