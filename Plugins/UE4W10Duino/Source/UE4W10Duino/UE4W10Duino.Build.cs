using UnrealBuildTool;

public class UE4W10Duino : ModuleRules
{
    public UE4W10Duino(TargetInfo target)
    {
        PublicIncludePaths.AddRange(new string[] { "UE4W10Duino/Public" });

        PrivateIncludePaths.AddRange(new string[] { "UE4W10Duino/Private" });

        PublicDependencyModuleNames.AddRange(new string[] { "Core" });

        PrivateDependencyModuleNames.AddRange(
            new string[]
			{
                "Engine",
                "InputCore",
                "Slate",
                "SlateCore",
                "EditorStyle",
                "CoreUObject"
            }
        );
    }
}