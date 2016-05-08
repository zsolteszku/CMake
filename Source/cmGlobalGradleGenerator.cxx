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

#include <cmsys/SystemTools.hxx>

namespace {
const char *CONFIG_TYPES = "CMAKE_CONFIGURATION_TYPES";
};

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
  if (!mf->GetDefinition(CONFIG_TYPES)) {
    mf->AddCacheDefinition(
        "CMAKE_CONFIGURATION_TYPES", "debug;release",
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
  // makeCommand.insert(makeCommand.end(), makeOptions.begin(),
  // makeOptions.end());
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
  auto mk = root->GetMakefile();
  {
    std::string gradleDir = root->GetCurrentBinaryDirectory();
    gradleDir += "/gradle";
    cmSystemTools::MakeDirectory(gradleDir.c_str());
    std::string wrapperDir =
        mk->GetRequiredDefinition("GRADLE_WRAPPER_DIRECTORY");
    if (!cmsys::SystemTools::FileIsFullPath(wrapperDir)) {
      wrapperDir = cmsys::SystemTools::CollapseFullPath(
          wrapperDir, root->GetCurrentSourceDirectory());
    }
    cmSystemTools::CopyADirectory(wrapperDir, gradleDir + "/wrapper");
  }
  {
    std::string manifest = mk->GetRequiredDefinition("GRADLE_ANDROID_MANIFEST");
    if (!cmsys::SystemTools::FileIsFullPath(manifest)) {
      manifest = cmsys::SystemTools::CollapseFullPath(
          manifest, root->GetCurrentSourceDirectory());
    }
    cmSystemTools::CopyAFile(
        manifest, std::string(root->GetCurrentBinaryDirectory()) +
                      "/src/main/" + cmSystemTools::GetFilenameName(manifest));
  }

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

cmGlobalGradleGenerator::JNIFlagsSettings::JNIFlagsSettings()
    : FunctionName("CFlags.addAll") {}

cmGradleFunctionCall *cmGlobalGradleGenerator::CreateFlagsFunctionCall(
    const std::string &flagsStr, const JNIFlagsSettings &settings) {

  std::vector<std::string> flags;
  SplitBySpaces(flagsStr, flags);
  // add additional flags
  flags.insert(flags.end(), settings.AdditionalFlags.begin(),
               settings.AdditionalFlags.end());
  return new cmGradleFunctionCall(
      settings.FunctionName,
      cmGradleListValue::CreateFromSimples(
          flags, cmGradleSimpleValue::Apostrope::SIMPLE));
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

  JNIFlagsSettings settings;
  std::vector<std::string> includeDirs;
  // TODO(zsessigkacso): do it properly
  root->GetIncludeDirectories(includeDirs, root->GetGeneratorTargets()[0]);
  for (const auto &includeDir : includeDirs) {
    settings.AdditionalFlags.push_back("-I" + includeDir);
  }
  ndkBlock->AppendChild(CreateFlagsFunctionCall(
      GetOptionalDefinition(mk, "JNI_FLAGS", ""), settings));
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

cmGlobalGradleGenerator::ConfigType
cmGlobalGradleGenerator::DetectConfigType(cmLocalGenerator *root,
                                          const std::string &configName) {
  auto mk = root->GetMakefile();
  {
    std::string uppercase_config_name = cmSystemTools::UpperCase(configName);
    // read from def
    const char *res = mk->GetDefinition(uppercase_config_name + "_CONFIG_TYPE");
    if (res != NULL) {
      std::string lowercase_res = cmSystemTools::LowerCase(res);
      if (lowercase_res == "debug")
        return ConfigType::DEBUG;
      else if (lowercase_res == "release")
        return ConfigType::RELESE;
    }
  }
  {
    // unknown type, try to detect
    std::string lower_config_name = cmSystemTools::LowerCase(configName);
    if (lower_config_name.find("debug") != std::string::npos) {
      return ConfigType::DEBUG;
    } else if (lower_config_name.find("release") != std::string::npos) {
      return ConfigType::RELESE;
    } else {
      // maybe warning?
      return ConfigType::RELESE;
    }
  }
}

void cmGlobalGradleGenerator::FillBuildTypesBlock(
    cmLocalGenerator *root, cmGradleBlock *buildTypesBlock) {
  auto mk = root->GetMakefile();
  std::vector<std::string> configs;
  cmSystemTools::ExpandListArgument(mk->GetRequiredDefinition(CONFIG_TYPES),
                                    configs);
  bool IsMinifyEnabled = cmSystemTools::IsOn(
      GetOptionalDefinition(mk, "GRADLE_ANDROID_MINIFY_ENABLED", "ON").c_str());
  for (const auto &config : configs) {
    cmsys::auto_ptr<cmGradleBlock> configBlock(new cmGradleBlock(config));
    std::string uppercaseConfig = cmSystemTools::UpperCase(config);
    {
      // flag support per config
      std::string configsFlags =
          GetOptionalDefinition(mk, "JNI_FLAGS_" + uppercaseConfig, "");
      if (!configsFlags.empty()) {
        JNIFlagsSettings settings;
        configBlock->AppendChild(
            CreateFlagsFunctionCall(configsFlags, settings));
      }
    }
    if (IsMinifyEnabled) {
      bool config_minify_enabled = false;
      // minify support
      const char *minify =
          mk->GetDefinition(uppercaseConfig + "_MINIFY_ENABLED");
      if (minify != NULL) {
        // minify is set
        config_minify_enabled = cmSystemTools::IsOn(minify);
      } else {
        // try detect
        ConfigType detectedType = DetectConfigType(root, config);
        config_minify_enabled = detectedType == ConfigType::RELESE;
      }
      configBlock->AppendChild(new cmGradleSetSetting(
          "minifyEnabled", new cmGradleBoolValue(config_minify_enabled)));
    }
    // TODO(zsessigkacso): implement propguard support
    buildTypesBlock->AppendChild(configBlock.release());
  }
}
namespace {
const std::unordered_map<std::string, const char *> &
SupportedABIAndArchPairs() {
  static std::unordered_map<std::string, const char *> knownABIAndArchPairs;
  if (knownABIAndArchPairs.empty()) {
    using Pair = std::unordered_map<std::string, const char *>::value_type;
    knownABIAndArchPairs.insert(Pair("armeabi", "arm"));
    knownABIAndArchPairs.insert(Pair("armeabi-v7a", "arm7"));
    knownABIAndArchPairs.insert(Pair("arm64-v8a", "arm8"));
    knownABIAndArchPairs.insert(Pair("x86", "x86"));
    knownABIAndArchPairs.insert(Pair("x86-64", "x86_64"));
    knownABIAndArchPairs.insert(Pair("mips", "mips"));
    knownABIAndArchPairs.insert(Pair("mips-64", "mips64"));
  }
  return knownABIAndArchPairs;
}

const std::string &DefaultSupportedABIsList() {
  static std::string res;
  if (res.empty()) {
    const auto &knownABIAndArchPairs = SupportedABIAndArchPairs();
    bool isFirst = true;
    for (const auto &p : knownABIAndArchPairs) {
      if (isFirst) {
        isFirst = false;
      } else {
        res.push_back(';');
      }
      res.append(p.first);
    }
  }
  return res;
}
}

void cmGlobalGradleGenerator::FillProductFlavorsBlock(
    cmLocalGenerator *root, cmGradleBlock *productFlavorsBlock) {
  auto mk = root->GetMakefile();
  productFlavorsBlock->AppendChild(new cmGradleComment(
      "for detailed abiFilter descriptions, refer to \"Supported ABIs\" @ \
                                                         \nhttps://developer.android.com/ndk/guides/abis.html#sa"));
  std::string supportedABISStr = GetOptionalDefinition(
      mk, "GRADLE_ANDROID_SUPPORTED_ABIS", DefaultSupportedABIsList());
  std::vector<std::string> supportedABIS;
  cmSystemTools::ExpandListArgument(supportedABISStr, supportedABIS);
  for (const auto &abi : supportedABIS) {
    std::string uppercase_abi = cmSystemTools::UpperCase(abi);
    {
      char *buffer = new char[uppercase_abi.length()];
      uppercase_abi.copy(buffer, uppercase_abi.length());
      cmSystemTools::ReplaceChars(buffer, "-", '_');
      uppercase_abi = std::string(buffer);
      delete[] buffer;
    }
    std::string arch = GetOptionalDefinition(
        mk, "GRADLE_ANDROID_" + uppercase_abi + "_ABI_ARCHITECTURE",
        GetABIDefaultArch(abi));
    // TODO(zsessigkacso) error handling
    if (!arch.empty()) {
      cmsys::auto_ptr<cmGradleBlock> block(
          new cmGradleBlock("create(\"" + arch + "\")"));
      block->AppendChild(new cmGradleFunctionCall(
          "ndk.abiFilters.add",
          new cmGradleSimpleValue(abi,
                                  cmGradleSimpleValue::Apostrope::SIMPLE)));
      productFlavorsBlock->AppendChild(block.release());
    }
  }
  productFlavorsBlock->AppendChild(new cmGradleComment(
      "To include all cpu architectures, leaves abiFilters empty"));
  productFlavorsBlock->AppendChild(new cmGradleFunctionCall(
      "create",
      new cmGradleSimpleValue("all", cmGradleSimpleValue::Apostrope::SIMPLE)));
}

const char *cmGlobalGradleGenerator::GetABIDefaultArch(const std::string &abi) {
  const auto &knownABIAndArchPairs = SupportedABIAndArchPairs();
  auto it = knownABIAndArchPairs.find(abi);
  if (it == knownABIAndArchPairs.end())
    return "";
  else
    return it->second;
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
    // defaultConfig
    cmsys::auto_ptr<cmGradleBlock> defaultConfigBlock(
        new cmGradleBlock("defaultConfig"));
    FillDefaultConfigBlock(root, defaultConfigBlock.get());
    androidBlock->AppendChild(defaultConfigBlock.release());
  }

  if (HaveNativeSourceFiles(mk)) {
    // ndk
    cmsys::auto_ptr<cmGradleBlock> ndkBlock(new cmGradleBlock("ndk"));
    FillNDKBlock(root, ndkBlock.get());
    androidBlock->AppendChild(ndkBlock.release());
  }
  {
    // buildTypes
    cmsys::auto_ptr<cmGradleBlock> buildTypesBlock(
        new cmGradleBlock("buildTypes"));
    FillBuildTypesBlock(root, buildTypesBlock.get());
    androidBlock->AppendChild(buildTypesBlock.release());
  }
  {
    // productFlavors
    cmsys::auto_ptr<cmGradleBlock> productFlavorsBlock(
        new cmGradleBlock("productFlavors"));
    FillProductFlavorsBlock(root, productFlavorsBlock.get());
    androidBlock->AppendChild(productFlavorsBlock.release());
  }
}

void cmGlobalGradleGenerator::FillSourceDir(cmLocalGenerator *root,
                                            cmGradleBlock *mainBlock,
                                            const std::string &group,
                                            bool useGlobalIncludeDirs) {
  auto mk = root->GetMakefile();
  std::vector<std::string> include_dirs;
  if (useGlobalIncludeDirs) {
    // TODO(zsessigkacso): do it properly
    root->GetIncludeDirectories(include_dirs, root->GetGeneratorTargets()[0]);
  } else {
    std::string group_uppercase = cmSystemTools::UpperCase(group);
    const char *res = mk->GetRequiredDefinition("GRADLE_" + group_uppercase +
                                                "_INCLUDE_DIRECTORIES");
    cmSystemTools::ExpandListArgument(res, include_dirs);
  }
  for (auto &include_dir : include_dirs) {
    if (!cmsys::SystemTools::FileIsFullPath(include_dir)) {
      include_dir = cmsys::SystemTools::CollapseFullPath(
          include_dir, root->GetSourceDirectory());
    }
  }
  if (!include_dirs.empty()) {
    cmsys::auto_ptr<cmGradleBlock> groupBlock(new cmGradleBlock(group));
    {
      cmsys::auto_ptr<cmGradleBlock> sourceBlock(new cmGradleBlock("source"));
      sourceBlock->AppendChild(new cmGradleSetSetting(
          "srcDirs",
          cmGradleListValue::CreateFromSimples(
              include_dirs, cmGradleSimpleValue::Apostrope::SIMPLE)));
      groupBlock->AppendChild(sourceBlock.release());
    }
    mainBlock->AppendChild(groupBlock.release());
  }
}

void cmGlobalGradleGenerator::FillAndroidSourcesBlock(
    cmLocalGenerator *root, cmGradleBlock *androidSourcesBlock) {
  cmsys::auto_ptr<cmGradleBlock> mainBlock(new cmGradleBlock("main"));
  FillSourceDir(root, mainBlock.get(), "java");
  FillSourceDir(root, mainBlock.get(), "jni", true);
  FillSourceDir(root, mainBlock.get(), "res");
  androidSourcesBlock->AppendChild(mainBlock.release());
}

void cmGlobalGradleGenerator::FillModelBlock(cmLocalGenerator *root,
                                             cmGradleBlock *modelBlock) {
  {
    cmsys::auto_ptr<cmGradleBlock> androidBlock(new cmGradleBlock("android"));
    FillAndroidBlock(root, androidBlock.get());
    modelBlock->AppendChild(androidBlock.release());
  }
  // sources
  {
    cmsys::auto_ptr<cmGradleBlock> androidSourcesBlock(
        new cmGradleBlock("android.sources"));
    FillAndroidSourcesBlock(root, androidSourcesBlock.get());
    modelBlock->AppendChild(androidSourcesBlock.release());
  }
}

void cmGlobalGradleGenerator::FillRepositoriesBlock(cmLocalGenerator *root,
                                                    cmGradleBlock *repoBlock) {
  auto mk = root->GetMakefile();
  std::vector<std::string> repos;
  cmSystemTools::ExpandListArgument(
      GetOptionalDefinition(mk, "GRADLE_REPOSITORIES", "jcenter"), repos);
  for (const auto &repo : repos) {
    repoBlock->AppendChild(new cmGradleFunctionCall(repo));
  }
}

void cmGlobalGradleGenerator::FillBuildscriptBlock(
    cmLocalGenerator *root, cmGradleBlock *buildScriptBlock) {
  auto mk = root->GetMakefile();
  {
    // repositories
    cmsys::auto_ptr<cmGradleBlock> repoBlock(new cmGradleBlock("repositories"));
    FillRepositoriesBlock(root, repoBlock.get());
    buildScriptBlock->AppendChild(repoBlock.release());
  }
  {
    // dependencies
    cmsys::auto_ptr<cmGradleBlock> dependencies(
        new cmGradleBlock("dependencies"));

    using Dependency = std::pair<std::string, std::string>;
    std::vector<Dependency> deps;
    {
      std::vector<std::string> depList;
      cmSystemTools::ExpandListArgument(
          GetOptionalDefinition(
              mk, "GRADLE_DEPENDENCIES",
              "classpath;com.android.tools.build:gradle-experimental:0.7.0"),
          depList);
      // TODO(zsessigkacso) error handling
      if (depList.size() % 2 == 0) {
        for (size_t idx = 0; idx + 1 < depList.size(); idx += 2) {
          deps.push_back(Dependency(depList[idx], depList[idx + 1]));
        }
      }
    }
    for (const auto &dep : deps) {
      dependencies->AppendChild(new cmGradleSetSetting(
          dep.first, new cmGradleSimpleValue(
                         dep.second, cmGradleSimpleValue::Apostrope::SIMPLE),
          cmGradleSetSetting::Equality::DO_NOT_USE));
    }
    buildScriptBlock->AppendChild(dependencies.release());
  }
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
  // buildscript
  {
    cmsys::auto_ptr<cmGradleBlock> buildScriptBlock(
        new cmGradleBlock("buildscript"));
    FillBuildscriptBlock(root, buildScriptBlock.get());
    Objects.push_back(buildScriptBlock.release());
  }
  // allprojects
  {
    cmsys::auto_ptr<cmGradleBlock> allProjectsBlock(
        new cmGradleBlock("allprojects"));
    {
      // repositories
      cmsys::auto_ptr<cmGradleBlock> repoBlock(
          new cmGradleBlock("repositories"));
      FillRepositoriesBlock(root, repoBlock.get());
      allProjectsBlock->AppendChild(repoBlock.release());
    }
    Objects.push_back(allProjectsBlock.release());
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
