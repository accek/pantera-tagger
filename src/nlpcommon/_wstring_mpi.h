#ifndef _WSTRING_MPI_H
#define _WSTRING_MPI_H

#include <string>
#include <boost/mpi/packed_iarchive.hpp>

#include <nlpcommon/util.h>

namespace boost {
namespace serialization {

static inline void serialize(boost::mpi::packed_iarchive& ar, std::wstring& ws,
        const unsigned int file_version) {
    std::string s;
    ar & s;
    ws = NLPCommon::utf8_to_wstring(s);
}

static inline void serialize(boost::mpi::packed_oarchive& ar, std::wstring& ws,
        const unsigned int file_version) {
    std::string s(NLPCommon::wstring_to_utf8(ws));
    ar & s;
}

} // serialization
} // namespace boost

#endif
