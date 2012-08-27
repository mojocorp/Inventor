#include <QImage>

#include "gtest/gtest.h"
#include "utils.h"

#include <Inventor/SoOffscreenRenderer.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoSeparator.h>

TEST(testOffscreen, offscreen) {
    SoSeparator * root = new SoSeparator;
    root->ref();

    SoDirectionalLight * light = new SoDirectionalLight;
    light->direction.setValue(-1, -1, -1);
    root->addChild(light);

    SoPerspectiveCamera * camera = new SoPerspectiveCamera;
    camera->position.setValue(4, 4, 4);
    camera->pointAt(SbVec3f(0,0,0));
    root->addChild(camera);

    SoCube * cube = new SoCube;
    root->addChild(cube);

    SbViewportRegion vpRegion(400, 300);
    SoOffscreenRenderer offscreenRenderer(vpRegion);

    EXPECT_TRUE(offscreenRenderer.render(root));

    root->unref();

    QImage ref(":/ivtest/baseline/offscreen/cube.png");
    QImage img = QImage(offscreenRenderer.getBuffer(), 400, 300, QImage::Format_RGB888).mirrored();

    EXPECT_IMG_EQ(ref, img);
}
