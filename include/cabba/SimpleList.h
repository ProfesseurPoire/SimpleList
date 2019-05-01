#pragma once

namespace cabba
{
template<class T>
class SimpleList
{
public:

    SimpleList() = default;
    SimpleList(const SimpleList&l) { copy(l); }
    ~SimpleList() { delete[] _items; }

    T& operator[](const int index) { return _items[index]; }

    SimpleList& operator=(const SimpleList& l)
    {
        copy(l);
        return *this;
    }

    void add(const T& item)
    {
        if (_capacity < (++_itemCount)) resize(_itemCount);
        _items[_itemCount - 1] = item;
    }

    int size() const { return _itemCount; }

    void resize(const int new_size)
    {
        T* values = new T[new_size];

        for (int i = 0; i < _capacity; ++i)
            values[i] = _items[i];

        delete[] _items;
        _items = values;
        _capacity = new_size;
    }

private:

    void copy(const SimpleList& l)
    {
        if (l.size() == 0)
            return;

        allocate(l.size());

        for (int i = 0; i < l.size(); ++i)
            _items[i] = l._items[i];
    }
};
}