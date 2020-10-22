#include "gtest/gtest.h"
#include "utils.h"

#include <SoDebug.h>
#include <Inventor/SoInteraction.h>
#include <Inventor/SbImage.h>
#include <Inventor/SoOffscreenRenderer.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoSeparator.h>

class ModelTest_GLCTX : public ::testing::TestWithParam<const char *> {
  protected:
    virtual void SetUp() {
        // init Inventor
        SoInteraction::init();

        oivhome = SoDebug::GetEnv("OIVHOME");

        ASSERT_FALSE(oivhome.isEmpty()) << "OIVHOME is not set or empty!";
    }

    virtual void TearDown() { SoInteraction::finish(); }

    SbImage snapFile(const SbString &src, const SbString &dst) {
        SoInput in;
        if (!in.openFile(src)) {
            ADD_FAILURE() << "Cannot open" << src.getString();
            return SbImage();
        }

        SoSeparator *model = SoDB::readAll(&in);
        if (!model) {
            ADD_FAILURE() << "Cannot read " << src.getString();
            return SbImage();
        }

        SoSeparator *root = new SoSeparator;
        root->ref();

        SoDirectionalLight *light = new SoDirectionalLight;
        light->direction.setValue(-1, -1, -1);
        root->addChild(light);

        SoPerspectiveCamera *camera = new SoPerspectiveCamera;
        root->addChild(camera);

        root->addChild(model);

        SbViewportRegion vpRegion(640, 480);

        camera->viewAll(root, vpRegion);

        SoOffscreenRenderer offscreenRenderer(vpRegion);

        EXPECT_TRUE(offscreenRenderer.render(root));

        const SbImage &img = offscreenRenderer.getImage();
        if (!SbFile::exists(dst)) {
            ADD_FAILURE() << "Baseline missing. Creating " << dst.getString();
            utils::mkpath(SbFile::dirName(dst).getString());
            utils::write_image(img, dst);
        }
        root->unref();

        return img;
    }

    SbString oivhome;
};

TEST_P(ModelTest_GLCTX, all) {
    const SbString srcFilename = oivhome + "/" + GetParam();
    ASSERT_TRUE(SbFile::exists(srcFilename));

    SbString basename = SbFile::baseName(GetParam());
    basename = basename.getSubString(0, basename.find(".") - 1);

    const SbString dstFilename(oivhome + "/tests/baseline/" +
                               SbFile::dirName(GetParam()) + basename + ".png");

    const SbImage ref = SbImage(dstFilename);
    const SbImage dst = snapFile(srcFilename, dstFilename);

    ASSERT_FALSE(ref.isNull());
    ASSERT_FALSE(dst.isNull());

    EXPECT_IMG_EQ(ref, dst);
}

INSTANTIATE_TEST_CASE_P(
    root, ModelTest_GLCTX,
    ::testing::Values("data/models/bird.iv", "data/models/chair.iv",
                      "data/models/diamond.iv", "data/models/engine.iv",
                      "data/models/heart.iv", "data/models/InventorLogo.iv",
                      "data/models/moon.iv", "data/models/shamrock.iv",
                      "data/models/shell.iv", "data/models/slotMachine.iv",
                      "data/models/spongetri4.iv", "data/models/star.iv",
                      "data/models/torus.iv", "data/models/wheel.iv"));
INSTANTIATE_TEST_CASE_P(buildings, ModelTest_GLCTX,
                        ::testing::Values("data/models/buildings/Barcelona.iv",
                                          "data/models/buildings/temple.iv",
                                          "data/models/buildings/windmill.iv"));
INSTANTIATE_TEST_CASE_P(chess, ModelTest_GLCTX,
                        ::testing::Values("data/models/chess/bishop.iv",
                                          "data/models/chess/chessboard.iv",
                                          "data/models/chess/king.iv",
                                          "data/models/chess/knight.iv",
                                          "data/models/chess/pawn.iv",
                                          "data/models/chess/queen.iv",
                                          "data/models/chess/rook.iv"));
INSTANTIATE_TEST_CASE_P(cyberhead, ModelTest_GLCTX,
                        ::testing::Values("data/models/CyberHeads/josie.iv",
                                          "data/models/CyberHeads/paul.iv",
                                          "data/models/CyberHeads/rikk.iv"));
INSTANTIATE_TEST_CASE_P(food, ModelTest_GLCTX,
                        ::testing::Values("data/models/food/apple.iv",
                                          "data/models/food/banana.iv",
                                          "data/models/food/mushroom.iv",
                                          "data/models/food/pear.iv"));
INSTANTIATE_TEST_CASE_P(
    furniture, ModelTest_GLCTX,
    ::testing::Values("data/models/furniture/ceilingLamp.iv",
                      "data/models/furniture/table.iv"));
INSTANTIATE_TEST_CASE_P(geography, ModelTest_GLCTX,
                        ::testing::Values("data/models/geography/coasts.iv",
                                          "data/models/geography/earth.iv"));
INSTANTIATE_TEST_CASE_P(
    household, ModelTest_GLCTX,
    ::testing::Values(
        "data/models/household/canstick.iv",
        "data/models/household/manillaFolder.iv",
        "data/models/household/martini.iv", "data/models/household/mug.iv",
        "data/models/household/outlet.iv", "data/models/household/plant.iv",
        "data/models/household/plug.iv", "data/models/household/pump.iv",
        "data/models/household/teapot.iv",
        "data/models/household/trackLights.iv"));
INSTANTIATE_TEST_CASE_P(
    machines, ModelTest_GLCTX,
    ::testing::Values("data/models/machines/coarseGear.iv",
                      "data/models/machines/compressor.iv",
                      "data/models/machines/fineGear.iv",
                      "data/models/machines/lathe.iv",
                      "data/models/machines/simpleEngine.iv",
                      "data/models/machines/simpleMonitor.iv",
                      "data/models/machines/vise.iv",
                      "data/models/machines/windTunnel.iv"));
INSTANTIATE_TEST_CASE_P(robots, ModelTest_GLCTX,
                        ::testing::Values("data/models/robots/cubeMan.iv",
                                          "data/models/robots/cylMan.iv",
                                          "data/models/robots/kitMan.iv",
                                          "data/models/robots/roboHand.iv",
                                          "data/models/robots/robotcop.iv",
                                          "data/models/robots/sphereMan.iv"));
INSTANTIATE_TEST_CASE_P(
    scenes, ModelTest_GLCTX,
    ::testing::Values("data/models/scenes/chesschairs.iv",
                      "data/models/scenes/curtain.iv",
                      "data/models/scenes/glider.iv",
                      "data/models/scenes/stagedancers.iv"));
INSTANTIATE_TEST_CASE_P(sgi, ModelTest_GLCTX,
                        ::testing::Values("data/models/sgi/indigo.iv",
                                          "data/models/sgi/logo.iv",
                                          "data/models/sgi/sgilogo.iv"));
INSTANTIATE_TEST_CASE_P(
    simple, ModelTest_GLCTX,
    ::testing::Values(
        "data/models/simple/10by10.iv", "data/models/simple/axis.iv",
        "data/models/simple/cone.iv", "data/models/simple/cube.iv",
        "data/models/simple/cylinder.iv", "data/models/simple/faceSet.iv",
        "data/models/simple/fog.iv", "data/models/simple/indexedFaceSet.iv",
        "data/models/simple/indexedTriangleMesh.iv",
        "data/models/simple/polygonOffset.iv", "data/models/simple/quadMesh.iv",
        "data/models/simple/rubik.iv",
        "data/models/simple/shapes complexity.iv",
        "data/models/simple/sphere.iv", "data/models/simple/star.iv",
        "data/models/simple/tet.iv", "data/models/simple/text.iv",
        "data/models/simple/text2-unicode.iv",
        "data/models/simple/text3-unicode.iv", "data/models/simple/text3.iv",
        "data/models/simple/texture.iv", "data/models/simple/three.iv",
        "data/models/simple/tri.iv", "data/models/simple/triangleStripSet.iv"));
INSTANTIATE_TEST_CASE_P(toys, ModelTest_GLCTX,
                        ::testing::Values("data/models/toys/cannon.iv",
                                          "data/models/toys/dart.iv",
                                          "data/models/toys/jackInTheBox.iv",
                                          "data/models/toys/lavalamp.iv",
                                          "data/models/toys/legoDog.iv",
                                          "data/models/toys/top.iv"));
INSTANTIATE_TEST_CASE_P(
    vehicules, ModelTest_GLCTX,
    ::testing::Values(
        "data/models/vehicles/767.iv", "data/models/vehicles/aircar.iv",
        "data/models/vehicles/f16.iv", "data/models/vehicles/fiero.iv",
        "data/models/vehicles/gunstar.iv", "data/models/vehicles/kittyHawk.iv",
        "data/models/vehicles/spacestation.iv", "data/models/vehicles/x29.iv"));
INSTANTIATE_TEST_CASE_P(
    nurbs, ModelTest_GLCTX,
    ::testing::Values("tests/data/nurbs/Circle.iv",
                      "tests/data/nurbs/SoIndexedNurbsCurve.iv",
                      "tests/data/nurbs/SoIndexedNurbsSurface.iv",
                      "tests/data/nurbs/SoNurbsCurve.iv",
                      "tests/data/nurbs/SoNurbsProfile.iv",
                      "tests/data/nurbs/SoNurbsSurface.iv"));
INSTANTIATE_TEST_CASE_P(shapes, ModelTest_GLCTX,
                        ::testing::Values("tests/data/shapes/creaseAngle.iv"));
INSTANTIATE_TEST_CASE_P(
    draggers, ModelTest_GLCTX,
    ::testing::Values("tests/data/draggers/CenterballDragger.iv",
                      "tests/data/draggers/DirectionalLightDragger.iv",
                      "tests/data/draggers/DragPointDragger.iv",
                      "tests/data/draggers/HandleBoxDragger.iv",
                      "tests/data/draggers/JackDragger.iv",
                      "tests/data/draggers/PointLightDragger.iv",
                      "tests/data/draggers/RotateCylindricalDragger.iv",
                      "tests/data/draggers/RotateDiscDragger.iv",
                      "tests/data/draggers/RotateSphericalDragger.iv",
                      "tests/data/draggers/Scale1Dragger.iv",
                      "tests/data/draggers/Scale2Dragger.iv",
                      "tests/data/draggers/Scale2UniformDragger.iv",
                      "tests/data/draggers/ScaleUniformDragger.iv",
                      "tests/data/draggers/SpotLightDragger.iv",
                      "tests/data/draggers/TabBoxDragger.iv",
                      "tests/data/draggers/TabPlaneDragger.iv",
                      "tests/data/draggers/TrackballDragger.iv",
                      "tests/data/draggers/TransformBoxDragger.iv",
                      "tests/data/draggers/TransformerDragger.iv",
                      "tests/data/draggers/Translate1Dragger.iv",
                      "tests/data/draggers/Translate2Dragger.iv"));

INSTANTIATE_TEST_CASE_P(apps, ModelTest_GLCTX,
                        ::testing::Values("apps/examples/data/bookshelf.iv",
                                          "apps/examples/data/desk.iv",
                                          "apps/examples/data/dogDish.iv",
                                          "apps/examples/data/duck.iv",
                                          "apps/examples/data/eatAtJosies.iv",
                                          "apps/examples/data/flower.iv",
                                          "apps/examples/data/jumpyMan.iv",
                                          "apps/examples/data/luxo.iv",
                                          "apps/examples/data/parkbench.iv"));
