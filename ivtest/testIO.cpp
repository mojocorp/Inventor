#include <QDir>

#include <gtest/gtest.h>

#include <Inventor/nodes/SoSeparator.h>
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
        QString tmp = QDir::tempPath();
        asciiModel = tmp + "/ascii.iv";
        binaryModel = tmp + "/binary.iv";

        SoInput in;
        in.setBuffer((void*)scenegraph, strlen(scenegraph));

        graph = SoDB::readAll(&in);
        ASSERT_TRUE(graph);
    }

    virtual void TearDown() {

    }

    SbRefPtr<SoSeparator> graph;
    QString asciiModel;
    QString binaryModel;
};

TEST_F(testIO, ascii) {
    SoOutput out;
    out.setBinary(FALSE);
    ASSERT_TRUE(out.openFile(qPrintable(asciiModel)));

    SoWriteAction wa(&out);
    wa.apply(graph.data());

    out.closeFile();

    SoInput in;
    ASSERT_TRUE(in.openFile(qPrintable(asciiModel)));
    EXPECT_FALSE(in.isBinary());

    SbRefPtr<SoSeparator> root = SoDB::readAll(&in);
    ASSERT_TRUE(root);

    SoSearchAction sa;
    sa.setType(SoNode::getClassTypeId());
    sa.setInterest(SoSearchAction::ALL);
    sa.setSearchingAll(TRUE);
    sa.apply(root.data());
    SoPathList & pl = sa.getPaths();

    EXPECT_EQ(2, pl.getLength());
}

TEST_F(testIO, binary) {
    SoOutput out;
    out.setBinary(TRUE);
    ASSERT_TRUE(out.openFile(qPrintable(binaryModel)));

    SoWriteAction wa(&out);
    wa.apply(graph.data());

    out.closeFile();

    SoInput in;
    ASSERT_TRUE(in.openFile(qPrintable(binaryModel)));
    EXPECT_TRUE(in.isBinary());

    SbRefPtr<SoSeparator> root = SoDB::readAll(&in);
    ASSERT_TRUE(root);

    SoSearchAction sa;
    sa.setType(SoNode::getClassTypeId());
    sa.setInterest(SoSearchAction::ALL);
    sa.setSearchingAll(TRUE);
    sa.apply(root.data());
    SoPathList & pl = sa.getPaths();

    EXPECT_EQ(2, pl.getLength());
}
