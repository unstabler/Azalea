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
        Optional<QString> case1;
        case1.set("Hello, World!");
        
        QVERIFY(case1.get() == "Hello, World!");
    }
    void optionalBoolean()
    {
        Optional<bool> optionalBool;
        optionalBool.set(true);
        
        QVERIFY(optionalBool.get() == true);
    }
};

QTEST_MAIN(TestOptional)
#include "test_optional.moc"