#include <limits.h>
#include "gtest/gtest.h"

#include <Inventor/SoType.h>
#include <SoUnknownNode.h>
#include <SoUnknownEngine.h>
#include <Inventor/misc/SoBase.h>
#include <Inventor/lists/SoTypeList.h>

TEST(CreationTest, All) {
  // This test is named "All", and belongs to the "CreationTest"
  // test case.
  //EXPECT_EQ(1, Factorial(-5));
  //EXPECT_EQ(1, Factorial(-1));
  //EXPECT_TRUE(Factorial(-10) \> 0);
  //EXPECT_FALSE(IsPrime(-1));

  SoTypeList tl;
  SoType::getAllDerivedFrom(SoType::fromName("SoBase"), tl);
  for (int j=0; j < tl.getLength(); j++) {
      SoType type = tl[j];
      if (type == SoUnknownNode::getClassTypeId() ||
          type == SoUnknownEngine::getClassTypeId())
          continue;

      ASSERT_FALSE(type.isBad());
      if (type.canCreateInstance()) {
          SoBase * base = (SoBase*)type.createInstance();
          ASSERT_TRUE(base);
          base->ref();
          base->unref();
      }
  }
}

