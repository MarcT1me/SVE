// ReSharper disable CppUsingResultOfAssignmentAsCondition
#include "Chunks.h"
// engine
#include "Chunk.h"
#include "voxel.h"


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


Chunk* Chunks::getChunk(int x, int y, int z) const
{
    if (x < 0 || y < 0 || z < 0 || x >= w || y >= h || z >= d) // NOLINT(clang-diagnostic-sign-compare)
        return nullptr;
    return chunks[(y * d + z) * w + x];
}

voxel* Chunks::get(int x, int y, int z) const
{
    int cx = x / CHUNK_W;
    int cy = y / CHUNK_H;
    int cz = z / CHUNK_D;
    if (x < 0) cx--;
    if (y < 0) cy--;
    if (z < 0) cz--;
    if (cx < 0 || cy < 0 || cz < 0 || cx >= w || cy >= h || cz >= d) // NOLINT(clang-diagnostic-sign-compare)
        return nullptr;
    const Chunk* chunk = chunks[(cy * d + cz) * w + cx];
    const int lx = x - cx * CHUNK_W;
    const int ly = y - cy * CHUNK_H;
    const int lz = z - cz * CHUNK_D;
    // ret
    return &chunk->voxels[(ly * CHUNK_D + lz) * CHUNK_W + lx];
}

void Chunks::set(int x, int y, int z, int id) const
{
    int cx = x / CHUNK_W;
    int cy = y / CHUNK_H;
    int cz = z / CHUNK_D;
    if (x < 0) cx--;
    if (y < 0) cy--;
    if (z < 0) cz--;
    if (cx < 0 || cy < 0 || cz < 0 || cx >= w || cy >= h || cz >= d) // NOLINT(clang-diagnostic-sign-compare)
        return;
    const Chunk* chunk = chunks[(cy * d + cz) * w + cx];
    const int lx = x - cx * CHUNK_W;
    const int ly = y - cy * CHUNK_H;
    const int lz = z - cz * CHUNK_D;
    // ret
    chunk->voxels[(ly * CHUNK_D + lz) * CHUNK_W + lx].id = id;
    chunk->modified = true;

    if (lx == 0 && (chunk = getChunk(cx - 1, cy, cz))) chunk->modified = true;
    if (ly == 0 && (chunk = getChunk(cx, cy - 1, cz))) chunk->modified = true;
    if (lz == 0 && (chunk = getChunk(cx, cy, cz - 1))) chunk->modified = true;

    if (lx == CHUNK_W - 1 && (chunk = getChunk(cx + 1, cy, cz))) chunk->modified = true;
    if (ly == CHUNK_H - 1 && (chunk = getChunk(cx, cy + 1, cz))) chunk->modified = true;
    if (lz == CHUNK_D - 1 && (chunk = getChunk(cx, cy, cz + 1))) chunk->modified = true;
}

voxel* Chunks::rayCast(glm::vec3 a, glm::vec3 dir, float maxDist, glm::vec3& end, glm::vec3& norm, glm::vec3& iend) const
{
    const float px = a.x;
    const float py = a.y;
    const float pz = a.z;

    const float dx = dir.x;
    const float dy = dir.y;
    const float dz = dir.z;

    float t = 0.0f;
    int ix = floor(px);
    int iy = floor(py);
    int iz = floor(pz);

    const float stepx = dx > 0.0f ? 1.0f : -1.0f;
    const float stepy = dy > 0.0f ? 1.0f : -1.0f;
    const float stepz = dz > 0.0f ? 1.0f : -1.0f;

    const float infinity = std::numeric_limits<float>::infinity();

    const float txDelta = dx == 0.0f ? infinity : abs(1.0f / dx);
    const float tyDelta = dy == 0.0f ? infinity : abs(1.0f / dy);
    const float tzDelta = dz == 0.0f ? infinity : abs(1.0f / dz);

    const float xdist = stepx > 0 ? ix + 1 - px : px - ix;
    const float ydist = stepy > 0 ? iy + 1 - py : py - iy;
    const float zdist = stepz > 0 ? iz + 1 - pz : pz - iz;

    float txMax = txDelta < infinity ? txDelta * xdist : infinity;
    float tyMax = tyDelta < infinity ? tyDelta * ydist : infinity;
    float tzMax = tzDelta < infinity ? tzDelta * zdist : infinity;

    int steppedIndex = -1;

    while (t <= maxDist)
    {
        voxel* voxel = get(ix, iy, iz);
        if (voxel == nullptr || voxel->id)
        {
            end.x = px + t * dx;
            end.y = py + t * dy;
            end.z = pz + t * dz;

            iend.x = ix;
            iend.y = iy;
            iend.z = iz;

            norm.x = norm.y = norm.z = 0.0f;
            if (steppedIndex == 0) norm.x = -stepx;
            if (steppedIndex == 1) norm.y = -stepy;
            if (steppedIndex == 2) norm.z = -stepz;
            return voxel;
        }
        if (txMax < tyMax)
        {
            if (txMax < tzMax)
            {
                ix += stepx;
                t = txMax;
                txMax += txDelta;
                steppedIndex = 0;
            }
            else
            {
                iz += stepz;
                t = tzMax;
                tzMax += tzDelta;
                steppedIndex = 2;
            }
        }
        else
        {
            if (tyMax < tzMax)
            {
                iy += stepy;
                t = tyMax;
                tyMax += tyDelta;
                steppedIndex = 1;
            }
            else
            {
                iz += stepz;
                t = tzMax;
                tzMax += tzDelta;
                steppedIndex = 2;
            }
        }
    }
    iend.x = ix;
    iend.y = iy;
    iend.z = iz;

    end.x = px + t * dx;
    end.y = py + t * dy;
    end.z = pz + t * dz;
    norm.x = norm.y = norm.z = 0.0f;
    return nullptr;
}
