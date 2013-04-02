#include <QAbstractFileEngineHandler>
#include <QFSFileEngine>

#include <Inventor/SoDB.h>
#include <Inventor/SoInteraction.h>
#include <Inventor/nodekits/SoNodeKit.h>

#include <gtest/gtest.h>

class LocalResourcesHandler : public QAbstractFileEngineHandler
{
public:
    QAbstractFileEngine *create(const QString &fileName) const
    {
        if (fileName.toLower().startsWith(":"))
        {
            QString newFileName = fileName;
            newFileName.replace(":", getenv("OIVHOME"));
            return new QFSFileEngine(newFileName);
        }
        return 0;
    }
};

int main(int argc, char **argv)
{
    if (!getenv("OIVHOME") || !QDir(getenv("OIVHOME") + QString("/ivtest")).exists()) {
        printf("ERROR: OIVHOME is not set or the directory doesn't exist.");
        return 1;
    }

    testing::InitGoogleTest(&argc, argv);

    // init Inventor
    SoDB::init();
    SoNodeKit::init();
    SoInteraction::init();

    LocalResourcesHandler engine;

    int res = RUN_ALL_TESTS();

    SoInteraction::finish();
    SoNodeKit::finish();
    SoDB::finish();

    return res;
}
