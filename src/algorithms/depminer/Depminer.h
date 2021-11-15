#pragma once

#include "CSVParser.h"
#include "FDAlgorithm.h"
#include "depminer/CMAXSet.h"

class Depminer : public FDAlgorithm {
private:
    void lhsForColumn(std::unique_ptr<Column> const& column, std::vector<CMAXSet> const& cmaxSets);
    CMAXSet genFirstLevel(std::vector<CMAXSet> const& cmaxSets, Column attribute, std::unordered_set<Vertical>& li) const;
    std::unordered_set<Vertical> genNextLevel(std::unordered_set<Vertical> const& li) const;
    static bool checkJoin(Vertical const& _p, Vertical const& _q);
    std::vector<CMAXSet> generateCMAXSets(std::unordered_set<Vertical> const& agreeSets);
    double progressStep;
public:
    
    explicit Depminer(std::filesystem::path const& path, char separator = ',', bool hasHeader = true) 
        : FDAlgorithm(path, separator, hasHeader, {"AgreeSets generation", "Finding CMAXSets", "Finding LHS"}) {}

    unsigned long long execute() override;
    std::unique_ptr<ColumnLayoutRelationData> relation;
    const RelationalSchema* schema;
};