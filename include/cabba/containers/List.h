#pragma once

#include <assert.h>

namespace cabba 
{
using SizeType = int;

/*!
 * @brief   Stores items in a contiguous dynamic array that can change size
 * 
 *          Use the [] operator to access an item by it's position.
 *           
 *          Use push_back() if you don't know how many item the list can hold.
 *          
 *          Use resize(const SizeType size) or List(const SizeType size) 
 *          if you know how many items your list will hold and add(ConstReference)
 *          
 *          The functions does 0 check (just assertions)
 */
template<class ValueType>
class List
{
    using Reference      = ValueType&;
    using ConstReference = const ValueType&;

    using Pointer        = ValueType*;
    using ConstPointer   = const ValueType*;

public:

// Constructors

    /*!
    * @brief Default constructor
    */
    List() = default;
    
    // Warning : this takes ownership of values
    List(Pointer values, int count)
        : _items(values), _capacity(count), _item_count(count){}

    List(const List&l)              {copy(l);}
    explicit List(const int size)   {allocate(size);}
    ~List()                         {delete [] _items;}

    /*!
    * @brief    Initialize the list with a certain @ref size
    *           Useful when you know in advance how many item you have to
    *           store in the list. This will reduce the need to resize the array,
    *           which is a very costly operation
    */

  /*  List(List&& l) 
        {
        move(l);
    }*/


// Subscript Operator

    /*!
     * @brief Returns a reference to the item stored at the given position
     * @pre @ref pos must in the list range
     */
    ConstReference operator[](const int pos) const
    {
        assert(in_range(pos));
        return _items[pos];
    }

    /*!
    * @brief Returns a const reference to the item stored the given position
    * @pre @ref pos must be in the list range
    */
    Reference operator[](const int pos)
    {
        assert(in_range(pos));
        return _items[pos];
    }

// Assignment Operators

    List& operator=(const List& l)      {copy(l); return *this;}
    List& operator=(List&& l) noexcept  {move(l); return *this;}

// Comparison operators

    /*!
     * @brief   Returns true if the compared list stores the same items in the 
     *          same order
     */
    bool operator==(const List& l) const 
    {
        if (l.size() != size())
            return false;

        for (int i = 0; i< _item_count; ++i)
        {
            if (_items[i] != l._items[i])
                return false;
        }
        return true;
    }

    /*!
     * @brief   Returns false if the two list stores the same items in the
     *          same order
     */
    bool operator!=(const List&l)const {return !(*this == l);}

// Methods

    /*!
     * @brief   Removes every item from the list.
     *
     *          Effectively set _item_count to 0. The memory is not unallocated.
     */
    void clear() { _item_count = 0;}

    /*!
    * @brief    Adds an item to the list.
    *           The list gets resized if no space to add the item is available
    */
    void push_back(ConstReference item)
    {
        // We only resize if we have a reason to do so
        if (_capacity < (++_item_count))
            resize(_item_count);

        _items[_item_count - 1] = item;
    }

    /*!
    * @brief    Adds an item to the list. Does not resize or reallocate the
    *           list. If there's no space available, undefined behavior occurs.
    *           The motivation behind this function is to avoid checks if you
    *           know the maximum amount of item you might have in your list.
    *           Not really sure how useful this is but it's there at least so
    */
    void add(ConstReference item) { _items[++_item_count - 1] = item;}

    void add(const List& list) 
    {
        const int last_item_count = _item_count;
        _item_count += list.size();

        if (_capacity < _item_count)
            resize(_item_count);

        for (int i = 0; i< list.size(); ++i)
            _items[last_item_count + i] = list._items[i];
    }

    /*!
    * @brief  Returns a const reference to the item stored at @ref index
    * position No checks are made to know if @ref index is within bounds
    */
    ConstReference at(const SizeType index)const
    {
        assert(in_range(index));
        return _items[index];
    }

    /*!
     * @brief   Returns how many item are stored by the list
     */
    SizeType size() const           {return _item_count;}

   /*!
    * @brief    Returns how many items the list can actually hold
    */ 
    SizeType capacity()const        {return _capacity;}

    /*!
     * @brief   Returns the size of the list in bytes
     */
    SizeType memory_size() const    {return _capacity * sizeof(ValueType);}

    /*!
     * @brief   Returns an iterator pointing to the first item
     *          Mainly implemented for ranged loop support
     */
    Pointer begin()                 {return &_items[0];}

    /*!
     * @brief   Returns an iterator pointing to the first item
     *          Mainly implemented for ranged loop support
     */
    ConstPointer begin() const      {return &_items[0];}

    /*!
     * @brief   Returns an iterator pointing to the last+1 item
     *          Mainly implemented for ranged loop support
     */
    Pointer end()                   {return &_items[_item_count];}

    /*!
     * @brief   Returns an iterator pointing to the last+1 item
     *          Mainly implemented for ranged loop support
     */
    ConstPointer end()const         {return &_items[_item_count];}

    /*!
     * @brief Force the list to have has much "real" space as items
     */
    void fit()  {_item_count = _capacity; resize(_item_count);}

    /*!
     * @brief Returns the index where @ref value is stored Returns -1 if
     * value couldn't be found inside the list
     */
    SizeType index(ConstReference value)const 
    {
        for (SizeType i = 0; i < size(); ++i)
        {
            if (_items[i] == value)
                return i;
        }
        return -1;
    }

    /*!
    * @brief Checks if @ref value is present inside the list
    * @return Returns true the value has been found
    */
    bool exist(ConstReference value) const 
    {
        for (int i = 0; i< _item_count; ++i)
        {
            if (_items[i] == value)
                return true;
        }
        return false;
    }

    /*!
     * @brief   Returns the list's first item
     * @pre     The list must not be empty or undefined behavior occurs
     * @returns Returns a reference to the first item in the list
     */
    ConstReference  first()const
    {
        assert(_item_count > 0);
        return _items[0];
    }

    Reference       first()
    {
        assert(_item_count > 0);
        return _items[0];
    }

    /*!
     * @brief   Returns the list's last item
     * @pre     The list must not be empty or undefined behavior occurs
     * @returns Returns a reference to the last item in the list
     */
    Reference last()
    {
        assert(_item_count > 0);
        return _items[_item_count - 1];
    }

    ConstReference last()const
    {
        assert(_item_count > 0);
        return _items[_item_count - 1];
    }

    /*!
     * @brief   Checks if the list is empty
     * @return  Returns true if the list is empty, false otherwise
     */
    bool empty()const               { return _item_count == 0; }

    /*!
     * @brief   Removes the item stored at the given index 
     *          Copy every items located after the item to remove 
     *          to a "N-1" position. So this is kinda slow
     *          
     * @pre     The index must be inside the list range or UB
     */
    void remove_at(const SizeType index)
    {
        assert(in_range(index));
        for (int i = index; i < _item_count; ++i)
            _items[i] = _items[i + 1];
        --_item_count;
    }

    /*!
     * @brief Removes the item equals to @ref value from the list
     */
    void remove(ConstReference value)   {remove_at(index(value));}

    /*!
     * @brief Makes the list larger by @ref growth
     */
    void grow(const SizeType growth)
    {
        assert(growth > 0);
        resize(size()+growth);
    }

    /*!
     * @brief   Resize the array to be able to store @ref new_size items
     * @param   new_size How many items the array can store
     */
    void resize(const SizeType new_size)
    {
        assert(new_size > 0);

        Pointer values = static_cast<Pointer>( operator new[](new_size * sizeof(ValueType)));

        for (int i = 0; i < _capacity; ++i)
            values[i] = _items[i];

        delete[] _items;
        _items      = values;
        _capacity   = new_size;
    }
    
    /*!
     * @brief   Swap item at index a with item at index b if a or b is out of
     *          bound, the swap operation is canceled
     * @pre     Parameters a and b must not be out of range or UB
     */
    void swap(const SizeType a, const SizeType b)
    {
        assert(in_range(a) && in_range(b));
        ValueType temp  = _items[a];
        _items[a]       = _items[b];
        _items[b]       = temp;
    }

    /*!
     * @brief   Returns true if the given parameter is within the list's range
     */
    bool in_range(const SizeType index)const { return index >= 0 && index < size(); }

private:

    /*!
     * @brief   Call by the Move constructor and assignment operator Transfer
     *          the unique_ptr ownership from l to this
     */
    void move(List& l)
    {
        //_items			= std::move(l._items);
        //_item_count		= l._item_count;
        //_capacity		= l._capacity;
        //l._item_count	= 0;
        //l._capacity		= 0;
    }

    void copy(const List& l)
    {
        allocate(l.size());
        for (int i = 0; i < l.size(); ++i)
            _items[i] = l._items[i];
    }

    /*!
    * @brief    Allocates memory to the underlying array so it can stores 
    *           as many items as the given parameter.
    * @pre      The length parameter must be superior or equal to zero
    * @param    size  The new size of the underlying array.
    */
    void allocate(const SizeType size)
    {
        assert(size >= 0);

        if (_capacity< size)
        {
            delete [] _items;

            // This has the advantage to avoid any constructor 
            // Meaning more performance and no problem with class without
            // a default constructor 

            _items          = static_cast<Pointer>( operator new[](size * sizeof(ValueType)));
            _capacity       = size;
            _item_count     = size;
        }
    }

    /*!
     * @brief   Stores the pointer to underlying array holding the list's items
     */
    Pointer     _items       = nullptr;

    /*!
     * @brief   Stores how many items the list can hold before reallocation
     */
    SizeType    _capacity      = 0;    

    /*!
     * @brief   Stores how many items the list actually holds
     */
    SizeType    _item_count    = 0;
};
}
