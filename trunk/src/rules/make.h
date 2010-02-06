template<class Lexeme, int Phase>
void make_p1_rules(const vector<const Tagset*>& tagsets,
        vector<PredicateTemplate<Lexeme>*>& templates) {
    templates.push_back(new NearbyTagPredicateTemplate<Lexeme, Phase, -1>(tagsets));
    templates.push_back(new NearbyTagPredicateTemplate<Lexeme, Phase, -2>(tagsets));
    templates.push_back(new Nearby2TagsPredicateTemplate<Lexeme, Phase, -1, -2>(tagsets));
    templates.push_back(new Nearby3TagsPredicateTemplate<Lexeme, Phase, -1, -2, -3>(tagsets));
    templates.push_back(new NearbyExact2TagsPredicateTemplate<Lexeme, Phase, -1, -2>(tagsets));
    templates.push_back(new NearbyExact2TagsPredicateTemplate<Lexeme, Phase, -1, 1>(tagsets));
    templates.push_back(new NearbyTagPredicateTemplate<Lexeme, Phase, 1>(tagsets));
    templates.push_back(new NearbyTagPredicateTemplate<Lexeme, Phase, 2>(tagsets));
    templates.push_back(new Nearby2TagsPredicateTemplate<Lexeme, Phase, 1, 2>(tagsets));
    templates.push_back(new Nearby3TagsPredicateTemplate<Lexeme, Phase, 1, 2, 3>(tagsets));
    templates.push_back(new NearbyExact2TagsPredicateTemplate<Lexeme, Phase, 1, 2>(tagsets));

    templates.push_back(new CCaseTagPredicateTemplate<Lexeme, Phase>(tagsets));
    templates.push_back(new Prefix2TagPredicateTemplate<Lexeme, Phase>(tagsets));
    templates.push_back(new Suffix2TagPredicateTemplate<Lexeme, Phase>(tagsets));

    templates.push_back(new Nearby1CatPredicateTemplate<Lexeme, Phase, -1>(tagsets));
    templates.push_back(new Nearby1CatPredicateTemplate<Lexeme, Phase, -2>(tagsets));
    templates.push_back(new Nearby2CatPredicateTemplate<Lexeme, Phase, -1, -2>(tagsets));
    templates.push_back(new Nearby3CatPredicateTemplate<Lexeme, Phase, -1, -2, -3>(tagsets));
    templates.push_back(new NearbyExact2CatPredicateTemplate<Lexeme, Phase, -1, -2>(tagsets));
    templates.push_back(new NearbyExact2CatPredicateTemplate<Lexeme, Phase, -1, 1>(tagsets));
    templates.push_back(new Nearby1CatPredicateTemplate<Lexeme, Phase, 1>(tagsets));
    templates.push_back(new Nearby1CatPredicateTemplate<Lexeme, Phase, 2>(tagsets));
    templates.push_back(new Nearby2CatPredicateTemplate<Lexeme, Phase, 1, 2>(tagsets));
    templates.push_back(new Nearby3CatPredicateTemplate<Lexeme, Phase, 1, 2, 3>(tagsets));
    templates.push_back(new NearbyExact2CatPredicateTemplate<Lexeme, Phase, 1, 2>(tagsets));
}

template<class Lexeme, int Phase>
RulesGenerator<Lexeme>* make_p1_rules_generator(
        const vector<const Tagset*>& tagsets) {
    vector<PredicateTemplate<Lexeme>*> ptemplates;
    make_p1_rules<Lexeme, Phase>(tagsets, ptemplates);

    vector<ActionTemplate<Lexeme>*> atemplates;
    atemplates.push_back(new ChangeTagActionTemplate<Lexeme, Phase>(tagsets));
    atemplates.push_back(new ChangeCatActionTemplate<Lexeme, Phase>(tagsets));
    atemplates.push_back(new ChangePosActionTemplate<Lexeme, Phase>(tagsets));

    return new AllPredicatesAllActionsGenerator<Lexeme>(ptemplates, atemplates);
}

template<class Lexeme, int Phase>
void make_p2_rules(const vector<const Tagset*>& tagsets,
        vector<PredicateTemplate<Lexeme>*>& templates) {

    make_p1_rules<Lexeme, Phase>(tagsets, templates);

    templates.push_back(new PartialNearbyTagPredicateTemplate<Lexeme, Phase, -1>(tagsets));
    templates.push_back(new PartialNearbyTagPredicateTemplate<Lexeme, Phase, -2>(tagsets));
    templates.push_back(new PartialNearby2TagsPredicateTemplate<Lexeme, Phase, -1, -2>(tagsets));
    templates.push_back(new PartialNearbyTagPredicateTemplate<Lexeme, Phase, 1>(tagsets));
    templates.push_back(new PartialNearbyTagPredicateTemplate<Lexeme, Phase, 2>(tagsets));
    templates.push_back(new PartialNearby2TagsPredicateTemplate<Lexeme, Phase, 1, 2>(tagsets));
}

template<class Lexeme, int Phase>
RulesGenerator<Lexeme>* make_p2_rules_generator(
        const vector<const Tagset*>& tagsets) {
    vector<PredicateTemplate<Lexeme>*> ptemplates;
    make_p2_rules<Lexeme, Phase>(tagsets, ptemplates);

    vector<ActionTemplate<Lexeme>*> atemplates;
    atemplates.push_back(new ChangeTagActionTemplate<Lexeme, Phase>(tagsets));
    atemplates.push_back(new ChangeCatActionTemplate<Lexeme, Phase>(tagsets));
    atemplates.push_back(new ChangePosActionTemplate<Lexeme, Phase>(tagsets));

    return new AllPredicatesAllActionsGenerator<Lexeme>(ptemplates, atemplates);
}
