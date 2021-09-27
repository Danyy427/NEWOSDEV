# Welcome to TaraOS Github Page
You can follow this readme file to build the project.
It is currently buildable on Darwin (OSX) and Windows.
---
Windows:

You will need several tools to be able to build it on windows.
Here is the list
  1. Mingw64: Mingw is a suite of tools that were made to replicate Linux build tools. Make sure it is 64 bit. Here is the download: https://www.mingw-w64.org/downloads/
  2. QEMU: QEMU is an emulator to run the OS. Here is the download: https://www.qemu.org/download/#windows
  3. Nasm: NASM is an assembler which can compile assembly files into binary. Here is the download: https://www.nasm.us/pub/nasm/releasebuilds/2.15.05/win64/
  4. Findutils: Findutils is a small piece of software that locates files in directories based on their attributes such as file extension. Here is the download: https://sourceforge.net/projects/gnuwin32/files/findutils/4.2.20-2/findutils-4.2.20-2.exe/download?use_mirror=netix&download=
  5. Git (Optional): I use git to manage the project. You can use any version control software you desire.

Instructions:
  After downloading each of the above, place them in a directory. To the windows search box type "This PC". Write click on somewhere in the window that opens and click on properties. Open "Advanced System Settings" from the panel on the left. Go to "System Variables" and find "PATH". If you are on Windows 10 double click on it and click the Add button on the right hand panel. Then Add the following directories as entries to your Path:
  1. If you downloaded your Mingw to C:\Mingw for example (it can be anywhere that you wish). You should add the C:\Mingw\Bin directory to your path. That may not be the exact path so before adding it to your path go to the directory in which you downloaded mingw and find the bin directory. Add that directory to your path.
  2. Likewise you should add the QEMU directory in your path. Note that QEMU doesn't have a bin directory. All the files are in the download directory that you selected (C:\QEMU for example). Add this directory to your path.
  3. Just like QEMU add Nasm's download directory to your path. (C:\Nasm or wherever you downloaded it)
  4. Finally add the Findutils' Bin directory to your path. There is also one more thing you should do with this. Since windows comes with a find command itself, it may confuse the find.exe with its own find. Therefore you should rename the find.exe in the Findutils\Bin directory, "gfind.exe". The g in the front stands for "GNU" since we are using the GNU Findutils.
  
  After you have done all of these clone this repository. Go into any of the version. (The current version under development is "new 64 bit"). and type make.
  
  ---
  Mac:
  
  Running this on mac is considerably easier. Download Homebrew: https://brew.sh/index_tr
  
  Open a terminal and type the following commands.
  
 ```brew install nasm```
 ```brew install gcc``` (there is probably no need for this but just do it just to be sure)
 ```brew install make```
 ```brew install x86_64-elf-gcc```
 ```brew install x86_64-elf-binutils```
 ```brew install qemu```
 
 findutils are already a part of mac so that shouldn't be a problem.
 
 After downloading each of the packages just clone the repository, go into the active version and type make
  
  ---
  
  Reach out to me if you have any questions or if any of the steps throws errors at your face.
  
  ozgur.guzeldereli@gmail.com



