//
// Created by cheesekun on 19/06/16.
//

#include "statusformatter.hpp"

StatusFormatter::StatusFormatter()
{

}

QString StatusFormatter::toRichText(v1::Status *status)
{
    QString content = status->content;
    for (auto emoji : status->emojis) {
        // 진짜 꼭 이래야 하나
        const QString html = "<img src=\"%1\" width=\"20\" height=\"20\">";
        content.replace(":" + emoji->shortcode + ":", html.arg(emoji->staticUrl.toString()));
    }
    
    return content;
}

// 이거 필요 없어진 거 아냐..?
QSharedPointer<v1::Emoji> StatusFormatter::resolveEmoji(v1::Status *status, const QString shortcode)
{
    auto iterator = std::find_if(
            status->emojis.begin(),
            status->emojis.end(),
            [shortcode] (auto emoji) {
                return shortcode == emoji->shortcode;
            }
    );
    
    if (iterator != status->emojis.end()) {
        return *iterator;
    } else {
        return nullptr;
    }
}
