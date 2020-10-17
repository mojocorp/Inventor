#include <gtest/gtest.h>

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoText2.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/SbRefPtr.h>

static const char *scenegraph = "#Inventor V2.1 ascii\n"
                                "\n"
                                "Separator {\n"
                                "  Text2 {\n"
                                "    string	\"The\"\n"
                                "    justification	RIGHT\n"
                                "  }\n"
                                "}\n";

class testIO : public ::testing::Test {
  protected:
    virtual void SetUp() {
        SbString tmp = "/tmp";
        asciiModel = tmp + "/ascii.iv";
        binaryModel = tmp + "/binary.iv";
        utf8Model = tmp + "/utf8.iv";

        SoInput in;
        in.setBuffer((void *)scenegraph, strlen(scenegraph));

        graph = SoDB::readAll(&in);
        ASSERT_TRUE(graph);
    }

    virtual void TearDown() {}

    SbRefPtr<SoSeparator> graph;
    SbString              asciiModel;
    SbString              binaryModel;
    SbString              utf8Model;
};

TEST_F(testIO, ascii) {
    SoOutput out;
    out.setFormat(SoOutput::ASCII);
    ASSERT_TRUE(out.openFile(asciiModel));

    SoWriteAction wa(&out);
    wa.apply(graph.get());

    out.closeFile();

    SoInput in;
    ASSERT_TRUE(in.openFile(asciiModel));
    EXPECT_FALSE(in.isBinary());

    SbRefPtr<SoSeparator> root = SoDB::readAll(&in);
    ASSERT_TRUE(root);

    SoSearchAction sa;
    sa.setType(SoNode::getClassTypeId());
    sa.setInterest(SoSearchAction::ALL);
    sa.setSearchingAll(TRUE);
    sa.apply(root.get());
    SoPathList &pl = sa.getPaths();

    EXPECT_EQ(2, pl.getLength());
}

TEST_F(testIO, binary) {
    SoOutput out;
    out.setFormat(SoOutput::BINARY);
    ASSERT_TRUE(out.openFile(binaryModel));

    SoWriteAction wa(&out);
    wa.apply(graph.get());

    out.closeFile();

    SoInput in;
    ASSERT_TRUE(in.openFile(binaryModel));
    EXPECT_TRUE(in.isBinary());

    SbRefPtr<SoSeparator> root = SoDB::readAll(&in);
    ASSERT_TRUE(root);

    SoSearchAction sa;
    sa.setType(SoNode::getClassTypeId());
    sa.setInterest(SoSearchAction::ALL);
    sa.setSearchingAll(TRUE);
    sa.apply(root.get());
    SoPathList &pl = sa.getPaths();

    EXPECT_EQ(2, pl.getLength());
}

TEST_F(testIO, utf8_text) {
    {
        SoSearchAction sa;
        sa.setType(SoText2::getClassTypeId());
        sa.setInterest(SoSearchAction::FIRST);
        sa.apply(graph.get());
        EXPECT_TRUE(sa.getPath()->getTail());

        SoText2 *text = dynamic_cast<SoText2 *>(sa.getPath()->getTail());
        ASSERT_TRUE(text);
        text->string = SbString::fromUtf8(
            "Kl\303\274ft skr\303\244ms inf\303\266r p\303\245 "
            "f\303\251d\303\251ral \303\251lectoral gro\303\237e");
    }

    SoOutput out;
    out.setFormat(SoOutput::UTF8);
    ASSERT_TRUE(out.openFile(utf8Model));

    SoWriteAction wa(&out);
    wa.apply(graph.get());

    out.closeFile();

    SoInput in;
    ASSERT_TRUE(in.openFile(utf8Model));
    EXPECT_FALSE(in.isBinary());

    SbRefPtr<SoSeparator> root = SoDB::readAll(&in);
    ASSERT_TRUE(root);

    {
        SoSearchAction sa;
        sa.setType(SoNode::getClassTypeId());
        sa.setInterest(SoSearchAction::ALL);
        sa.setSearchingAll(TRUE);
        sa.apply(root.get());
        SoPathList &pl = sa.getPaths();

        EXPECT_EQ(2, pl.getLength());
    }

    {
        SoSearchAction sa;
        sa.setType(SoText2::getClassTypeId());
        sa.setInterest(SoSearchAction::FIRST);
        sa.apply(graph.get());
        EXPECT_TRUE(sa.getPath()->getTail());

        SoText2 *text = dynamic_cast<SoText2 *>(sa.getPath()->getTail());
        ASSERT_TRUE(text);
        EXPECT_TRUE(text->string[0] ==
                    SbString::fromUtf8(
                        "Kl\303\274ft skr\303\244ms inf\303\266r p\303\245 "
                        "f\303\251d\303\251ral \303\251lectoral gro\303\237e"));
    }
}

TEST_F(testIO, utf8_binary) {
    {
        SoSearchAction sa;
        sa.setType(SoText2::getClassTypeId());
        sa.setInterest(SoSearchAction::FIRST);
        sa.apply(graph.get());
        EXPECT_TRUE(sa.getPath()->getTail());

        SoText2 *text = dynamic_cast<SoText2 *>(sa.getPath()->getTail());
        ASSERT_TRUE(text);
        text->string = SbString::fromUtf8(
            "Kl\303\274ft skr\303\244ms inf\303\266r p\303\245 "
            "f\303\251d\303\251ral \303\251lectoral gro\303\237e");
    }

    SoOutput out;
    out.setFormat(SoOutput::BINARY);
    ASSERT_TRUE(out.openFile(utf8Model));

    SoWriteAction wa(&out);
    wa.apply(graph.get());

    out.closeFile();

    SoInput in;
    ASSERT_TRUE(in.openFile(utf8Model));
    EXPECT_TRUE(in.isBinary());

    SbRefPtr<SoSeparator> root = SoDB::readAll(&in);
    ASSERT_TRUE(root);

    {
        SoSearchAction sa;
        sa.setType(SoNode::getClassTypeId());
        sa.setInterest(SoSearchAction::ALL);
        sa.setSearchingAll(TRUE);
        sa.apply(root.get());
        SoPathList &pl = sa.getPaths();

        EXPECT_EQ(2, pl.getLength());
    }

    {
        SoSearchAction sa;
        sa.setType(SoText2::getClassTypeId());
        sa.setInterest(SoSearchAction::FIRST);
        sa.apply(graph.get());
        EXPECT_TRUE(sa.getPath()->getTail());

        SoText2 *text = dynamic_cast<SoText2 *>(sa.getPath()->getTail());
        ASSERT_TRUE(text);
        EXPECT_TRUE(text->string[0] ==
                    SbString::fromUtf8(
                        "Kl\303\274ft skr\303\244ms inf\303\266r p\303\245 "
                        "f\303\251d\303\251ral \303\251lectoral gro\303\237e"));
    }
}
