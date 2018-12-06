// Gordian by Daniel Luna

#pragma once

#include <type_traits>
#include <bitset>

namespace Gordian
{
 
/// Wraps std::bitset and enforces enum parity with the bitmask.
///
/// Any Enum that you want to use in a FBitSet must have a MAX_VALUE that denotes the largest
/// value in the enumeration. Each entry in the enum will always be treated as a distinct flag.
template<typename T>
struct TBitSet
{
	static_assert(std::is_enum<T>::value, "Must be an enum type");

public:

    TBitSet& operator|=(const T& EnumAsFlag)
    {
        BitSet_.set(static_cast<UnderlyingType_>(EnumAsFlag));
        return *this;
    }

    TBitSet& operator&=(const T& EnumAsFlag)
    {
        bool bWasEnumSet = BitSet_.test(static_cast<UnderlyingType_>(EnumAsFlag));
        BitSet_.reset();
        BitSet_.set(static_cast<UnderlyingType_>(EnumAsFlag), bWasEnumSet);
        return *this;
    }

	TBitSet& operator^=(const T& EnumAsFlag)
	{
		BitSet_.flip(static_cast<UnderlyingType_>(EnumAsFlag));
		return *this;
	}

    TBitSet& operator|=(const TBitSet& MatchingBitSet)
    {
        BitSet_ |= MatchingBitSet;
        return *this;
    }

    TBitSet& operator&=(const TBitSet& MatchingBitSet)
    {
        BitSet_ &= MatchingBitSet;
        return *this;
    }

	TBitSet& operator^=(const TBitSet& MatchingBitSet)
	{
		BitSet_ ^= MatchingBitSet;
		return *this;
	}

    
    TBitSet operator|(const T& EnumAsFlag) const
    {
        TBitSet ReturnValue(*this);
        ReturnValue |= EnumAsFlag;
        return ReturnValue;;
    }

    TBitSet operator&(const T& EnumAsFlag) const
    {
        TBitSet ReturnValue(*this);
        ReturnValue &= EnumAsFlag;
        return ReturnValue;
    }

	TBitSet operator^(const T& EnumAsFlag) const
	{
		TBitSet ReturnValue(*this);
		ReturnValue ^= EnumAsFlag;
		return ReturnValue;
	}
    
    TBitSet operator|(const TBitSet& MatchingBitSet) const
    {
        TBitSet ReturnValue(*this);
        ReturnValue.BitSet_ |= MatchingBitSet.BitSet_;
        return ReturnValue;
    }

    TBitSet operator&(const TBitSet& MatchingBitSet) const
    {
        TBitSet ReturnValue(*this);
        ReturnValue.BitSet_ &= MatchingBitSet.BitSet_;
        return ReturnValue;
    }

	TBitSet operator^(const TBitSet& MatchingBitSet) const
	{
		TBitSet ReturnValue(*this);
		ReturnValue.BitSet_ ^= MatchingBitSet.BitSet_;
		return ReturnValue;
	}


    TBitSet operator~() const
    {
        TBitSet ReturnValue(*this);
        ReturnValue.BitSet_.flip();
        return ReturnValue;
    }

    explicit operator bool() const
    {
        return BitSet_.any();
    }

    bool operator==(const TBitSet& MatchingBitSet) const
    {
        return BitSet_ == MatchingBitSet.BitSet_;
    }

   
    /// Returns size of bitset (equal to T::MAX_VALUE).
    std::size_t size() const
    {
        return BitSet_.size();
    }

    /// Returns number of set bits.
    std::size_t count() const
    {
        return BitSet_.count();
    }

    /// Sets all bits to true.
    TBitSet& set()
    {
        BitSet_.set();
        return *this;
    }

    /// Sets all bits to false.
    TBitSet& reset()
    {
        BitSet_.reset();
        return *this;
    }

    /// Flips all bits.
    TBitSet& flip()
    {
        BitSet_.flip();
        return *this;
    }

    /// Set EnumAsFlag bit to Value.
    TBitSet& set(const T& EnumAsFlag, bool Value = true)
    {
        BitSet_.set(static_cast<UnderlyingType_>(EnumAsFlag));
        return *this;
    }

    /// Set EnumAsFlag bit to false.
    TBitSet& reset(const T& EnumAsFlag)
    {
        BitSet_.reset(static_cast<UnderlyingType_>(EnumAsFlag));
        return *this;
    }

    /// Flips EnumAsFlag bit.
    TBitSet& flip(const T& EnumAsFlag)
    {
        BitSet_.flip(static_cast<UnderlyingType_>(EnumAsFlag));
        return *this;
    }

    bool operator[](const T& EnumAsFlag) const
    {
        return BitSet_[static_cast<UnderlyingType_>(EnumAsFlag)];
    }

    friend std::ostream& operator<<(std::ostream& stream, const TBitSet& BitSet)
    {
        return stream << BitSet.BitSet_;
    }

private:

    using UnderlyingType_ = typename std::underlying_type<T>::type;
    std::bitset<static_cast<UnderlyingType_>(T::MAX_VALUE)> BitSet_;
};

/// These operators will combine any two matching enums to create
/// the appropriate BitSet.
template<typename T>
std::enable_if_t<std::is_enum<T>::value, TBitSet<T>>
operator|(const T& lhs, const T& rhs)
{
    TBitSet<T> CombinedBitSet;
    CombinedBitSet |= lhs;
    CombinedBitSet |= rhs;

    return CombinedBitSet;
}


};
