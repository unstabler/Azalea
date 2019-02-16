#pragma once

#include <QObject>
#include <QSharedPointer>
#include "apicontext.hpp"

#include "oauthapi.hpp"
#include "v1.hpp"

class MastodonAPI : public QObject
{
        Q_OBJECT

    public:
        explicit MastodonAPI(APIContext *context);

        APIContext *context() const;

        OAuthAPI *oauth() const;
        v1::AppsAPI *apps() const;
        v1::TimelinesAPI *timelines() const;

    private:
        APIContext  *_context;

        OAuthAPI *_oauth;
        v1::AppsAPI *_apps;
        v1::TimelinesAPI *_timelines;

};

