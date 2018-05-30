// Copyright (c) 2016-2018 Antony Polukhin
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/core/lightweight_test.hpp>

#include <iostream>
#include <typeinfo>
#include <tuple>
#include <sstream>
#include <set>
#include <string>


#ifdef BOOST_PFR_TEST_FLAT
#include <boost/pfr/flat/ops.hpp>
#define BOOST_PFR_TEST_NAMESPECE boost::pfr::flat_ops
#endif

#ifdef BOOST_PFR_TEST_PRECISE
#include <boost/pfr/precise/ops.hpp>
#define BOOST_PFR_TEST_NAMESPECE boost::pfr::ops
#endif

unsigned test_union_counter = 0;

union test_union {
    int i;
    float f;
};

inline bool operator< (test_union l, test_union r) noexcept { ++test_union_counter; return l.i <  r.i; }
inline bool operator<=(test_union l, test_union r) noexcept { ++test_union_counter; return l.i <= r.i; }
inline bool operator> (test_union l, test_union r) noexcept { ++test_union_counter; return l.i >  r.i; }
inline bool operator>=(test_union l, test_union r) noexcept { ++test_union_counter; return l.i >= r.i; }
inline bool operator==(test_union l, test_union r) noexcept { ++test_union_counter; return l.i == r.i; }
inline bool operator!=(test_union l, test_union r) noexcept { ++test_union_counter; return l.i != r.i; }
inline std::ostream& operator<<(std::ostream& os, test_union src) { ++test_union_counter; return os << src.i; }
inline std::istream& operator>>(std::istream& is, test_union& src) { ++test_union_counter; return is >> src.i; }


template <class T>
void test_comparable_struct() {
    using namespace BOOST_PFR_TEST_NAMESPECE;
    T s1 {0, 1, false, 6,7,8,9,10,11};
    T s2 = s1;
    T s3 {0, 1, false, 6,7,8,9,10,11111};
    BOOST_TEST(s1 == s2);
    BOOST_TEST(s1 <= s2);
    BOOST_TEST(s1 >= s2);
    BOOST_TEST(!(s1 != s2));
    BOOST_TEST(!(s1 == s3));
    BOOST_TEST(s1 != s3);
    BOOST_TEST(s1 < s3);
    BOOST_TEST(s3 > s2);
    BOOST_TEST(s1 <= s3);
    BOOST_TEST(s3 >= s2);

    std::cout << s1 << std::endl;

    T s4;
    std::stringstream ss;
    ss.exceptions ( std::ios::failbit);
    ss << s1;
    ss >> s4;
    std::cout << s4 << std::endl;
    BOOST_TEST(s1 == s4);
    int i = 1, j = 2;
    BOOST_TEST_NE(i, j);
}

void test_empty_struct() {
    struct empty {};
    using namespace BOOST_PFR_TEST_NAMESPECE;
    std::cout << empty{};
    BOOST_TEST(empty{} == empty{});
}

void test_implicit_conversions() {
    using namespace BOOST_PFR_TEST_NAMESPECE;
    std::stringstream ss;
    ss << std::true_type{};
    BOOST_TEST_EQ(ss.str(), "1"); // Does not break implicit conversion
}


namespace foo {
struct comparable_struct {
    int i; short s; bool bl; int a,b,c,d,e,f;
};
}

int main() {
    test_comparable_struct<foo::comparable_struct>();

    struct local_comparable_struct {
        int i; short s; bool bl; int a,b,c,d,e,f;
    };
    test_comparable_struct<local_comparable_struct>();

#if defined(BOOST_PFR_TEST_PRECISE)
    struct local_comparable_struct_with_union {
        int i; short s; bool bl; int a,b,c,d,e; test_union u;
    };
    test_comparable_struct<local_comparable_struct_with_union>();
    
    // Making sure that test_union overloaded operations were called.
    BOOST_TEST_EQ(test_union_counter, 17);
#endif

    test_empty_struct();
    test_implicit_conversions();

    return boost::report_errors();
}


