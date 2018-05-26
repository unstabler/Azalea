#pragma once

#include <QObject>

class ConfigManager : public QObject
{
    Q_OBJECT
public:
    explicit ConfigManager(QObject *parent = nullptr);

    /** 데이터 디렉토리를 반환합니다 */
    QString dataDirectory() const;

protected:
    /** 데이터 디렉토리가 파일 시스템 상에 존재하는지 확인합니다 */
    bool isDataDirectoryExists() const;
    /** 데이터 디렉토리를 생성합니다 */
    void tryCreateDataDirectory() const;

signals:

public slots:

};
