#ifndef FINDERRORS_H_
#define FINDERRORS_H_

#include <boost/foreach.hpp>
#include <boost/unordered_map.hpp>
#include <boost/functional/hash.hpp>
#include <ostream>
#include <algorithm>
#include <vector>

namespace NLPCommon {

using std::vector;

template<class Lexeme>
class TaggingErrorCategory {
    public:
        typedef typename Lexeme::tag_type tag_type;

        enum Type {
            TAG_MISMATCH,
            POS_MISMATCH,
            CAT_MISMATCH,
            INVALID_TYPE
        };

    private:
        const Tagset* _tagset;
        Type _type;
        vector<tag_type> _golden_tags;
        vector<tag_type> _autoselected_tags;
        int _category;
        int _actual_value, _expected_value;

        TaggingErrorCategory(const Tagset* tagset, Type type)
                : _tagset(tagset), _type(type) { }

        TaggingErrorCategory(const Tagset* tagset, Type type,
                const vector<tag_type>& golden_tags,
                const vector<tag_type>& autoselected_tags)
                : _tagset(tagset), _type(type), _category(0),
                  _actual_value(0), _expected_value(0) {
            _golden_tags = golden_tags;
            std::sort(_golden_tags.begin(), _golden_tags.end());
            _autoselected_tags = autoselected_tags;
            std::sort(_autoselected_tags.begin(), _autoselected_tags.end());
        }

        TaggingErrorCategory(const Tagset* tagset, Type type,
                int actual_value, int expected_value, int category = 0)
                : _tagset(tagset), _type(type), _category(category),
                  _actual_value(actual_value), _expected_value(expected_value)
        {
        }

        friend std::wostream& operator<<(std::wostream& stream,
                const TaggingErrorCategory<Lexeme> category) {
            switch (category.getType()) {
                case TAG_MISMATCH:
                    stream << "TAG-MISMATCH";
                    writeTagsDifference(stream, category.getTagset(),
                            category.getAutoselectedTags(),
                            category.getGoldenTags());
                    break;

                case POS_MISMATCH:
                    stream << "POS-MISMATCH *" <<
                        category.getTagset()->getPartOfSpeech(
                                category.getActualValue())->getWName()
                        << " [" <<
                        category.getTagset()->getPartOfSpeech(
                                category.getExpectedValue())->getWName()
                        << ']';
                    break;

                case CAT_MISMATCH:
                {
                    int cat_id = category.getCategory();
                    const Category* cat =
                        category.getTagset()->getCategory(cat_id);

                    stream << "CAT-MISMATCH " << cat->getWName() << " *" <<
                        cat->getWValue(category.getActualValue())
                        << " [" <<
                        cat->getWValue(category.getExpectedValue())
                        << ']';
                    break;
                }
            }
            return stream;
        }

    public:
        TaggingErrorCategory() : _tagset(NULL), _type(INVALID_TYPE) { }

        bool operator==(const TaggingErrorCategory& other) const {
            return getTagset() == other.getTagset()
                && getType() == other.getType()
                && getGoldenTags() == other.getGoldenTags()
                && getAutoselectedTags() == other.getAutoselectedTags()
                && getCategory() == other.getCategory()
                && getExpectedValue() == other.getExpectedValue()
                && getActualValue() == other.getActualValue();
        }

        static TaggingErrorCategory<Lexeme> makeTagMismatch(
            const Tagset* tagset, const Lexeme& lex) {
            return TaggingErrorCategory<Lexeme>(tagset, TAG_MISMATCH,
                    lex.getGoldenTags(), lex.getAutoselectedTags());
        }

        static TaggingErrorCategory<Lexeme> makePosMismatch(
                const Tagset* tagset, int actual_pos, int expected_pos) {
            return TaggingErrorCategory<Lexeme>(tagset, POS_MISMATCH,
                    actual_pos, expected_pos);
        }

        static TaggingErrorCategory<Lexeme> makeCatMismatch(
                const Tagset* tagset, int category_id,
                int actual_value, int expected_value) {
            return TaggingErrorCategory<Lexeme>(tagset, CAT_MISMATCH,
                    actual_value, expected_value, category_id);
        }

        const Tagset* getTagset() const
            { return _tagset; }
        Type getType() const
            { return _type; }
        const vector<tag_type>& getGoldenTags() const
            { return _golden_tags; }
        const vector<tag_type>& getAutoselectedTags() const
            { return _autoselected_tags; }
        int getCategory() const
            { return _category; }
        int getActualValue() const
            { return _actual_value; }
        int getExpectedValue() const
            { return _expected_value; }
};

template<class Lexeme>
std::size_t hash_value(const TaggingErrorCategory<Lexeme>& cat) {
    std::size_t seed = 0;
    boost::hash_combine(seed, cat.getTagset());
    boost::hash_combine(seed, cat.getType());
    boost::hash_combine(seed, cat.getGoldenTags());
    boost::hash_combine(seed, cat.getAutoselectedTags());
    boost::hash_combine(seed, cat.getCategory());
    return seed;
}

template<class Lexeme>
class TaggingError {
private:
    const Tagset* tagset;
    TaggingErrorCategory<Lexeme> category;
    const vector<Lexeme>* text;
    int offset;

public:
    static const int DEFAULT_CONTEXT_SIZE = 3;

    TaggingError(const Tagset* tagset,
            const TaggingErrorCategory<Lexeme>& category,
            const vector<Lexeme>& text, int offset)
        : tagset(tagset), category(category), text(&text), offset(offset) { }

    const Tagset* getTagset() const
        { return tagset; }
    const TaggingErrorCategory<Lexeme>& getCategory() const
        { return category; }
    const vector<Lexeme>& getText() const
        { return *text; }
    int getIndex() const
        { return index; }

    void writeToStream(std::wostream& stream,
            int context_size = DEFAULT_CONTEXT_SIZE) const {
        int a = std::max(0, offset - context_size);
        int b = std::min<int>(text->size() - 1, offset + context_size);
        for (int i = a; i <= b; i++) {
            if (i == offset)
                stream << ">> ";
            else
                stream << "   ";
            (*text)[i].writeToStreamWithTags(stream, tagset);
        }
    }

    friend std::wostream& operator<<(std::wostream& stream, const TaggingError& e)
    {
        e.writeToStream(stream);
        return stream;
    }
};

template<class Lexeme>
class TaggingErrorsCollector
{
public:
    typedef TaggingError<Lexeme> error_type;
    typedef TaggingErrorCategory<Lexeme> category_type;
    typedef std::pair<category_type, vector<error_type> > group_type;

private:
    const Tagset* _tagset;
    vector<error_type> _errors;
    mutable vector<group_type> _groups;
    mutable bool _groupsReady;

protected:
    void groupErrors() const {
        boost::unordered_map<category_type, vector<error_type> > map;
        typedef std::pair<category_type, vector<error_type> > map_entry_type;
        BOOST_FOREACH(const error_type& err, _errors)
            map[err.getCategory()].push_back(err);

        typedef std::pair<int, category_type > freqs_entry_type;
        vector<freqs_entry_type> freqs;
        BOOST_FOREACH(const map_entry_type& e, map)
            freqs.push_back(std::make_pair(e.second.size(), e.first));
        std::sort(freqs.begin(), freqs.end(), greater_first<freqs_entry_type>);

        _groups.clear();
        BOOST_FOREACH(const freqs_entry_type& e, freqs)
            _groups.push_back(std::make_pair(e.second, map[e.second]));

        _groupsReady = true;
    }


public:
    TaggingErrorsCollector(const Tagset* tagset)
        : _tagset(tagset), _groupsReady(false) { }

    const Tagset* getTagset() const { return _tagset; }
    const vector<error_type>& getErrors() const { return _errors; }
    const vector<group_type>& getGroups() const {
        if (!_groupsReady)
            groupErrors();
        return _groups;
    }

    void addTaggingErrors(const vector<Lexeme>& text) {
        for (int i = 0; i < text.size(); i++) {
            const Lexeme& lex = text[i];
            if (lex.getAutoselectedTags() != lex.getGoldenTags()) {
                // First: general tag mismatch
                category_type category = category_type::makeTagMismatch(
                        getTagset(), lex);
                _errors.push_back(error_type(getTagset(), category, text, i));

                if (lex.getAutoselectedTags().size() == 1 &&
                        lex.getGoldenTags().size() == 1) {
                    const typename Lexeme::tag_type actual_tag =
                        lex.getAutoselectedTags()[0];
                    const typename Lexeme::tag_type expected_tag =
                        lex.getGoldenTags()[0];

                    if (actual_tag.getPos() != expected_tag.getPos()) {
                        // POS mismatch
                        category_type c =
                            category_type::makePosMismatch(getTagset(),
                                    actual_tag.getPos(), expected_tag.getPos());
                        _errors.push_back(error_type(getTagset(), c, text, i));
                    } else {
                        // or Category mismatch
                        const PartOfSpeech* pos =
                            getTagset()->getPartOfSpeech(actual_tag.getPos());
                        BOOST_FOREACH(const Category* cat, pos->getCategories())
                        {
                            int cat_idx = getTagset()->getCategoryIndex(cat);
                            if (actual_tag.getValue(cat_idx) !=
                                    expected_tag.getValue(cat_idx)) {
                                category_type c =
                                    category_type::makeCatMismatch(getTagset(),
                                            cat_idx,
                                            actual_tag.getValue(cat_idx),
                                            expected_tag.getValue(cat_idx));
                                _errors.push_back(error_type(getTagset(), c,
                                            text, i));
                            }
                        }
                    }
                }
                _groupsReady = false;
            }
        }
    }

};




} // namespace NLPCommon

#endif
