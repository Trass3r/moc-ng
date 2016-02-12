/****************************************************************************
 *  Copyright (C) 2013-2016 Woboq GmbH
 *  Olivier Goffart <contact at woboq.com>
 *  https://woboq.com/
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QtTest/QtTest>

class tst_Templates : public QObject
{ Q_OBJECT
private slots:
    void templatesMethod_data();
    void templatesMethod();
};

struct MyStruct {};
struct MyStruct2 {};



template<typename T, int I = 5 + 4, bool = (I > 5), class S = void>
class TestTemplate : public QObject {
    Q_OBJECT
public slots:
    void mySlot(const T&) {}
signals:
    void mySignal(const T&);
};

template<typename T>
class TestTemplate2 : public TestTemplate<T>
{
    Q_OBJECT
public slots:
    void sl2() {}
signals:
    void si2();
};

template<typename O, typename T1, void (O::*)(const T1&), void (O::*F2)(const T1&)>
class FunctionTemplateParameter : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString member MEMBER member)
    QString member;
signals:
    void hello();
};

template<template<typename> class Container1, template <typename TT> class , template <typename, int > class Container3>
class TemplateTemplateParameter : public QObject {
    Q_OBJECT
signals:
    void hello();
};

template<typename, typename, typename> struct ReduceKernel;
struct Functor { typedef int result_type; };

template <typename ReducedResultType,
          typename Iterator,
          typename MapFunctor,
          typename ReduceFunctor,
          typename Reducer = ReduceKernel<ReduceFunctor,
                                          ReducedResultType,
                                          typename MapFunctor::result_type> >
class MappedReducedKernel : public QObject
{
    Q_OBJECT
signals:
    void hello(Reducer*);
};

Q_DECLARE_METATYPE(const QMetaObject*);

void tst_Templates::templatesMethod_data()
{
    QTest::addColumn<const QMetaObject *>("metaObject");
    QTest::addColumn<QByteArray>("method");
    QTest::addColumn<bool>("exist");
    typedef QByteArray _;

    QTest::newRow("TestTemplate<QString> mySlot(T)")
        << &TestTemplate<QString>::staticMetaObject << _("mySlot(T)") << true;
    QTest::newRow("TestTemplate<QString> mySlot(QString)")
        << &TestTemplate<QString>::staticMetaObject << _("mySlot(QString)") << false;
    QTest::newRow("TestTemplate<QString> mySlot(MyStruct)")
        << &TestTemplate<QString>::staticMetaObject << _("mySlot(MyStruct)") << false;
    QTest::newRow("TestTemplate<QString> mySignal(T)")
        << &TestTemplate<QString>::staticMetaObject << _("mySignal(T)") << true;
    QTest::newRow("TestTemplate<QString> mySignal(QString)")
        << &TestTemplate<QString>::staticMetaObject << _("mySignal(QString)") << false;
    QTest::newRow("TestTemplate<QString> mySignal(MyStruct)")
        << &TestTemplate<QString>::staticMetaObject << _("mySignal(MyStruct)") << false;

    QTest::newRow("TestTemplate<MyStruct> mySlot(T)")
        << &TestTemplate<MyStruct>::staticMetaObject << _("mySlot(T)") << true;
    QTest::newRow("TestTemplate<MyStruct> mySlot(QString)")
        << &TestTemplate<MyStruct>::staticMetaObject << _("mySlot(QString)") << false;
    QTest::newRow("TestTemplate<MyStruct> mySlot(MyStruct)")
        << &TestTemplate<MyStruct>::staticMetaObject << _("mySlot(MyStruct)") << false;
    QTest::newRow("TestTemplate<MyStruct> mySignal(T)")
        << &TestTemplate<MyStruct>::staticMetaObject << _("mySignal(T)") << true;
    QTest::newRow("TestTemplate<MyStruct> mySignal(QString)")
        << &TestTemplate<MyStruct>::staticMetaObject << _("mySignal(QString)") << false;
    QTest::newRow("TestTemplate<MyStruct> mySignal(MyStruct)")
        << &TestTemplate<MyStruct>::staticMetaObject << _("mySignal(MyStruct)") << false;

    QTest::newRow("TestTemplate2<QString> mySlot(T)")
        << &TestTemplate2<QString>::staticMetaObject << _("mySlot(T)") << true;

    QTest::newRow("FunctionTemplateParameter")
        << &FunctionTemplateParameter<TestTemplate<QString>, QString, &TestTemplate<QString>::mySlot, &TestTemplate<QString>::mySignal>::staticMetaObject
        << _("hello()") << true;

    QTest::newRow("TemplateTemplateParameter")
        << &TemplateTemplateParameter<QList, QVector, QVarLengthArray>::staticMetaObject
        << _("hello()") << true;

    QTest::newRow("MappedReducedKernel")
        << &MappedReducedKernel<Functor, Functor, Functor, Functor>::staticMetaObject
        << _("hello(Reducer*)") << true;

}


void tst_Templates::templatesMethod()
{
    QFETCH(const QMetaObject *, metaObject);
    QFETCH(QByteArray, method);
    QFETCH(bool, exist);

    int index = metaObject->indexOfMethod(method);
    QCOMPARE(index != -1, exist);
}

#include "tst_moc_templates.moc"