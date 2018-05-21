#pragma once

#include <QObject>
#include <QSharedPointer>
#include "apicontext.hpp"

#include "v1.hpp"

class MastodonAPI : public QObject
{
        Q_OBJECT

    public:
        explicit MastodonAPI(APIContext *context);

        APIContext*  context() const;
        v1::AppsAPI* apps() const;

    private:
        APIContext  *_context;
        v1::AppsAPI *_apps;

};

