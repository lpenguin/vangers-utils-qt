#ifndef SPLAY_H
#define SPLAY_H


#include <QIODevice>
#include <vector>
#include <cstdint>
#include "span.h"

namespace splay {
struct SplayTree
{
    int32_t tree1[512];
    int32_t tree2[512];
};

class Splay
{
public:
    Splay(QIODevice* device);
	void expand(const vangers::core::span::span<uint8_t>& input, std::vector<uint8_t>& output1, std::vector<uint8_t>& outpu2);
private:
    SplayTree _splayTree;

    static void readSplayTree(SplayTree& splayTree, QIODevice* device);
    static void writeSplayTree(const SplayTree& splayTree, QIODevice* device);
    static size_t decompress(const int32_t tree[],
							const vangers::core::span::span<uint8_t>& input,
                            std::vector<uint8_t>& output,
                            const std::function<uint8_t(uint8_t, uint8_t)>& fun);



};

}

#endif // SPLAY_H
