#include "gtest/gtest.h"

#include <Inventor/SbString.h>

#include <QString>

TEST(SbString, Empty) {

    SbString sbstring;

    EXPECT_TRUE(sbstring.isEmpty());

    sbstring= SbString("String");

    EXPECT_FALSE(sbstring.isEmpty());

    sbstring.makeEmpty();

    EXPECT_TRUE(sbstring.isEmpty());
}

TEST(SbString, Length) {

    QString qstring;
    SbString sbstring;

    EXPECT_EQ(qstring.isEmpty(), sbstring.isEmpty());

    qstring = QString("String");
    sbstring= SbString("String");

    EXPECT_EQ(qstring.length(), sbstring.getLength());

    qstring += QString(" ") + QString("test");
    sbstring += SbString(" ") + SbString("test");

    EXPECT_EQ(qstring.length(), sbstring.getLength());
}

TEST(SbString, Unicode) {

    QString German = QString::fromUtf8("Ä ä Ü ü ß");
    QString Russian = QString::fromUtf8("Я Б Г Д Ж Й");
    QString Polish = QString::fromUtf8("Ł Ą Ż Ę Ć Ń Ś Ź");
    QString Japanese = QString::fromUtf8("ｱｲｳｴｵｶｷｸｹｺｻｼｽｾｿﾀﾁﾂﾃ");

    printf("utf8: %s\n", Japanese.toUtf8().constData());
}
