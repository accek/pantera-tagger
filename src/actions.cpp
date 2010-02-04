template<class Lexeme, int Phase>
class ChangeTagActionTemplate : public ActionTemplate<Lexeme> {
public:
    typedef typename Lexeme::tag_type tag_type;

    ChangeTagActionTemplate(const std::vector<const Tagset*>& tagsets)
            : ActionTemplate<Lexeme>(tagsets) { }

    virtual bool actionApplicable(const Action<Lexeme>& a,
            std::vector<Lexeme>& text, int index) {
        const Lexeme& lex = text[index];
        return lex.isConsideredTag(a.params.tag) &&
            a.params.tag != lex.chosen_tag[Phase];
    }

    virtual void findMatchingRules(const Predicate<Lexeme>& p,
            std::vector<Rule<Lexeme> >& rules,
            std::vector<Lexeme>& text, int index) {
        typedef typename Lexeme::tag_type tag_type;
        const Lexeme& lex = text[index];
        Action<Lexeme> act(this);
        BOOST_FOREACH(const tag_type& ctag, lex.considered_tags)
            if (ctag != lex.chosen_tag[Phase]) {
                act.params.tag = ctag;
                rules.push_back(Rule<Lexeme>(p, act));
            }
    }

    virtual const tag_type& changedTag(const Action<Lexeme>& a,
            const tag_type& original_tag) const {
        return a.params.tag;
    }

    virtual string actionAsString(const Action<Lexeme>& a) {
        return boost::str(boost::format("T[0] := %1%") %
            a.params.tag.asString(this->tagsets[Phase]));
    }
};
