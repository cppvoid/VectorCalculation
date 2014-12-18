#include <array>
#include <initializer_list>
#include <iostream>
#include <algorithm>
#include <stdexcept>

template< std::size_t size, std::size_t range >
struct is_greater
{
    enum{ value = ( size > range ) ? 1 : 0 };
};

template< typename T, std::size_t size, typename EnableType = void, typename EnableSize = void >
class Vector;

template< typename T, std::size_t size >
class Vector< T, size, typename std::enable_if< std::is_arithmetic<T>::value >::type,
                       typename std::enable_if< is_greater< size, 0 >::value >::type >
{
public:
    using iterator = typename std::array<T, size>::iterator;

    using const_iterator = typename std::array<T, size>::const_iterator;

public:
    Vector()
    {
        clear();
    }

    Vector( const Vector& vec ) = default;

    Vector( Vector&& vec ) = default;

    Vector( std::initializer_list<T> l )
    {
        if( l.size() != size )
        {
            throw std::range_error("list size != size");
        }
        auto iter = l.begin();
        for( std::size_t i = 0; i < size; ++i )
        {
            values[i] = *iter;
            ++iter;
        }
    }

    ~Vector() {}

    void clear()
    {
        for( std::size_t i = 0; i < size; ++i )
        {
            values[i] = 0;
        }
    }

    constexpr std::size_t getSize() const
    {
        return size;
    }

    iterator begin()
    {
        return values.begin();
    }

    const_iterator begin() const
    {
        return values.cbegin();
    }

    iterator end()
    {
        return values.end();
    }

    const_iterator end() const
    {
        return values.cend();
    }

    const_iterator cbegin() const
    {
        return values.cbegin();
    }

    const_iterator cend() const
    {
        return values.cend();
    }

    T& operator[]( const std::size_t pos )
    {
        if( pos > size -1 )
        {
            throw std::range_error("pos > size");
        }
        return values[pos];
    }

    const T& operator[]( const std::size_t pos ) const
    {
        if( pos > size -1 )
        {
            throw std::range_error("pos > size");
        }
        return values[pos];
    }

    Vector& operator=( const Vector& rhs  )
    {
        if( this != &rhs )
        {
            for( std::size_t i = 0; i < size; ++i )
            {
                values[i] = rhs[i];
            }
        }
        return *this;
    }

    Vector<T,size> operator+( const Vector<T,size>& vec )
    {
        return plus( vec );
    }

    Vector<T,size>& operator+=( const Vector<T,size>& vec )
    {
        for( std::size_t i = 0; i < size; ++i )
        {
            values[i] += vec[i];
        }
        return *this;
    }

    Vector<T,size> operator-( const Vector<T,size>& vec )
    {
        return minus( vec );
    }

    Vector<T,size>& operator-=( const Vector<T,size>& vec )
    {
        for( std::size_t i = 0; i < size; ++i )
        {
            values[i] -= vec[i];
        }
        return *this;
    }

    Vector<T,size> operator*( const Vector<T,size>& vec )
    {
        return multiply( vec );
    }

    Vector<T,size>& operator*=( const Vector<T,size>& vec )
    {
        for( std::size_t i = 0; i < size; ++i )
        {
            values[i] *= vec[i];
        }
        return *this;
    }

    Vector<T,size> operator/( const Vector<T,size>& vec )
    {
        return divide( vec );
    }

    Vector<T,size>& operator/=( const Vector<T,size>& vec )
    {
        for( std::size_t i = 0; i < size; ++i )
        {
            values[i] /= vec[i];
        }
        return *this;
    }

    inline bool operator==( const Vector<T, size>& rhs ) const
    {
        for( std::size_t i = 0; i < size; ++i )
        {
            if( values[i] != rhs[i] )
                return false;
        }
        return true;
    }

    inline bool operator!=( const Vector<T, size>& rhs ) const
    {
        return !operator==( rhs );
    }

    inline bool operator< ( const Vector<T, size>& rhs ) const
    {
        T aggregateValue{0};
        T rhsAggregateValue{0};
        for( std::size_t i = 0; i < size; ++i )
        {
            aggregateValue += values[i];
            rhsAggregateValue += rhs[i];
        }

        return aggregateValue < rhsAggregateValue;
    }

    inline bool operator> ( const Vector<T, size>& rhs ) const
    {
        return !operator<( rhs );
    }

    inline bool operator<=( const Vector<T,size>& rhs ) const
    {
        return !operator>( rhs );
    }

    constexpr inline bool operator>=( const Vector<T,size>& rhs ) const
    {
        return !operator<( rhs );
    }

    friend std::ostream& operator<<( std::ostream& out, const Vector<T,size>& vec )
    {
        for( std::size_t i = 0; i < vec.getSize(); ++i )
        {
            std::cout << vec[i];
            if( i+1 != vec.getSize() )
            {
                std::cout << ", ";
            }
        }
        std::cout << "\n";

        return out;
    }

private:
    Vector plus( const Vector& vec )
    {
        Vector tmp( *this );
        for( std::size_t i = 0; i < size; ++i )
        {
            tmp[i] += vec[i];
        }

        return tmp;
    }

    Vector minus( const Vector& vec )
    {
        Vector tmp( *this );
        for( std::size_t i = 0; i < size; ++i )
        {
            tmp[i] -= vec[i];
        }

        return tmp;
    }

    Vector multiply( const Vector& vec )
    {
        Vector tmp( *this );
        for( std::size_t i = 0; i < size; ++i )
        {
            tmp[i] *= vec[i];
        }

        return tmp;
    }

    Vector divide( const Vector& vec )
    {
        Vector tmp( *this );
        for( std::size_t i = 0; i < size; ++i )
        {
            tmp[i] /= vec[i];
        }

        return tmp;
    }

private:
    std::array< T, size > values;
};

int main()
{
    Vector< int, 4 > v1 = { 12, 13, 14, 15 };
    Vector< int, 4 > v2 = { 12, 13, 14, 15 };

    std::cout << v1 + v2;
    std::cout << v1 - v2;
    std::cout << v1 * v2;
    std::cout << v1 / v2;

    if( v1 == v2 )
    {
        std::cout << "v1 == v2\n";
    }
    if( v1 != v2 )
    {
        std::cout << "v1 != v2\n";
    }

    Vector< int, 4 > v3 = { 1, 3, 2, 4 };

    if( v1 > v3 )
    {
        std::cout << "v1 > v3\n";
    }
}
