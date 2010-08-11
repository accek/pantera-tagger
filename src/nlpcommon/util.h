/*
 * util.h
 *
 *  Created on: Jan 2, 2010
 *      Author: accek
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/format.hpp>
#include <boost/mpi.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <ostream>
#include <algorithm>
#include <utility>
#include <functional>
#include <numeric>

// Various utility functions.

namespace NLPCommon {

using std::string;
using std::wstring;
using std::vector;
namespace fs = boost::filesystem;
namespace po = boost::program_options;
namespace mpi = boost::mpi;

class Tagset;


// Returns a part of the passed haystack before first occurence of needle.
// Updates haystack to contain only the part after needle.
// If needle is not found, entire haystack is returned and updated to empty
// string.
string find_prefix(string& haystack, char needle);

// String - wstring conversion
wstring ascii_to_wstring(const string& wstring);
wstring utf8_to_wstring(const string& s);
string wstring_to_utf8(const wstring& s);


// Output a difference between actual and expected tags on one token
// in a human-readable way.
template<class Tag>
void writeTagsDifference(std::wostream& stream,
        const Tagset* tagset,
        const vector<Tag>& actual_tags,
        const vector<Tag>& expected_tags) {
    BOOST_FOREACH(const Tag& tag, actual_tags) {
        stream << ' ';
        if (std::find(expected_tags.begin(), expected_tags.end(), tag) ==
                expected_tags.end())
            stream << '*';
        stream << tag.asWString(tagset);
    }
    BOOST_FOREACH(const Tag& tag, expected_tags)
        if (std::find(actual_tags.begin(), actual_tags.end(), tag) ==
                actual_tags.end())
            stream << " [" << tag.asWString(tagset) << ']';
}

// Comparator functions for std::pair objects which compares
// only the first elements.
template<class Pair>
bool less_first(const Pair& a, const Pair& b) {
    return std::less<typename Pair::first_type>()(a.first, b.first);
}
template<class Pair>
bool greater_first(const Pair& a, const Pair& b) {
    return std::greater<typename Pair::first_type>()(a.first, b.first);
}

// Utility path functions.

fs::path find_with_path(const string& path, const string& filename);

// Locale.

std::locale& get_locale(const char* name);

// MPI progress controller.

class MPIProgressController
{
private:
    mpi::communicator& mpi_world;
    vector<float> progress;
    int progress_done;
    string progress_msg;
    float progress_sum;

    enum {
        MSG_PROGRESS = 501,
        MSG_DONE = 502
    };

protected:
    virtual void print() {
        float p = std::accumulate(progress.begin(), progress.end(), 0.0)
            / progress_sum;
        std::cerr << '\r' << progress_msg
            << boost::format("  %.2f%%\x1b[K") % (p*100.0) << '\r';
    }

public:
    MPIProgressController(mpi::communicator& mpi_world)
        : mpi_world(mpi_world) { }

    void init(const string& msg, float expected_sum = 0.0) {
        mpi_world.barrier();
        if (mpi_world.rank() == 0) {
            progress.clear();
            progress.resize(mpi_world.size());
            progress_done = 0;
            progress_msg = msg;
            if (expected_sum == 0.0) {
                progress_sum = progress.size();
            } else {
                progress_sum = expected_sum;
            }
        }
    }

    void set(float p) {
        if (mpi_world.rank() == 0) {
            boost::optional<mpi::status> status;
            while ((status = mpi_world.iprobe(mpi::any_source, MSG_PROGRESS))) {
                float recv_p;
                mpi_world.recv(status.get().source(), MSG_PROGRESS, recv_p);
                assert(progress[status.get().source()] <= recv_p);
                progress[status.get().source()] = recv_p;
            }

            progress[0] = p;
            print();
        } else {
            mpi_world.isend(0, MSG_PROGRESS, p);
        }
    }

    void finish() {
        if (mpi_world.rank() == 0) {
            progress_done++;
            progress[0] = 1.0;
            while (progress_done < mpi_world.size()) {
                mpi::status status = mpi_world.probe(mpi::any_source, mpi::any_tag);
                if (status.tag() == MSG_PROGRESS) {
                    float p;
                    mpi_world.recv(status.source(), MSG_PROGRESS, p);
                    assert(progress[status.source()] <= p);
                    progress[status.source()] = p;
                    print();
                } else if (status.tag() == MSG_DONE) {
                    mpi_world.recv(status.source(), MSG_DONE);
                    progress[status.source()] = 1.0;
                    progress_done++;
                    print();
                } else {
                    assert(false);
                }
            }

            std::cerr << '\r' << progress_msg << "  done.\x1b[K" << std::endl;
        } else {
            mpi_world.send(0, MSG_DONE);
        }
        mpi_world.barrier();
    }
};

} // namespace NLPCommon

extern boost::program_options::variables_map options;

#endif /* COMMON_H_ */
