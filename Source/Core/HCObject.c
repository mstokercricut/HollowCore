//
//  HCObject.c
//  HollowCore
//
//  Created by Matt Stoker on 12/28/18.
//  Copyright © 2018 HollowCore. All rights reserved.
//

#include "../Core/HCObject_Internal.h"
#include <string.h>

/// If the decrement of the reference count returns this value the object will be destroyed.
///
/// - Note: This value is not @c 0 because atomic decrements return the previous value.
const static HCInteger HCObjectReferenceCountDestructionValue = 1;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type Query
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCTypeHasAncestor(HCType type, HCType other) {
    // Reject type checks against the null type
    if (type == NULL) {
        return false;
    }
    
    // Determine if the type or that of any of its ancestors are of the given type
    for (HCType ancestor = type->ancestor; ancestor != NULL; ancestor = ancestor->ancestor) {
        if (HCTypeIsOfType(ancestor, other)) {
            return true;
        }
    }
    return false;
}

HCBoolean HCTypeIsOfType(HCType type, HCType other) {
    return (type != NULL && other != NULL) && (type == other || strcmp(type->name, other->name) == 0);
}

HCBoolean HCTypeIsOfKind(HCType type, HCType other) {
    return HCTypeIsOfType(type, other) || HCTypeHasAncestor(type, other);
}

HCTypeName HCObjectName(HCRef object) {
    return ((HCObjectRef)object)->type->name;
}

HCType HCObjectAncestor(HCRef object) {
    return ((HCObjectRef)object)->type->ancestor;
}

HCBoolean HCObjectHasAncestor(HCRef object, HCType type) {
    return object != NULL && HCTypeHasAncestor(((HCObjectRef)object)->type, type);
}

HCBoolean HCObjectIsOfType(HCRef object, HCType type) {
    return object != NULL && HCTypeIsOfType(((HCObjectRef)object)->type, type);
}

HCBoolean HCObjectIsOfKind(HCRef object, HCType type) {
    return object != NULL && HCTypeIsOfKind(((HCObjectRef)object)->type, type);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Memory Management
//----------------------------------------------------------------------------------------------------------------------------------
HCRef HCRetain(HCRef self) {
    // Retain on the null reference is a no-op
    if (self == NULL) {
        return NULL;
    }

    atomic_fetch_add(&((HCObjectRef)self)->referenceCount, 1);
    return self;
}

void HCRelease(HCRef self) {
    // Release on the null reference is a no-op
    if (self == NULL) {
        return;
    }

    if (atomic_fetch_sub(&((HCObjectRef)self)->referenceCount, 1) == HCObjectReferenceCountDestructionValue) {
        for (HCType type = ((HCObjectRef)self)->type; type != NULL; type = type->ancestor) {
            ((HCObjectTypeData*)type)->destroy(self);
        }
        free(self);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCIsEqual(HCRef self, HCRef other) {
//    TODO: Find the highest common ancestor to perform the equality check
    return self != NULL && other != NULL && ((HCObjectTypeData*)((HCObjectRef)self)->type)->isEqual(self, other);
}

HCInteger HCHashValue(HCRef self) {
    return self == NULL ? 0 : ((HCObjectTypeData*)((HCObjectRef)self)->type)->hashValue(self);
}

void HCPrint(HCRef self, FILE* stream) {
    ((HCObjectTypeData*)((HCObjectRef)self)->type)->print(self, stream);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
HCObjectTypeData HCObjectTypeDataInstance = {
    .base = {
        .ancestor = NULL,
        .name = "HCObject",
    },
    .isEqual = (void*)HCObjectIsEqual,
    .hashValue = (void*)HCObjectHashValue,
    .print = (void*)HCObjectPrint,
    .destroy = (void*)HCObjectDestroy,
};
HCType HCObjectType = &HCObjectTypeDataInstance.base;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
void HCObjectInit(void* memory) {
    HCObjectRef self = memory;
    self->type = HCObjectType;
    self->referenceCount = 1;
}

void HCObjectDestroy(HCObjectRef self) {
    (void)self; // Unused
}

void HCObjectSetType(void* object, HCType type) {
    HCObjectRef self = object;
    self->type = type;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCObjectIsEqual(HCObjectRef self, HCObjectRef other) {
    return self == other;
}

HCInteger HCObjectHashValue(HCObjectRef self) {
    return (HCInteger)self;
}

void HCObjectPrint(HCObjectRef self, FILE* stream) {
    fprintf(stream, "<%s@%p>", self->type->name, self);
}