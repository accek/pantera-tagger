template<class Lexeme, int Phase>
class PartialPTagPredicateTemplateP2 : public PredicateTemplate<Lexeme>
{
public:
PartialPTagPredicateTemplateP2(const vector<const Tagset*> tagsets) : PredicateTemplate<Lexeme>(tagsets) { }

void findMatchingPredicates(vector<Predicate<Lexeme> >& v,
                                                      vector<Lexeme>& text,
                                                      int index) {

    Predicate<Lexeme> p = Predicate<Lexeme>(this);
    p.params.segment_tag = text[index].chosen_tag[Phase - 1];
    p.params.segment_subtag = text[index].chosen_tag[Phase];
    p.params.tag = text[index - 1].chosen_tag[Phase - 1];
    v.push_back(p);

}
bool predicateMatches(const Predicate<Lexeme>& p,
            vector<Lexeme>& text, int index) {
    return (p.params.tag == text[index - 1].chosen_tag[Phase - 1])
            && p.params.segment_tag == text[index].chosen_tag[Phase - 1]
            && p.params.segment_subtag == text[index].chosen_tag[Phase];
}
string predicateAsString(const Predicate<Lexeme>& p) {
    char str[STR_SIZE];
    sprintf(str, "T[-1] = %s AND T[0] = %s/%s", T(tag), T(segment_tag), S(segment_subtag));
    return string(str);
}
};

template<class Lexeme, int Phase>
class FullPTagPredicateTemplateP2 : public PredicateTemplate<Lexeme>
{
public:
FullPTagPredicateTemplateP2(const vector<const Tagset*> tagsets) : PredicateTemplate<Lexeme>(tagsets) { }

void findMatchingPredicates(vector<Predicate<Lexeme> >& v,
                                                      vector<Lexeme>& text,
                                                      int index) {

    Predicate<Lexeme> p = Predicate<Lexeme>(this);
    p.params.segment_tag = text[index].chosen_tag[Phase - 1];
    p.params.segment_subtag = text[index].chosen_tag[Phase];
    p.params.tags[0] = text[index - 1].chosen_tag[Phase - 1];
    p.params.tags[1] = text[index - 1].chosen_tag[Phase];
    v.push_back(p);

}
bool predicateMatches(const Predicate<Lexeme>& p,
            vector<Lexeme>& text, int index) {
    return (p.params.tags[0] == text[index - 1].chosen_tag[Phase - 1]
            && p.params.tags[1] == text[index - 1].chosen_tag[Phase])
            && p.params.segment_tag == text[index].chosen_tag[Phase - 1]
            && p.params.segment_subtag == text[index].chosen_tag[Phase];
}
string predicateAsString(const Predicate<Lexeme>& p) {
    char str[STR_SIZE];
    sprintf(str, "T[-1] = %s/%s AND T[0] = %s/%s", T(tag), S(tag), T(segment_tag), S(segment_subtag));
    return string(str);
}
};

template<class Lexeme, int Phase>
class PartialP2TagPredicateTemplateP2 : public PredicateTemplate<Lexeme>
{
public:
PartialP2TagPredicateTemplateP2(const vector<const Tagset*> tagsets) : PredicateTemplate<Lexeme>(tagsets) { }

void findMatchingPredicates(vector<Predicate<Lexeme> >& v,
                                                      vector<Lexeme>& text,
                                                      int index) {

    Predicate<Lexeme> p = Predicate<Lexeme>(this);
    p.params.segment_tag = text[index].chosen_tag[Phase - 1];
    p.params.segment_subtag = text[index].chosen_tag[Phase];
    p.params.tag = text[index - 2].chosen_tag[Phase - 1];
    v.push_back(p);

}
bool predicateMatches(const Predicate<Lexeme>& p,
            vector<Lexeme>& text, int index) {
    return (p.params.tag == text[index - 2].chosen_tag[Phase - 1])
            && p.params.segment_tag == text[index].chosen_tag[Phase - 1]
            && p.params.segment_subtag == text[index].chosen_tag[Phase];
}
string predicateAsString(const Predicate<Lexeme>& p) {
    char str[STR_SIZE];
    sprintf(str, "T[-2] = %s AND T[0] = %s/%s", T(tag), T(segment_tag), S(segment_subtag));
    return string(str);
}
};

template<class Lexeme, int Phase>
class FullP2TagPredicateTemplateP2 : public PredicateTemplate<Lexeme>
{
public:
FullP2TagPredicateTemplateP2(const vector<const Tagset*> tagsets) : PredicateTemplate<Lexeme>(tagsets) { }

void findMatchingPredicates(vector<Predicate<Lexeme> >& v,
                                                      vector<Lexeme>& text,
                                                      int index) {

    Predicate<Lexeme> p = Predicate<Lexeme>(this);
    p.params.segment_tag = text[index].chosen_tag[Phase - 1];
    p.params.segment_subtag = text[index].chosen_tag[Phase];
    p.params.tags[0] = text[index - 2].chosen_tag[Phase - 1];
    p.params.tags[1] = text[index - 2].chosen_tag[Phase];
    v.push_back(p);

}
bool predicateMatches(const Predicate<Lexeme>& p,
            vector<Lexeme>& text, int index) {
    return (p.params.tags[0] == text[index - 2].chosen_tag[Phase - 1]
            && p.params.tags[1] == text[index - 2].chosen_tag[Phase])
            && p.params.segment_tag == text[index].chosen_tag[Phase - 1]
            && p.params.segment_subtag == text[index].chosen_tag[Phase];
}
string predicateAsString(const Predicate<Lexeme>& p) {
    char str[STR_SIZE];
    sprintf(str, "T[-1] = %s AND T[0] = %s/%s", T(tag), T(segment_tag), S(segment_subtag));
    return string(str);
}
};


template<class Lexeme, int Phase>
class PartialPPTagPredicateTemplateP2 : public PredicateTemplate<Lexeme>
{
public:
PartialPPTagPredicateTemplateP2(const vector<const Tagset*> tagsets) : PredicateTemplate<Lexeme>(tagsets) { }

void findMatchingPredicates(vector<Predicate<Lexeme> >& v,
                                                      vector<Lexeme>& text,
                                                      int index) {
    Predicate<Lexeme> p = Predicate<Lexeme>(this);
    p.params.segment_tag = text[index].chosen_tag[Phase - 1];
    p.params.segment_subtag = text[index].chosen_tag[Phase];
    p.params.tag = text[index - 1].chosen_tag[Phase - 1];
    v.push_back(p);
    

    if (text[index - 2].chosen_tag[Phase - 1] != text[index - 1].chosen_tag[Phase - 1]) {
        p = Predicate<Lexeme>(this);
        p.params.segment_tag = text[index].chosen_tag[Phase - 1];
        p.params.segment_subtag = text[index].chosen_tag[Phase];
        p.params.tag = text[index - 2].chosen_tag[Phase - 1];
        v.push_back(p);
    }
}
bool predicateMatches(const Predicate<Lexeme>& p,
            vector<Lexeme>& text, int index) {
    return (p.params.tag == text[index - 1].chosen_tag[Phase - 1] ||
            p.params.tag == text[index - 2].chosen_tag[Phase - 1])
            && p.params.segment_tag == text[index].chosen_tag[Phase - 1]
            && p.params.segment_subtag == text[index].chosen_tag[Phase];
}
string predicateAsString(const Predicate<Lexeme>& p) {
    char str[STR_SIZE];
    sprintf(str, "(T[-2] = %s OR T[-1] = %s) AND T[0] = %s/%s", T(tag), T(tag), T(segment_tag), S(segment_subtag));
    return string(str);
}
};

template<class Lexeme, int Phase>
class FullPPTagPredicateTemplateP2 : public PredicateTemplate<Lexeme>
{
public:
FullPPTagPredicateTemplateP2(const vector<const Tagset*> tagsets) : PredicateTemplate<Lexeme>(tagsets) { }

void findMatchingPredicates(vector<Predicate<Lexeme> >& v,
                                                      vector<Lexeme>& text,
                                                      int index) {

    Predicate<Lexeme> p = Predicate<Lexeme>(this);
    p.params.segment_tag = text[index].chosen_tag[Phase - 1];
    p.params.segment_subtag = text[index].chosen_tag[Phase];
    p.params.tags[0] = text[index - 1].chosen_tag[Phase - 1];
    p.params.tags[1] = text[index - 1].chosen_tag[Phase];
    v.push_back(p);

    if (text[index - 2].chosen_tag[Phase] != text[index - 1].chosen_tag[Phase] ||
            text[index - 2].chosen_tag[Phase - 1] != text[index - 1].chosen_tag[Phase - 1]) {
        p = Predicate<Lexeme>(this);
        p.params.segment_tag = text[index].chosen_tag[Phase - 1];
        p.params.segment_subtag = text[index].chosen_tag[Phase];
        p.params.tags[0] = text[index - 2].chosen_tag[Phase - 1];
        p.params.tags[1] = text[index - 2].chosen_tag[Phase];
        v.push_back(p);

    }
}
bool predicateMatches(const Predicate<Lexeme>& p,
            vector<Lexeme>& text, int index) {
    return ((p.params.tags[0] == text[index - 1].chosen_tag[Phase - 1]
            && p.params.tags[1] == text[index - 1].chosen_tag[Phase])
            || (p.params.tags[0] == text[index - 2].chosen_tag[Phase - 1] 
            && p.params.tags[1] == text[index - 2].chosen_tag[Phase]))
            && p.params.segment_tag == text[index].chosen_tag[Phase - 1]
            && p.params.segment_subtag == text[index].chosen_tag[Phase];
}
string predicateAsString(const Predicate<Lexeme>& p) {
    char str[STR_SIZE];
    sprintf(str, "(T[-2] = %s/%s OR T[-1] = %s/%s) AND T[0] = %s/%s", T(tags[0]), S(tags[1]), T(tags[0]), S(tags[1]), T(segment_tag), S(segment_subtag));
    return string(str);
}
};

template<class Lexeme, int Phase>
class PartialNTagPredicateTemplateP2 : public PredicateTemplate<Lexeme>
{
public:
PartialNTagPredicateTemplateP2(const vector<const Tagset*> tagsets) : PredicateTemplate<Lexeme>(tagsets) { }

void findMatchingPredicates(vector<Predicate<Lexeme> >& v,
                                                      vector<Lexeme>& text,
                                                      int index) {

    Predicate<Lexeme> p = Predicate<Lexeme>(this);
    p.params.segment_tag = text[index].chosen_tag[Phase - 1];
    p.params.segment_subtag = text[index].chosen_tag[Phase];
    p.params.tag = text[index + 1].chosen_tag[Phase - 1];
    v.push_back(p);

}
bool predicateMatches(const Predicate<Lexeme>& p,
            vector<Lexeme>& text, int index) {
    return (p.params.tag == text[index + 1].chosen_tag[Phase - 1])
            && p.params.segment_tag == text[index].chosen_tag[Phase - 1]
            && p.params.segment_subtag == text[index].chosen_tag[Phase];
}
string predicateAsString(const Predicate<Lexeme>& p) {
    char str[STR_SIZE];
    sprintf(str, "T[0] = %s/%s AND T[+1] = %s", T(segment_tag), S(segment_subtag), T(tag));
    return string(str);
}
};

template<class Lexeme, int Phase>
class FullNTagPredicateTemplateP2 : public PredicateTemplate<Lexeme>
{
public:
FullNTagPredicateTemplateP2(const vector<const Tagset*> tagsets) : PredicateTemplate<Lexeme>(tagsets) { }

void findMatchingPredicates(vector<Predicate<Lexeme> >& v,
                                                      vector<Lexeme>& text,
                                                      int index) {

    Predicate<Lexeme> p = Predicate<Lexeme>(this);
    p.params.segment_tag = text[index].chosen_tag[Phase - 1];
    p.params.segment_subtag = text[index].chosen_tag[Phase];
    p.params.tags[0] = text[index + 1].chosen_tag[Phase - 1];
    p.params.tags[1] = text[index + 1].chosen_tag[Phase];
    v.push_back(p);

}
bool predicateMatches(const Predicate<Lexeme>& p,
            vector<Lexeme>& text, int index) {
    return (p.params.tags[0] == text[index + 1].chosen_tag[Phase - 1]
            && p.params.tags[1] == text[index + 1].chosen_tag[Phase])
            && p.params.segment_tag == text[index].chosen_tag[Phase - 1]
            && p.params.segment_subtag == text[index].chosen_tag[Phase];
}
string predicateAsString(const Predicate<Lexeme>& p) {
    char str[STR_SIZE];
    sprintf(str, "T[0] = %s/%s AND T[+1] = %s/%s", T(segment_tag), S(segment_subtag), T(tags[0]), S(tags[1]));
    return string(str);
}
};

template<class Lexeme, int Phase>
class PartialN2TagPredicateTemplateP2 : public PredicateTemplate<Lexeme>
{
public:
PartialN2TagPredicateTemplateP2(const vector<const Tagset*> tagsets) : PredicateTemplate<Lexeme>(tagsets) { }

void findMatchingPredicates(vector<Predicate<Lexeme> >& v,
                                                      vector<Lexeme>& text,
                                                      int index) {

    Predicate<Lexeme> p = Predicate<Lexeme>(this);
    p.params.segment_tag = text[index].chosen_tag[Phase - 1];
    p.params.segment_subtag = text[index].chosen_tag[Phase];
    p.params.tag = text[index + 2].chosen_tag[Phase - 1];
    v.push_back(p);

}
bool predicateMatches(const Predicate<Lexeme>& p,
            vector<Lexeme>& text, int index) {
    return (p.params.tag == text[index + 2].chosen_tag[Phase - 1])
            && p.params.segment_tag == text[index].chosen_tag[Phase - 1]
            && p.params.segment_subtag == text[index].chosen_tag[Phase];
}
string predicateAsString(const Predicate<Lexeme>& p) {
    char str[STR_SIZE];
    sprintf(str, "T[0] = %s/%s AND T[+2] = %s", T(segment_tag), S(segment_subtag), T(tag));
    return string(str);
}
};

template<class Lexeme, int Phase>
class FullN2TagPredicateTemplateP2 : public PredicateTemplate<Lexeme>
{
public:
FullN2TagPredicateTemplateP2(const vector<const Tagset*> tagsets) : PredicateTemplate<Lexeme>(tagsets) { }

void findMatchingPredicates(vector<Predicate<Lexeme> >& v,
                                                      vector<Lexeme>& text,
                                                      int index) {

    Predicate<Lexeme> p = Predicate<Lexeme>(this);
    p.params.segment_tag = text[index].chosen_tag[Phase - 1];
    p.params.segment_subtag = text[index].chosen_tag[Phase];
    p.params.tags[0] = text[index + 2].chosen_tag[Phase - 1];
    p.params.tags[1] = text[index + 2].chosen_tag[Phase];
    v.push_back(p);

}
bool predicateMatches(const Predicate<Lexeme>& p,
            vector<Lexeme>& text, int index) {
    return (p.params.tags[0] == text[index + 2].chosen_tag[Phase - 1]
            && p.params.tags[1] == text[index + 2].chosen_tag[Phase])
            && p.params.segment_tag == text[index].chosen_tag[Phase - 1]
            && p.params.segment_subtag == text[index].chosen_tag[Phase];
}
string predicateAsString(const Predicate<Lexeme>& p) {
    char str[STR_SIZE];
    sprintf(str, "T[0] = %s/%s AND T[+2] = %s/%s", T(segment_tag), S(segment_subtag), T(tags[0]), S(tags[1]));
    return string(str);
}
};


template<class Lexeme, int Phase>
class PartialNNTagPredicateTemplateP2 : public PredicateTemplate<Lexeme>
{
public:
PartialNNTagPredicateTemplateP2(const vector<const Tagset*> tagsets) : PredicateTemplate<Lexeme>(tagsets) { }

void findMatchingPredicates(vector<Predicate<Lexeme> >& v,
                                                      vector<Lexeme>& text,
                                                      int index) {

    Predicate<Lexeme> p = Predicate<Lexeme>(this);
    p.params.segment_tag = text[index].chosen_tag[Phase - 1];
    p.params.segment_subtag = text[index].chosen_tag[Phase];
    p.params.tag = text[index + 1].chosen_tag[Phase - 1];
    v.push_back(p);
    
    if (text[index + 2].chosen_tag[Phase - 1] != text[index + 1].chosen_tag[Phase - 1]) {
        p = Predicate<Lexeme>(this);
        p.params.segment_tag = text[index].chosen_tag[Phase - 1];
        p.params.segment_subtag = text[index].chosen_tag[Phase];
        p.params.tag = text[index + 2].chosen_tag[Phase - 1];
        v.push_back(p);
    }
}
bool predicateMatches(const Predicate<Lexeme>& p,
            vector<Lexeme>& text, int index) {
    return (p.params.tag == text[index + 1].chosen_tag[Phase - 1] ||
            p.params.tag == text[index + 2].chosen_tag[Phase - 1])
            && p.params.segment_tag == text[index].chosen_tag[Phase - 1]
            && p.params.segment_subtag == text[index].chosen_tag[Phase];
}
string predicateAsString(const Predicate<Lexeme>& p) {
    char str[STR_SIZE];
    sprintf(str, "T[0] = %s/%s AND (T[+1] = %s OR T[+2] = %s)", T(segment_tag), S(segment_subtag), T(tag), T(tag));
    return string(str);
}
};

template<class Lexeme, int Phase>
class FullNNTagPredicateTemplateP2 : public PredicateTemplate<Lexeme>
{
public:
FullNNTagPredicateTemplateP2(const vector<const Tagset*> tagsets) : PredicateTemplate<Lexeme>(tagsets) { }

void findMatchingPredicates(vector<Predicate<Lexeme> >& v,
                                                      vector<Lexeme>& text,
                                                      int index) {

    Predicate<Lexeme> p = Predicate<Lexeme>(this);
    p.params.segment_tag = text[index].chosen_tag[Phase - 1];
    p.params.segment_subtag = text[index].chosen_tag[Phase];
    p.params.tags[0] = text[index + 1].chosen_tag[Phase - 1];
    p.params.tags[1] = text[index + 1].chosen_tag[Phase];
    v.push_back(p);

    if (text[index + 2].chosen_tag[Phase] != text[index + 1].chosen_tag[Phase] ||
            text[index + 2].chosen_tag[Phase - 1] != text[index + 1].chosen_tag[Phase - 1]) {
        p = Predicate<Lexeme>(this);
        p.params.segment_tag = text[index].chosen_tag[Phase - 1];
        p.params.segment_subtag = text[index].chosen_tag[Phase];
        p.params.tags[0] = text[index + 2].chosen_tag[Phase - 1];
        p.params.tags[1] = text[index + 2].chosen_tag[Phase];
        v.push_back(p);

    }
}
bool predicateMatches(const Predicate<Lexeme>& p,
            vector<Lexeme>& text, int index) {
    return ((p.params.tags[0] == text[index + 1].chosen_tag[Phase - 1]
            && p.params.tags[1] == text[index + 1].chosen_tag[Phase])
            || (p.params.tags[0] == text[index + 2].chosen_tag[Phase - 1] 
            && p.params.tags[1] == text[index + 2].chosen_tag[Phase]))
            && p.params.segment_tag == text[index].chosen_tag[Phase - 1]
            && p.params.segment_subtag == text[index].chosen_tag[Phase];
}
string predicateAsString(const Predicate<Lexeme>& p) {
    char str[STR_SIZE];
    sprintf(str, "T[0] = %s/%s AND (T[+1] = %s/%s OR T[+2] = %s/%s)", T(segment_tag), S(segment_subtag), T(tags[0]), S(tags[1]), T(tags[0]), S(tags[1]));
    return string(str);
}
};

template<class Lexeme, int Phase>
class SubtagPTagPredicateTemplateP2 : public PredicateTemplate<Lexeme>
{
public:
SubtagPTagPredicateTemplateP2(const vector<const Tagset*> tagsets) : PredicateTemplate<Lexeme>(tagsets) { }

void findMatchingPredicates(vector<Predicate<Lexeme> >& v,
                                                      vector<Lexeme>& text,
                                                      int index) {

    Predicate<Lexeme> p = Predicate<Lexeme>(this);
    p.params.segment_tag = text[index].chosen_tag[Phase];
    p.params.tag = text[index - 1].chosen_tag[Phase];
    v.push_back(p);

}
bool predicateMatches(const Predicate<Lexeme>& p,
            vector<Lexeme>& text, int index) {
    return (p.params.tag == text[index - 1].chosen_tag[Phase])
            && p.params.segment_tag == text[index].chosen_tag[Phase];
}
string predicateAsString(const Predicate<Lexeme>& p) {
    char str[STR_SIZE];
    sprintf(str, "T[-1] = */%s AND T[0] = */%s", S(tag), S(segment_tag));
    return string(str);
}
};

template<class Lexeme, int Phase>
class SubtagP2TagPredicateTemplateP2 : public PredicateTemplate<Lexeme>
{
public:
SubtagP2TagPredicateTemplateP2(const vector<const Tagset*> tagsets) : PredicateTemplate<Lexeme>(tagsets) { }

void findMatchingPredicates(vector<Predicate<Lexeme> >& v,
                                                      vector<Lexeme>& text,
                                                      int index) {

    Predicate<Lexeme> p = Predicate<Lexeme>(this);
    p.params.segment_tag = text[index].chosen_tag[Phase];
    p.params.tag = text[index - 2].chosen_tag[Phase];
    v.push_back(p);

}
bool predicateMatches(const Predicate<Lexeme>& p,
            vector<Lexeme>& text, int index) {
    return (p.params.tag == text[index - 2].chosen_tag[Phase])
            && p.params.segment_tag == text[index].chosen_tag[Phase];
}
string predicateAsString(const Predicate<Lexeme>& p) {
    char str[STR_SIZE];
    sprintf(str, "T[-2] = */%s AND T[0] = */%s", S(tag), S(segment_tag));
    return string(str);
}
};

template<class Lexeme, int Phase>
class SubtagPPTagPredicateTemplateP2 : public PredicateTemplate<Lexeme>
{
public:
SubtagPPTagPredicateTemplateP2(const vector<const Tagset*> tagsets) : PredicateTemplate<Lexeme>(tagsets) { }

void findMatchingPredicates(vector<Predicate<Lexeme> >& v,
                                                      vector<Lexeme>& text,
                                                      int index) {

    Predicate<Lexeme> p = Predicate<Lexeme>(this);
    p.params.segment_tag = text[index].chosen_tag[Phase];
    p.params.tag = text[index - 1].chosen_tag[Phase];
    v.push_back(p);

    if (text[index - 2].chosen_tag[Phase] != text[index - 1].chosen_tag[Phase]) {
        p = Predicate<Lexeme>(this);
        p.params.segment_tag = text[index].chosen_tag[Phase];
        p.params.tag = text[index - 2].chosen_tag[Phase];
        v.push_back(p);
    }

}
bool predicateMatches(const Predicate<Lexeme>& p,
            vector<Lexeme>& text, int index) {
    return (p.params.tag == text[index - 1].chosen_tag[Phase]
            || p.params.tag == text[index - 2].chosen_tag[Phase])
            && p.params.segment_tag == text[index].chosen_tag[Phase];
}
string predicateAsString(const Predicate<Lexeme>& p) {
    char str[STR_SIZE];
    sprintf(str, "(T[-2] = */%s OR T[-1] = */%s) AND T[0] = */%s", S(tag), S(tag), S(segment_tag));
    return string(str);
}
};

template<class Lexeme, int Phase>
class SubtagPPPTagPredicateTemplateP2 : public PredicateTemplate<Lexeme>
{
public:
SubtagPPPTagPredicateTemplateP2(const vector<const Tagset*> tagsets) : PredicateTemplate<Lexeme>(tagsets) { }

void findMatchingPredicates(vector<Predicate<Lexeme> >& v,
                                                      vector<Lexeme>& text,
                                                      int index) {

    Predicate<Lexeme> p = Predicate<Lexeme>(this);
    p.params.segment_tag = text[index].chosen_tag[Phase];
    p.params.tag = text[index - 1].chosen_tag[Phase];
    v.push_back(p);

    if (text[index - 2].chosen_tag[Phase] != text[index - 1].chosen_tag[Phase]) {
        p = Predicate<Lexeme>(this);
        p.params.segment_tag = text[index].chosen_tag[Phase];
        p.params.tag = text[index - 2].chosen_tag[Phase];
        v.push_back(p);
    }

    if (text[index - 3].chosen_tag[Phase] != text[index - 1].chosen_tag[Phase] &&
            text[index - 3].chosen_tag[Phase] != text[index - 2].chosen_tag[Phase]) {
        p = Predicate<Lexeme>(this);
        p.params.segment_tag = text[index].chosen_tag[Phase];
        p.params.tag = text[index - 3].chosen_tag[Phase];
        v.push_back(p);
    }
}
bool predicateMatches(const Predicate<Lexeme>& p,
            vector<Lexeme>& text, int index) {
    return (p.params.tag == text[index - 1].chosen_tag[Phase]
            || p.params.tag == text[index - 2].chosen_tag[Phase]
            || p.params.tag == text[index - 3].chosen_tag[Phase])
            && p.params.segment_tag == text[index].chosen_tag[Phase];
}
string predicateAsString(const Predicate<Lexeme>& p) {
    char str[STR_SIZE];
    sprintf(str, "(T[-3] = */%s OR T[-2] = */%s OR T[-1] = */%s) AND T[0] = */%s", S(tag), S(tag), S(tag), S(segment_tag));
    return string(str);
}
};

template<class Lexeme, int Phase>
class SubtagBothPPTagPredicateTemplateP2 : public PredicateTemplate<Lexeme>
{
public:
SubtagBothPPTagPredicateTemplateP2(const vector<const Tagset*> tagsets) : PredicateTemplate<Lexeme>(tagsets) { }

void findMatchingPredicates(vector<Predicate<Lexeme> >& v,
                                                      vector<Lexeme>& text,
                                                      int index) {

    Predicate<Lexeme> p = Predicate<Lexeme>(this);
    p.params.segment_tag = text[index].chosen_tag[Phase];
    p.params.tags[0] = text[index - 1].chosen_tag[Phase];
    p.params.tags[1] = text[index - 2].chosen_tag[Phase];
    v.push_back(p);


}
bool predicateMatches(const Predicate<Lexeme>& p,
            vector<Lexeme>& text, int index) {
    return (p.params.tags[0] == text[index - 1].chosen_tag[Phase]
            && p.params.tags[1] == text[index - 2].chosen_tag[Phase])
            && p.params.segment_tag == text[index].chosen_tag[Phase];
}
string predicateAsString(const Predicate<Lexeme>& p) {
    char str[STR_SIZE];
    sprintf(str, "T[-2] = */%s AND T[-1] = */%s AND T[0] = */%s", S(tags[1]), S(tags[0]), S(segment_tag));
    return string(str);
}
};

template<class Lexeme, int Phase>
class SubtagBothPNTagPredicateTemplateP2 : public PredicateTemplate<Lexeme>
{
public:
SubtagBothPNTagPredicateTemplateP2(const vector<const Tagset*> tagsets) : PredicateTemplate<Lexeme>(tagsets) { }

void findMatchingPredicates(vector<Predicate<Lexeme> >& v,
                                                      vector<Lexeme>& text,
                                                      int index) {

    Predicate<Lexeme> p = Predicate<Lexeme>(this);
    p.params.segment_tag = text[index].chosen_tag[Phase];
    p.params.tags[0] = text[index - 1].chosen_tag[Phase];
    p.params.tags[1] = text[index + 1].chosen_tag[Phase];
    v.push_back(p);


}
bool predicateMatches(const Predicate<Lexeme>& p,
            vector<Lexeme>& text, int index) {
    return (p.params.tags[0] == text[index - 1].chosen_tag[Phase]
            && p.params.tags[1] == text[index + 1].chosen_tag[Phase])
            && p.params.segment_tag == text[index].chosen_tag[Phase];
}
string predicateAsString(const Predicate<Lexeme>& p) {
    char str[STR_SIZE];
    sprintf(str, "T[-1] = */%s AND T[0] = */%s AND T[+1] = */%s", S(tags[0]), S(segment_tag), S(tags[1]));
    return string(str);
}
};

template<class Lexeme, int Phase>
class SubtagNTagPredicateTemplateP2 : public PredicateTemplate<Lexeme>
{
public:
SubtagNTagPredicateTemplateP2(const vector<const Tagset*> tagsets) : PredicateTemplate<Lexeme>(tagsets) { }

void findMatchingPredicates(vector<Predicate<Lexeme> >& v,
                                                      vector<Lexeme>& text,
                                                      int index) {

    Predicate<Lexeme> p = Predicate<Lexeme>(this);
    p.params.segment_tag = text[index].chosen_tag[Phase];
    p.params.tag = text[index + 1].chosen_tag[Phase];
    v.push_back(p);

}
bool predicateMatches(const Predicate<Lexeme>& p,
            vector<Lexeme>& text, int index) {
    return (p.params.tag == text[index + 1].chosen_tag[Phase])
            && p.params.segment_tag == text[index].chosen_tag[Phase];
}
string predicateAsString(const Predicate<Lexeme>& p) {
    char str[STR_SIZE];
    sprintf(str, "T[0] = */%s AND T[+1] = */%s", S(segment_tag), S(tag));
    return string(str);
}
};


template<class Lexeme, int Phase>
class SubtagN2TagPredicateTemplateP2 : public PredicateTemplate<Lexeme>
{
public:
SubtagN2TagPredicateTemplateP2(const vector<const Tagset*> tagsets) : PredicateTemplate<Lexeme>(tagsets) { }

void findMatchingPredicates(vector<Predicate<Lexeme> >& v,
                                                      vector<Lexeme>& text,
                                                      int index) {

    Predicate<Lexeme> p = Predicate<Lexeme>(this);
    p.params.segment_tag = text[index].chosen_tag[Phase];
    p.params.tag = text[index + 2].chosen_tag[Phase];
    v.push_back(p);

}
bool predicateMatches(const Predicate<Lexeme>& p,
            vector<Lexeme>& text, int index) {
    return (p.params.tag == text[index + 2].chosen_tag[Phase])
            && p.params.segment_tag == text[index].chosen_tag[Phase];
}
string predicateAsString(const Predicate<Lexeme>& p) {
    char str[STR_SIZE];
    sprintf(str, "T[0] = */%s AND T[+2] = */%s", S(segment_tag), S(tag));
    return string(str);
}
};

template<class Lexeme, int Phase>
class SubtagNNTagPredicateTemplateP2 : public PredicateTemplate<Lexeme>
{
public:
SubtagNNTagPredicateTemplateP2(const vector<const Tagset*> tagsets) : PredicateTemplate<Lexeme>(tagsets) { }

void findMatchingPredicates(vector<Predicate<Lexeme> >& v,
                                                      vector<Lexeme>& text,
                                                      int index) {

    Predicate<Lexeme> p = Predicate<Lexeme>(this);
    p.params.segment_tag = text[index].chosen_tag[Phase];
    p.params.tag = text[index + 1].chosen_tag[Phase];
    v.push_back(p);

    if (text[index + 2].chosen_tag[Phase] != text[index + 1].chosen_tag[Phase]) {
        p = Predicate<Lexeme>(this);
        p.params.segment_tag = text[index].chosen_tag[Phase];
        p.params.tag = text[index + 2].chosen_tag[Phase];
        v.push_back(p);
    }

}
bool predicateMatches(const Predicate<Lexeme>& p,
            vector<Lexeme>& text, int index) {
    return (p.params.tag == text[index + 1].chosen_tag[Phase]
            || p.params.tag == text[index + 2].chosen_tag[Phase])
            && p.params.segment_tag == text[index].chosen_tag[Phase];
}
string predicateAsString(const Predicate<Lexeme>& p) {

    char str[STR_SIZE];
    sprintf(str, "T[0] = */%s AND (T[+1] = */%s OR T[+2] = */%s)", S(segment_tag), S(tag), S(tag));
    return string(str);
}
};

template<class Lexeme, int Phase>
class SubtagNNNTagPredicateTemplateP2 : public PredicateTemplate<Lexeme>
{
public:
SubtagNNNTagPredicateTemplateP2(const vector<const Tagset*> tagsets) : PredicateTemplate<Lexeme>(tagsets) { }

void findMatchingPredicates(vector<Predicate<Lexeme> >& v,
                                                      vector<Lexeme>& text,
                                                      int index) {

    Predicate<Lexeme> p = Predicate<Lexeme>(this);
    p.params.segment_tag = text[index].chosen_tag[Phase];
    p.params.tag = text[index + 1].chosen_tag[Phase];
    v.push_back(p);

    if (text[index + 2].chosen_tag[Phase] != text[index + 1].chosen_tag[Phase]) {
        p = Predicate<Lexeme>(this);
        p.params.segment_tag = text[index].chosen_tag[Phase];
        p.params.tag = text[index + 2].chosen_tag[Phase];
        v.push_back(p);
    }

    if (text[index + 2].chosen_tag[Phase] != text[index + 3].chosen_tag[Phase] && 
            text[index + 3].chosen_tag[Phase] != text[index + 1].chosen_tag[Phase]) {
        p = Predicate<Lexeme>(this);
        p.params.segment_tag = text[index].chosen_tag[Phase];
        p.params.tag = text[index + 3].chosen_tag[Phase];
        v.push_back(p);
    }
}
bool predicateMatches(const Predicate<Lexeme>& p,
            vector<Lexeme>& text, int index) {
    return (p.params.tag == text[index + 1].chosen_tag[Phase]
            || p.params.tag == text[index + 2].chosen_tag[Phase]
            || p.params.tag == text[index + 3].chosen_tag[Phase])
            && p.params.segment_tag == text[index].chosen_tag[Phase];
}
string predicateAsString(const Predicate<Lexeme>& p) {

    char str[STR_SIZE];
    sprintf(str, "T[0] = */%s AND (T[+1] = */%s OR T[+2] = */%s OR T[+3] = */%s)", S(segment_tag), S(tag), S(tag), S(tag));
    return string(str);
}
};

template<class Lexeme, int Phase>
class SubtagBothNNTagPredicateTemplateP2 : public PredicateTemplate<Lexeme>
{
public:
SubtagBothNNTagPredicateTemplateP2(const vector<const Tagset*> tagsets) : PredicateTemplate<Lexeme>(tagsets) { }

void findMatchingPredicates(vector<Predicate<Lexeme> >& v,
                                                      vector<Lexeme>& text,
                                                      int index) {

    Predicate<Lexeme> p = Predicate<Lexeme>(this);
    p.params.segment_tag = text[index].chosen_tag[Phase];
    p.params.tags[0] = text[index + 1].chosen_tag[Phase];
    p.params.tags[1] = text[index + 2].chosen_tag[Phase];
    v.push_back(p);
}
bool predicateMatches(const Predicate<Lexeme>& p,
            vector<Lexeme>& text, int index) {
    return (p.params.tags[0] == text[index + 1].chosen_tag[Phase]
            && p.params.tags[1] == text[index + 2].chosen_tag[Phase])
            && p.params.segment_tag == text[index].chosen_tag[Phase];
}
string predicateAsString(const Predicate<Lexeme>& p) {

    char str[STR_SIZE];
    sprintf(str, "T[0] = */%s AND T[+1] = */%s AND T[+2] = */%s", S(segment_tag), S(tags[0]), S(tags[1]));
    return string(str);
}
};

template<class Lexeme, int Phase>
class SubtagSuffix2TagPredicateTemplateP2 : public PredicateTemplate<Lexeme>
{
public:
SubtagSuffix2TagPredicateTemplateP2(const vector<const Tagset*> tagsets) : PredicateTemplate<Lexeme>(tagsets) { }

void findMatchingPredicates(vector<Predicate<Lexeme> >& v,
                                                      vector<Lexeme>& text,
                                                      int index) {

    int len = text[index].getOrth().length();
    if (len >= 6) {
        Predicate<Lexeme> p = Predicate<Lexeme>(this);
        p.params.segment_subtag = text[index].chosen_tag[Phase];
        p.params.chars[0] = text[index].getOrth()[len - 2];
        p.params.chars[1] = text[index].getOrth()[len - 1];
        v.push_back(p);
    }
}
bool predicateMatches(const Predicate<Lexeme>& p,
            vector<Lexeme>& text, int index) {
    int len = text[index].getOrth().length();
    return (len >= 6 && p.params.segment_subtag == text[index].chosen_tag[Phase]
            && text[index].getOrth()[len - 2] == p.params.chars[0]
            && text[index].getOrth()[len - 1] == p.params.chars[1]);
}
string predicateAsString(const Predicate<Lexeme>& p) {

    char str[STR_SIZE];
    sprintf(str, "T[0] = %s AND ORTH ends with %c%c", T(segment_tag), p.params.chars[0], p.params.chars[1]);
    return string(str);
}
};


template<class Lexeme, int Phase>
vector<PredicateTemplate<Lexeme>*> make_p2_rules(const vector<const Tagset*>& tagsets) {
    vector<PredicateTemplate<Lexeme>*> templates;

    templates.push_back(new PartialPTagPredicateTemplateP2<Lexeme, Phase>(tagsets));
    templates.push_back(new FullPTagPredicateTemplateP2<Lexeme, Phase>(tagsets));
    templates.push_back(new PartialP2TagPredicateTemplateP2<Lexeme, Phase>(tagsets));
    templates.push_back(new FullP2TagPredicateTemplateP2<Lexeme, Phase>(tagsets));
    templates.push_back(new PartialPPTagPredicateTemplateP2<Lexeme, Phase>(tagsets));
    templates.push_back(new FullPPTagPredicateTemplateP2<Lexeme, Phase>(tagsets));
    templates.push_back(new PartialNTagPredicateTemplateP2<Lexeme, Phase>(tagsets));
    templates.push_back(new FullNTagPredicateTemplateP2<Lexeme, Phase>(tagsets));
    templates.push_back(new PartialN2TagPredicateTemplateP2<Lexeme, Phase>(tagsets));
    templates.push_back(new FullN2TagPredicateTemplateP2<Lexeme, Phase>(tagsets));
    templates.push_back(new PartialNNTagPredicateTemplateP2<Lexeme, Phase>(tagsets));
    templates.push_back(new FullNNTagPredicateTemplateP2<Lexeme, Phase>(tagsets));

    templates.push_back(new SubtagPTagPredicateTemplateP2<Lexeme, Phase>(tagsets));
    templates.push_back(new SubtagP2TagPredicateTemplateP2<Lexeme, Phase>(tagsets));
    templates.push_back(new SubtagPPTagPredicateTemplateP2<Lexeme, Phase>(tagsets));
    templates.push_back(new SubtagPPPTagPredicateTemplateP2<Lexeme, Phase>(tagsets));
    templates.push_back(new SubtagBothPPTagPredicateTemplateP2<Lexeme, Phase>(tagsets));
    templates.push_back(new SubtagBothPNTagPredicateTemplateP2<Lexeme, Phase>(tagsets));
    templates.push_back(new SubtagNTagPredicateTemplateP2<Lexeme, Phase>(tagsets));
    templates.push_back(new SubtagN2TagPredicateTemplateP2<Lexeme, Phase>(tagsets));
    templates.push_back(new SubtagNNTagPredicateTemplateP2<Lexeme, Phase>(tagsets));
    templates.push_back(new SubtagNNNTagPredicateTemplateP2<Lexeme, Phase>(tagsets));
    templates.push_back(new SubtagBothNNTagPredicateTemplateP2<Lexeme, Phase>(tagsets));
//    templates.push_back(new SubtagSuffix2TagPredicateTemplateP2<Lexeme, Phase>(tagsets));

    return templates;
}

