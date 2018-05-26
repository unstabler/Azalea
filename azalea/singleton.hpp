/*
 * 유사-Mozc식 싱글톤 클래스
 *
 * 구현에 Google의 Mozc의 다음 소스 코드를 참고하였습니다
 * https://github.com/google/mozc/blob/master/src/base/singleton.h
 * https://github.com/google/mozc/blob/master/src/base/singleton.cc
 */

#pragma once

#include <functional>
#include <mutex>

#include <QtGlobal>
#include <QScopedPointer>

class SingletonFinalizer
{
    public:
        using FinalizerFunc = std::function<void()>;

        /**
         * @brief Finalizer를 추가합니다
         * @param func Singleton::destroy();
         */
        static void addFinalizer(FinalizerFunc func);

        /**
         * @brief 모든 Singleton 인스턴스를 메모리에서 정리합니다.
         *        애플리케이션 종료 시 호출하여 주십시오.
         */
        static void finalize();
};


/**
 * Mozc식 싱글톤 클래스
 */
template <class T>
class Singleton final
{
    public:
        static T& getInstance()
        {
            std::call_once(initFlag, create);
            Q_ASSERT(instance);
            return *instance;
        }

    private:
        static void create()
        {
            instance = new T;
            SingletonFinalizer::addFinalizer(&Singleton<T>::destroy);
        }

        static void destroy()
        {
            delete instance;
        }

        static std::once_flag initFlag;
        static T* instance;
};

template <class T> std::once_flag Singleton<T>::initFlag;
template <class T> T* Singleton<T>::instance = nullptr;
