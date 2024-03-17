#include "Chunk.h"
#include "voxel.h"

#include <cmath>


Chunk::Chunk(int xPos, int yPos, int zPos) : x(xPos), y(yPos), z(zPos)
{
    voxels = new voxel[CHUNK_VOL];
    for (int _y = 0; _y < CHUNK_H; _y++)
    {
        for (int _z = 0; _z < CHUNK_D; _z++)
        {
            for (int _x = 0; _x < CHUNK_W; _x++)
            {
                const int real_x = _x + x * CHUNK_W;
                const int real_y = _y + y * CHUNK_H;
                // const int real_z = _z + this->z * CHUNK_D;
                uint8_t id = real_y <= (std::sin(real_x * 0.1f) * 0.5f + 0.5f) * 10;
                if (real_y <= 2)
                    id = 2;
                voxels[(_y * CHUNK_D + _z) * CHUNK_W + _x].id = id;
            }
        }
    }
}

Chunk::~Chunk()
{
    delete[] voxels;
}
