"""
Generics support for JOCKY compiler.

Implements:
- Type variable unification algorithm
- Type inference from function arguments
- Monomorphization (instantiation of generic functions)
"""

from typing import Dict, Optional, Tuple, List
from .ast import JType, FuncDecl, Param


class TypeBindings:
    """Maps type variable names to concrete types."""

    def __init__(self, bindings: Optional[Dict[str, JType]] = None):
        self.bindings = bindings or {}

    def bind(self, var_name: str, typ: JType) -> None:
        """Bind a type variable to a concrete type."""
        if var_name in self.bindings:
            existing = self.bindings[var_name]
            if not self._types_equal(existing, typ):
                raise TypeError(f"Type variable {var_name} already bound to {existing}, cannot rebind to {typ}")
        self.bindings[var_name] = typ

    def get(self, var_name: str) -> Optional[JType]:
        """Get a binding for a type variable."""
        return self.bindings.get(var_name)

    def copy(self) -> "TypeBindings":
        """Create a copy of bindings."""
        return TypeBindings(dict(self.bindings))

    def _types_equal(self, t1: JType, t2: JType) -> bool:
        """Check if two types are structurally equal."""
        return (t1.name == t2.name and
                t1.is_pointer == t2.is_pointer and
                t1.is_array == t2.is_array and
                t1.array_size == t2.array_size and
                t1.is_type_var == t2.is_type_var)


class Unifier:
    """Implements the unification algorithm for type inference."""

    @staticmethod
    def unify(expected: JType, actual: JType, bindings: TypeBindings) -> bool:
        """
        Try to make expected and actual types match, collecting bindings.
        Returns True if unification succeeds, raises TypeError if it fails.
        """
        # Resolve type variables in both types
        expected = Unifier._resolve_type(expected, bindings)
        actual = Unifier._resolve_type(actual, bindings)

        # If expected is type variable, bind it
        if expected.is_type_var:
            if expected.type_var_name:
                existing = bindings.get(expected.type_var_name)
                if existing is not None:
                    return Unifier.unify(existing, actual, bindings)
                else:
                    bindings.bind(expected.type_var_name, actual)
                    return True

        # If actual is type variable, bind it
        if actual.is_type_var:
            if actual.type_var_name:
                existing = bindings.get(actual.type_var_name)
                if existing is not None:
                    return Unifier.unify(expected, existing, bindings)
                else:
                    bindings.bind(actual.type_var_name, expected)
                    return True

        # Both concrete types - must match exactly
        if (expected.name == actual.name and
            expected.is_pointer == actual.is_pointer and
            expected.is_array == actual.is_array and
            expected.array_size == actual.array_size):
            return True

        raise TypeError(f"Cannot unify {expected} with {actual}")

    @staticmethod
    def _resolve_type(typ: JType, bindings: TypeBindings) -> JType:
        """Resolve a type by following type variable bindings."""
        if typ.is_type_var and typ.type_var_name:
            bound = bindings.get(typ.type_var_name)
            if bound is not None:
                return Unifier._resolve_type(bound, bindings)
        return typ


class Monomorphizer:
    """Handles instantiation of generic functions into monomorphic versions."""

    def __init__(self):
        self.instances: Dict[str, FuncDecl] = {}

    def instantiate(self, generic_func: FuncDecl, bindings: TypeBindings) -> FuncDecl:
        """
        Create a concrete instance of a generic function.

        Args:
            generic_func: The generic function template
            bindings: Type variable bindings for this instantiation

        Returns:
            A monomorphic FuncDecl with a unique name
        """
        # Generate unique instance name
        instance_name = self._generate_instance_name(generic_func.name, bindings)

        # Check if we already instantiated this
        if instance_name in self.instances:
            return self.instances[instance_name]

        # Substitute type variables in parameters
        instantiated_params = []
        for param in generic_func.params:
            new_type = self._substitute_type(param.type, bindings)
            instantiated_params.append(Param(param.name, new_type))

        # Substitute in return type
        instantiated_ret = self._substitute_type(generic_func.ret_type, bindings)

        # Create monomorphic function
        instance = FuncDecl(
            name=instance_name,
            params=instantiated_params,
            ret_type=instantiated_ret,
            body=generic_func.body,  # Reuse body
            attributes=generic_func.attributes,
            type_params=[],  # No longer generic
            is_generic=False
        )

        self.instances[instance_name] = instance
        return instance

    def _generate_instance_name(self, func_name: str, bindings: TypeBindings) -> str:
        """Generate a unique name for a function instance."""
        if not bindings.bindings:
            return func_name

        # Sort bindings for deterministic naming
        binding_strs = []
        for var_name in sorted(bindings.bindings.keys()):
            typ = bindings.bindings[var_name]
            # Simplify type name for instance naming
            type_str = typ.name
            if typ.is_pointer:
                type_str += "p"
            if typ.is_array:
                type_str += f"a{typ.array_size}" if typ.array_size else "a"
            binding_strs.append(f"{var_name}_{type_str}")

        return f"{func_name}__{'.'.join(binding_strs)}"

    def _substitute_type(self, typ: JType, bindings: TypeBindings) -> JType:
        """Replace type variables with their bindings."""
        if typ.is_type_var and typ.type_var_name:
            bound = bindings.get(typ.type_var_name)
            if bound is not None:
                # Recursively substitute in case the binding itself contains type vars
                return self._substitute_type(bound, bindings)

        # For non-type-var types, preserve structure but don't substitute
        return typ


class GenericTypeChecker:
    """Type checker specialized for generic functions."""

    @staticmethod
    def infer_type_bindings(
        generic_func: FuncDecl,
        arg_types: List[JType]
    ) -> TypeBindings:
        """
        Infer type variable bindings from function call arguments.

        Args:
            generic_func: The generic function being called
            arg_types: Types of the arguments passed to the function

        Returns:
            TypeBindings mapping type variables to concrete types
        """
        if len(arg_types) != len(generic_func.params):
            raise TypeError(
                f"Function {generic_func.name} expects {len(generic_func.params)} "
                f"arguments, got {len(arg_types)}"
            )

        bindings = TypeBindings()

        # Unify each argument with corresponding parameter
        for i, (arg_type, param) in enumerate(zip(arg_types, generic_func.params)):
            try:
                Unifier.unify(param.type, arg_type, bindings)
            except TypeError as e:
                raise TypeError(
                    f"Type mismatch in argument {i} to {generic_func.name}: {e}"
                )

        return bindings
