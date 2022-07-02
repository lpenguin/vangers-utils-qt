#ifndef C3DVIEW_H
#define C3DVIEW_H

#include <QObject>
#include <QVector3D>
#include <Qt3DCore/QEntity>

#include <m3d/m3d.h>

namespace vangers::plugins::model{
	class C3DView : public Qt3DCore::QNode
	{
		Q_OBJECT
	public:
		explicit C3DView(const m3d::C3D& c3d, const QVector3D& position, Qt3DCore::QNode* parentEntity);

		void setVisible(bool visible);
	signals:
	private:
		void constructHierarchy(const m3d::C3D& c3d, const QVector3D& position);

		Qt3DCore::QEntity *_rootEntity;
	};

}

#endif // C3DVIEW_H
