#ifndef SERIALIZATIONFACADEIMPL_H
#define SERIALIZATIONFACADEIMPL_H

#include "model/serializationfacade.h"

class SerializationFacadeImpl : public SerializationFacade
{
public:
	SerializationFacadeImpl();
	~SerializationFacadeImpl() = default;

    void serializeSimulationContext(SimulationUnitContext* context, QDataStream& stream) const override;
    void deserializeSimulationContext(SimulationUnitContext* prevContext, QDataStream& stream) const override;

	void serializeSimulationParameters(SimulationParameters* parameters, QDataStream& stream) const override;
	SimulationParameters* deserializeSimulationParameters(QDataStream& stream) const override;

	void serializeSymbolTable(SymbolTable* symbolTable, QDataStream& stream) const override;
	SymbolTable* deserializeSymbolTable(QDataStream& stream) const override;

    void serializeCellCluster(CellCluster* cluster, QDataStream& stream) const override;
    virtual CellCluster* deserializeCellCluster(QDataStream& stream, SimulationUnitContext* context) const override;

    void serializeFeaturedCell(Cell* cell, QDataStream& stream) const override;
    Cell* deserializeFeaturedCell(QDataStream& stream, SimulationUnitContext* context) const override;

    void serializeEnergyParticle(EnergyParticle* particle, QDataStream& stream) const override;
    EnergyParticle* deserializeEnergyParticle(QDataStream& stream, SimulationUnitContext* context) const override;

    void serializeToken(Token* token, QDataStream& stream) const override;
    Token* deserializeToken(QDataStream& stream, SimulationUnitContext* context) const override;

private:
    CellCluster* deserializeCellCluster(QDataStream& stream, QMap< quint64, quint64 >& oldNewClusterIdMap
        , QMap< quint64, quint64 >& oldNewCellIdMap, QMap< quint64, Cell* >& oldIdCellMap
        , SimulationUnitContext* context) const;

    EnergyParticle* deserializeEnergyParticle(QDataStream& stream
        , QMap< quint64, EnergyParticle* >& oldIdEnergyMap, SimulationUnitContext* context) const;

    Cell* deserializeFeaturedCell(QDataStream& stream, QMap< quint64
        , QList< quint64 > >& connectingCells, SimulationUnitContext* context) const;
};

#endif //SERIALIZATIONFACADEIMPL_H