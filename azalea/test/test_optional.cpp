//
// Created by cheesekun on 19/05/19.
//

#include <QObject>
#include <QtTest/QtTest>
#include "../optional.hpp"

class TestOptional : public QObject {
    Q_OBJECT
    
private slots:
    void optionalString()
    {
        Optional<QString> case1, case2;
        case1.set(QString("Hello, World!"));
        QVERIFY(case1.get() == "Hello, World!");
        QVERIFY(!case2.isPresent());
    }
    
    void optionalBoolean()
    {
        Optional<bool> case1, case2, case3;
        case1.set(false);
        case2.set(true);
        
        
        QVERIFY(case1.get() == false);
        QVERIFY(case2.get() == true);
        QVERIFY(!case3.isPresent());
    }
    
};

QTEST_MAIN(TestOptional)
#include "test_optional.moc"