#include <QObject>
#include <QtTest/QtTest>

#include "serialization.hpp"
#include "mastodon/v1/entities/mastodon.hpp"
#include "ui/timeline/statusformatter.hpp"


class TestStatusFormatter : public QObject {
    Q_OBJECT
    
    static QJsonObject RESOLVE_EMOJI_TESTCASE01() {
        static const QString json = "{\"id\":\"102279515401665566\",\"created_at\":\"2019-06-16T05:05:50.936Z\",\"in_reply_to_id\":null,\"in_reply_to_account_id\":null,\"sensitive\":false,\"spoiler_text\":\"\",\"visibility\":\"public\",\"language\":\"ko\",\"uri\":\"https://twingyeo.kr/users/cheesekun/statuses/102279515401665566\",\"url\":\"https://twingyeo.kr/@cheesekun/102279515401665566\",\"replies_count\":0,\"reblogs_count\":0,\"favourites_count\":0,\"favourited\":false,\"reblogged\":false,\"muted\":false,\"pinned\":false,\"content\":\"\\u003cp\\u003e테스트 :blobowo:\\u003c/p\\u003e\",\"reblog\":null,\"application\":{\"name\":\"Web\",\"website\":null},\"account\":{\"id\":\"1\",\"username\":\"cheesekun\",\"acct\":\"cheesekun\",\"display_name\":\"치즈군★\",\"locked\":false,\"bot\":false,\"created_at\":\"2017-04-19T03:06:54.557Z\",\"note\":\"\\u003cp\\u003e남자 좋아하는 오타쿠 프로그래머 아저씨. J-POP 여러가지 들어요. \\u003ca href=\\\"https://twingyeo.kr/tags/%EC%B9%98%EC%A6%88%EA%B8%B0%EC%97%AC%EC%96%B4\\\" class=\\\"mention hashtag\\\" rel=\\\"tag\\\"\\u003e#\\u003cspan\\u003e치즈기여어\\u003c/span\\u003e\\u003c/a\\u003e\\u003c/p\\u003e\",\"url\":\"https://twingyeo.kr/@cheesekun\",\"avatar\":\"https://storage.googleapis.com/twingyeo-kr-media/accounts/avatars/000/000/001/original/8NBCS567PAXE.png\",\"avatar_static\":\"https://storage.googleapis.com/twingyeo-kr-media/accounts/avatars/000/000/001/original/8NBCS567PAXE.png\",\"header\":\"https://storage.googleapis.com/twingyeo-kr-media/accounts/headers/000/000/001/original/41e4868f55a1f969.jpg\",\"header_static\":\"https://storage.googleapis.com/twingyeo-kr-media/accounts/headers/000/000/001/original/41e4868f55a1f969.jpg\",\"followers_count\":241,\"following_count\":230,\"statuses_count\":16275,\"emojis\":[],\"fields\":[{\"name\":\"website\",\"value\":\"\\u003ca href=\\\"https://unstabler.pl\\\" rel=\\\"me nofollow noopener\\\" target=\\\"_blank\\\"\\u003e\\u003cspan class=\\\"invisible\\\"\\u003ehttps://\\u003c/span\\u003e\\u003cspan class=\\\"\\\"\\u003eunstabler.pl\\u003c/span\\u003e\\u003cspan class=\\\"invisible\\\"\\u003e\\u003c/span\\u003e\\u003c/a\\u003e\",\"verified_at\":\"2018-11-03T12:34:20.922+00:00\"},{\"name\":\"twitter\",\"value\":\"\\u003ca href=\\\"https://twitter.com/cheese_rulez\\\" rel=\\\"me nofollow noopener\\\" target=\\\"_blank\\\"\\u003e\\u003cspan class=\\\"invisible\\\"\\u003ehttps://\\u003c/span\\u003e\\u003cspan class=\\\"\\\"\\u003etwitter.com/cheese_rulez\\u003c/span\\u003e\\u003cspan class=\\\"invisible\\\"\\u003e\\u003c/span\\u003e\\u003c/a\\u003e\",\"verified_at\":null},{\"name\":\"icon\",\"value\":\"Project DIVA: Future Tone (PS4)\",\"verified_at\":null}]},\"media_attachments\":[],\"mentions\":[],\"tags\":[],\"emojis\":[{\"shortcode\":\"blobowo\",\"url\":\"https://storage.googleapis.com/twingyeo-kr-media/custom_emojis/images/000/001/363/original/f3426adea81a45f9.png\",\"static_url\":\"https://storage.googleapis.com/twingyeo-kr-media/custom_emojis/images/000/001/363/static/f3426adea81a45f9.png\",\"visible_in_picker\":true}],\"card\":null,\"poll\":null}";
        auto document = QJsonDocument::fromJson(json.toUtf8());
        return document.object();
    }
    
private slots:
    void testResolveEmoji() {
        StatusFormatter formatter;
        auto *status = deserialization::OBJECT<v1::Status>(RESOLVE_EMOJI_TESTCASE01());
        auto resolved = formatter.resolveEmoji(status, "blobowo");
        
        QVERIFY(!resolved.isNull());
        QVERIFY(resolved->shortcode == "blobowo");
    }
    void testReoslveEmoji2() {
        StatusFormatter formatter;
        auto *status = deserialization::OBJECT<v1::Status>(RESOLVE_EMOJI_TESTCASE01());
        auto resolved = formatter.resolveEmoji(status, "blobuwu");
        
        QVERIFY(resolved.isNull());
    }
};

QTEST_MAIN(TestStatusFormatter)
#include "test_statusformatter.moc"