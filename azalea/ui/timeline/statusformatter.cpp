//
// Created by cheesekun on 19/06/16.
//

#include "statusformatter.hpp"

StatusFormatter::StatusFormatter()
{

}

QString StatusFormatter::toRichText(v1::Status *status)
{
    return "";
}

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
