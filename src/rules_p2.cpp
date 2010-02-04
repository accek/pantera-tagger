
template<class Lexeme, int Phase, int Offset>
class PartialNearbyTagPredicateTemplate : public PredicateTemplate<Lexeme>
{
public:
PartialNearbyTagPredicateTemplate(const vector<const Tagset*> tagsets) : PredicateTemplate<Lexeme>(tagsets) { }

void findMatchingPredicates(vector<Predicate<Lexeme> >& v,
                                                      vector<Lexeme>& text,
                                                      int index) {

    Predicate<Lexeme> p = Predicate<Lexeme>(this);
    p.params.tags[0] = text[index].chosen_tag[Phase];
    p.params.tags[1] = text[index + Offset].chosen_tag[Phase - 1];
    v.push_back(p);

}
bool predicateMatches(const Predicate<Lexeme>& p,
            vector<Lexeme>& text, int index) {
    return (p.params.tags[1] == text[index + Offset].chosen_tag[Phase - 1])
            && p.params.tags[0] == text[index].chosen_tag[Phase];
}
string predicateAsString(const Predicate<Lexeme>& p) {
    char str[STR_SIZE];
    sprintf(str, "T[%d] = %s AND T[0] = %s", Offset, T(tags[1]), S(tags[0]));
    return string(str);
}
};


template<class Lexeme, int Phase, int Offset1, int Offset2>
class PartialNearby2TagsPredicateTemplate : public PredicateTemplate<Lexeme>
{
public:
PartialNearby2TagsPredicateTemplate(const vector<const Tagset*> tagsets) : PredicateTemplate<Lexeme>(tagsets) { }

void findMatchingPredicates(vector<Predicate<Lexeme> >& v,
                                                      vector<Lexeme>& text,
                                                      int index) {
    Predicate<Lexeme> p = Predicate<Lexeme>(this);
    p.params.tags[0] = text[index].chosen_tag[Phase];
    p.params.tags[1] = text[index + Offset1].chosen_tag[Phase - 1];
    v.push_back(p);
    

    if (text[index + Offset2].chosen_tag[Phase - 1] != text[index + Offset1].chosen_tag[Phase - 1]) {
        p.params.tags[1] = text[index + Offset2].chosen_tag[Phase - 1];
        v.push_back(p);
    }
}
bool predicateMatches(const Predicate<Lexeme>& p,
            vector<Lexeme>& text, int index) {
    return (p.params.tags[1] == text[index + Offset1].chosen_tag[Phase - 1] ||
            p.params.tags[1] == text[index + Offset2].chosen_tag[Phase - 1])
            && p.params.tags[0] == text[index].chosen_tag[Phase];
}
string predicateAsString(const Predicate<Lexeme>& p) {
    char str[STR_SIZE];
    sprintf(str, "(T[%d] = %s OR T[%d] = %s) AND T[0] = %s", Offset1, T(tags[1]),
            Offset2, T(tags[1]), S(tags[0]));
    return string(str);
}
};

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

    return new AllPredicatesAllActionsGenerator<Lexeme>(ptemplates, atemplates);
}
