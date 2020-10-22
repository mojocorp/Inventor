#include <limits.h>
#include "gtest/gtest.h"

#include <Inventor/SoType.h>
#include <Inventor/SoInteraction.h>
#include <SoUnknownNode.h>
#include <SoUnknownEngine.h>
#include <Inventor/misc/SoBase.h>
#include <Inventor/SoLists.h>

class testCreation : public ::testing::Test {
  protected:
    virtual void SetUp() {
        // init Inventor
        SoInteraction::init();
    }

    virtual void TearDown() { SoInteraction::finish(); }
};

TEST_F(testCreation, All) {
    SoTypeList tl;
    SoType::getAllDerivedFrom(SoType::fromName("SoBase"), tl);
    for (int j = 0; j < tl.getLength(); j++) {
        SoType type = tl[j];
        if (type == SoUnknownNode::getClassTypeId() ||
            type == SoUnknownEngine::getClassTypeId())
            continue;

        ASSERT_FALSE(type.isBad());
        if (type.canCreateInstance()) {
            SoBase *base = (SoBase *)type.createInstance();
            ASSERT_TRUE(base);
            base->ref();
            base->unref();
        }
    }
}
