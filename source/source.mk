##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=source
ConfigurationName      :=Debug
WorkspacePath          :=/home/kyle/Desktop/Switch-Homebrew/switch-Examples/graphics/base/source
ProjectPath            :=/home/kyle/Desktop/Switch-Homebrew/switch-Examples/graphics/base/source
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Kyle
Date                   :=20/08/18
CodeLitePath           :=/home/kyle/.codelite
LinkerName             :=/usr/bin/x86_64-linux-gnu-g++
SharedObjectLinkerName :=/usr/bin/x86_64-linux-gnu-g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="source.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -O0
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch). $(LibraryPathSwitch)Debug 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/x86_64-linux-gnu-ar rcu
CXX      := /usr/bin/x86_64-linux-gnu-g++
CC       := /usr/bin/x86_64-linux-gnu-gcc
CXXFLAGS :=  -g -Wall $(Preprocessors)
CFLAGS   :=   $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/x86_64-linux-gnu-as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/mathVec.cpp$(ObjectSuffix) $(IntermediateDirectory)/upng.cpp$(ObjectSuffix) $(IntermediateDirectory)/shapes.cpp$(ObjectSuffix) $(IntermediateDirectory)/controls.cpp$(ObjectSuffix) $(IntermediateDirectory)/menu.cpp$(ObjectSuffix) $(IntermediateDirectory)/sDraw.cpp$(ObjectSuffix) $(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IntermediateDirectory)/json.cpp$(ObjectSuffix) $(IntermediateDirectory)/settings.cpp$(ObjectSuffix) $(IntermediateDirectory)/fonts.cpp$(ObjectSuffix) \
	



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/mathVec.cpp$(ObjectSuffix): mathVec.cpp $(IntermediateDirectory)/mathVec.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/kyle/Desktop/Switch-Homebrew/switch-Examples/graphics/base/source/mathVec.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/mathVec.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/mathVec.cpp$(DependSuffix): mathVec.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/mathVec.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/mathVec.cpp$(DependSuffix) -MM mathVec.cpp

$(IntermediateDirectory)/mathVec.cpp$(PreprocessSuffix): mathVec.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/mathVec.cpp$(PreprocessSuffix) mathVec.cpp

$(IntermediateDirectory)/upng.cpp$(ObjectSuffix): upng.cpp $(IntermediateDirectory)/upng.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/kyle/Desktop/Switch-Homebrew/switch-Examples/graphics/base/source/upng.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/upng.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/upng.cpp$(DependSuffix): upng.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/upng.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/upng.cpp$(DependSuffix) -MM upng.cpp

$(IntermediateDirectory)/upng.cpp$(PreprocessSuffix): upng.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/upng.cpp$(PreprocessSuffix) upng.cpp

$(IntermediateDirectory)/shapes.cpp$(ObjectSuffix): shapes.cpp $(IntermediateDirectory)/shapes.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/kyle/Desktop/Switch-Homebrew/switch-Examples/graphics/base/source/shapes.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/shapes.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/shapes.cpp$(DependSuffix): shapes.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/shapes.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/shapes.cpp$(DependSuffix) -MM shapes.cpp

$(IntermediateDirectory)/shapes.cpp$(PreprocessSuffix): shapes.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/shapes.cpp$(PreprocessSuffix) shapes.cpp

$(IntermediateDirectory)/controls.cpp$(ObjectSuffix): controls.cpp $(IntermediateDirectory)/controls.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/kyle/Desktop/Switch-Homebrew/switch-Examples/graphics/base/source/controls.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/controls.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/controls.cpp$(DependSuffix): controls.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/controls.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/controls.cpp$(DependSuffix) -MM controls.cpp

$(IntermediateDirectory)/controls.cpp$(PreprocessSuffix): controls.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/controls.cpp$(PreprocessSuffix) controls.cpp

$(IntermediateDirectory)/menu.cpp$(ObjectSuffix): menu.cpp $(IntermediateDirectory)/menu.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/kyle/Desktop/Switch-Homebrew/switch-Examples/graphics/base/source/menu.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/menu.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/menu.cpp$(DependSuffix): menu.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/menu.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/menu.cpp$(DependSuffix) -MM menu.cpp

$(IntermediateDirectory)/menu.cpp$(PreprocessSuffix): menu.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/menu.cpp$(PreprocessSuffix) menu.cpp

$(IntermediateDirectory)/sDraw.cpp$(ObjectSuffix): sDraw.cpp $(IntermediateDirectory)/sDraw.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/kyle/Desktop/Switch-Homebrew/switch-Examples/graphics/base/source/sDraw.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sDraw.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sDraw.cpp$(DependSuffix): sDraw.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sDraw.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/sDraw.cpp$(DependSuffix) -MM sDraw.cpp

$(IntermediateDirectory)/sDraw.cpp$(PreprocessSuffix): sDraw.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sDraw.cpp$(PreprocessSuffix) sDraw.cpp

$(IntermediateDirectory)/main.cpp$(ObjectSuffix): main.cpp $(IntermediateDirectory)/main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/kyle/Desktop/Switch-Homebrew/switch-Examples/graphics/base/source/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main.cpp$(DependSuffix): main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/main.cpp$(DependSuffix) -MM main.cpp

$(IntermediateDirectory)/main.cpp$(PreprocessSuffix): main.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main.cpp$(PreprocessSuffix) main.cpp

$(IntermediateDirectory)/json.cpp$(ObjectSuffix): json.cpp $(IntermediateDirectory)/json.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/kyle/Desktop/Switch-Homebrew/switch-Examples/graphics/base/source/json.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/json.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/json.cpp$(DependSuffix): json.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/json.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/json.cpp$(DependSuffix) -MM json.cpp

$(IntermediateDirectory)/json.cpp$(PreprocessSuffix): json.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/json.cpp$(PreprocessSuffix) json.cpp

$(IntermediateDirectory)/settings.cpp$(ObjectSuffix): settings.cpp $(IntermediateDirectory)/settings.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/kyle/Desktop/Switch-Homebrew/switch-Examples/graphics/base/source/settings.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/settings.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/settings.cpp$(DependSuffix): settings.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/settings.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/settings.cpp$(DependSuffix) -MM settings.cpp

$(IntermediateDirectory)/settings.cpp$(PreprocessSuffix): settings.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/settings.cpp$(PreprocessSuffix) settings.cpp

$(IntermediateDirectory)/fonts.cpp$(ObjectSuffix): fonts.cpp $(IntermediateDirectory)/fonts.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/kyle/Desktop/Switch-Homebrew/switch-Examples/graphics/base/source/fonts.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/fonts.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/fonts.cpp$(DependSuffix): fonts.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/fonts.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/fonts.cpp$(DependSuffix) -MM fonts.cpp

$(IntermediateDirectory)/fonts.cpp$(PreprocessSuffix): fonts.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/fonts.cpp$(PreprocessSuffix) fonts.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


