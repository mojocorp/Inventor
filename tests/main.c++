#include <SoDebug.h>
#include <Inventor/SoDB.h>
#include <Inventor/SoInteraction.h>
#include <Inventor/nodekits/SoNodeKit.h>

#include <gtest/gtest.h>

int
main(int argc, char **argv) {
    if (!SoDebug::GetEnv("OIVHOME")) {
        printf("\n\nERROR: OIVHOME is not set or the directory doesn't "
               "exist.\n\n");
        return 1;
    }

    testing::InitGoogleTest(&argc, argv);

    // init Inventor
    SoDB::init();
    SoNodeKit::init();
    SoInteraction::init();

    int res = RUN_ALL_TESTS();

    SoInteraction::finish();
    SoNodeKit::finish();
    SoDB::finish();

    return res;
}
