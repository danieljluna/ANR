// Gordian by Daniel Luna (2019)

#pragma once

//////////////////////////////////////////////////////////////////////////
// RSTRUCTs are lightweight reflection, with minimal bloat.
//////////////////////////////////////////////////////////////////////////

// Class used to store type descriptor info for structs
#define __RSTRUCT_TYPE Gordian::OType_Struct
// Static type member name
#define __RSTRUCT_MEMBER_STATIC _StaticType
// Static member accessor
#define __RSTRUCT_FN_ACCESSOR GetStaticType
// Member function that initializes type descriptor info
#define __RSTRUCT_FN_INIT _InitializeTypeDescriptor


// Declares a reflection macro

// Enables reflection for a class. Use in the body of the declaration.
#define REFLECT_STRUCT(BASE_STRUCT)														\
private:																				\
	using Parent = BASE_STRUCT;															\
	friend struct Gordian::FDefaultTypeResolver;										\
	static __RSTRUCT_TYPE __RSTRUCT_MEMBER_STATIC;										\
	static void __RSTRUCT_FN_INIT(__RSTRUCT_TYPE*);										\
																						\
public:																					\
	static inline const __RSTRUCT_TYPE* __RSTRUCT_FN_ACCESSOR()							\
	{																					\
		return &__RSTRUCT_MEMBER_STATIC;												\
	}																					\


// Reflection chunks define the reflection values. 
//	They should be placed in a compiled source file

//	Starts a Reflection Chunk. Should be followed by *_END()
#define RSTRUCT_MEMBER_BEGIN(STRUCT)										\
	__RSTRUCT_TYPE __RSTRUCT_MEMBER_STATIC{STRUCT::__RSTRUCT_FN_INIT};		\
																			\
	void STRUCT::__RSTRUCT_FN_INIT(__RSTRUCT_TYPE* TypeDesc)				\
	{																		\
		using T = STRUCT;													\
		TypeDesc->SetName(#STRUCT);											\
		TypeDesc->SetSize(sizeof(T));										\
		TypeDesc->SetParentClass<T::Parent>((T::Parent*) nullptr);			\
		TypeDesc->DeclareMembers({											\


// Used to define a member value in a struct. 
//	Use multiple times as the meat of a Reflection Chunk.
#define RSTRUCT_MEMBER_ADD(MEMBER)									\
		{#MEMBER, offsetof(T, MEMBER), Gordian::FTypeResolver<decltype(T::MEMBER)>::Get()},		\


// Ends a Reflection Chunk
#define RSTRUCT_MEMBER_END()	\
		});						\
	}




//////////////////////////////////////////////////////////////////////////
// RCLASSes are heavier reflection.
//	RClass macros must be called in any child of OObject
//////////////////////////////////////////////////////////////////////////

// Class used to store type descriptor info for RCLASSes
#define __RCLASS_TYPE __RSTRUCT_TYPE
// Static member name
#define __RCLASS_MEMBER_STATIC __RSTRUCT_MEMBER_STATIC
// Static member accessor
#define __RCLASS_FN_ACCESSOR __RSTRUCT_FN_ACCESSOR
// Member function that initializes type descriptor info
#define __RCLASS_FN_INIT __RSTRUCT_FN_INIT


// Declares a reflection macro

// Enables reflection for a class. Use in the body of the declaration.
#define REFLECT_CLASS(BASE_CLASS)														\
private:																				\
	using Parent = BASE_CLASS;															\
	friend struct Gordian::FDefaultTypeResolver;										\
	static __RCLASS_TYPE __RCLASS_MEMBER_STATIC;										\
	static void __RCLASS_FN_INIT(__RCLASS_TYPE*);										\
																						\
public:																					\
	static inline const __RCLASS_TYPE* __RCLASS_FN_ACCESSOR()							\
	{																					\
		return &__RCLASS_MEMBER_STATIC;													\
	}																					\


// Reflection chunks define the reflection values. 
//	They should be placed in a compiled source file

//	Starts a Reflection Member Chunk.
#define RCLASS_INITIALIZE(CLASS)											\
	__RCLASS_TYPE CLASS::__RCLASS_MEMBER_STATIC(CLASS::__RCLASS_FN_INIT);	\
	void CLASS::__RCLASS_FN_INIT(__RCLASS_TYPE* TypeDesc)					\
	{																		\
		using T = CLASS;													\
		TypeDesc->SetName(#CLASS);											\
		TypeDesc->SetSize(sizeof(T));										\
		TypeDesc->SetParentClass<T::Parent>((T::Parent*) nullptr);			\

//	Used to define a reflection chunk that contains no new members or functions
#define RCLASS_INITIALIZE_EMPTY(CLASS)										\
	RCLASS_INITIALIZE(CLASS)												\
	}

// Starts a member list. Finish with RCLASS_END_LIST
#define RCLASS_BEGIN_MEMBER_LIST()											\
		TypeDesc->DeclareMembers({											\


// Used to define a member value in a struct. 
//	Use multiple times inside a MEMBER_LIST.
#define RCLASS_MEMBER_ADD(MEMBER)											\
		{#MEMBER, offsetof(T, MEMBER), Gordian::FTypeResolver<decltype(T::MEMBER)>::Get()},		\

// Starts a function list. Finish with RCLASS_END_LIST
#define RCLASS_BEGIN_FUNCTION_LIST()										\
		TypeDesc->DeclareFunctions({										\

// Used to define a function in a struct. 
//	Use multiple times inside a FUNCTION_LIST.
// ONLY SUPPORTS NO PARAMS
#define RCLASS_FUNCTION_ADD(FUNCTION)										\
		{#FUNCTION, &T::FUNCTION, Gordian::FFunctionResolver<decltype(T::FUNCTION())>::Get(), {}, EFunctionType::Static},					\


// Ends a Reflection List without ending the entire chunk
#define RCLASS_END_LIST()		\
		});

// Ends Reflection init, ending the existing list
#define RCLASS_END_INIT()		\
		});						\
	}
