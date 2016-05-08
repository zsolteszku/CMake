/*============================================================================
 CMake - Cross Platform Makefile Generator
 Copyright 2000-2009 Kitware, Inc., Insight Software Consortium

 Distributed under the OSI-approved BSD License (the "License");
 see accompanying file Copyright.txt for details.

 This software is distributed WITHOUT ANY WARRANTY; without even the
 implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 See the License for more information.
 ============================================================================*/
#include "cmGlobalGradleGenerator.h"

#include "cmGeneratedFileStream.h"
#include "cmGlobalGeneratorFactory.h"
#include "cmLocalGenerator.h"
#include "cmLocalGradleGenerator.h"
#include "cmMakefile.h"
#include "cmSourceFile.h"
#include "cmSystemTools.h"

class cmGlobalGradleGenerator::Factory : public cmGlobalGeneratorFactory {
public:
  virtual cmGlobalGenerator *CreateGlobalGenerator(const std::string &name,
                                                   cmake *cm) const;

  virtual void GetDocumentation(cmDocumentationEntry &entry) const {
    cmGlobalGradleGenerator::GetDocumentation(entry);
  }

  virtual void GetGenerators(std::vector<std::string> &names) const {
    names.push_back(cmGlobalGradleGenerator::GetActualName());
  }

  virtual bool SupportsToolset() const { return true; }
};

//----------------------------------------------------------------------------
cmGlobalGradleGenerator::cmGlobalGradleGenerator(cmake *cm)
    : cmGlobalGenerator(cm) {
  // cm->GetState()->AddCommand(NULL);
}

void cmGlobalGradleGenerator::GetDocumentation(cmDocumentationEntry &entry) {
  entry.Name = cmGlobalGradleGenerator::GetActualName();
  entry.Brief = "Generate Gradle project files.";
}

cmGlobalGenerator *
cmGlobalGradleGenerator::Factory::CreateGlobalGenerator(const std::string &name,
                                                        cmake *cm) const {
  if (name != GetActualName())
    return 0;
  return new cmGlobalGradleGenerator(cm);
}

cmGlobalGeneratorFactory *cmGlobalGradleGenerator::NewFactory() {
  return new Factory();
}

//----------------------------------------------------------------------------
void cmGlobalGradleGenerator::FindMakeProgram(cmMakefile *mf) {
  // The Xcode generator knows how to lookup its build tool
  // directly instead of needing a helper module to do it, so we
  // do not actually need to put CMAKE_MAKE_PROGRAM into the cache.
  if (cmSystemTools::IsOff(mf->GetDefinition("CMAKE_MAKE_PROGRAM"))) {
    mf->AddDefinition("CMAKE_MAKE_PROGRAM",
                      this->GetGradleBuildCommand().c_str());
  }
}

std::string cmGlobalGradleGenerator::GetGradleBuildCommand() const {
  return "gradle";
}

//----------------------------------------------------------------------------
void cmGlobalGradleGenerator::EnableLanguage(
    std::vector<std::string> const &lang, cmMakefile *mf, bool optional) {
  if (!mf->GetDefinition("CMAKE_CONFIGURATION_TYPES")) {
    mf->AddCacheDefinition(
        "CMAKE_CONFIGURATION_TYPES", "Debug;Release;MinSizeRel;RelWithDebInfo",
        "Semicolon separated list of supported configuration types, "
        "only supports Debug, Release, MinSizeRel, and RelWithDebInfo, "
        "anything else will be ignored.",
        cmState::STRING);
  }
  // wtf is this shit?
  mf->AddDefinition("CMAKE_GENERATOR_NO_COMPILER_ENV", "1");
  this->cmGlobalGenerator::EnableLanguage(lang, mf, optional);
}

void cmGlobalGradleGenerator::GenerateBuildCommand(
    std::vector<std::string> &makeCommand, const std::string &makeProgram,
    const std::string &projectName, const std::string &projectDir,
    const std::string &targetName, const std::string &config, bool fast,
    bool verbose, std::vector<std::string> const &makeOptions) {
  // TODO(zsessigkacso): implement
  makeCommand.insert(makeCommand.end(), makeOptions.begin(), makeOptions.end());
}

void cmGlobalGradleGenerator::AppendDirectoryForConfig(
    const std::string &prefix, const std::string &config,
    const std::string &suffix, std::string &dir) {
  // TODO(zsessigkacso): implement
  if (!config.empty()) {
    dir += prefix;
    dir += config;
    dir += suffix;
  }
}

//----------------------------------------------------------------------------
///! Create a local generator appropriate to this Global Generator
cmLocalGenerator *
cmGlobalGradleGenerator::CreateLocalGenerator(cmMakefile *mf) {
  // TODO(zsessigkacso): implement
  return new cmLocalGradleGenerator(this, mf);
}

const char *cmGlobalGradleGenerator::GetCMakeCFGIntDir() const {
  // TODO(zsessigkacso): implement
  return this->cmGlobalGenerator::GetCMakeCFGIntDir();
}

std::string
cmGlobalGradleGenerator::ExpandCFGIntDir(const std::string &str,
                                         const std::string &config) const {
  // TODO(zsessigkacso): implement
  return this->cmGlobalGenerator::ExpandCFGIntDir(str, config);
}

bool cmGlobalGradleGenerator::IsMultiConfig() { return true; }

bool cmGlobalGradleGenerator::SetGeneratorToolset(const std::string &ts,
                                                  cmMakefile *mf) {
  // TODO(zsessigkacso): implement
  return cmGlobalGenerator::SetGeneratorToolset(ts, mf);
}

void cmGlobalGradleGenerator::AppendFlag(std::string &flags,
                                         const std::string &flag) {
  // TODO(zsessigkacso): implement
  flags += " " + flag;
}

void cmGlobalGradleGenerator::AddExtraIDETargets() {
  // clang-format off
  std::map<std::string, std::vector<cmLocalGenerator *> >::iterator it;
  // clang-format on
  // make sure extra targets are added before calling
  // the parent generate which will call trace depends
  for (it = this->ProjectMap.begin(); it != this->ProjectMap.end(); ++it) {
    cmLocalGenerator *root = it->second[0];
    this->SetGenerationRoot(root);
    // add ALL_BUILD, INSTALL, etc
    this->AddExtraTargets(root, it->second);
  }
}

void cmGlobalGradleGenerator::SetGenerationRoot(cmLocalGenerator *root) {
  // this->CurrentProject = root->GetProjectName();
  // this->SetCurrentLocalGenerator(root);
  //  std::vector<std::string> ProjectSourceDirectoryComponents; // members in
  //  XCode
  //  std::vector<std::string> ProjectOutputDirectoryComponents;
  //  cmSystemTools::SplitPath(root->GetCurrentSourceDirectory(),
  //                           ProjectSourceDirectoryComponents);
  //  cmSystemTools::SplitPath(root->GetCurrentBinaryDirectory(),
  //                           ProjectOutputDirectoryComponents);

  // cmSystemTools::MakeDirectory(root->GetCurrentBinaryDirectory().c_str());
}

void cmGlobalGradleGenerator::AddExtraTargets(
    cmLocalGenerator *root, std::vector<cmLocalGenerator *> &gens) {
  // TODO(zsessigkacso): implement
  // Add ALL_BUILD
}

void cmGlobalGradleGenerator::Generate() {
  this->cmGlobalGenerator::Generate();
  if (cmSystemTools::GetErrorOccuredFlag()) {
    return;
  }
  // clang-format off
  std::map<std::string, std::vector<cmLocalGenerator *> >::iterator it;
  // clang-format on
  for (it = this->ProjectMap.begin(); it != this->ProjectMap.end(); ++it) {
    cmLocalGenerator *root = it->second[0];
    this->SetGenerationRoot(root);
    // now create the project
    this->OutputGradleProject(root, it->second);
  }
}

void cmGlobalGradleGenerator::OutputGradleProject(
    cmLocalGenerator *root, std::vector<cmLocalGenerator *> &generators) {
  if (generators.size() == 0) {
    return;
  }
  // Skip local generators that are excluded from this project.
  for (std::vector<cmLocalGenerator *>::iterator g = generators.begin();
       g != generators.end(); ++g) {
    if (this->IsExcluded(root, *g)) {
      continue;
    }
  }
  if (!this->CreateGradleObjects(root, generators)) {
    return;
  }
  std::string gradleDir = root->GetCurrentBinaryDirectory();
  gradleDir += "/gradle/wrapper";
  cmSystemTools::MakeDirectory(gradleDir.c_str());

  std::string gradleFile = root->GetCurrentBinaryDirectory();
  gradleFile += "/build.gradle";
  cmGeneratedFileStream fout(gradleFile.c_str());
  fout.SetCopyIfDifferent(true);
  if (!fout) {
    return;
  }

  this->WriteBuildGradle(fout, root, generators);
  // this->ClearXCodeObjects();

  // Since this call may have created new cache entries, save the cache:
  //
  root->GetMakefile()->GetCMakeInstance()->SaveCache(
      root->GetBinaryDirectory());
}

std::string cmGlobalGradleGenerator::GetOptionalDefinition(
    cmMakefile *mk, const std::string &name, const std::string &defaultValue) {
  const char *what = mk->GetDefinition(name);
  if (what == NULL)
    return defaultValue;
  return what;
}

void cmGlobalGradleGenerator::FillDefaultConfigBlock(
    cmLocalGenerator *root, cmGradleBlock *defaultConfigBlock) {
  auto mk = root->GetMakefile();
  defaultConfigBlock->AppendChild(new cmGradleSetSetting(
      "applicationId",
      new cmGradleSimpleValue(
          mk->GetRequiredDefinition("GRADLE_ANDROID_APPLICATION_ID"),
          cmGradleSimpleValue::Apostrope::SIMPLE)));

  defaultConfigBlock->AppendChild(new cmGradleSetSetting(
      "minSdkVersion.apiLevel",
      new cmGradleSimpleValue(
          GetOptionalDefinition(mk, "GRADLE_ANDROID_MIN_SDK_API_LEVEL", "4"))));

  defaultConfigBlock->AppendChild(new cmGradleSetSetting(
      "targetSdkVersion.apiLevel",
      new cmGradleSimpleValue(GetOptionalDefinition(
          mk, "GRADLE_ANDROID_TARGET_SDK_API_LEVEL", "23"))));
}

void cmGlobalGradleGenerator::SplitBySpaces(const std::string &str,
                                            std::vector<std::string> &res) {
  const char sep = ' ';
  size_t size = str.size();
  size_t end = 0;
  size_t start = end;
  for (end = 0; end < size && (end = str.find(sep, end)) != std::string::npos;
       start = ++end) {
    size_t currSize = end - start;
    if (currSize != 0) {
      res.push_back(str.substr(start, currSize));
    }
  }
  if (start < size) {
    res.push_back(str.substr(start));
  }
}

void cmGlobalGradleGenerator::FillNDKBlock(cmLocalGenerator *root,
                                           cmGradleBlock *ndkBlock) {
  auto mk = root->GetMakefile();
  ndkBlock->AppendChild(new cmGradleSetSetting(
      "platformVersion", new cmGradleSimpleValue(GetOptionalDefinition(
                             mk, "GRADLE_ANDROID_NDK_VERSION", "9"))));

  ndkBlock->AppendChild(new cmGradleSetSetting(
      "moduleName",
      new cmGradleSimpleValue(
          GetOptionalDefinition(mk, "GRADLE_ANDROID_JNI_MODULE_NAME",
                                root->GetProjectName() + "-jni"),
          cmGradleSimpleValue::Apostrope::SIMPLE)));

  ndkBlock->AppendChild(new cmGradleSetSetting(
      "toolchain",
      new cmGradleSimpleValue(
          mk->GetRequiredDefinition("GRADLE_ANDROID_NDK_TOOLCHAIN"),
          cmGradleSimpleValue::Apostrope::SIMPLE)));
  {
    std::string jni_flags = GetOptionalDefinition(mk, "JNI_FLAGS", "");
    std::vector<std::string> flags;
    SplitBySpaces(jni_flags, flags);
    {
      cmsys::auto_ptr<cmGradleListValue> jniFlagsList(new cmGradleListValue);
      for (const auto &flag : flags) {
        jniFlagsList->AppendArgument(new cmGradleSimpleValue(
            flag, cmGradleSimpleValue::Apostrope::SIMPLE));
      }
      ndkBlock->AppendChild(
          new cmGradleFunctionCall("CFlags.addAll", jniFlagsList.release()));
    }
  }
}

cmGlobalGradleGenerator::SourceFileType
cmGlobalGradleGenerator::GetSourceFileType(cmSourceFile *sourceFile) const {
  if (sourceFile->GetLanguage() == "C" || sourceFile->GetLanguage() == "CXX")
    return SourceFileType::CPP;
  if (sourceFile->GetExtension() == "java")
    return SourceFileType::JAVA;
  return SourceFileType::UNKOWN;
}

bool cmGlobalGradleGenerator::HaveNativeSourceFiles(cmMakefile *mk) const {
  for (const auto &sourceFile : mk->GetSourceFiles()) {
    if (GetSourceFileType(sourceFile) == SourceFileType::CPP)
      return true;
  }
  return false;
}

void cmGlobalGradleGenerator::FillAndroidBlock(cmLocalGenerator *root,
                                               cmGradleBlock *androidBlock) {
  auto mk = root->GetMakefile();
  androidBlock->AppendChild(new cmGradleSetSetting(
      "compileSdkVersion",
      new cmGradleSimpleValue(GetOptionalDefinition(
          mk, "GRADLE_ANDROID_COMPILE_SDK_VERSION", "23"))));
  androidBlock->AppendChild(new cmGradleSetSetting(
      "buildToolsVersion",
      new cmGradleSimpleValue(
          GetOptionalDefinition(mk, "GRADLE_ANDROID_BUILD_TOOLS_VERSION",
                                "23.0.2"),
          cmGradleSimpleValue::Apostrope::SIMPLE)));

  {
    cmsys::auto_ptr<cmGradleBlock> defaultConfigBlock(
        new cmGradleBlock("defaultConfig"));
    FillDefaultConfigBlock(root, defaultConfigBlock.get());
    androidBlock->AppendChild(defaultConfigBlock.release());
  }

  if (HaveNativeSourceFiles(mk)) {
    cmsys::auto_ptr<cmGradleBlock> ndkBlock(new cmGradleBlock("ndk"));
    FillNDKBlock(root, ndkBlock.get());
    androidBlock->AppendChild(ndkBlock.release());
  }
}

void cmGlobalGradleGenerator::FillModelBlock(cmLocalGenerator *root,
                                             cmGradleBlock *modelBlock) {
  cmsys::auto_ptr<cmGradleBlock> androidBlock(new cmGradleBlock("android"));
  FillAndroidBlock(root, androidBlock.get());
  modelBlock->AppendChild(androidBlock.release());
}

bool cmGlobalGradleGenerator::CreateGradleObjects(
    cmLocalGenerator *root, std::vector<cmLocalGenerator *> &generators) {
  // apply plugins
  cmMakefile *mk = root->GetMakefile();
  const char *pluginsStr = mk->GetDefinition("GRADLE_PLUGINS");
  if (pluginsStr != NULL) {
    std::vector<std::string> plugins;
    cmSystemTools::ExpandListArgument(pluginsStr, plugins);
    for (const auto &plugin : plugins) {
      Objects.push_back(new cmGradlePlugin(plugin));
    }
  }

  // create model
  {
    cmsys::auto_ptr<cmGradleBlock> modelBlock(new cmGradleBlock(
        GetOptionalDefinition(mk, "GRADLE_MODEL_NAME", "model")));
    FillModelBlock(root, modelBlock.get());
    Objects.push_back(modelBlock.release());
  }

  return true;
}

void cmGlobalGradleGenerator::WriteBuildGradle(
    std::ostream &fout, cmLocalGenerator *root,
    std::vector<cmLocalGenerator *> &generators) {
  cmGradleCurrentState state;
  cmGradleObject::Type prevType = cmGradleObject::Type::PLUGIN;
  for (const auto &object : Objects) {
    cmGradleObject::Type currType = object->GetType();
    if (currType != prevType) {
      fout << std::endl;
      prevType = currType;
    }
    object->Write(fout, state);
  }
}

cmGlobalGradleGenerator::~cmGlobalGradleGenerator() {
  for (const auto &obj : Objects) {
    delete obj;
  }
  Objects.clear();
}
