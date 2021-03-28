// Gordian by Daniel Luna (2019)

#include <cstdlib>

namespace Gordian
{

////////////////////////////////////////////////////////////////////////////////
// TypeOfNth recursively fetches the type of the Nth template param.
//	Expects an unsigned integer specifying which param to read.
////////////////////////////////////////////////////////////////////////////////

template <std::size_t N, typename T, typename ... UnparsedTypes>
struct TypeOfNth
{
	// N != 0, go deeper
	using type = typename TypeOfNth<N - 1U, UnparsedTypes...>::type;
};

template <typename T, typename ... UnparsedTypes>
struct TypeOfNth<0U, T, UnparsedTypes...>
{
	// N == 0, type is T
	using type = T;
};

////////////////////////////////////////////////////////////////////////////////
// Specializes FunctionParamType to return the type of the Nth parameter
////////////////////////////////////////////////////////////////////////////////

template <std::size_t, typename>
struct FunctionParamType;

template <std::size_t N, typename RET, typename ... PARAMS>
struct FunctionParamType<N, RET(PARAMS...)>
{
	using type = typename TypeOfNth<N, PARAMS...>::type;
};

////////////////////////////////////////////////////////////////////////////////
// Specializes FunctionReturnType to return the type of the function's return
////////////////////////////////////////////////////////////////////////////////

template <typename>
struct FunctionReturnType;

template <typename RET, typename ... PARAMS>
struct FunctionReturnType<RET(PARAMS...)>
{
	using type = RET;
};


}; // namespace Gordian
