#include "gtest/gtest.h"

#include <Inventor/SbString.h>

#include <QString>
#include <math.h>

TEST(SbString, Constructors) {
    SbString string1("an ascii string");

    EXPECT_TRUE(string1 == "an ascii string");

    SbString string2("an ascii string", 3, 7);

    EXPECT_TRUE(string2 == "ascii");

    SbString string3(123456789);

    EXPECT_TRUE(string3 == "123456789");
}

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

TEST(SbString, Find) {
    SbString str ("There are two needles in this haystack with needles.");
    SbString str2 ("needle");

    EXPECT_EQ(14, str.find(str2));
    EXPECT_EQ(44, str.find("needles",15));
    EXPECT_EQ(-1, str.find("There", 14));
}

TEST(SbString, Rfind) {
    SbString str ("There are two needles in this haystack with needles.");
    SbString str2 ("needle");

    EXPECT_EQ(44, str.rfind(str2));
    EXPECT_EQ(14, str.rfind("needles",43));
    EXPECT_EQ(-1, str.rfind("haystack", 14));
}

TEST(SbString, Operators) {

    SbString sbstring;

    sbstring = NULL;

    EXPECT_TRUE(sbstring.isEmpty());

    SbString stringA = "an ascii string";
    SbString stringB = "an ascii string";

    EXPECT_TRUE(stringA == stringB);
    EXPECT_TRUE(stringA == "an ascii string");

    stringB.makeEmpty();
    stringB += stringA;

    EXPECT_TRUE(stringA == stringB);

    stringB.makeEmpty();
    stringB += "an ascii string";

    EXPECT_TRUE(stringA == stringB);
}

TEST(SbString, UTF8) {

    //Swedish "Kluft skrams infor pa federal electoral groe"
    SbString string1 = SbString::fromUtf8("Kl\303\274ft skr\303\244ms inf\303\266r p\303\245 f\303\251d\303\251ral \303\251lectoral gro\303\237e");
    SbString string2 = SbString::fromUtf8("Kl\303\274ft skr\303\244ms inf\303\266r ");
    SbString string3 = SbString::fromUtf8("p\303\245 f\303\251d\303\251ral \303\251lectoral gro\303\237e");

    EXPECT_EQ(45, string1.getLength());
    EXPECT_EQ(19, string2.getLength());
    EXPECT_EQ(26, string3.getLength());

    SbString string4 = string2 + string3;
    EXPECT_EQ(45, string4.getLength());

    EXPECT_TRUE(string1 == string4);
}


int Dec2Oct(int n)
{
    int i=0,r,b=0;
    while(n!=0)
    {
        r=n%8;
        b=b+pow(10.0f,i)*r;
        n=n/8;
        i++;
    }
    return b;
}

void printstring(const SbString & str) {
    const unsigned char *data = (unsigned char*)str.getString();
    for (size_t i=0; i<strlen(str.getString()); i++) {
        if (data[i] < 0x80) {
            printf("%c", data[i]);
        } else {
            printf("\\%d", Dec2Oct(data[i]));
        }
    }
    printf("\n");
}

TEST(SbString, WideChar) {
    SbString string1 = SbString::fromWideChar(L"Hello World");
    SbString string2 = SbString::fromWideChar(L"Hello ");
    SbString string3 = SbString::fromWideChar(L"World");

    EXPECT_EQ(11, string1.getLength());
    EXPECT_EQ(6, string2.getLength());
    EXPECT_EQ(5, string3.getLength());

    SbString string4 = string2 + string3;
    EXPECT_EQ(11, string4.getLength());

    EXPECT_TRUE(string1 == string4);

    std::wstring stdstring = L"Hello World";
    EXPECT_EQ(string1.getLength(), stdstring.length());
    EXPECT_TRUE(stdstring == string1.toStdWString());

    SbString string5 = SbString::fromUtf8("Kl\303\274ft skr\303\244ms inf\303\266r p\303\245 f\303\251d\303\251ral \303\251lectoral gro\303\237e");
    stdstring = string5.toStdWString();
    SbString string6 = SbString::fromWideChar(stdstring.data());
    EXPECT_TRUE(string5 == string6);

    //std::wcout << stdstring << std::endl;

    SbString string7 = SbString::fromUtf8("Hello World");
    EXPECT_TRUE(string1 == string7);

    // Japaneese: the quick brown fox jumped over the lazy dog
    SbString string8 = SbString::fromUtf8("\351\200\237\343\201\204\350\214\266\350\211\262\343\201\256\343\202\255\343\203\204\343\203\215\343\201\257\346\200\240\346\203\260\343\201\252\347\212\254\345\242\227\344\273\245\344\270\212");
    EXPECT_EQ(16, string8.getLength());
    stdstring = string8.toStdWString();
    EXPECT_EQ(16, stdstring.length());
}
