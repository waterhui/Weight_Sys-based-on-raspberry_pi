#-------------------------------------------------
#
# Project created by QtCreator 2016-08-29T17:50:58
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = weight
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    tourtest.cpp \
    keyboard.cpp \
    datahelper.cpp \
    register.cpp \
    member_center.cpp \
    qcustomplot.cpp \
    member_info.cpp \
    modify_user_info.cpp \
    weight_info.cpp \
    detail_weight.cpp \
    admin_center.cpp \
    manage_user_info.cpp \
    comment_info.cpp \
    manage_comment_info.cpp \
    modify_comment.cpp \
    admin_modify_pwd.cpp \
    time_setting.cpp \
    message_box.cpp \
    measuring_weight.cpp \
    hx711.cpp \
    cbo_select.cpp

HEADERS  += widget.h \
    tourtest.h \
    keyboard.h \
    datahelper.h \
    register.h \
    member_center.h \
    qcustomplot.h \
    member_info.h \
    modify_user_info.h \
    weight_info.h \
    detail_weight.h \
    admin_center.h \
    manage_user_info.h \
    comment_info.h \
    manage_comment_info.h \
    modify_comment.h \
    admin_modify_pwd.h \
    time_setting.h \
    message_box.h \
    measuring_weight.h \
    hx711.h \
    cbo_select.h

FORMS    += widget.ui \
    tourtest.ui \
    keyboard.ui \
    register.ui \
    member_center.ui \
    modify_user_info.ui \
    detail_weight.ui \
    admin_center.ui \
    manage_user_info.ui \
    manage_comment_info.ui \
    modify_comment.ui \
    admin_modify_pwd.ui \
    time_setting.ui \
    message_box.ui \
    measuring_weight.ui \
    cbo_select.ui

RESOURCES += \
    imageIcon.qrc \
    font.qrc \
    imagebackground.qrc

CONFIG +=c++11

LIBS      += -lwiringPi
