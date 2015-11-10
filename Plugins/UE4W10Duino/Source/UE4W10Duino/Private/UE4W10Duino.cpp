// Some copyright should be here...

#include "UE4W10DuinoPrivatePCH.h"

DEFINE_LOG_CATEGORY(ModuleLog)

#define LOCTEXT_NAMESPACE "FUE4W10DuinoModule"

void FUE4W10DuinoModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	UE_LOG(ModuleLog, Warning, TEXT("UE4W10Duino: Log Started"));
}

void FUE4W10DuinoModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	
	UE_LOG(ModuleLog, Warning, TEXT("UE4W10Duino: Log Ended"));
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FUE4W10DuinoModule, UE4W10Duino)