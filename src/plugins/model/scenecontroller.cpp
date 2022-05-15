#include "scenecontroller.h"

#include <Qt3DCore/QEntity>
#include <Qt3DCore/QNode>
#include <Qt3DCore/QTransform>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QPointLight>
#include <Qt3DRender/QEffect>
#include <Qt3DRender/QTechnique>
#include <Qt3DRender/QRenderPass>
#include <Qt3DRender/QRenderState>
#include <Qt3DRender/QDepthTest>

#include <Qt3DExtras/Qt3DWindow>

#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/QPerVertexColorMaterial>
#include <Qt3DExtras/QCylinderMesh>
#include <Qt3DExtras/QCuboidMesh>
#include <Qt3DExtras/QSphereMesh>
#include <Qt3DInput/QInputAspect>
#include <Qt3DExtras/QForwardRenderer>

#include "c3drenderer.h"
#include "gridmesh.h"
#include "myorbitcameracontroller.h"

SceneController::SceneController(Qt3DExtras::Qt3DWindow* view, QObject* parent)
	: QObject(parent)
	, _view(view)
	, _rootEntity(nullptr)
	, _modelsEntity(nullptr)
{
	init();
}

void SceneController::init()
{
	_rootEntity = new Qt3DCore::QEntity();
	_view->setRootEntity(_rootEntity);
//	_view->defaultFrameGraph()->setShowDebugOverlay(true);
	_view->defaultFrameGraph()->setClearColor(QColor(QRgb(0x4d4d4f)));
	Qt3DInput::QInputAspect *input = new Qt3DInput::QInputAspect;
	_view->registerAspect(input);

//	QQmlContext* rootContext = _ui->quickWidget->rootContext();


	// Camera
	Qt3DRender::QCamera *cameraEntity = _view->camera();

	cameraEntity->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
	cameraEntity->setPosition(QVector3D(0, 20.0f, 0));
	cameraEntity->setUpVector(QVector3D(0, 0, 1.0f));
	cameraEntity->setViewCenter(QVector3D(0, 0, 0));

	Qt3DCore::QEntity *lightEntity = new Qt3DCore::QEntity(_rootEntity);
	Qt3DRender::QPointLight *light = new Qt3DRender::QPointLight(lightEntity);
	light->setColor("white");
	light->setIntensity(1);
	lightEntity->addComponent(light);
	Qt3DCore::QTransform *lightTransform = new Qt3DCore::QTransform(lightEntity);
	lightTransform->setTranslation(cameraEntity->position());
	lightEntity->addComponent(lightTransform);

	MyOrbitCameraController *camController = new MyOrbitCameraController(_rootEntity);
	camController->setUpVector({0, 0, 1.0f});
	camController->setSensitivity(0.01f);
	camController->setLookSpeed(1500.0f);
	camController->setCamera(cameraEntity);

	_modelsEntity = new Qt3DCore::QEntity(_rootEntity);


	Qt3DExtras::QCuboidMesh *originMesh = new Qt3DExtras::QCuboidMesh();

	Qt3DCore::QTransform *originTransformX = new Qt3DCore::QTransform();
	originTransformX->setTranslation(QVector3D(5, 0, 0));
	originTransformX->setScale3D(QVector3D(10.0f, 0.1f, 0.1f));

	Qt3DExtras::QPhongMaterial *originMaterialX= new Qt3DExtras::QPhongMaterial();
	originMaterialX->setAmbient(QColor(0xFF0000));

	auto* originEntityX= new Qt3DCore::QEntity(_rootEntity);
	originEntityX->addComponent(originMesh);
	originEntityX->addComponent(originMaterialX);
	originEntityX->addComponent(originTransformX );

	Qt3DCore::QTransform *originTransformY = new Qt3DCore::QTransform();
	originTransformY->setTranslation(QVector3D(0, 5, 0));
	originTransformY->setScale3D(QVector3D(0.1f, 10.0f, 0.1f));
	Qt3DExtras::QPhongMaterial *originMaterialY= new Qt3DExtras::QPhongMaterial();
	originMaterialY->setAmbient(QColor(0x00FF00));

	auto* originEntityY= new Qt3DCore::QEntity(_rootEntity);
	originEntityY->addComponent(originMesh);
	originEntityY->addComponent(originTransformY);
	originEntityY->addComponent(originMaterialY );

	Qt3DCore::QTransform *originTransformZ = new Qt3DCore::QTransform();
	originTransformZ->setTranslation(QVector3D(0, 0, 5));
	originTransformZ->setScale3D(QVector3D(0.1f, 0.1f, 10.0f));
	Qt3DExtras::QPhongMaterial *originMaterialZ= new Qt3DExtras::QPhongMaterial();
	originMaterialZ->setAmbient(QColor(0x0000FF));

	auto* originEntityZ= new Qt3DCore::QEntity(_rootEntity);
	originEntityZ->addComponent(originMesh);
	originEntityZ->addComponent(originMaterialZ);
	originEntityZ->addComponent(originTransformZ );

//	GridMesh* gridMesh = new GridMesh();
////	gridMesh->gridGeometry()->setLineStep(5/16.0f);
////	gridMesh->gridGeometry()->setLineCount(10);
//	Qt3DExtras::QPhongMaterial *gridMaterial= new Qt3DExtras::QPhongMaterial();
//	gridMaterial->setAmbient(QColor(0xdddddddd));

//	Qt3DCore::QEntity* gridEntity = new Qt3DCore::QEntity(_rootEntity);
//	Qt3DCore::QTransform *gridTransform = new Qt3DCore::QTransform();
//	gridTransform->setScale(1);
//	gridTransform->setTranslation(QVector3D(0, 0, 0));

//	gridEntity->addComponent(gridMesh);
//	gridEntity->addComponent(gridMaterial);
//	gridEntity->addComponent(gridTransform);

//	originMaterial->setAmbient(QColor(QRgb(0xFFFF00)));
//	for(auto* tech : originMaterial->effect()->techniques()){
//		for(auto* rpass : tech->renderPasses()){
//			if(rpass->renderStates().size() == 0){
//				auto dTest = new Qt3DRender::QDepthTest();
//				dTest->setDepthFunction(Qt3DRender::QDepthTest::Always);
//				rpass->addRenderState(dTest);
//			} else {
//				for(auto* rstate: rpass->renderStates()){
//					Qt3DRender::QDepthTest* dTest = dynamic_cast<Qt3DRender::QDepthTest*>(rstate);
//					if(dTest == nullptr){
//						continue;
//					}
//					dTest->setDepthFunction(Qt3DRender::QDepthTest::Always);
//				}
//			}

//		}
//	}
//	material->setDiffuse(QColor(QRgb(0x928327)));


}

void addSlots(Qt3DCore::QEntity*root, const model::M3D& m3d){
	for( int i =0 ; i < 3; i++){
		if(!((1 << i) & m3d.slotsExistence)){
			continue;
		}
		const model::Slot& slot_ = m3d.rSlots[i];
		Qt3DExtras::QSphereMesh *slotMesh = new Qt3DExtras::QSphereMesh();
		slotMesh->setRadius(1);
		slotMesh->setRings(100);
		slotMesh->setSlices(20);

		QVector3D slotPosition(
					slot_.rSlots.x,
					slot_.rSlots.y,
					slot_.rSlots.z
					);

		Qt3DCore::QTransform *slotTransform = new Qt3DCore::QTransform();
		slotTransform->setScale(1.0f);
		slotTransform->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(1.0f, 0.0f, 0.0f), 0.0f));
		slotTransform->setTranslation(slotPosition/ 16.0f);

		Qt3DExtras::QPhongMaterial *material= new Qt3DExtras::QPhongMaterial();
		material->setDiffuse(QColor(QRgb(0x928327)));

		auto* slotEntity= new Qt3DCore::QEntity(root);
		slotEntity->addComponent(slotMesh);
		slotEntity->addComponent(material);
		slotEntity->addComponent(slotTransform );
	}

}

void addWheels(Qt3DCore::QEntity*root, const model::M3D& m3d){
	for(const model::Wheel& wheel: m3d.wheels){
		if(wheel.steer == 0){
			continue;
		}

		auto ptr = QSharedPointer<model::C3D>::create(wheel.model[0]);
		C3DMesh *mesh = new C3DMesh(ptr);

//		Qt3DExtras::QSphereMesh *mesh = new Qt3DExtras::QSphereMesh();
//		mesh->setRadius(8);
//		mesh->setRings(100);
//		mesh->setSlices(20);

		QVector3D slotPosition(
					wheel.f.x,
					wheel.f.y,
					wheel.f.z
					);

		Qt3DCore::QTransform *transform = new Qt3DCore::QTransform();
		transform->setScale(1.0f / 16.0f);
		transform->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(1.0f, 0.0f, 0.0f), 0.0f));
		transform->setTranslation(slotPosition/ 16.0f);

		Qt3DExtras::QPerVertexColorMaterial *material= new Qt3DExtras::QPerVertexColorMaterial();

		auto* entity= new Qt3DCore::QEntity(root);
		entity->addComponent(mesh);
		entity->addComponent(material);
		entity->addComponent(transform );
	}

}

void SceneController::setM3D(const QSharedPointer<model::M3D>& m3d)
{
	setC3D(m3d->body);
	addSlots(_modelsEntity, *m3d);
	addWheels(_modelsEntity, *m3d);
}

void SceneController::setC3D(const model::C3D& c3d)
{
	for(Qt3DCore::QNode* child: _modelsEntity->childNodes()){
		child->deleteLater();
	}

	auto ptr = QSharedPointer<model::C3D>::create(c3d);
	C3DMesh* c3dMesh = new C3DMesh(ptr);

	Qt3DCore::QTransform *c3dTransform = new Qt3DCore::QTransform();
	c3dTransform->setScale(1/16.0f);

	Qt3DExtras::QPerVertexColorMaterial *c3dMaterial = new Qt3DExtras::QPerVertexColorMaterial();

	auto* c3dEntity = new Qt3DCore::QEntity(_modelsEntity);
	c3dEntity->addComponent(c3dMesh);
	c3dEntity->addComponent(c3dMaterial);
	c3dEntity->addComponent(c3dTransform);


}

void SceneController::resetView()
{
	auto* camera = _view->camera();
	camera->setPosition(QVector3D(0, 20.0f, 0));
	camera->setUpVector(QVector3D(0, 0, 1.0f));
	camera->setViewCenter(QVector3D(0, 0, 0));

}


