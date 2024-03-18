#include "Chunk.h"
// standard
#include <cmath>
//other
#include <glm/gtc/noise.hpp>
// engine
#include "voxel.h"


Chunk::Chunk(int xPos, int yPos, int zPos) : x(xPos), y(yPos), z(zPos)
{
    voxels = new voxel[CHUNK_VOL];

    for (int z = 0; z < CHUNK_D; z++)
    {
        for (int x = 0; x < CHUNK_W; x++)
        {
            const int real_x = x + this->x * CHUNK_W;
            const int real_z = z + this->z * CHUNK_D;
            for (int y = 0; y < CHUNK_H; y++)
            {
                const int real_y = y + this->y * CHUNK_H;
                uint8_t id = perlin(glm::vec3(real_x * 0.0125f, real_y * 0.0125f, real_z * 0.0125f)) > 0.1f;
                if (real_y <= 2)
                    id = 145;
                voxels[(y * CHUNK_D + z) * CHUNK_W + x].id = id;
            }
        }
    }
}

Chunk::~Chunk()
{
    delete[] voxels;
}
