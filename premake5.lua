
project "AuroraAudioLibrary"
   kind "SharedLib"
   language "C++"
   cppdialect "C++17"
   location "generated\\"
   compileas "C++"
   targetdir "%{_OPTIONS['corelocation']}/bin/%{cfg.buildcfg}"
   staticruntime "on"

   files {
       --"src/**",
        "include/**",
         ".editorconfig",
         "vendor/alsoft/include/**",
         "%{_OPTIONS['utilslocation']}/include/**" ,
         "%{_OPTIONS['utilslocation']}/vendor/glm/glm/**" ,
         "testresources/**",
         "vendor/dr_libs/*.h"
      }
   includedirs {
       "include/", 
       "vendor/alsoft/include/",
       "vendor/dr_libs/",
       "%{_OPTIONS['utilslocation']}".."/include", 
       "%{_OPTIONS['utilslocation']}".."/vendor/glm" 
      }

      links {"OpenAL32"}

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"
      optimize "Off"
      debugdir "./"
      runtime "Debug"
      libdirs {"vendor/alsoft/lib/Debug"}
      staticruntime "off"


   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"
      libdirs {"vendor/alsoft/lib/Release"}
      staticruntime "off"
      runtime "Release"
   
   