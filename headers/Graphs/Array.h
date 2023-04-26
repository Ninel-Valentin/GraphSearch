#include <iostream>
class Array
{
private:
    int *values;
    int len = 0;

public:
    Array();
    Array(int);
    ~Array();
    void resize(int);
    void set(int, int);
    int getValue(int);
    int getIndex(int);
    void add(int);
    void add(int, int);
    void add(int, int, bool);
    int fifo();
    int filo();
    int first();
    int last();
    int length();
    void removeByValue(int);
    void removeByIndex(int);
    int &operator[](int);
    bool operator!=(Array);
    bool operator==(Array);
    // friend std::istream &operator>>(std::istream &, Array &);
    // friend std::ostream &operator<<(std::ostream &, Array &);
};
Array::Array() {}
Array::Array(int _length)
{
    if (_length > 0)
    {
        this->values = new int[_length];
        this->len = _length;
    }
}
Array::~Array()
{
    if (this->len > 0)
        delete[] this->values;
}

// This function resizes the array keeping the required members and filling (if it's the case) the rest with 0
void Array::resize(int _length)
{
    if (this->len == 0)
    {
        this->len = _length;
        this->values = new int[_length];
        return;
    }

    int *temp = new int[_length];
    // Iterate until the min length and copy the values to temp
    for (int i = 0; i < (_length > this->len ? this->len : _length); i++)
        temp[i] = this->values[i];
    delete[] this->values;

    this->values = new int[_length];
    // Iterate until the max length and copy the values from temp and fill the rest with 0 (if it's the case)
    for (int i = 0; i < (_length < this->len ? this->len : _length); i++)
        if (i >= this->len)
            values[i] = 0;
        else
            values[i] = temp[i];

    // Save the new length as it was required earlier
    this->len = _length;
    delete[] temp;
}

void Array::set(int _index, int _value)
{
    if (_index >= 0 && _index < this->len)
        this->values[_index] = _value;
}

int Array::getValue(int _index)
{
    if (_index >= 0 && _index < this->len)
        return this->values[_index];
    return -1;
}

// This function returns the index of the provided value or -1 if the value is not in the array.
int Array::getIndex(int _value)
{
    for (int i = 0; i < this->len; i++)
        if (this->values[i] == _value)
            return i;
    return -1;
}

void Array::add(int _value)
{
    this->add(_value, this->len, false);
}

void Array::add(int _value, int _index)
{
    this->add(_value, _index, false);
}

void Array::add(int _value, int _index, bool _checkForDuplicate)
{
    // If we should not duplicate it and the array contains the value we stop
    if (_checkForDuplicate && this->getIndex(_index) != -1)
        return;
    int *temp = new int[this->len + 1];
    for (int i = 0; i <= this->len; i++)
        if (i == _index)
            temp[i] = _value;
        else
            temp[i + (i > _index ? 1 : 0)] = this->values[i];

    delete[] this->values;
    this->values = new int[++this->len];
    for (int i = 0; i < this->len; i++)
        this->values[i] = temp[i];
    delete[] temp;
}

// This function removes and returns the first entry of the array. Returns -1 if the array is emptyint Array::fifo()
int Array::fifo()
{
    if (this->len > 0)
    {
        int first = values[0];
        int *temp = new int[this->len - 1];
        for (int i = 1; i < this->len; i++)
            temp[i - 1] = this->values[i];
        delete[] this->values;

        this->values = new int[--this->len];
        for (int i = 0; i < this->len; i++)
            this->values[i] = temp[i];
        delete[] temp;
        return first;
    }
    return -1;
}

// This function removes and returns the last entry of the array. Returns -1 if the array is empty
int Array::filo()
{
    if (this->len > 0)
    {
        int last = values[this->len - 1];
        int *temp = new int[this->len - 1];
        for (int i = 0; i < this->len - 1; i++)
            temp[i] = this->values[i];
        delete[] this->values;

        this->values = new int[--this->len];
        for (int i = 0; i < this->len; i++)
            this->values[i] = temp[i];
        delete[] temp;
        return last;
    }
    return -1;
}

int Array::first()
{
    return this->values[0];
}

int Array::last()
{
    return this->values[this->len - 1];
}

int Array::length()
{
    return this->len;
}

// This function removes the value (if found) and resizes the array
void Array::removeByValue(int _value)
{
    int index = this->getIndex(_value);
    this->removeByIndex(index);
}

// This function removes the value at the index (if found) and resizes the array
void Array::removeByIndex(int _index)
{
    if (_index >= 0 && _index < this->len)
    {
        int *temp = new int[this->len - 1];
        // Copy all nodes except the removed one
        for (int i = 0; i < this->len - 1; i++)
            temp[i] = this->values[i + (i >= _index ? 1 : 0)];
        delete[] this->values;
        this->values = new int[--this->len];
        for (int i = 0; i < this->len; i++)
            this->values[i] = temp[i];
        delete[] temp;
    }
}

int &Array::operator[](int _index)
{
    if (_index >= 0 && _index < this->len)
        return this->values[_index];
    else
        exit(1);
}

bool Array::operator!=(Array _a)
{
    if (this == &_a)
        return false;
    if (this->len != _a.len)
        return true;
    for (int i = 0; i < this->len; i++)
        if (_a[i] != (*this)[i])
            return true;
    return false;
}

bool Array::operator==(Array _a)
{
    if (this == &_a)
        return true;
    if (this->len == _a.len)
        for (int i = 0; i < this->len; i++)
            if (_a[i] != (*this)[i])
                return false;
    return true;
}

// std::istream &operator>>(std::istream &str, Array &_a)
// {
// }

// std::ostream &operator<<(std::ostream &str, Array &_a)
// {
// }