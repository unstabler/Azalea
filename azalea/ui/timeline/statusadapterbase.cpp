//
// Created by cheesekun on 19/06/16.
//

#include "statusadapterbase.hpp"


QString StatusAdapterBase::formatAuthor(QSharedPointer<v1::Account> account)
{
    return QString("%1 (@%2)").arg(
            account->displayName,
            account->acct
    );
}
