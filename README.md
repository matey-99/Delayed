# PBL
Game Engine with editor, created as part of PBL

## INSTRUCTION
1. Clone the repository
```
git clone https://github.com/matey-99/PBL.git
```
2. Download FMOD Engine from https://www.fmod.com/download (FMOD Engine 2.02 for Windows) and install.
3. Find FMOD SoundSystem which was installed on your computer and go to api/core/lib/x64/ and copy all dlls and libs to Engine/ThirdParty/fmod/lib/x64/.
4. Download FreeType Windows libraries from https://github.com/ubawurinna/freetype-windows-binaries (FreeType 2.12.1) and copy .dll and .lib files from release dll/win64/ to Engine/ThirdParty/freetype/lib/win64/.
5. Launch ```GenerateProject.bat```. This file contains simple commands that make Build directory and create project files inside.
6. Open Visual Studio solution.
7. Select PBL or PBL_Editor as startup project.
8. Build.
