#include <gtest/gtest.h>

#include <Inventor/fields/SoMFInt32.h>

TEST(testFields, set1Value) {
    SoMFInt32 field;

    for (int i=0; i<100; i++) {
        field.set1Value(i, i);
    }

    EXPECT_EQ(100, field.getNum());

    for (int i=0; i<field.getNum(); i++) {
        EXPECT_EQ(i, field[i]);
        EXPECT_EQ(i, field.getValues(0)[i]);
    }

    field.setNum(50);

    EXPECT_EQ(50, field.getNum());

    for (int i=0; i<field.getNum(); i++) {
        EXPECT_EQ(i, field[i]);
        EXPECT_EQ(i, field.getValues(0)[i]);
    }
}

TEST(testFields, editing) {
    SoMFInt32 field;

    field.setNum(100);
    int32_t *ptr = field.startEditing();
    for (int i=0; i<field.getNum(); i++) {
        ptr[i] = i;
    }
    field.finishEditing();

    ptr = field.startEditing();
    for (int i=0; i<field.getNum(); i++) {
        EXPECT_EQ(i, field[i]);
        EXPECT_EQ(i, field.getValues(0)[i]);
        EXPECT_EQ(i, ptr[i]);
    }
    field.finishEditing();

    field.setNum(50);
    ptr = field.startEditing();
    for (int i=0; i<field.getNum(); i++) {
        EXPECT_EQ(i, field[i]);
        EXPECT_EQ(i, field.getValues(0)[i]);
        EXPECT_EQ(i, ptr[i]);
    }
    field.finishEditing();

    field.setNum(200);

    ptr = field.startEditing();
    for (int i=0; i<field.getNum(); i++) {
        ptr[i] = i;
    }
    field.finishEditing();

    ptr = field.startEditing();
    for (int i=0; i<field.getNum(); i++) {
        EXPECT_EQ(i, field[i]);
        EXPECT_EQ(i, field.getValues(0)[i]);
        EXPECT_EQ(i, ptr[i]);
    }
    field.finishEditing();
}
