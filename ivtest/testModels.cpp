#include <QImage>
#include <QDir>

#include "gtest/gtest.h"
#include "utils.h"

#include <Inventor/SoOffscreenRenderer.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoSeparator.h>

class ModelTest_GLCTX : public ::testing::TestWithParam<const char*>
{
protected:
    QImage snapFile(const QFileInfo & src, const QFileInfo & dst)
    {
        SoInput in;
        if (!in.openFile(SbString::fromUtf8(src.filePath().toUtf8().data()))) {
            printf("ERROR: cannot open %s\n", qPrintable(src.filePath()));
            return QImage();
        }

        SoSeparator * model = SoDB::readAll(&in);
        if (!model) {
            printf("ERROR: SoDB::readAll cannot read %s\n", qPrintable(src.filePath()));
            return QImage();
        }

        SoSeparator * root = new SoSeparator;
        root->ref();

        SoDirectionalLight * light = new SoDirectionalLight;
        light->direction.setValue(-1, -1, -1);
        root->addChild(light);

        SoPerspectiveCamera * camera = new SoPerspectiveCamera;
        root->addChild(camera);

        root->addChild(model);

        SbViewportRegion vpRegion(640, 480);

        camera->viewAll(root, vpRegion);

        SoOffscreenRenderer offscreenRenderer(vpRegion);

        EXPECT_TRUE(offscreenRenderer.render(root));

        QImage img = QImage(offscreenRenderer.getBuffer(), 640, 480, QImage::Format_RGB888).convertToFormat(QImage::Format_RGB32).mirrored();
        //img.save(dst.fileName());
        if (!dst.exists()) {
            img.save(dst.filePath());
        }
        root->unref();

        return img;
    }

};

TEST_P(ModelTest_GLCTX, all) {
    QString oivhome(getenv("OIV_HOME"));

    QFileInfo srcFilename = oivhome + "/" + GetParam();
    ASSERT_TRUE(srcFilename.exists());

    QFileInfo dstFilename(GetParam());
    dstFilename = oivhome + "/ivtest/baseline/" + dstFilename.path() + "/" + dstFilename.baseName() + ".png";
    QDir dstDir(dstFilename.absoluteDir());
    if (!dstDir.exists())
        dstDir.mkpath(dstDir.absolutePath());

    QImage ref = QImage(dstFilename.filePath()).convertToFormat(QImage::Format_RGB32);
    QImage dst = snapFile(srcFilename, dstFilename);

    ASSERT_FALSE(ref.isNull());
    ASSERT_FALSE(dst.isNull());

    EXPECT_IMG_EQ(ref, dst);
}

INSTANTIATE_TEST_CASE_P(root,
                        ModelTest_GLCTX,
                        ::testing::Values(
                            "data/models/bird.iv",
                            "data/models/chair.iv",
                            "data/models/diamond.iv",
                            "data/models/engine.iv",
                            "data/models/heart.iv",
                            "data/models/InventorLogo.iv",
                            "data/models/moon.iv",
                            "data/models/shamrock.iv",
                            "data/models/shell.iv",
                            "data/models/slotMachine.iv",
                            "data/models/spongetri4.iv",
                            "data/models/star.iv",
                            "data/models/torus.iv",
                            "data/models/wheel.iv"));
INSTANTIATE_TEST_CASE_P(buildings,
                        ModelTest_GLCTX,
                        ::testing::Values(
                            "data/models/buildings/Barcelona.iv",
                            "data/models/buildings/temple.iv",
                            "data/models/buildings/windmill.iv"));
INSTANTIATE_TEST_CASE_P(chess,
                        ModelTest_GLCTX,
                        ::testing::Values(
                            "data/models/chess/bishop.iv",
                            "data/models/chess/chessboard.iv",
                            "data/models/chess/king.iv",
                            "data/models/chess/knight.iv",
                            "data/models/chess/pawn.iv",
                            "data/models/chess/queen.iv",
                            "data/models/chess/rook.iv"));
INSTANTIATE_TEST_CASE_P(cyberhead,
                        ModelTest_GLCTX,
                        ::testing::Values(
                            "data/models/CyberHeads/josie.iv",
                            "data/models/CyberHeads/paul.iv",
                            "data/models/CyberHeads/rikk.iv"));
INSTANTIATE_TEST_CASE_P(food,
                        ModelTest_GLCTX,
                        ::testing::Values(
                            "data/models/food/apple.iv",
                            "data/models/food/banana.iv",
                            "data/models/food/mushroom.iv",
                            "data/models/food/pear.iv"));
INSTANTIATE_TEST_CASE_P(furniture,
                        ModelTest_GLCTX,
                        ::testing::Values(
                            "data/models/furniture/ceilingLamp.iv",
                            "data/models/furniture/table.iv"));
INSTANTIATE_TEST_CASE_P(geography,
                        ModelTest_GLCTX,
                        ::testing::Values(
                            "data/models/geography/coasts.iv",
                            "data/models/geography/earth.iv"));
INSTANTIATE_TEST_CASE_P(household,
                        ModelTest_GLCTX,
                        ::testing::Values(
                            "data/models/household/canstick.iv",
                            "data/models/household/manillaFolder.iv",
                            "data/models/household/martini.iv",
                            "data/models/household/mug.iv",
                            "data/models/household/outlet.iv",
                            "data/models/household/plant.iv",
                            "data/models/household/plug.iv",
                            "data/models/household/pump.iv",
                            "data/models/household/teapot.iv",
                            "data/models/household/trackLights.iv"));
INSTANTIATE_TEST_CASE_P(machines,
                        ModelTest_GLCTX,
                        ::testing::Values(
                            "data/models/machines/coarseGear.iv",
                            "data/models/machines/compressor.iv",
                            "data/models/machines/fineGear.iv",
                            "data/models/machines/lathe.iv",
                            "data/models/machines/simpleEngine.iv",
                            "data/models/machines/simpleMonitor.iv",
                            "data/models/machines/vise.iv",
                            "data/models/machines/windTunnel.iv"));
INSTANTIATE_TEST_CASE_P(robots,
                        ModelTest_GLCTX,
                        ::testing::Values(
                            "data/models/robots/cubeMan.iv",
                            "data/models/robots/cylMan.iv",
                            "data/models/robots/kitMan.iv",
                            "data/models/robots/roboHand.iv",
                            "data/models/robots/robotcop.iv",
                            "data/models/robots/sphereMan.iv"));
INSTANTIATE_TEST_CASE_P(scenes,
                        ModelTest_GLCTX,
                        ::testing::Values(
                            "data/models/scenes/chesschairs.iv",
                            "data/models/scenes/curtain.iv",
                            "data/models/scenes/glider.iv",
                            "data/models/scenes/stagedancers.iv"));
INSTANTIATE_TEST_CASE_P(sgi,
                        ModelTest_GLCTX,
                        ::testing::Values(
                            "data/models/sgi/indigo.iv",
                            "data/models/sgi/logo.iv",
                            "data/models/sgi/sgilogo.iv"));
INSTANTIATE_TEST_CASE_P(simple,
                        ModelTest_GLCTX,
                        ::testing::Values(
                            "data/models/simple/10by10.iv",
                            "data/models/simple/axis.iv",
                            "data/models/simple/cone.iv",
                            "data/models/simple/cube.iv",
                            "data/models/simple/cylinder.iv",
                            "data/models/simple/faceSet.iv",
                            "data/models/simple/indexedFaceSet.iv",
                            "data/models/simple/indexedTriangleMesh.iv",
                            "data/models/simple/polygonOffset.iv",
                            "data/models/simple/quadMesh.iv",
                            "data/models/simple/sphere.iv",
                            "data/models/simple/star.iv",
                            "data/models/simple/tet.iv",
                            "data/models/simple/text.iv",
                            "data/models/simple/text2-unicode.iv",
                            "data/models/simple/text3-unicode.iv",
                            "data/models/simple/text3.iv",
                            "data/models/simple/texture.iv",
                            "data/models/simple/three.iv",
                            "data/models/simple/tri.iv",
                            "data/models/simple/triangleStripSet.iv"));
INSTANTIATE_TEST_CASE_P(toys,
                        ModelTest_GLCTX,
                        ::testing::Values(
                            "data/models/toys/cannon.iv",
                            "data/models/toys/dart.iv",
                            "data/models/toys/jackInTheBox.iv",
                            "data/models/toys/lavalamp.iv",
                            "data/models/toys/legoDog.iv",
                            "data/models/toys/top.iv"));
INSTANTIATE_TEST_CASE_P(vehicules,
                        ModelTest_GLCTX,
                        ::testing::Values(
                            "data/models/vehicles/767.iv",
                            "data/models/vehicles/aircar.iv",
                            "data/models/vehicles/f16.iv",
                            "data/models/vehicles/fiero.iv",
                            "data/models/vehicles/gunstar.iv",
                            "data/models/vehicles/kittyHawk.iv",
                            "data/models/vehicles/spacestation.iv",
                            "data/models/vehicles/x29.iv"));
INSTANTIATE_TEST_CASE_P(nurbs,
                        ModelTest_GLCTX,
                        ::testing::Values(
                            "ivtest/data/nurbs/Circle.iv",
                            "ivtest/data/nurbs/SoIndexedNurbsCurve.iv",
                            "ivtest/data/nurbs/SoIndexedNurbsSurface.iv",
                            "ivtest/data/nurbs/SoNurbsCurve.iv",
                            "ivtest/data/nurbs/SoNurbsProfile.iv",
                            "ivtest/data/nurbs/SoNurbsSurface.iv"));
INSTANTIATE_TEST_CASE_P(shapes,
                        ModelTest_GLCTX,
                        ::testing::Values(
                            "ivtest/data/shapes/creaseAngle.iv"));
INSTANTIATE_TEST_CASE_P(draggers,
                        ModelTest_GLCTX,
                        ::testing::Values(
                            "ivtest/data/draggers/CenterballDragger.iv",
                            "ivtest/data/draggers/DirectionalLightDragger.iv",
                            "ivtest/data/draggers/DragPointDragger.iv",
                            "ivtest/data/draggers/HandleBoxDragger.iv",
                            "ivtest/data/draggers/JackDragger.iv",
                            "ivtest/data/draggers/PointLightDragger.iv",
                            "ivtest/data/draggers/RotateCylindricalDragger.iv",
                            "ivtest/data/draggers/RotateDiscDragger.iv",
                            "ivtest/data/draggers/RotateSphericalDragger.iv",
                            "ivtest/data/draggers/Scale1Dragger.iv",
                            "ivtest/data/draggers/Scale2Dragger.iv",
                            "ivtest/data/draggers/Scale2UniformDragger.iv",
                            "ivtest/data/draggers/ScaleUniformDragger.iv",
                            "ivtest/data/draggers/SpotLightDragger.iv",
                            "ivtest/data/draggers/TabBoxDragger.iv",
                            "ivtest/data/draggers/TabPlaneDragger.iv",
                            "ivtest/data/draggers/TrackballDragger.iv",
                            "ivtest/data/draggers/TransformBoxDragger.iv",
                            "ivtest/data/draggers/TransformerDragger.iv",
                            "ivtest/data/draggers/Translate1Dragger.iv",
                            "ivtest/data/draggers/Translate2Dragger.iv"
                            ));
