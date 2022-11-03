#ifndef SLOTVIEW_H
#define SLOTVIEW_H

#include <Qt3DCore/QNode>
#include <Qt3DCore/QEntity>

#include <m3d/m3d.h>

namespace vangers::plugins::model{
	class SlotView : public QObject {
	public:
		SlotView(Qt3DCore::QEntity* parentNode, QObject* parent);
		void setVisible(bool visible);
		void setSlot(const m3d::Slot& slot);
	private:
		Qt3DCore::QEntity *_rootEntity;
		Qt3DCore::QEntity* _entity;
	};
}


#endif // SLOTVIEW_H
