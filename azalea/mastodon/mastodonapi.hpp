#pragma once

#include <QObject>
#include "apicontext.hpp"

class MastodonAPI : public QObject
{
        Q_OBJECT

    public:
        explicit MastodonAPI(APIContext *context);
        APIContext* context() const;

    private:
        APIContext* _context;

};

