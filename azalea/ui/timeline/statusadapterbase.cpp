//
// Created by cheesekun on 19/06/16.
//


#include <QtConcurrent/QtConcurrent>
#include "statusadapterbase.hpp"


QString StatusAdapterBase::formatAuthor(QSharedPointer<v1::Account> account)
{
    return QString("%1 (@%2)").arg(
            account->displayName,
            account->acct
    );
}

QStringList StatusAdapterBase::filterAttachmentUrls(QSharedPointer<v1::Status> status, const QString type)
{
    QStringList attachmentUrls;
    if (status == nullptr || status->mediaAttachements.length() == 0) {
        return attachmentUrls;
    }
    
    QList<QSharedPointer<v1::Attachment>> filteredAttachments;
    std::remove_copy_if(
        status->mediaAttachements.begin(),
        status->mediaAttachements.end(),
        std::back_inserter(filteredAttachments),
        [type] (const QSharedPointer<v1::Attachment> &attachment) {
            return attachment->type != type;
        }
    );
    
    std::transform(
        filteredAttachments.begin(),
        filteredAttachments.end(),
        std::back_inserter(attachmentUrls),
        [] (const QSharedPointer<v1::Attachment> &attachment) {
            return attachment->url;
        }
    );
    
    return attachmentUrls;
}
