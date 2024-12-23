-- Dependencies
IncludeDir = {}
IncludeDir["spdlog"] = "%{wks.location}/vendor/spdlog/include"
IncludeDir["Vulkan"] = os.getenv("VULKAN_SDK") .. "/Include"
IncludeDir["glm"] = "%{wks.location}/vendor/glm"
IncludeDir["stb_image"] = "%{wks.location}/vendor/stb_image"

LibraryDir = {}
LibraryDir["Vulkan"] = os.getenv("VULKAN_SDK") .. "/Lib"

Library = {}
Library["Vulkan"] = "vulkan-1"
Library["MoltenVK"] = "MoltenVK"
