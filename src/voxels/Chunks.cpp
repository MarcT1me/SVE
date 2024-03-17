#include "Chunks.h"
#include "Chunk.h"


Chunks::Chunks(int w, int h, int d) : w(w), h(h), d(d)
{
    volume = w * h * d; // NOLINT(bugprone-implicit-widening-of-multiplication-result)
    chunks = new Chunk*[volume];

    int index = 0;
    for (int y = 0; y < h; y++)
    {
        for (int z = 0; z < d; z++)
        {
            for (int x = 0; x < w; x++, index++)
            {
                auto* chunk = new Chunk(x, y, z);
                chunks[index] = chunk;
            }
        }
    }
}

Chunks::~Chunks()
{
    for (size_t i = 0; i < volume; i++)
    {
        delete chunks[i];
    }
    delete[] chunks;
}
