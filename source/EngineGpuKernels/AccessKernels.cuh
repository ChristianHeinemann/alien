#pragma once

#include "cuda_runtime_api.h"
#include "sm_60_atomic_functions.h"

#include "AccessTOs.cuh"
#include "Base.cuh"
#include "Map.cuh"
#include "EntityFactory.cuh"
#include "CleanupKernels.cuh"

#include "SimulationData.cuh"

#define SELECTION_RADIUS 30

__device__ void copyString(
    int& targetLen,
    int& targetStringIndex,
    int sourceLen,
    char* sourceString,
    int& numStringBytes,
    char*& stringBytes)
{
    targetLen = sourceLen;
    if (sourceLen > 0) {
        targetStringIndex = atomicAdd(&numStringBytes, sourceLen);
        for (int i = 0; i < sourceLen; ++i) {
            stringBytes[targetStringIndex + i] = sourceString[i];
        }
    }
}

__global__ void getClusterAccessData(int2 universeSize, int2 rectUpperLeft, int2 rectLowerRight,
    Array<Cluster*> clusters, DataAccessTO dataTO)
{
    PartitionData clusterBlock =
        calcPartition(clusters.getNumEntries(), blockIdx.x, gridDim.x);

    for (int clusterIndex = clusterBlock.startIndex; clusterIndex <= clusterBlock.endIndex; ++clusterIndex) {

        auto const& cluster = clusters.at(clusterIndex);
        if (nullptr == cluster) {
            continue;
        }

        PartitionData cellBlock = calcPartition(cluster->numCellPointers, threadIdx.x, blockDim.x);

        __shared__ bool containedInRect;
        __shared__ MapInfo map;
        if (0 == threadIdx.x) {
            containedInRect = false;
            map.init(universeSize);
        }
        __syncthreads();

        for (auto cellIndex = cellBlock.startIndex; cellIndex <= cellBlock.endIndex; ++cellIndex) {
            auto pos = cluster->cellPointers[cellIndex]->absPos;
            map.mapPosCorrection(pos);
            if (isContainedInRect(rectUpperLeft, rectLowerRight, pos)) {
                containedInRect = true;
            }
        }
        __syncthreads();

        if (containedInRect) {
            __shared__ int cellTOIndex;
            __shared__ int tokenTOIndex;
            __shared__ CellAccessTO* cellTOs;
            __shared__ TokenAccessTO* tokenTOs;

            if (0 == threadIdx.x) {
                int clusterAccessIndex = atomicAdd(dataTO.numClusters, 1);
                cellTOIndex = atomicAdd(dataTO.numCells, cluster->numCellPointers);
                cellTOs = &dataTO.cells[cellTOIndex];

                tokenTOIndex = atomicAdd(dataTO.numTokens, cluster->numTokenPointers);
                tokenTOs = &dataTO.tokens[tokenTOIndex];

                ClusterAccessTO& clusterTO = dataTO.clusters[clusterAccessIndex];
                clusterTO.id = cluster->id;
                clusterTO.pos = { 0, 0};
                clusterTO.vel = {0, 0};
                clusterTO.angle = 0;
                clusterTO.angularVel = 0;
                clusterTO.numCells = cluster->numCellPointers;
                clusterTO.numTokens = cluster->numTokenPointers;
                clusterTO.cellStartIndex = cellTOIndex;
                clusterTO.tokenStartIndex = tokenTOIndex;

                copyString(
                    clusterTO.metadata.nameLen,
                    clusterTO.metadata.nameStringIndex,
                    cluster->metadata.nameLen,
                    cluster->metadata.name,
                    *dataTO.numStringBytes,
                    dataTO.stringBytes);
            }
            __syncthreads();

            cluster->tagCellByIndex_block(cellBlock);

            for (auto cellIndex = cellBlock.startIndex; cellIndex <= cellBlock.endIndex; ++cellIndex) {
                Cell& cell = *cluster->cellPointers[cellIndex];
                CellAccessTO& cellTO = cellTOs[cellIndex];
                cellTO.id = cell.id;
                cellTO.pos = cell.absPos;
                cellTO.energy = cell.getEnergy_safe();
                cellTO.maxConnections = cell.maxConnections;
                cellTO.numConnections = cell.numConnections;
                cellTO.branchNumber = cell.branchNumber;
                cellTO.tokenBlocked = cell.tokenBlocked;
                cellTO.cellFunctionType = cell.getCellFunctionType();
                cellTO.numStaticBytes = cell.numStaticBytes;
                cellTO.tokenUsages = cell.tokenUsages;
                cellTO.metadata.color = cell.metadata.color;

                copyString(
                    cellTO.metadata.nameLen,
                    cellTO.metadata.nameStringIndex,
                    cell.metadata.nameLen,
                    cell.metadata.name,
                    *dataTO.numStringBytes,
                    dataTO.stringBytes);
                copyString(
                    cellTO.metadata.descriptionLen,
                    cellTO.metadata.descriptionStringIndex,
                    cell.metadata.descriptionLen,
                    cell.metadata.description,
                    *dataTO.numStringBytes,
                    dataTO.stringBytes);
                copyString(
                    cellTO.metadata.sourceCodeLen,
                    cellTO.metadata.sourceCodeStringIndex,
                    cell.metadata.sourceCodeLen,
                    cell.metadata.sourceCode,
                    *dataTO.numStringBytes,
                    dataTO.stringBytes);

                for (int i = 0; i < MAX_CELL_STATIC_BYTES; ++i) {
                    cellTO.staticData[i] = cell.staticData[i];
                }
                cellTO.numMutableBytes = cell.numMutableBytes;
                for (int i = 0; i < MAX_CELL_MUTABLE_BYTES; ++i) {
                    cellTO.mutableData[i] = cell.mutableData[i];
                }
                for (int i = 0; i < cell.numConnections; ++i) {
                    int connectingCellIndex = cell.connections[i].cell->tag + cellTOIndex;
                    cellTO.connectionIndices[i] = connectingCellIndex;
                }
            }

            PartitionData tokenBlock = calcPartition(cluster->numTokenPointers, threadIdx.x, blockDim.x);
            for (auto tokenIndex = tokenBlock.startIndex; tokenIndex <= tokenBlock.endIndex; ++tokenIndex) {
                Token const& token = *cluster->tokenPointers[tokenIndex];
                TokenAccessTO& tokenTO = tokenTOs[tokenIndex];
                tokenTO.energy = token.getEnergy();
                for (int i = 0; i < cudaSimulationParameters.tokenMemorySize; ++i) {
                    tokenTO.memory[i] = token.memory[i];
                }
                int tokenCellIndex = token.cell->tag + cellTOIndex;
                tokenTO.cellIndex = tokenCellIndex;
            }
        }
    }
}

__global__ void getParticleAccessData(int2 rectUpperLeft, int2 rectLowerRight,
    SimulationData data, DataAccessTO access)
{
    PartitionData particleBlock =
        calcPartition(data.entities.particlePointers.getNumEntries(), threadIdx.x + blockIdx.x * blockDim.x, blockDim.x * gridDim.x);

    for (int particleIndex = particleBlock.startIndex; particleIndex <= particleBlock.endIndex; ++particleIndex) {
        auto const& particle = *data.entities.particlePointers.at(particleIndex);
        if (isContainedInRect(rectUpperLeft, rectLowerRight, particle.absPos)) {
            int particleAccessIndex = atomicAdd(access.numParticles, 1);
            ParticleAccessTO& particleAccess = access.particles[particleAccessIndex];

            particleAccess.id = particle.id;
            particleAccess.pos = particle.absPos;
            particleAccess.vel = particle.vel;
            particleAccess.energy = particle.getEnergy();
        }
    }
}

__device__ void filterCluster(int2 const& rectUpperLeft, int2 const& rectLowerRight,
    Array<Cluster*> clusters, int clusterIndex)
{
    auto& cluster = clusters.at(clusterIndex);

    PartitionData cellBlock =
        calcPartition(cluster->numCellPointers, threadIdx.x, blockDim.x);

    __shared__ bool containedInRect;
    if (0 == threadIdx.x) {
        containedInRect = false;
    }
    __syncthreads();

    for (auto cellIndex = cellBlock.startIndex; cellIndex <= cellBlock.endIndex; ++cellIndex) {
        Cell const& cell = *cluster->cellPointers[cellIndex];
        if (isContainedInRect(rectUpperLeft, rectLowerRight, cell.absPos)) {
            containedInRect = true;
        }
    }
    __syncthreads();

    if (containedInRect && 0 == threadIdx.x) {
        cluster = nullptr;
    }
    __syncthreads();
}

__device__ void filterParticle(int2 const& rectUpperLeft, int2 const& rectLowerRight,
    Array<Particle*> particles, int particleIndex)
{
    auto& particle = particles.getArrayForDevice()[particleIndex];
    if (isContainedInRect(rectUpperLeft, rectLowerRight, particle->absPos)) {
        particle = nullptr;
    }
}

__global__ void filterClusters(int2 rectUpperLeft, int2 rectLowerRight, Array<Cluster*> clusters)
{
    PartitionData clusterBlock = calcPartition(clusters.getNumEntries(), blockIdx.x, gridDim.x);
    for (int clusterIndex = clusterBlock.startIndex; clusterIndex <= clusterBlock.endIndex; ++clusterIndex) {
        filterCluster(rectUpperLeft, rectLowerRight, clusters, clusterIndex);
    }
    __syncthreads();
}

__global__ void filterParticles(int2 rectUpperLeft, int2 rectLowerRight, Array<Particle*> particles)
{
    PartitionData particleBlock =
        calcPartition(particles.getNumEntries(), threadIdx.x + blockIdx.x * blockDim.x, blockDim.x * gridDim.x);
    for (int particleIndex = particleBlock.startIndex; particleIndex <= particleBlock.endIndex; ++particleIndex) {
        filterParticle(rectUpperLeft, rectLowerRight, particles, particleIndex);
    }
    __syncthreads();
}


__global__ void createDataFromTO(SimulationData data, DataAccessTO simulationTO)
{
    __shared__ EntityFactory factory;
    if (0 == threadIdx.x) {
        factory.init(&data);
    }
    __syncthreads();

    PartitionData clusterBlock = calcPartition(*simulationTO.numClusters, blockIdx.x, gridDim.x);

    for (int clusterIndex = clusterBlock.startIndex; clusterIndex <= clusterBlock.endIndex; ++clusterIndex) {
        factory.createClusterFromTO_block(simulationTO.clusters[clusterIndex], &simulationTO);
    }

    PartitionData particleBlock =
        calcPartition(*simulationTO.numParticles, threadIdx.x + blockIdx.x * blockDim.x, blockDim.x * gridDim.x);
    for (int particleIndex = particleBlock.startIndex; particleIndex <= particleBlock.endIndex; ++particleIndex) {
        factory.createParticleFromTO(simulationTO.particles[particleIndex]);
    }
}

__global__ void selectClusters(int2 pos, Array<Cluster*> clusters)
{
    auto const clusterBlock = calcPartition(clusters.getNumEntries(), blockIdx.x, gridDim.x);
    for (int clusterIndex = clusterBlock.startIndex; clusterIndex <= clusterBlock.endIndex; ++clusterIndex) {
        auto& cluster = clusters.at(clusterIndex);

        auto const cellBlock = calcPartition(cluster->numCellPointers, threadIdx.x, blockDim.x);
        for (auto cellIndex = cellBlock.startIndex; cellIndex <= cellBlock.endIndex; ++cellIndex) {
            auto const& cell = cluster->cellPointers[cellIndex];
            if (Math::lengthSquared(cell->absPos - pos) < SELECTION_RADIUS) {
                cell->cluster->setSelected(true);
            }
        }
    }
}

__global__ void selectParticles(int2 pos, Array<Particle*> particles)
{
    auto const particleBlock = calcPartition(particles.getNumEntries(), threadIdx.x + blockIdx.x * blockDim.x, blockDim.x * gridDim.x);
    for (int index = particleBlock.startIndex; index <= particleBlock.endIndex; ++index) {
        auto const& particle = particles.at(index);

        if (Math::lengthSquared(particle->absPos - pos) < SELECTION_RADIUS) {
            particle->setSelected(true);
        }
    }
}

__global__ void deselectClusters(Array<Cluster*> clusters)
{
    auto const clusterBlock = calcPartition(clusters.getNumEntries(), threadIdx.x + blockIdx.x * blockDim.x, blockDim.x * gridDim.x);
    for (int clusterIndex = clusterBlock.startIndex; clusterIndex <= clusterBlock.endIndex; ++clusterIndex) {
        auto const& cluster = clusters.at(clusterIndex);
        cluster->setSelected(false);
    }
}

__global__ void deselectParticles(Array<Particle*> particles)
{
    auto const particleBlock = calcPartition(particles.getNumEntries(), threadIdx.x + blockIdx.x * blockDim.x, blockDim.x * gridDim.x);
    for (int index = particleBlock.startIndex; index <= particleBlock.endIndex; ++index) {
        auto const& particle = particles.at(index);
        particle->setSelected(false);
    }
}


/************************************************************************/
/* Main      															*/
/************************************************************************/

__global__ void cudaGetSimulationAccessData(int2 rectUpperLeft, int2 rectLowerRight,
    SimulationData data, DataAccessTO access)
{
    *access.numClusters = 0;
    *access.numCells = 0;
    *access.numParticles = 0;
    *access.numTokens = 0;
    *access.numStringBytes = 0;

    KERNEL_CALL(getClusterAccessData, data.size, rectUpperLeft, rectLowerRight, data.entities.clusterPointers, access);
    KERNEL_CALL(getClusterAccessData, data.size, rectUpperLeft, rectLowerRight, data.entities.clusterFreezedPointers, access);
    KERNEL_CALL(getParticleAccessData, rectUpperLeft, rectLowerRight, data, access);
}

__global__ void cudaSetSimulationAccessData(int2 rectUpperLeft, int2 rectLowerRight,
    SimulationData data, DataAccessTO access)
{
    KERNEL_CALL_1_1(unfreeze, data);
    data.entities.clusterFreezedPointers.reset();

    KERNEL_CALL(filterClusters, rectUpperLeft, rectLowerRight, data.entities.clusterPointers);
    KERNEL_CALL(filterParticles, rectUpperLeft, rectLowerRight, data.entities.particlePointers);
    KERNEL_CALL_1_1(cleanupAfterDataManipulation, data);
    KERNEL_CALL(createDataFromTO, data, access);
}

__global__ void cudaClearData(SimulationData data)
{
    data.entities.clusterFreezedPointers.reset();
    data.entities.clusterPointers.reset();
    data.entities.cellPointers.reset();
    data.entities.tokenPointers.reset();
    data.entities.particlePointers.reset();
    data.entities.clusters.reset();
    data.entities.cells.reset();
    data.entities.tokens.reset();
    data.entities.particles.reset();
}

__global__ void cudaSelectData(int2 pos, SimulationData data)
{
    KERNEL_CALL(selectClusters, pos, data.entities.clusterPointers);
    KERNEL_CALL(selectClusters, pos, data.entities.clusterFreezedPointers);
    KERNEL_CALL(selectParticles, pos, data.entities.particlePointers);
}

__global__ void cudaDeselectData(SimulationData data)
{
    KERNEL_CALL(deselectClusters, data.entities.clusterPointers);
    KERNEL_CALL(deselectClusters, data.entities.clusterFreezedPointers);
    KERNEL_CALL(deselectParticles, data.entities.particlePointers);
}