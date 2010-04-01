template<class Lexeme, int Phase>
class MatchTagPredicateTemplate : public PredicateTemplate<Lexeme>
{
public:
MatchTagPredicateTemplate(const vector<const Tagset*> tagsets) : PredicateTemplate<Lexeme>(tagsets) { }

void findMatchingPredicates(vector<Predicate<Lexeme> >& v,
                                                      vector<Lexeme>& text,
                                                      int index) {

    Predicate<Lexeme> p = Predicate<Lexeme>(this);
    p.params.tags[0] = text[index].chosen_tag[Phase];
    v.push_back(p);

}
bool predicateMatches(const Predicate<Lexeme>& p,
            vector<Lexeme>& text, int index) {
    return p.params.tags[0] == text[index].chosen_tag[Phase];
}
string predicateAsString(const Predicate<Lexeme>& p) {
    char str[STR_SIZE];
    sprintf(str, "T[0] = %s", T(tags[0]));
    return string(str);
}
};


template<class Lexeme, int Phase, int Offset>
class NearbyTagPredicateTemplate : public PredicateTemplate<Lexeme>
{
public:
NearbyTagPredicateTemplate(const vector<const Tagset*> tagsets) : PredicateTemplate<Lexeme>(tagsets) { }

void findMatchingPredicates(vector<Predicate<Lexeme> >& v,
                                                      vector<Lexeme>& text,
                                                      int index) {

    Predicate<Lexeme> p = Predicate<Lexeme>(this);
    p.params.tags[0] = text[index].chosen_tag[Phase];
    p.params.tags[1] = text[index + Offset].chosen_tag[Phase];
    v.push_back(p);

}
bool predicateMatches(const Predicate<Lexeme>& p,
            vector<Lexeme>& text, int index) {
    return (p.params.tags[1] == text[index + Offset].chosen_tag[Phase])
            && p.params.tags[0] == text[index].chosen_tag[Phase];
}
string predicateAsString(const Predicate<Lexeme>& p) {
    char str[STR_SIZE];
    sprintf(str, "T[%d] = %s AND T[0] = %s", Offset, T(tags[1]), T(tags[0]));
    return string(str);
}
};

template<class Lexeme, int Phase, int Offset1, int Offset2>
class Nearby2TagsPredicateTemplate : public PredicateTemplate<Lexeme>
{
public:
Nearby2TagsPredicateTemplate(const vector<const Tagset*> tagsets) : PredicateTemplate<Lexeme>(tagsets) { }

void findMatchingPredicates(vector<Predicate<Lexeme> >& v,
                                                      vector<Lexeme>& text,
                                                      int index) {

    Predicate<Lexeme> p = Predicate<Lexeme>(this);
    p.params.tags[0] = text[index].chosen_tag[Phase];
    p.params.tags[1] = text[index + Offset1].chosen_tag[Phase];
    v.push_back(p);
    p.params.tags[1] = text[index + Offset2].chosen_tag[Phase];
    v.push_back(p);
}
bool predicateMatches(const Predicate<Lexeme>& p,
            vector<Lexeme>& text, int index) {
    return (p.params.tags[1] == text[index + Offset1].chosen_tag[Phase]
            || p.params.tags[1] == text[index + Offset2].chosen_tag[Phase])
            && p.params.tags[0] == text[index].chosen_tag[Phase];
}
string predicateAsString(const Predicate<Lexeme>& p) {
    char str[STR_SIZE];
    sprintf(str, "(T[%d] = %s OR T[%d] = %s) AND T[0] = %s",
            Offset1, T(tags[1]), Offset2, T(tags[1]), T(tags[0]));
    return string(str);
}
};

template<class Lexeme, int Phase, int Offset1, int Offset2, int Offset3>
class Nearby3TagsPredicateTemplate : public PredicateTemplate<Lexeme>
{
public:
Nearby3TagsPredicateTemplate(const vector<const Tagset*> tagsets) : PredicateTemplate<Lexeme>(tagsets) { }

void findMatchingPredicates(vector<Predicate<Lexeme> >& v,
                                                      vector<Lexeme>& text,
                                                      int index) {

    Predicate<Lexeme> p = Predicate<Lexeme>(this);
    p.params.tags[0] = text[index].chosen_tag[Phase];
    p.params.tags[1] = text[index + Offset1].chosen_tag[Phase];
    v.push_back(p);
    p.params.tags[1] = text[index + Offset2].chosen_tag[Phase];
    v.push_back(p);
    p.params.tags[1] = text[index + Offset3].chosen_tag[Phase];
    v.push_back(p);
}
bool predicateMatches(const Predicate<Lexeme>& p,
            vector<Lexeme>& text, int index) {
    return (p.params.tags[1] == text[index + Offset1].chosen_tag[Phase]
            || p.params.tags[1] == text[index + Offset2].chosen_tag[Phase]
            || p.params.tags[1] == text[index + Offset3].chosen_tag[Phase])
            && p.params.tags[0] == text[index].chosen_tag[Phase];
}
string predicateAsString(const Predicate<Lexeme>& p) {
    char str[STR_SIZE];
    sprintf(str, "(T[%d] = %s OR T[%d] = %s OR T[%d] = %s) AND T[0] = %s",
            Offset1, T(tags[1]), Offset2, T(tags[1]), Offset3, T(tags[1]), T(tags[0]));
    return string(str);
}
};

template<class Lexeme, int Phase, int Offset1, int Offset2>
class NearbyExact2TagsPredicateTemplate : public PredicateTemplate<Lexeme>
{
public:
NearbyExact2TagsPredicateTemplate(const vector<const Tagset*> tagsets) : PredicateTemplate<Lexeme>(tagsets) { }

void findMatchingPredicates(vector<Predicate<Lexeme> >& v,
                                                      vector<Lexeme>& text,
                                                      int index) {

    Predicate<Lexeme> p = Predicate<Lexeme>(this);
    p.params.tags[0] = text[index].chosen_tag[Phase];
    p.params.tags[1] = text[index + Offset1].chosen_tag[Phase];
    p.params.tags[2] = text[index + Offset2].chosen_tag[Phase];
    v.push_back(p);


}
bool predicateMatches(const Predicate<Lexeme>& p,
            vector<Lexeme>& text, int index) {
    return (p.params.tags[1] == text[index + Offset1].chosen_tag[Phase]
            && p.params.tags[2] == text[index + Offset2].chosen_tag[Phase])
            && p.params.tags[0] == text[index].chosen_tag[Phase];
}
string predicateAsString(const Predicate<Lexeme>& p) {
    char str[STR_SIZE];
    sprintf(str, "T[%d] = %s AND T[%d] = %s AND T[0] = %s", 
            Offset1, T(tags[1]), Offset2, T(tags[2]), T(tags[0]));
    return string(str);
}
};

template<class Lexeme, int Phase>
class CCaseTagPredicateTemplate : public PredicateTemplate<Lexeme>
{
public:
CCaseTagPredicateTemplate(const vector<const Tagset*> tagsets) : PredicateTemplate<Lexeme>(tagsets) { }

void findMatchingPredicates(vector<Predicate<Lexeme> >& v,
                                                      vector<Lexeme>& text,
                                                      int index) {
    if (isupper(text[index].getOrth()[0], this->tagsets[Phase]->getLocale())) {
        Predicate<Lexeme> p = Predicate<Lexeme>(this);
        p.params.tags[0] = text[index].chosen_tag[Phase];
        v.push_back(p);
    }
}
bool predicateMatches(const Predicate<Lexeme>& p,
            vector<Lexeme>& text, int index) {
    return (p.params.tags[0] == text[index].chosen_tag[Phase]
            && isupper(text[index].getOrth()[0], this->tagsets[Phase]->getLocale()));
}
string predicateAsString(const Predicate<Lexeme>& p) {

    char str[STR_SIZE];
    sprintf(str, "T[0] = %s AND ORTH[0] starts with capital letter", T(tags[0]));
    return string(str);
}
};

template<class Lexeme, int Phase>
class Prefix2TagPredicateTemplate : public PredicateTemplate<Lexeme>
{
public:
Prefix2TagPredicateTemplate(const vector<const Tagset*> tagsets) : PredicateTemplate<Lexeme>(tagsets) { }

void findMatchingPredicates(vector<Predicate<Lexeme> >& v,
                                                      vector<Lexeme>& text,
                                                      int index) {

    const wstring& orth = text[index].getOrth();
    int len = orth.length();
    if (len >= 2) {
        Predicate<Lexeme> p = Predicate<Lexeme>(this);
        p.params.tags[0] = text[index].chosen_tag[Phase];
        p.params.chars[0] = orth[0];
        p.params.chars[1] = orth[1];
        v.push_back(p);
    }
}
bool predicateMatches(const Predicate<Lexeme>& p,
            vector<Lexeme>& text, int index) {
    const wstring& orth = text[index].getOrth();
    int len = orth.length();
    return (len >= 2 && p.params.tags[0] == text[index].chosen_tag[Phase]
            && orth[0] == p.params.chars[0]
            && orth[1] == p.params.chars[1]);
}
string predicateAsString(const Predicate<Lexeme>& p) {

    char str[STR_SIZE];
    sprintf(str, "T[0] = %s AND ORTH starts with %lc%lc", T(tags[0]), p.params.chars[0], p.params.chars[1]);
    return string(str);
}
};

template<class Lexeme, int Phase>
class Suffix2TagPredicateTemplate : public PredicateTemplate<Lexeme>
{
public:
Suffix2TagPredicateTemplate(const vector<const Tagset*> tagsets) : PredicateTemplate<Lexeme>(tagsets) { }

void findMatchingPredicates(vector<Predicate<Lexeme> >& v,
                                                      vector<Lexeme>& text,
                                                      int index) {

    const wstring& orth = text[index].getOrth();
    int len = orth.length();
    if (len >= 2) {
        Predicate<Lexeme> p = Predicate<Lexeme>(this);
        p.params.tags[0] = text[index].chosen_tag[Phase];
        p.params.chars[0] = orth[len - 2];
        p.params.chars[1] = orth[len - 1];
        v.push_back(p);
    }
}
bool predicateMatches(const Predicate<Lexeme>& p,
            vector<Lexeme>& text, int index) {
    const wstring& orth = text[index].getOrth();
    int len = orth.length();
    return (len >= 2 && p.params.tags[0] == text[index].chosen_tag[Phase]
            && orth[len - 2] == p.params.chars[0]
            && orth[len - 1] == p.params.chars[1]);
}
string predicateAsString(const Predicate<Lexeme>& p) {

    char str[STR_SIZE];
    sprintf(str, "T[0] = %s AND ORTH ends with %lc%lc", T(tags[0]), p.params.chars[0], p.params.chars[1]);
    return string(str);
}
};



