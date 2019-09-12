set(AZALEA_SRC
    ${AZALEA_SRC}
    ${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/postarea.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/settingswindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/postarea/replyindicator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/timeline/statusadapterbase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/timeline/statusadapter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/timeline/notificationstatusadapter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/timeline/timelinemodel.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/timeline/statusformatter.cpp
    PARENT_SCOPE
)