#include <gtest/gtest.h>

#include <Inventor/SbMatrix.h>

TEST(SbMatrix, Size) {
    //  WARNING!!!!!  Transcription of arrays of this class assume that the
    //                only data stored in this class are 16 consecutive values.
    //                Do not add any extra data members!!!
    EXPECT_EQ(16 * sizeof(float), sizeof(SbMatrix));
}

TEST(SbMatrix, constructors) {
    SbMatrix m(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
               0.0f, 0.0f, 0.0f, 0.0f, 1.0f);

    EXPECT_FLOAT_EQ(1.0f, m[0][0]);
    EXPECT_FLOAT_EQ(0.0f, m[0][1]);
    EXPECT_FLOAT_EQ(0.0f, m[0][2]);
    EXPECT_FLOAT_EQ(0.0f, m[0][3]);

    EXPECT_FLOAT_EQ(0.0f, m[1][0]);
    EXPECT_FLOAT_EQ(1.0f, m[1][1]);
    EXPECT_FLOAT_EQ(0.0f, m[1][2]);
    EXPECT_FLOAT_EQ(0.0f, m[1][3]);

    EXPECT_FLOAT_EQ(0.0f, m[2][0]);
    EXPECT_FLOAT_EQ(0.0f, m[2][1]);
    EXPECT_FLOAT_EQ(1.0f, m[2][2]);
    EXPECT_FLOAT_EQ(0.0f, m[2][3]);

    EXPECT_FLOAT_EQ(0.0f, m[3][0]);
    EXPECT_FLOAT_EQ(0.0f, m[3][1]);
    EXPECT_FLOAT_EQ(0.0f, m[3][2]);
    EXPECT_FLOAT_EQ(1.0f, m[3][3]);

    SbMat m2 = {{1.0f, 0.0f, 0.0f, 0.0f},
                {0.0f, 1.0f, 0.0f, 0.0f},
                {0.0f, 0.0f, 1.0f, 0.0f},
                {0.0f, 0.0f, 0.0f, 1.0f}};

    SbMatrix m3(m2);

    EXPECT_TRUE(m == m3);
}

TEST(SbMatrix, setValue) {
    SbMatrix m(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
               0.0f, 0.0f, 0.0f, 0.0f, 1.0f);

    SbMatrix m2;
    m2.setValue(m);

    EXPECT_TRUE(m2 == m);
}

TEST(SbMatrix, makeIdentity) {
    SbMatrix m;
    m.makeIdentity();

    EXPECT_TRUE(m == SbMatrix::identity());
}

TEST(SbMatrix, multiplication) {
    SbMatrix M(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f,
               11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f);
    SbMatrix N;
    N.makeIdentity();

    N = N * M;

    EXPECT_TRUE(N == M);
}

TEST(SbMatrix, inverse) {
    SbMatrix m1(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

    EXPECT_TRUE(m1 * m1.inverse() == SbMatrix::identity());

    SbMatrix m2(0, 1, 0, 0, -1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

    EXPECT_TRUE(m2 * m2.inverse() == SbMatrix::identity());

    SbMatrix m3(1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, -1, 0, 0, 1, 0);

    EXPECT_TRUE(m3 * m3.inverse() == SbMatrix::identity());

    SbMatrix m4(4.683281e-01, -8.749647e-01, 1.229049e-01, 0.000000e+00,
                1.251189e-02, 1.456563e-01, 9.892561e-01, 0.000000e+00,
                -8.834660e-01, -4.617587e-01, 7.916244e-02, 0.000000e+00,
                -4.726541e+00, 3.044795e+00, -6.737138e+00, 1.000000e+00);

    EXPECT_TRUE((m4 * m4.inverse()).equals(SbMatrix::identity(), 1E-6));

    SbMatrix m5(4.683281e-01, -8.749647e-01, 1.229049e-01, 1.000000e+00,
                1.251189e-02, 1.456563e-01, 9.892561e-01, 2.000000e+00,
                -8.834660e-01, -4.617587e-01, 7.916244e-02, 3.000000e+00,
                -4.726541e+00, 3.044795e+00, -6.737138e+00, 4.000000e+00);

    EXPECT_TRUE((m5 * m5.inverse()).equals(SbMatrix::identity(), 1E-6));
}
