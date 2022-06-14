#include <Qt3DCore/QTransform>
#include <Qt3DExtras/QPerVertexColorMaterial>

#include "c3drenderer.h"
#include "c3dview.h"

using namespace m3d;
using namespace vangers::plugins::model;

C3DView::C3DView(const C3D& c3d, const QVector3D& position, Qt3DCore::QNode* parentEntity)
	: Qt3DCore::QNode(parentEntity)
	, _rootEntity(new Qt3DCore::QEntity(this))
{
	constructHierarchy(c3d, position);
}


void C3DView::setVisible(bool visible)
{
	_rootEntity->setEnabled(visible);
}

void C3DView::constructHierarchy(const C3D& c3d, const QVector3D& position)
{
	for(Qt3DCore::QNode* child: _rootEntity->childNodes()){
		child->deleteLater();
	}

	C3DMesh* c3dMesh = new C3DMesh(c3d);

	Qt3DCore::QTransform *c3dTransform = new Qt3DCore::QTransform();
	c3dTransform->setScale(1/16.0f);
	c3dTransform->setTranslation(position / 16.0f);

	Qt3DExtras::QPerVertexColorMaterial *c3dMaterial = new Qt3DExtras::QPerVertexColorMaterial();

	auto* c3dEntity = new Qt3DCore::QEntity(_rootEntity);
	c3dEntity->addComponent(c3dMesh);
	c3dEntity->addComponent(c3dMaterial);
	c3dEntity->addComponent(c3dTransform);
}
