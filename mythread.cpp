/**
  问题：1，无法使用argv,args
       2, 传递给updaTadaFilename的不应该是QString 而应该是Buffer<List>
       3，用户可以根据设置
       4，在buffer中不应该只保存路径，还应该保存时间和转换类型
*/

#include "mythread.h"

MyThread::MyThread(QObject *parent) :
    QThread(parent)
{
}

void MyThread::updataFilename(QString file)
{
    filename = file;
}

osg::ref_ptr<osg::Node> MyThread::CraetePlane(QString fileName)
{
    std::string filename = fileName.toStdString();
    // geom
    osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;

    // Defination of vertices
    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array;
    geom->setVertexArray(v.get());
    v->push_back(osg::Vec3(-2.f,0.f,-2.f));
    v->push_back(osg::Vec3(2.f,0.f,-2.f));
    v->push_back(osg::Vec3(2.f,0.f,2.f));
    v->push_back(osg::Vec3(-2.f,0.f,2.f));

    // defination of colors
    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array;
    geom->setColorArray(c.get());
    geom->setColorBinding(osg::Geometry::BIND_OVERALL);
    c->push_back(osg::Vec4(1.f,1.f,1.f,1.f));

    // defination coordinate
    osg::ref_ptr<osg::Vec2Array> t = new osg::Vec2Array;
    //geom->setTexCoordArray(0,t,osg::Ar);
    geom->setTexCoordArray(0,t);
    //geom->sett
    t->push_back(osg::Vec2(0.f,0.f));
    t->push_back(osg::Vec2(1.f,0.f));
    t->push_back(osg::Vec2(1.f,1.f));
    t->push_back(osg::Vec2(0.f,1.f));

    // Setup texture
    osg::ref_ptr<osg::Image> image;
    osg::ref_ptr<osg::Texture2D> texture2D = new osg::Texture2D;
    image = osgDB::readImageFile(filename);
    if(image.valid())
        texture2D->setImage(image);


    //defination of normal
    osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array;
    geom->setNormalArray(n.get());
    geom->setNormalBinding(osg::Geometry::BIND_OVERALL);
    n->push_back(osg::Vec3(0.f,-1.f,0.f));

    // vertex affine
    geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS,0,4));

    // geometry node
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    geode->getOrCreateStateSet()->setTextureAttributeAndModes(0,texture2D.get(),osg::StateAttribute::ON);
    geode->addDrawable(geom.get());
    return geode.get();
}

void MyThread::run()
{
    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;
    osg::ref_ptr<osg::Group> root = new osg::Group;

    root->addChild(CraetePlane(filename));
//    root->addChild();

    viewer->setSceneData(root.get());

    viewer->addEventHandler(new osgViewer::ToggleSyncToVBlankHandler);
    viewer->addEventHandler(new osgViewer::WindowSizeHandler);
    viewer->addEventHandler(new osgViewer::StatsHandler);
//  viewer->addEventHandler(new osgViewer::HelpHandler(arguments.getApplicationUsage()));
    viewer->addEventHandler(new osgViewer::RecordCameraPathHandler);
    viewer->addEventHandler(new osgViewer::LODScaleHandler);
    viewer->addEventHandler(new osgViewer::ScreenCaptureHandler);

    viewer->realize();

    //! return directly
    viewer->run();
}
