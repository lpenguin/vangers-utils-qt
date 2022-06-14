#ifndef PNGPALETTEACCESS_H
#define PNGPALETTEACCESS_H

#include <vangers/core//palette/palette.h>
#include <vangers/core//plugin/resourceaccess.h>
#include <vangers/plugins/image/image.h>

#include <QSharedPointer>


namespace vangers::plugins::palette {
	namespace decl {
		using namespace vangers::core::palette;
	}


	class PngPaletteAccess: public vangers::core::plugin::AbstractResourceAccess<decl::Palette>
	{
	public:
		PngPaletteAccess(int nCols = 16, const QString& format = "png")
			:_nCols(nCols)
			,_format(format)
		{}
		bool read(decl::Palette& palette, QIODevice &device) override;
		void write(const decl::Palette &resource, QIODevice &device) override;
	private:
		int _nCols;
		QString _format;
	};

}


#endif // PNGPALETTEACCESS_H
