#include <QFile>
#include <iostream>
#include <string>
#include <vlc/vlcbinaryreaderext.h>
#include <vlc/vlcjsonext.h>


using namespace vangers::core::binario;
using namespace vlc;

int convert(const std::string& from_file, const std::string& to_file){

	QFile f(from_file.c_str());
	f.open(QFile::ReadOnly);
	BinaryReader reader(&f);
	vlc::Vlc v;
	if(!reader.tryRead(v)) return 1;
	f.close();

	nlohmann::json j;
	to_json(j, v);

	QFile fo(to_file.c_str());
	fo.open(QFile::WriteOnly);

	std::string s = j.dump(4);
	fo.write(s.c_str());
	fo.close();
	return 0;
}

int main(int argc, char *argv[]){
	if(argc == 3){
		return convert(argv[1], argv[2]);
	}
//	if(argc > 1){
//		return cli_convert(argc, argv);
//	} else {
//		return gui_convert(argc, argv);
//	}
	return 1;
}
