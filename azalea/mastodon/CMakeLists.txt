set(AZALEA_SRC
    ${AZALEA_SRC}
    ${CMAKE_CURRENT_SOURCE_DIR}/apibase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/apicontext.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mastodonapi.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/oauthapi.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/v1/appsapi.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/v1/timelinesapi.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/v1/entities/oauth2.serialization.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/v1/entities/mastodon.serialization.cpp
    
    PARENT_SCOPE
)