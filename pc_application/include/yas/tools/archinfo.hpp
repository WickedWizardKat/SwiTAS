
// Copyright (c) 2010-2019 niXman (i dot nixman dog gmail dot com). All
// rights reserved.
//
// This file is part of YAS(https://github.com/niXman/yas) project.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//
//
// Boost Software License - Version 1.0 - August 17th, 2003
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
//
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#ifndef __yas__tools__archinfo_hpp
#define __yas__tools__archinfo_hpp

#include <yas/binary_oarchive.hpp>
#include <yas/binary_iarchive.hpp>
#include <yas/text_oarchive.hpp>
#include <yas/text_iarchive.hpp>

namespace yas {

/***************************************************************************/

template<typename IS>
inline bool is_yas_archive(IS &is) {
    constexpr auto yas_id_size = sizeof(detail::header::yas_id);
    char buf[yas_id_size];
    __YAS_THROW_READ_ERROR(yas_id_size != is.read(buf, yas_id_size));

    return std::memcmp(buf, detail::header::yas_id, yas_id_size) == 0;
}

inline bool is_yas_archive(const yas::shared_buffer &buf) {
    mem_istream is(buf);

    return is_yas_archive(is);
}

inline bool is_yas_archive(const yas::intrusive_buffer &buf) {
    mem_istream is(buf);

    return is_yas_archive(is);
}

inline bool is_yas_archive(const char *fname) {
    file_istream is(fname);

    return is_yas_archive(is);
}

/***************************************************************************/

template<typename IS>
inline detail::header::archive_header read_header(IS &is) {
    detail::header::archive_header header;
    detail::header::read_header(is, header);

    return header;
}

inline detail::header::archive_header read_header(const yas::intrusive_buffer &buf) {
    detail::header::archive_header header;
    mem_istream is(buf);
    detail::header::read_header(is, header);

    return header;
}

inline detail::header::archive_header read_header(const yas::shared_buffer &buf) {
    detail::header::archive_header header;
    mem_istream is(buf);
    detail::header::read_header(is, header);

    return header;
}

inline detail::header::archive_header read_header(const char *fname) {
    detail::header::archive_header header;
    file_istream is(fname);
    detail::header::read_header(is, header);

    return header;
}

/***************************************************************************/

inline std::size_t archive_version(const detail::header::archive_header &h) {
    return h.bits.version;
}

inline std::size_t archive_version(const yas::intrusive_buffer &buf) {
    const auto header = read_header(buf);

    return archive_version(header);
}

inline std::size_t archive_version(const yas::shared_buffer &buf) {
    const auto header = read_header(buf);

    return archive_version(header);
}

inline std::size_t archive_version(const char *fname) {
    const auto header = read_header(fname);

    return archive_version(header);
}

/***************************************************************************/

inline options archive_type(const detail::header::archive_header &h) {
    return __YAS_SCAST(options, h.bits.type);
}

inline options archive_type(const yas::intrusive_buffer &buf) {
    const auto header = read_header(buf);

    return archive_type(header);
}

inline options archive_type(const yas::shared_buffer &buf) {
    const auto header = read_header(buf);

    return archive_type(header);
}

inline options archive_type(const char *fname) {
    const auto header = read_header(fname);

    return archive_type(header);
}

/***************************************************************************/

inline options archive_endian(const detail::header::archive_header &h) {
    return h.bits.endian ? options::ebig : options::elittle;
}

inline options archive_endian(const yas::intrusive_buffer &buf) {
    const auto header = read_header(buf);

    return archive_endian(header);
}

inline options archive_endian(const yas::shared_buffer &buf) {
    const auto header = read_header(buf);

    return archive_endian(header);
}

inline options archive_endian(const char *fname) {
    const auto header = read_header(fname);

    return archive_endian(header);
}

/***************************************************************************/

inline bool archive_is_compacted(const detail::header::archive_header &h) {
    return h.bits.compacted;
}

inline bool archive_is_compacted(const yas::intrusive_buffer &buf) {
    const auto header = read_header(buf);

    return archive_is_compacted(header);
}

inline bool archive_is_compacted(const yas::shared_buffer &buf) {
    const auto header = read_header(buf);

    return archive_is_compacted(header);
}

inline bool archive_is_compacted(const char *fname) {
    const auto header = read_header(fname);

    return archive_is_compacted(header);
}

/***************************************************************************/

} // namespace yas

#endif // __yas__tools__archinfo_hpp
