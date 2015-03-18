#include "Assignment.h"
#include "CreateNewObject.h"

UCreateNewObject::UCreateNewObject(const class FObjectInitializer& PCIP)
    : Super(PCIP)
{

}

UObject* UCreateNewObject::NewObjectFromBlueprint(UObject* WorldContextObject, TSubclassOf<UObject> UC)
{
    UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject);
    UObject* tempObject = StaticConstructObject(UC);

    return tempObject;
}
