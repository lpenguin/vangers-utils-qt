#ifndef TERRAINIMAGELAYER_H
#define TERRAINIMAGELAYER_H

#include "abstractimagelayer.h"
#include <vangers/core//palette/palette.h>

namespace vangers::plugins::vmap {
	class TerrainImageLayer : public AbstractImageLayer
	{
	public:
		TerrainImageLayer(QObject *parent = nullptr);

		// AbstractImageLayer interface
	public:
		QSharedPointer<QImage> getImage(const  Vmap& vmap, Level level) const;
		vangers::core::palette::Palette getPalette(const Vmap &vmap) const;
		void fillData(const Vmap &vmap, Level level, std::vector<uint8_t>& data) const;
	};

}

#endif // TERRAINIMAGELAYER_H
