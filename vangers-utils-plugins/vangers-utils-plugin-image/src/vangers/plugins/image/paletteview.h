#ifndef PALETTEVIEW_H
#define PALETTEVIEW_H

#include <QWidget>

namespace Ui {
class PaletteView;
}

namespace vangers::plugins::image {
	class PaletteView : public QWidget
	{
		Q_OBJECT

	public:
		explicit PaletteView(QWidget *parent = nullptr);
		~PaletteView();
		QString currentPalette() const;
		void setEnabled(bool enabled);
	signals:
		void paletteChanged(QString paletteName);
		void useTransparentColor(bool use, int index);
	private slots:
		void palette_currentChanged(const QModelIndex & current, const QModelIndex & previous);
		void paletteGrid_colorSelected(int colorIndex);
		void transparentCheckbox_clicked(bool value);
	private:
		Ui::PaletteView *ui;
	};
}


#endif // PALETTEVIEW_H
