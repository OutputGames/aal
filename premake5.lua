
project "AuroraAudioLibrary"
   kind "SharedLib"
   language "C++"
   cppdialect "C++17"
   location "generated\\"
   compileas "C++"
   targetdir "%{_OPTIONS['corelocation']}/bin/%{cfg.buildcfg}"
   staticruntime "on"

   defines {"_CRT_SECURE_NO_WARNINGS"}

   files {
       --"src/**",
        "include/**",
         ".editorconfig",
         "vendor/alsoft/include/**",
         "%{_OPTIONS['utilslocation']}/include/**" ,
         "%{_OPTIONS['utilslocation']}/vendor/glm/glm/**" ,
         "testresources/**",
         "vendor/dr_libs/*.h",
         "%{_OPTIONS['utilslocation']}".."/vendor/sdl2/include/**" , 
      }
   includedirs {
       "include/", 
       "vendor/alsoft/include/",
       "vendor/dr_libs/",
       "%{_OPTIONS['utilslocation']}".."/include", 
       "%{_OPTIONS['utilslocation']}".."/vendor/glm",
       "%{_OPTIONS['utilslocation']}".."/vendor/sdl2/include" 
      }
      libdirs {
         "%{_OPTIONS['utilslocation']}".."/vendor/sdl2/lib"
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
      links {"SDL2d", "SDL2maind"}


   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"
      libdirs {"vendor/alsoft/lib/Release"}
      staticruntime "off"
      runtime "Release"
      links {"SDL2", "SDL2main"}
   
   