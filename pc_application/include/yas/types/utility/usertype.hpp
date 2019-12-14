
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

#ifndef __yas__types__utility__usertype_hpp
#define __yas__types__utility__usertype_hpp

#include <yas/detail/type_traits/serializer.hpp>

namespace yas {
namespace detail {

/***************************************************************************/

template<std::size_t F, typename T>
struct serializer <
	 type_prop::not_a_fundamental,
	 ser_case::has_split_functions,
	 F,
	 T
> {
	 template<typename Archive>
	 static Archive& save(Archive& ar, const T& v) {
		  serialize(ar, v);
		  return ar;
	 }

	 template<typename Archive>
	 static Archive& load(Archive& ar, T& v) {
		  serialize(ar, v);
		  return ar;
	 }
};

/***************************************************************************/

template<std::size_t F, typename T>
struct serializer <
	 type_prop::not_a_fundamental,
	 ser_case::has_one_function,
	 F,
	 T
> {
	 template<typename Archive>
	 static Archive& save(Archive& ar, const T& v) {
		  serialize(ar, const_cast<T&>(v));
		  return ar;
	 }

	 template<typename Archive>
	 static Archive& load(Archive& ar, T& v) {
		  serialize(ar, v);
		  return ar;
	 }
};

/***************************************************************************/
/***************************************************************************/
/***************************************************************************/

template<std::size_t F, typename T>
struct serializer<
	 type_prop::not_a_fundamental,
	 ser_case::has_one_memfn,
	 F,
	 T
> {
	 template<typename Archive>
	 static Archive& save(Archive& ar, const T& v) {
		  const_cast<T&>(v).serialize(ar);
		  return ar;
	 }

	 template<typename Archive>
	 static Archive& load(Archive& ar, T& v) {
		  v.serialize(ar);
		  return ar;
	 }
};

/***************************************************************************/

template<std::size_t F, typename T>
struct serializer<
	 type_prop::not_a_fundamental,
	 ser_case::has_split_memfns,
	 F,
	 T
> {
	 template<typename Archive>
	 static Archive& save(Archive& ar, const T& v) {
		  v.serialize(ar);
		  return ar;
	 }

	 template<typename Archive>
	 static Archive& load(Archive& ar, T& v) {
		  v.serialize(ar);
		  return ar;
	 }
};

/***************************************************************************/

} // namespace detail
} // namespace yas

#endif // __yas__types__utility__usertype_hpp
