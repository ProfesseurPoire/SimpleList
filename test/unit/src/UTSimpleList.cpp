#include <cabba/test/test.h>
#include <cabba/containers/List.h>

using namespace cabba::test;
using namespace cabba;

struct ListTesting : public Test
{
    cabba::List<int> list;
};


TEST_F(ListTesting, push_back)
{
    list.push_back(12);

    assert_that(list.size(), equals(1));
    assert_that(list[0], equals(12));
}

TEST_F(ListTesting, removeAt)
{
    list.push_back(12);
    list.push_back(23);
    list.push_back(30);

    assert_that(list.size(), equals(3));
    assert_that(list[0], equals(12));
    assert_that(list[1], equals(23));
    assert_that(list[2], equals(30));

    list.remove_at(1);

    assert_that(list.size(), equals(2));
    assert_that(list[0], equals(12));
    assert_that(list[1], equals(30));
}

TEST_F(ListTesting, remove)
{
    // Arrange

    list.push_back(12);
    list.push_back(23);
    list.push_back(30);

    assert_that(list.size(), equals(3));
    assert_that(list[0], equals(12));
    assert_that(list[1], equals(23));
    assert_that(list[2], equals(30));

    // Act

    list.remove(12);

    // Assert 

    assert_that(list.size(), equals(2));
    assert_that(list[0], equals(23));
    assert_that(list[1], equals(30));
}

TEST_F(ListTesting, copy)
{
    // Arrange

    list.push_back(12);
    list.push_back(23);

    // Act

    cabba::List<int> c1(list);
    cabba::List<int> c2;
    c2 = list;

    // Assert

    assert_that(c1.size(), equals(2));
    assert_that(c1[0], equals(12));
    assert_that(c1[1], equals(23));

    assert_that(c2.size(), equals(2));
    assert_that(c2[0], equals(12));
    assert_that(c2[1], equals(23));
}

TEST_F(ListTesting, comparison)
{
    list.push_back(132);
    list.push_back(30);

    cabba::List<int> c;
    c.push_back(132);
    c.push_back(30);

    cabba::List<int> c2;
    c2.push_back(23);

    assert_that(c == list, equals(true));
    assert_that(c2 != list, equals(true));
}

TEST_F(ListTesting, clear)
{
    list.push_back(2);
    list.push_back(4);

    assert_that(list.size(), equals(2));

    list.clear();

    assert_that(list.size(), equals(0));
}

TEST_F(ListTesting, capacity)
{
    list.push_back(2);
    list.push_back(3);

    list.clear();

    assert_that(list.size(), equals(0));
    assert_that(list.capacity(), equals(2));
}

TEST_F(ListTesting, fit)
{
    list.push_back(2);
    list.push_back(3);

    list.clear();

    assert_that(list.size(), equals(0));

    list.fit();

    assert_that(list.size(), equals(2));
}

TEST_F(ListTesting, exist)
{
    list.push_back(2);
    list.push_back(3);

    assert_that(list.exist(3), equals(true));
    assert_that(list.exist(4), equals(false));
}

TEST_F(ListTesting, firstAndLast)
{
    list.push_back(2);
    list.push_back(3);
    list.push_back(4);

    assert_that(list.first(), equals(2));
    assert_that(list.last(), equals(4));
}

TEST_F(ListTesting, empty)
{
    assert_that(list.empty(), equals(true));

    list.push_back(1);

    assert_that(list.empty(), equals(false));
}

TEST_F(ListTesting, resize)
{
    list.resize(10);

    assert_that(list.size(), equals(0));
    assert_that(list.capacity(), equals(10));
}

TEST_F(ListTesting, swap)
{
    list.push_back(2);
    list.push_back(3);

    list.swap(0, 1);

    assert_that(list[0], equals(3));
    assert_that(list[1], equals(2));
}

TEST_F(ListTesting, memorySize)
{
    list.push_back(2);
    list.push_back(3);

    assert_that(list.memory_size(), equals((int)sizeof(int) * 2));
}

TEST_F(ListTesting, index)
{
    list.push_back(2);
    list.push_back(3);

    assert_that(list.index(2), equals(0));
    assert_that(list.index(3), equals(1));
    assert_that(list.index(4), equals(-1));
}

TEST_F(ListTesting, add)
{
    list.resize(3);

    list.add(3);
    list.add(5);
    list.add(1);

    assert_that(list[0], equals(3));
    assert_that(list[1], equals(5));
    assert_that(list[2], equals(1));
}