m4_include(`forloop.m4')

m4_define(`__MAKE_PTEMPLATE_CLASSHDR', `
class $1 : public PredicateTemplate<Lexeme>
{
public:
    $1(const vector<const Tagset*> tagsets) : PredicateTemplate<Lexeme>(tagsets) { }
')

m4_define(`PTEMPLATE_BEGIN', `
template<class Lexeme, int Phase`'m4_dnl
m4_define(`PTEMPLATE_NUM_OFFSETS', m4_ifelse(`$2', `', `0', `$2'))m4_dnl
forloop(`i', `1', PTEMPLATE_NUM_OFFSETS, `, int `Offset'i')m4_dnl
$3>
__MAKE_PTEMPLATE_CLASSHDR($1)
')

m4_define(`PTEMPLATE_FOR_EACH_OFFSET', `
forloop(`O', `1', PTEMPLATE_NUM_OFFSETS, `m4_dnl
m4_define(`Offset', ``Offset'O')m4_dnl
m4_ifelse(O, `1', `', `$2')m4_dnl
$1`'m4_dnl
m4_undefine(``Offset'')`'')
')

m4_define(`PTEMPLATE_LOOP_EACH_OFFSET', `
forloop(`O', `1', PTEMPLATE_NUM_OFFSETS, `m4_dnl
m4_define(`Offset', ``Offset'O')m4_dnl
m4_ifelse(O, `1', `', `$2')m4_dnl
do { $1 } while(0);`'m4_dnl
m4_undefine(``Offset'')`'')
')

m4_define(`PTEMPLATE_FIND_PREDICATES', `
    void findMatchingPredicates(vector<Predicate<Lexeme> >& v,
                                                          vector<Lexeme>& text,
                                                          int index)m4_dnl
')

m4_define(`PTEMPLATE_MATCH', `
    bool predicateMatches(const Predicate<Lexeme>& p,
                vector<Lexeme>& text, int index)m4_dnl
')

m4_define(`PTEMPLATE_STRING_REPR', `
    string predicateAsString(const Predicate<Lexeme>& p) {
        char str[STR_SIZE];
        sprintf(str, $@);
        return string(str);
    }m4_dnl
')

m4_define(`PTEMPLATE_USES_CATEGORY0', `
    bool usesCategory0() {
        m4_ifelse($1, `yes', `return true;', `return false;')
    }m4_dnl
')

m4_define(`PTEMPLATE_END', `
};m4_dnl
')

