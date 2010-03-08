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

    virtual void findMatchingRules(const TemplatesStore<Lexeme>* tstore,
            const Predicate<Lexeme>& p,
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

    virtual tag_type changedTag(const Action<Lexeme>& a,
            std::vector<Lexeme>& text, int index) const {
        return a.params.tag;
    }

    virtual string actionAsString(const Action<Lexeme>& a) {
        return boost::str(boost::format("T[0] := %1%") %
            a.params.tag.asString(this->tagsets[Phase]));
    }
};

template<class Lexeme>
typename Lexeme::tag_type findNearestTag(const Tagset* tagset,
        const typename Lexeme::tag_type& tag,
        const vector<typename Lexeme::tag_type>& considered_tags,
        int fixed_category = -1) {
    int p = tag.getPos();
    const PartOfSpeech* pos = tagset->getPartOfSpeech(p);
    typename Lexeme::tag_type best_tag = Lexeme::tag_type::getNullTag();
    int best_tag_score = -1;

    BOOST_FOREACH(const typename Lexeme::tag_type& ctag, considered_tags) {
        if (ctag.getPos() == p && (fixed_category == -1 ||
                    ctag.getValue(fixed_category) ==
                    tag.getValue(fixed_category))) {
            int score = 0;
            BOOST_FOREACH(const Category* cat, pos->getCategories()) {
                int c = tagset->getCategoryIndex(cat);
                if (ctag.getValue(c) == tag.getValue(c))
                    score++;
            }
            if (score > best_tag_score) {
                best_tag_score = score;
                best_tag = ctag;
            }
        }
    }

    return best_tag;
}

template<class Lexeme, int Phase>
class ChangeCatActionTemplate : public ActionTemplate<Lexeme> {
public:
    typedef typename Lexeme::tag_type tag_type;

    ChangeCatActionTemplate(const std::vector<const Tagset*>& tagsets)
            : ActionTemplate<Lexeme>(tagsets) { }

    virtual bool actionApplicable(const Action<Lexeme>& a,
            std::vector<Lexeme>& text, int index) {
        const Lexeme& lex = text[index];
        tag_type tag = changedTag(a, text, index);
        return tag != tag_type::getNullTag() && tag != lex.chosen_tag[Phase];
    }

    virtual void findMatchingRules(const TemplatesStore<Lexeme>* tstore,
            const Predicate<Lexeme>& p,
            std::vector<Rule<Lexeme> >& rules,
            std::vector<Lexeme>& text, int index) {
        if (!tstore->getPTemplate(p.tpl_id)->usesCategory0())
            return;
        int c = p.params.categories[0];
        if (c == -1)
            return;

        Action<Lexeme> act(this);
        act.params.category = c;
        const Lexeme& lex = text[index];
        const tag_type& original_tag = lex.chosen_tag[Phase];
        int pindex = original_tag.getPos();
        BOOST_FOREACH(const tag_type& ctag, lex.considered_tags) {
            if (pindex == ctag.getPos()
                    && ctag.getValue(c) != original_tag.getValue(c)) {
                act.params.value = ctag.getValue(c);
                rules.push_back(Rule<Lexeme>(p, act));
            }
        }
    }

    virtual tag_type changedTag(const Action<Lexeme>& a,
            std::vector<Lexeme>& text, int index) const {
        tag_type tag(text[index].chosen_tag[Phase]);
        tag.setValue(a.params.category, a.params.value);
        return findNearestTag<Lexeme>(this->tagsets[Phase], tag,
                text[index].considered_tags, a.params.category);
    }

    virtual string actionAsString(const Action<Lexeme>& a) {
        return boost::str(boost::format("T[0]|%1% := %2%") %
                this->tagsets[Phase]->getCategory(a.params.category)->getName() %
                this->tagsets[Phase]->getCategory(a.params.category)->getValue(a.params.value));
    }
};

template<class Lexeme, int Phase>
class ChangePosActionTemplate : public ActionTemplate<Lexeme> {
public:
    typedef typename Lexeme::tag_type tag_type;

    ChangePosActionTemplate(const std::vector<const Tagset*>& tagsets)
            : ActionTemplate<Lexeme>(tagsets) { }

    virtual bool actionApplicable(const Action<Lexeme>& a,
            std::vector<Lexeme>& text, int index) {
        const Lexeme& lex = text[index];
        tag_type tag = changedTag(a, text, index);
        return tag != tag_type::getNullTag() && tag != lex.chosen_tag[Phase];
    }

    virtual void findMatchingRules(const TemplatesStore<Lexeme>* tstore,
            const Predicate<Lexeme>& p,
            std::vector<Rule<Lexeme> >& rules,
            std::vector<Lexeme>& text, int index) {
        Action<Lexeme> act(this);
        const Lexeme& lex = text[index];
        const tag_type& original_tag = lex.chosen_tag[Phase];
        BOOST_FOREACH(const tag_type& ctag, lex.considered_tags) {
            if (original_tag.getPos() != ctag.getPos()) {
                act.params.pos = ctag.getPos();
                rules.push_back(Rule<Lexeme>(p, act));
            }
        }
    }

    virtual tag_type changedTag(const Action<Lexeme>& a,
            std::vector<Lexeme>& text, int index) const {
        tag_type tag(text[index].chosen_tag[Phase]);
        tag.setPos(a.params.pos);
        return findNearestTag<Lexeme>(this->tagsets[Phase], tag,
                text[index].considered_tags);
    }

    virtual string actionAsString(const Action<Lexeme>& a) {
        return boost::str(boost::format("T[0]|pos := %1%") %
                this->tagsets[Phase]->getPartOfSpeech(a.params.pos)->getName());
    }
};
