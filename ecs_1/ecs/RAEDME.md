# Entity-Component-System v1

This version implements only Entity-Component. Components inherit from the class Component, Entity maintains an array with a fixed number of (possible) components, and Manager maintains a list of entities. It also support groups of entities and handlers (assigning identifiers to entities for fast global access.

In this version, the identifier of each component is stored as a static variable of the class, for example:

```
class Transfrom: public ecs::Component {
public:
	constexpr static ecs::cmpId_type id = ecs::cmpId::_TRANSFORM;
     ...
}
```

This we only pass the type to add component, e.g., ``e->addComponent<T>``

