#include "vmapaccess.h"
#include <QSharedPointer>
#include <splay/splay.h>
#include <QSettings>
#include <binaryio.h>
#include <span.h>

using namespace vangers;

QSharedPointer<Vmap> VmapAccess::read(QIODevice &device)
{
    BinaryReader reader(&device);

    int32_t offsets[_sizeY];
    int16_t sizes[_sizeY];

    for(int i = 0; i < _sizeY; i++){
        offsets[i] = reader.read<int32_t>();
        sizes[i] = reader.read<int16_t>();
    }

    Splay splay(&device);
    std::vector<uint8_t> dataHeight(_sizeX);
    dataHeight.resize(_sizeX);
    std::vector<uint8_t> dataMeta(_sizeX);
    dataMeta.resize(_sizeX);

	Matrix<uint8_t> heights(_sizeX, _sizeY);
	Matrix<uint8_t> metas(_sizeX, _sizeY);

    for(int iRow = 0; iRow < _sizeY; iRow++){
        if(!device.seek(offsets[iRow])){
            return {};
        }

        std::vector<uint8_t> bytes = reader.readArray<uint8_t>(sizes[iRow]);
        splay.expand(span<uint8_t>{bytes}, dataHeight, dataMeta);

		std::copy(dataHeight.data(), dataHeight.data() + _sizeX, heights.data() + (iRow * _sizeX));
		std::copy(dataMeta.data(), dataMeta.data() + _sizeX, metas.data() + (iRow * _sizeX));
//		std::copy(dataMeta.begin(), dataMeta.end(), metas.begin() + (iRow * _sizeX));
    }

	return QSharedPointer<Vmap>::create(_sizeX, _sizeY, heights, metas, _palette, _terrainColorOffsets);
}

void VmapAccess::write(const QSharedPointer<Vmap> &resource, QIODevice &device)
{

}
