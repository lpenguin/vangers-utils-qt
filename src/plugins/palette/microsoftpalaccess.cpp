#include "microsoftpalaccess.h"
#include "binaryio.h"

// https://worms2d.info/Palette_file

bool vangers::MicrosoftPalAccess::read(vangers::Palette& palette, QIODevice &device)
{
    BinaryReader reader(&device);
    if(reader.read(4) != "RIFF"){
		return false;
    }

    int32_t fileSize1 = reader.read<int32_t>();
    if(reader.read(4) != "PAL "){
		return false;
    }

    while(!device.atEnd()){
        auto chunkType = reader.read(4);
        int32_t chunkSize = reader.read<int32_t>();
        if(chunkType != "data"){
            reader.read(chunkSize);
            continue;
        }

		palette.clear();

        int16_t version = reader.read<int16_t>();
        int16_t numEntries = reader.read<int16_t>();
        for(int i = 0; i < numEntries; i++){
            int8_t red = reader.read<int8_t>();
            int8_t green = reader.read<int8_t>();
            int8_t blue = reader.read<int8_t>();
            int8_t flags = reader.read<int8_t>();
			palette.append(qRgb(red, green, blue));
        }

		return true;
    }
	return false;
}

void vangers::MicrosoftPalAccess::write(const Palette &resource, QIODevice &device)
{
	int32_t length = 4 + 4 + 4 + 4 + 2 + 2 + resource.size() * 4;
    BinaryWriter writer(&device);
    writer.write("RIFF", 4);
    writer.write<int32_t>(length);
    writer.write("PAL ", 4);

    writer.write("data", 4);
	writer.write<int32_t>(resource.size() * 4 + 4);
    writer.write<int16_t>(0x0300);
	writer.write<int16_t>(resource.size());
	for(const QRgb& color: resource){
        writer.write<int8_t>(qRed(color));
        writer.write<int8_t>(qGreen(color));
        writer.write<int8_t>(qBlue(color));
        writer.write<int8_t>(0);
    }
}
