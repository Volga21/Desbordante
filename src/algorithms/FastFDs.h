#pragma once

#include <functional>

#include "Vertical.h"
#include "FDAlgorithm.h"
#include "ColumnLayoutRelationData.h"



class FastFDs : public FDAlgorithm {
public:
    enum class AgreeSetsGenMethod {
        kUsingVectorOfIDSets = 0,
        kUsingMapOfIDSets       ,
        kUsingGetAgreeSet       , // Simplest method by Wyss et al
        kUsingMCAndGetAgreeSet  , // From maximal representation using getAgreeSet()
    };

    using FDAlgorithm::FDAlgorithm;
    unsigned long long execute() override;
private:
    using OrderingComparator = std::function<bool (Column const&, Column const&)>;

    // Computes all difference sets of `relation_` by complementing agree sets
    void genDiffSets();

    // Computes all agree sets of `relation_`
    template<AgreeSetsGenMethod method = AgreeSetsGenMethod::kUsingVectorOfIDSets>
    std::set<std::shared_ptr<Vertical>> genAgreeSets() const;

    /* Computes minimal difference sets
     * of `relation_` modulo `col`
     */
    std::vector<std::shared_ptr<Vertical>> getDiffSetsMod(Column const& col) const;
    std::shared_ptr<Vertical> getAgreeSet(std::vector<int> const& tuple1,
                                          std::vector<int> const& tuple2) const;
    /* Returns initial ordering,
     * the total ordering of { schema_->getColumns() \ `attribute` } according to `diff_sets`
     */
    std::set<Column, OrderingComparator> getInitOrdering(std::vector<std::shared_ptr<Vertical>> const& diff_sets,
                                                         Column const& attribute) const;
    /* Returns next ordering,
     * the total ordering of { B in schema_->getColumns() | B > `attribute` (in `cur_ordering`) }
     * according to `diff_sets`
     */
    std::set<Column, OrderingComparator> getNextOrdering(std::vector<std::shared_ptr<Vertical>> const& diff_sets,
                                                         Column const& attribute,
                                                         std::set<Column, OrderingComparator> const& cur_ordering) const;
    std::set<std::vector<int>> getPLIMaxRepresentation() const;
    void findCovers(Column const& attribute, std::vector<std::shared_ptr<Vertical>> const& diff_sets_mod,
                    std::vector<std::shared_ptr<Vertical>> const& cur_diff_sets, Vertical const& path,
                    std::set<Column, OrderingComparator> const& ordering);
    /* Returns true if `cover` is the minimal cover of `diff_sets_mod`,
     * false otherwise
     */
    bool coverMinimal(Vertical const& cover, std::vector<std::shared_ptr<Vertical>> const& diff_sets_mod) const;
    /* Returns true if `candidate` covers `sets`,
     * false otherwise
     */
    bool isCover(Vertical const& candidate, std::vector<std::shared_ptr<Vertical>> const& sets) const;
    // static non member function?
    void calculateSupersets(std::set<std::vector<int>>& max_representation,
                            std::deque<std::vector<int>> partition) const;
    /* Returns true if `l_col` > `r_col`,
     * false otherwise.
     * `l_col` > `r_col` iff
     * `l_col` covers more sets in `diff_sets` than `r_col` or
     * `l_col` and `r_col` cover the same number of sets but
     * `l_col` index less than `r_col` index
     */
    bool orderingComp(vector<shared_ptr<Vertical>> const& diff_sets,
                      Column const& l_col, Column const& r_col) const;
    bool columnContainsOnlyEqualValues(Column const& column) const;

    std::shared_ptr<ColumnLayoutRelationData> relation_;
    shared_ptr<RelationalSchema> schema_;
    std::vector<std::shared_ptr<Vertical>> diff_sets_;
};
