#include "splay.h"
#include <vangers/core//binaryio.h>


using namespace vangers::core::binario;
using namespace vangers::core::span;
using namespace splay;

Splay::Splay(QIODevice* device)
{
    readSplayTree(_splayTree, device);
}

void Splay::expand(const span<uint8_t> &input, std::vector<uint8_t> &output1, std::vector<uint8_t> &output2)
{
    auto sum = [](uint8_t a, uint8_t b) {return (a + b);};
    auto xor_ = [](uint8_t a, uint8_t b) {return (a ^ b);};

    size_t off1 = decompress(_splayTree.tree1, input, output1, sum);
    size_t off2 = decompress(_splayTree.tree2, input.sub(off1), output2, xor_);
    assert(off1 + off2 == input.size());
}

void Splay::readSplayTree(SplayTree &splayTree, QIODevice *device)
{
    BinaryReader reader(device);

    for(int i = 0; i < 512; i++){
        splayTree.tree1[i] = reader.read<int32_t>();
    }

    for(int i = 0; i < 512; i++){
        splayTree.tree2[i] = reader.read<int32_t>();
    }

}

void Splay::writeSplayTree(const SplayTree &splayTree, QIODevice *device)
{
    BinaryWriter writer(device);
    for(int i = 0; i < 512; i++){
        writer.write<int32_t>(splayTree.tree1[i]);
    }

    for(int i = 0; i < 512; i++){
        writer.write<int32_t>(splayTree.tree2[i]);
    }
}

size_t Splay::decompress(const int32_t tree[],
        const span<uint8_t> &input,
        std::vector<uint8_t>& output,
        const std::function<uint8_t (uint8_t, uint8_t)> &fun)
{

    size_t kInput = 0;
    uint8_t lastChar = 0;
    int bit = 0;
    uint8_t cur = 0;

    for(uint8_t& out: output){
        int32_t code = 1;
        while(code > 0){
            if(bit == 0){
                cur = input[kInput];
                kInput += 1;
                bit = 7;
            } else {
                bit -= 1;
            }

            auto i = (code << 1) + ((cur >> bit) & 1);
            code = tree[i];
        }

        lastChar = fun(lastChar, static_cast<uint8_t>(-code));

        out = lastChar;
    }

    return kInput;
}
