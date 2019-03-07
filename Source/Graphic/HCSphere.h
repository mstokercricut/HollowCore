//
//  HCSphere.h
//  HollowCore
//
//  Created by Matt Stoker on 3/5/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#ifndef HCSphere_h
#define HCSphere_h

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
extern HCType HCSphereType;
typedef struct HCSphere* HCSphereRef;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCSphereRef HCSphereCreate(HCReal center, HCReal radius);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCSphereIsEqual(HCSphereRef self, HCSphereRef other);
HCInteger HCSphereHashValue(HCSphereRef self);
void HCSpherePrint(HCSphereRef self, FILE* stream);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Primitive Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCSphereIntersect(HCSphereRef self, HCRef other);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Attributes
//----------------------------------------------------------------------------------------------------------------------------------
HCReal HCSphereGetCenter(HCSphereRef self);
HCReal HCSphereGetRadius(HCSphereRef self);

#endif /* HCSphere_h */
