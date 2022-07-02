#include "slotview.h"

#include <Qt3DCore/QTransform>
#include <Qt3DExtras/QConeMesh>
#include <Qt3DExtras/QPhongMaterial>
#include <QVector3D>
#include <QColor>

using namespace vangers::plugins::model;
using namespace m3d;

SlotView::SlotView(Qt3DCore::QEntity* parentNode, QObject* parent)
	: QObject(parent)
	, _rootEntity(parentNode)
	, _entity(nullptr)
{
}

void SlotView::setVisible(bool visible)
{
	_entity->setEnabled(visible);
}

void SlotView::setSlot(const Slot& slot_)
{
	Qt3DExtras::QConeMesh *slotMesh = new Qt3DExtras::QConeMesh();
	slotMesh->setBottomRadius(0.3f);
	slotMesh->setTopRadius(0);
	slotMesh->setRings(100);
	slotMesh->setSlices(20);

	QVector3D slotPosition(
				slot_.rSlots.x,
				slot_.rSlots.y,
				slot_.rSlots.z
				);

	Qt3DCore::QTransform *slotTransform = new Qt3DCore::QTransform();
	slotTransform->setScale(1.0f);
	slotTransform->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(1.0f, 0.0f, 0.0f), 90.0f));
	slotTransform->setTranslation(slotPosition/ 16.0f);

	Qt3DExtras::QPhongMaterial *material= new Qt3DExtras::QPhongMaterial();
	material->setDiffuse(QColor(QRgb(0x928327)));
	material->setSpecular(QColorConstants::Black);

	_entity = new Qt3DCore::QEntity(_rootEntity);
	_entity->addComponent(slotMesh);
	_entity->addComponent(material);
	_entity->addComponent(slotTransform );
}
