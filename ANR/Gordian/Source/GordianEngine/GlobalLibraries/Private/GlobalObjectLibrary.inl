// Gordian by Daniel Luna


template<typename TargetClass, typename std::enable_if<Gordian::FDefaultTypeResolver::IsReflected<TargetClass>::value, int>::type>
TargetClass* Gordian::Cast(Gordian::OObject* Source)
{
	check(Source->GetType() != nullptr);
	if (Source->GetType()->IsChildClassOf(TargetClass::GetStaticType()))
	{
		return static_cast<TargetClass*>(Source);
	}

	return nullptr;
}

template<typename TargetClass, typename std::enable_if<Gordian::FDefaultTypeResolver::IsReflected<TargetClass>::value, int>::type>
const TargetClass* Gordian::Cast(const Gordian::OObject* Source)
{
	check(Source->GetType() != nullptr);
	if (Source->GetType()->IsChildClassOf(TargetClass::GetStaticType()))
	{
		return static_cast<const TargetClass*>(Source);
	}

	return nullptr;
}

template<typename T, typename std::enable_if<
									std::is_base_of<Gordian::OObject, T>::value, int>::type>
T* Gordian::FGlobalObjectLibrary::CreateObject(Gordian::OObject* OwningObject,
												const Gordian::OType_Struct* ObjectType,
												const std::string& ObjectName)
{
	// todo: create object based on ObjectType not T

	// todo: this should eventually use pooled objects where possible
	Gordian::OObject* NewObject = new T(ObjectName != "" ? ObjectName : ObjectType->GetName()
								, OwningObject);

	check(NewObject != nullptr);
	NewObject->_PrivateType = ObjectType;
	NewObject->Initialize();

	return static_cast<T*>(NewObject);
}
