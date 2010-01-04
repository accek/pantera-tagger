template<class Lexeme>
class PTagPredicateTemplateP1 : public PredicateTemplate<Lexeme>
{
public:
PTagPredicateTemplateP1(const vector<const Tagset*> tagsets) : PredicateTemplate<Lexeme>(tagsets) { }

void findMatchingPredicates(vector<Predicate<Lexeme> >& v,
                                                      vector<Lexeme>& text,
                                                      int index) {

    Predicate<Lexeme> p = Predicate<Lexeme>(this);
    p.params.segment_tag = text[index].chosen_tag[0];
    p.params.tag = text[index - 1].chosen_tag[0];
    v.push_back(p);

}
bool predicateMatches(const Predicate<Lexeme>& p,
            vector<Lexeme>& text, int index) {
    return (p.params.tag == text[index - 1].chosen_tag[0])
            && p.params.segment_tag == text[index].chosen_tag[0];
}
string predicateAsString(const Predicate<Lexeme>& p) {
    char str[STR_SIZE];
    sprintf(str, "T[-1] = %s AND T[0] = %s", T(tag), T(segment_tag));
    return string(str);
}
};

template<class Lexeme>
class P2TagPredicateTemplateP1 : public PredicateTemplate<Lexeme>
{
public:
P2TagPredicateTemplateP1(const vector<const Tagset*> tagsets) : PredicateTemplate<Lexeme>(tagsets) { }

void findMatchingPredicates(vector<Predicate<Lexeme> >& v,
                                                      vector<Lexeme>& text,
                                                      int index) {

    Predicate<Lexeme> p = Predicate<Lexeme>(this);
    p.params.segment_tag = text[index].chosen_tag[0];
    p.params.tag = text[index - 2].chosen_tag[0];
    v.push_back(p);

}
bool predicateMatches(const Predicate<Lexeme>& p,
            vector<Lexeme>& text, int index) {
    return (p.params.tag == text[index - 2].chosen_tag[0])
            && p.params.segment_tag == text[index].chosen_tag[0];
}
string predicateAsString(const Predicate<Lexeme>& p) {
    char str[STR_SIZE];
    sprintf(str, "T[-2] = %s AND T[0] = %s", T(tag), T(segment_tag));
    return string(str);
}
};

template<class Lexeme>
class PPTagPredicateTemplateP1 : public PredicateTemplate<Lexeme>
{
public:
PPTagPredicateTemplateP1(const vector<const Tagset*> tagsets) : PredicateTemplate<Lexeme>(tagsets) { }

void findMatchingPredicates(vector<Predicate<Lexeme> >& v,
                                                      vector<Lexeme>& text,
                                                      int index) {

    Predicate<Lexeme> p = Predicate<Lexeme>(this);
    p.params.segment_tag = text[index].chosen_tag[0];
    p.params.tag = text[index - 1].chosen_tag[0];
    v.push_back(p);

    if (text[index - 2].chosen_tag[0] != text[index - 1].chosen_tag[0]) {
        p = Predicate<Lexeme>(this);
        p.params.segment_tag = text[index].chosen_tag[0];
        p.params.tag = text[index - 2].chosen_tag[0];
        v.push_back(p);
    }

}
bool predicateMatches(const Predicate<Lexeme>& p,
            vector<Lexeme>& text, int index) {
    return (p.params.tag == text[index - 1].chosen_tag[0]
            || p.params.tag == text[index - 2].chosen_tag[0])
            && p.params.segment_tag == text[index].chosen_tag[0];
}
string predicateAsString(const Predicate<Lexeme>& p) {
    char str[STR_SIZE];
    sprintf(str, "(T[-2] = %s OR T[-1] = %s) AND T[0] = %s", T(tag), T(tag), T(segment_tag));
    return string(str);
}
};

template<class Lexeme>
class PPPTagPredicateTemplateP1 : public PredicateTemplate<Lexeme>
{
public:
PPPTagPredicateTemplateP1(const vector<const Tagset*> tagsets) : PredicateTemplate<Lexeme>(tagsets) { }

void findMatchingPredicates(vector<Predicate<Lexeme> >& v,
                                                      vector<Lexeme>& text,
                                                      int index) {

    Predicate<Lexeme> p = Predicate<Lexeme>(this);
    p.params.segment_tag = text[index].chosen_tag[0];
    p.params.tag = text[index - 1].chosen_tag[0];
    v.push_back(p);

    if (text[index - 2].chosen_tag[0] != text[index - 1].chosen_tag[0]) {
        p = Predicate<Lexeme>(this);
        p.params.segment_tag = text[index].chosen_tag[0];
        p.params.tag = text[index - 2].chosen_tag[0];
        v.push_back(p);
    }

    if (text[index - 3].chosen_tag[0] != text[index - 1].chosen_tag[0] &&
            text[index - 3].chosen_tag[0] != text[index - 2].chosen_tag[0]) {
        p = Predicate<Lexeme>(this);
        p.params.segment_tag = text[index].chosen_tag[0];
        p.params.tag = text[index - 3].chosen_tag[0];
        v.push_back(p);
    }
}
bool predicateMatches(const Predicate<Lexeme>& p,
            vector<Lexeme>& text, int index) {
    return (p.params.tag == text[index - 1].chosen_tag[0]
            || p.params.tag == text[index - 2].chosen_tag[0]
            || p.params.tag == text[index - 3].chosen_tag[0])
            && p.params.segment_tag == text[index].chosen_tag[0];
}
string predicateAsString(const Predicate<Lexeme>& p) {
    char str[STR_SIZE];
    sprintf(str, "(T[-3] = %s OR T[-2] = %s OR T[-1] = %s) AND T[0] = %s", T(tag), T(tag), T(tag), T(segment_tag));
    return string(str);
}
};

template<class Lexeme>
class BothPPTagPredicateTemplateP1 : public PredicateTemplate<Lexeme>
{
public:
BothPPTagPredicateTemplateP1(const vector<const Tagset*> tagsets) : PredicateTemplate<Lexeme>(tagsets) { }

void findMatchingPredicates(vector<Predicate<Lexeme> >& v,
                                                      vector<Lexeme>& text,
                                                      int index) {

    Predicate<Lexeme> p = Predicate<Lexeme>(this);
    p.params.segment_tag = text[index].chosen_tag[0];
    p.params.tags[0] = text[index - 1].chosen_tag[0];
    p.params.tags[1] = text[index - 2].chosen_tag[0];
    v.push_back(p);


}
bool predicateMatches(const Predicate<Lexeme>& p,
            vector<Lexeme>& text, int index) {
    return (p.params.tags[0] == text[index - 1].chosen_tag[0]
            && p.params.tags[1] == text[index - 2].chosen_tag[0])
            && p.params.segment_tag == text[index].chosen_tag[0];
}
string predicateAsString(const Predicate<Lexeme>& p) {
    char str[STR_SIZE];
    sprintf(str, "T[-2] = %s AND T[-1] = %s AND T[0] = %s", T(tags[1]), T(tags[0]), T(segment_tag));
    return string(str);
}
};

template<class Lexeme>
class BothPNTagPredicateTemplateP1 : public PredicateTemplate<Lexeme>
{
public:
BothPNTagPredicateTemplateP1(const vector<const Tagset*> tagsets) : PredicateTemplate<Lexeme>(tagsets) { }

void findMatchingPredicates(vector<Predicate<Lexeme> >& v,
                                                      vector<Lexeme>& text,
                                                      int index) {

    Predicate<Lexeme> p = Predicate<Lexeme>(this);
    p.params.segment_tag = text[index].chosen_tag[0];
    p.params.tags[0] = text[index - 1].chosen_tag[0];
    p.params.tags[1] = text[index + 1].chosen_tag[0];
    v.push_back(p);


}
bool predicateMatches(const Predicate<Lexeme>& p,
            vector<Lexeme>& text, int index) {
    return (p.params.tags[0] == text[index - 1].chosen_tag[0]
            && p.params.tags[1] == text[index + 1].chosen_tag[0])
            && p.params.segment_tag == text[index].chosen_tag[0];
}
string predicateAsString(const Predicate<Lexeme>& p) {
    char str[STR_SIZE];
    sprintf(str, "T[-1] = %s AND T[0] = %s AND T[+1] = %s", T(tags[0]), T(segment_tag), T(tags[1]));
    return string(str);
}
};

template<class Lexeme>
class NTagPredicateTemplateP1 : public PredicateTemplate<Lexeme>
{
public:
NTagPredicateTemplateP1(const vector<const Tagset*> tagsets) : PredicateTemplate<Lexeme>(tagsets) { }

void findMatchingPredicates(vector<Predicate<Lexeme> >& v,
                                                      vector<Lexeme>& text,
                                                      int index) {

    Predicate<Lexeme> p = Predicate<Lexeme>(this);
    p.params.segment_tag = text[index].chosen_tag[0];
    p.params.tag = text[index + 1].chosen_tag[0];
    v.push_back(p);

}
bool predicateMatches(const Predicate<Lexeme>& p,
            vector<Lexeme>& text, int index) {
    return (p.params.tag == text[index + 1].chosen_tag[0])
            && p.params.segment_tag == text[index].chosen_tag[0];
}
string predicateAsString(const Predicate<Lexeme>& p) {
    char str[STR_SIZE];
    sprintf(str, "T[0] = %s AND T[+1] = %s", T(segment_tag), T(tag));
    return string(str);
}
};


template<class Lexeme>
class N2TagPredicateTemplateP1 : public PredicateTemplate<Lexeme>
{
public:
N2TagPredicateTemplateP1(const vector<const Tagset*> tagsets) : PredicateTemplate<Lexeme>(tagsets) { }

void findMatchingPredicates(vector<Predicate<Lexeme> >& v,
                                                      vector<Lexeme>& text,
                                                      int index) {

    Predicate<Lexeme> p = Predicate<Lexeme>(this);
    p.params.segment_tag = text[index].chosen_tag[0];
    p.params.tag = text[index + 2].chosen_tag[0];
    v.push_back(p);

}
bool predicateMatches(const Predicate<Lexeme>& p,
            vector<Lexeme>& text, int index) {
    return (p.params.tag == text[index + 2].chosen_tag[0])
            && p.params.segment_tag == text[index].chosen_tag[0];
}
string predicateAsString(const Predicate<Lexeme>& p) {
    char str[STR_SIZE];
    sprintf(str, "T[0] = %s AND T[+2] = %s", T(segment_tag), T(tag));
    return string(str);
}
};

template<class Lexeme>
class NNTagPredicateTemplateP1 : public PredicateTemplate<Lexeme>
{
public:
NNTagPredicateTemplateP1(const vector<const Tagset*> tagsets) : PredicateTemplate<Lexeme>(tagsets) { }

void findMatchingPredicates(vector<Predicate<Lexeme> >& v,
                                                      vector<Lexeme>& text,
                                                      int index) {

    Predicate<Lexeme> p = Predicate<Lexeme>(this);
    p.params.segment_tag = text[index].chosen_tag[0];
    p.params.tag = text[index + 1].chosen_tag[0];
    v.push_back(p);

    if (text[index + 2].chosen_tag[0] != text[index + 1].chosen_tag[0]) {
        p = Predicate<Lexeme>(this);
        p.params.segment_tag = text[index].chosen_tag[0];
        p.params.tag = text[index + 2].chosen_tag[0];
        v.push_back(p);
    }

}
bool predicateMatches(const Predicate<Lexeme>& p,
            vector<Lexeme>& text, int index) {
    return (p.params.tag == text[index + 1].chosen_tag[0]
            || p.params.tag == text[index + 2].chosen_tag[0])
            && p.params.segment_tag == text[index].chosen_tag[0];
}
string predicateAsString(const Predicate<Lexeme>& p) {

    char str[STR_SIZE];
    sprintf(str, "T[0] = %s AND (T[+1] = %s OR T[+2] = %s)", T(segment_tag), T(tag), T(tag));
    return string(str);
}
};

template<class Lexeme>
class NNNTagPredicateTemplateP1 : public PredicateTemplate<Lexeme>
{
public:
NNNTagPredicateTemplateP1(const vector<const Tagset*> tagsets) : PredicateTemplate<Lexeme>(tagsets) { }

void findMatchingPredicates(vector<Predicate<Lexeme> >& v,
                                                      vector<Lexeme>& text,
                                                      int index) {

    Predicate<Lexeme> p = Predicate<Lexeme>(this);
    p.params.segment_tag = text[index].chosen_tag[0];
    p.params.tag = text[index + 1].chosen_tag[0];
    v.push_back(p);

    if (text[index + 2].chosen_tag[0] != text[index + 1].chosen_tag[0]) {
        p = Predicate<Lexeme>(this);
        p.params.segment_tag = text[index].chosen_tag[0];
        p.params.tag = text[index + 2].chosen_tag[0];
        v.push_back(p);
    }

    if (text[index + 2].chosen_tag[0] != text[index + 3].chosen_tag[0] && 
            text[index + 3].chosen_tag[0] != text[index + 1].chosen_tag[0]) {
        p = Predicate<Lexeme>(this);
        p.params.segment_tag = text[index].chosen_tag[0];
        p.params.tag = text[index + 3].chosen_tag[0];
        v.push_back(p);
    }
}
bool predicateMatches(const Predicate<Lexeme>& p,
            vector<Lexeme>& text, int index) {
    return (p.params.tag == text[index + 1].chosen_tag[0]
            || p.params.tag == text[index + 2].chosen_tag[0]
            || p.params.tag == text[index + 3].chosen_tag[0])
            && p.params.segment_tag == text[index].chosen_tag[0];
}
string predicateAsString(const Predicate<Lexeme>& p) {

    char str[STR_SIZE];
    sprintf(str, "T[0] = %s AND (T[+1] = %s OR T[+2] = %s OR T[+3] = %s)", T(segment_tag), T(tag), T(tag), T(tag));
    return string(str);
}
};

template<class Lexeme>
class BothNNTagPredicateTemplateP1 : public PredicateTemplate<Lexeme>
{
public:
BothNNTagPredicateTemplateP1(const vector<const Tagset*> tagsets) : PredicateTemplate<Lexeme>(tagsets) { }

void findMatchingPredicates(vector<Predicate<Lexeme> >& v,
                                                      vector<Lexeme>& text,
                                                      int index) {

    Predicate<Lexeme> p = Predicate<Lexeme>(this);
    p.params.segment_tag = text[index].chosen_tag[0];
    p.params.tags[0] = text[index + 1].chosen_tag[0];
    p.params.tags[1] = text[index + 2].chosen_tag[0];
    v.push_back(p);
}
bool predicateMatches(const Predicate<Lexeme>& p,
            vector<Lexeme>& text, int index) {
    return (p.params.tags[0] == text[index + 1].chosen_tag[0]
            && p.params.tags[1] == text[index + 2].chosen_tag[0])
            && p.params.segment_tag == text[index].chosen_tag[0];
}
string predicateAsString(const Predicate<Lexeme>& p) {

    char str[STR_SIZE];
    sprintf(str, "T[0] = %s AND T[+1] = %s AND T[+2] = %s", T(segment_tag), T(tags[0]), T(tags[1]));
    return string(str);
}
};

template<class Lexeme>
class CCaseTagPredicateTemplateP1 : public PredicateTemplate<Lexeme>
{
public:
CCaseTagPredicateTemplateP1(const vector<const Tagset*> tagsets) : PredicateTemplate<Lexeme>(tagsets) { }

void findMatchingPredicates(vector<Predicate<Lexeme> >& v,
                                                      vector<Lexeme>& text,
                                                      int index) {

    if (text[index].getOrth()[0] >= 'A' && text[index].getOrth()[0] <= 'Z') {
        Predicate<Lexeme> p = Predicate<Lexeme>(this);
        p.params.segment_tag = text[index].chosen_tag[0];
        v.push_back(p);
    }
}
bool predicateMatches(const Predicate<Lexeme>& p,
            vector<Lexeme>& text, int index) {
    return (p.params.segment_tag == text[index].chosen_tag[0]
            && text[index].getOrth()[0] >= 'A' && text[index].getOrth()[0] <= 'Z');
}
string predicateAsString(const Predicate<Lexeme>& p) {

    char str[STR_SIZE];
    sprintf(str, "T[0] = %s AND ORTH[0] starts with capital letter", T(segment_tag));
    return string(str);
}
};

template<class Lexeme>
class Prefix2TagPredicateTemplateP1 : public PredicateTemplate<Lexeme>
{
public:
Prefix2TagPredicateTemplateP1(const vector<const Tagset*> tagsets) : PredicateTemplate<Lexeme>(tagsets) { }

void findMatchingPredicates(vector<Predicate<Lexeme> >& v,
                                                      vector<Lexeme>& text,
                                                      int index) {

    const string& orth = text[index].getOrth();
    int len = orth.length();
    if (len >= 2) {
        Predicate<Lexeme> p = Predicate<Lexeme>(this);
        p.params.segment_tag = text[index].chosen_tag[0];
        p.params.chars[0] = orth[0];
        p.params.chars[1] = orth[1];
        v.push_back(p);
    }
}
bool predicateMatches(const Predicate<Lexeme>& p,
            vector<Lexeme>& text, int index) {
    const string& orth = text[index].getOrth();
    int len = orth.length();
    return (len >= 2 && p.params.segment_tag == text[index].chosen_tag[0]
            && orth[0] == p.params.chars[0]
            && orth[1] == p.params.chars[1]);
}
string predicateAsString(const Predicate<Lexeme>& p) {

    char str[STR_SIZE];
    sprintf(str, "T[0] = %s AND ORTH starts with %c%c", T(segment_tag), p.params.chars[0], p.params.chars[1]);
    return string(str);
}
};

template<class Lexeme>
class Suffix2TagPredicateTemplateP1 : public PredicateTemplate<Lexeme>
{
public:
Suffix2TagPredicateTemplateP1(const vector<const Tagset*> tagsets) : PredicateTemplate<Lexeme>(tagsets) { }

void findMatchingPredicates(vector<Predicate<Lexeme> >& v,
                                                      vector<Lexeme>& text,
                                                      int index) {

    const string& orth = text[index].getOrth();
    int len = orth.length();
    if (len >= 2) {
        Predicate<Lexeme> p = Predicate<Lexeme>(this);
        p.params.segment_tag = text[index].chosen_tag[0];
        p.params.chars[0] = orth[len - 2];
        p.params.chars[1] = orth[len - 1];
        v.push_back(p);
    }
}
bool predicateMatches(const Predicate<Lexeme>& p,
            vector<Lexeme>& text, int index) {
    const string& orth = text[index].getOrth();
    int len = orth.length();
    return (len >= 2 && p.params.segment_tag == text[index].chosen_tag[0]
            && orth[len - 2] == p.params.chars[0]
            && orth[len - 1] == p.params.chars[1]);
}
string predicateAsString(const Predicate<Lexeme>& p) {

    char str[STR_SIZE];
    sprintf(str, "T[0] = %s AND ORTH ends with %c%c", T(segment_tag), p.params.chars[0], p.params.chars[1]);
    return string(str);
}
};



// UTILITY FUNCTIONS

template<class Lexeme>
vector<PredicateTemplate<Lexeme>*> make_p1_rules(const vector<const Tagset*>& tagsets) {
    vector<PredicateTemplate<Lexeme>*> templates;

    templates.push_back(new PTagPredicateTemplateP1<Lexeme>(tagsets));
    templates.push_back(new P2TagPredicateTemplateP1<Lexeme>(tagsets));
    templates.push_back(new PPTagPredicateTemplateP1<Lexeme>(tagsets));
    templates.push_back(new PPPTagPredicateTemplateP1<Lexeme>(tagsets));
    templates.push_back(new BothPPTagPredicateTemplateP1<Lexeme>(tagsets));
    templates.push_back(new BothPNTagPredicateTemplateP1<Lexeme>(tagsets));
    templates.push_back(new NTagPredicateTemplateP1<Lexeme>(tagsets));
    templates.push_back(new N2TagPredicateTemplateP1<Lexeme>(tagsets));
    templates.push_back(new NNTagPredicateTemplateP1<Lexeme>(tagsets));
    templates.push_back(new NNNTagPredicateTemplateP1<Lexeme>(tagsets));
    templates.push_back(new BothNNTagPredicateTemplateP1<Lexeme>(tagsets));
    templates.push_back(new CCaseTagPredicateTemplateP1<Lexeme>(tagsets));
    templates.push_back(new Prefix2TagPredicateTemplateP1<Lexeme>(tagsets));
    templates.push_back(new Suffix2TagPredicateTemplateP1<Lexeme>(tagsets));

    return templates;
}

