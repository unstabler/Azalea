#include <sstream>

#include <QtTest/QtTest>
#include <QObject>

#include <cereal/archives/json.hpp>

#include "../azalea/serialization.hpp"
#include "../azalea/mastodon/v1/entities/application.hpp"

class TestDeserialization : public QObject
{
        Q_OBJECT
    private slots:
        void testDeserialization()
        {
            auto rawJSON = ("{"
            "    \"id\": \"TEST_ID\","
            "    \"client_id\": \"TEST_CLIENT_ID\","
            "    \"client_secret\": \"TEST_SECRET_KEY\""
            "}");
            std::stringstream stream(rawJSON);

            Application application;
            cereal::JSONInputArchive archive(stream);
            serialize(archive, application);

            QCOMPARE(application.id, "TEST_ID");
            QCOMPARE(application.clientId, "TEST_CLIENT_ID");
            QCOMPARE(application.clientSecret, "TEST_SECRET_KEY");
        }
};

QTEST_MAIN(TestDeserialization)
#include "test_deserialize.moc"
