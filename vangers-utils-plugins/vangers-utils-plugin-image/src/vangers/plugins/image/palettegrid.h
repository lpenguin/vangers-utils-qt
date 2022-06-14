#ifndef PALETTEGRID_H
#define PALETTEGRID_H

#include <QWidget>
#include <vangers/core//palette/palette.h>

namespace Ui {
class PaletteGrid;
}

namespace vangers::plugins::image {
	using namespace vangers::core::palette;

	class PaletteGrid : public QWidget
	{
		Q_OBJECT

	public:
		explicit PaletteGrid(QWidget *parent = nullptr);
		~PaletteGrid();

		void setPalette(const Palette& palette);

		const Palette& vangersPalette() const {
			return _palette;
		}
	private:
		Palette _palette;
	protected:
		void paintEvent(QPaintEvent *event) override;
		void mousePressEvent(QMouseEvent *event) override;
	signals:
		void colorSelected(int colorIndex);
	};

}

#endif // PALETTEGRID_H
