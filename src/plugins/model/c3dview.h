#ifndef C3DVIEW_H
#define C3DVIEW_H

#include <QObject>
#include <QVector3D>
#include <Qt3DCore/QEntity>

#include <plugins/model/m3d/m3d.h>

namespace vangers::model::view {
	using namespace vangers::model::m3d;

	class C3DView : public Qt3DCore::QNode
	{
		Q_OBJECT
	public:
		explicit C3DView(const C3D& c3d, const QVector3D& position, Qt3DCore::QNode* parentEntity);

		void setVisible(bool visible);
	signals:
	private:
		void constructHierarchy(const C3D& c3d, const QVector3D& position);

		Qt3DCore::QEntity *_rootEntity;
	};

}

#endif // C3DVIEW_H
