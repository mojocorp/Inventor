#include "gtest/gtest.h"
#include "utils.h"

#include <SoDebug.h>
#include <Inventor/SbImage.h>
#include <Inventor/SoOffscreenRenderer.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoSeparator.h>

TEST(testOffscreen_GLCTX, offscreen) {
    const SbString oivhome = SoDebug::GetEnv("OIVHOME");

    ASSERT_FALSE(oivhome.isEmpty()) << "OIVHOME is not set or empty!";

    SoDB::init();

    SoSeparator *root = new SoSeparator;
    root->ref();

    SoDirectionalLight *light = new SoDirectionalLight;
    light->direction.setValue(-1, -1, -1);
    root->addChild(light);

    SoPerspectiveCamera *camera = new SoPerspectiveCamera;
    camera->position.setValue(4, 4, 4);
    camera->pointAt(SbVec3f(0, 0, 0));
    root->addChild(camera);

    SoCube *cube = new SoCube;
    root->addChild(cube);

    SbViewportRegion    vpRegion(400, 300);
    SoOffscreenRenderer offscreenRenderer(vpRegion);
    offscreenRenderer.setComponents(SoOffscreenRenderer::RGB);

    {
        EXPECT_TRUE(offscreenRenderer.render(root));

        const SbImage ref(oivhome +
                          "/tests/baseline/offscreen/cube400x300.png");
        const SbImage img = offscreenRenderer.getImage();

        EXPECT_FALSE(ref.isNull());
        EXPECT_FALSE(img.isNull());
        EXPECT_IMG_EQ(ref, img);
    }

    {
        offscreenRenderer.setViewportRegion(SbViewportRegion(800, 600));
        EXPECT_TRUE(offscreenRenderer.render(root));

        const SbImage ref(oivhome +
                          "/tests/baseline/offscreen/cube800x600.png");
        const SbImage img = offscreenRenderer.getImage();

        EXPECT_FALSE(ref.isNull());
        EXPECT_FALSE(img.isNull());
        EXPECT_IMG_EQ(ref, img);
    }
    root->unref();
}
