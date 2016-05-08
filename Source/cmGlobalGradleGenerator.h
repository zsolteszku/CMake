/*============================================================================
 CMake - Cross Platform Makefile Generator
 Copyright 2000-2009 Kitware, Inc., Insight Software Consortium

 Distributed under the OSI-approved BSD License (the "License");
 see accompanying file Copyright.txt for details.

 This software is distributed WITHOUT ANY WARRANTY; without even the
 implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 See the License for more information.
 ============================================================================*/
#ifndef cmGlobalGradleGenerator_h
#define cmGlobalGradleGenerator_h

#include "cmGlobalGenerator.h"
#include "cmGradleObject.h"

#include <cmsys/auto_ptr.hxx>

class cmGlobalGeneratorFactory;
class cmSourceFile;
class cmSourceGroup;

/*
 * cmGlobalGradleGenerator
 */
class cmGlobalGradleGenerator : public cmGlobalGenerator {

public:
  cmGlobalGradleGenerator(cmake *cm);

  virtual ~cmGlobalGradleGenerator();
  static cmGlobalGeneratorFactory *NewFactory();

  ///! Get the name for the generator.
  virtual std::string GetName() const {
    return cmGlobalGradleGenerator::GetActualName();
  }
  static std::string GetActualName() { return "Gradle"; }

  /** Get the documentation entry for this generator.  */
  static void GetDocumentation(cmDocumentationEntry &entry);

  /**
   * Try to determine system information such as shared library
   * extension, pthreads, byte order etc.
   */
  virtual void EnableLanguage(std::vector<std::string> const &languages,
                              cmMakefile *, bool optional);

  /**
   * Try running cmake and building a file. This is used for dynalically
   * loaded commands, not as part of the usual build process.
   */
  virtual void GenerateBuildCommand(
      std::vector<std::string> &makeCommand, const std::string &makeProgram,
      const std::string &projectName, const std::string &projectDir,
      const std::string &targetName, const std::string &config, bool fast,
      bool verbose,
      std::vector<std::string> const &makeOptions = std::vector<std::string>());

  virtual void FindMakeProgram(cmMakefile *);

  /** Append the subdirectory for the given configuration.  */
  virtual void AppendDirectoryForConfig(const std::string &prefix,
                                        const std::string &config,
                                        const std::string &suffix,
                                        std::string &dir);

  ///! Create a local generator appropriate to this Global Generator
  virtual cmLocalGenerator *CreateLocalGenerator(cmMakefile *mf);

  ///! What is the configurations directory variable called?
  virtual const char *GetCMakeCFGIntDir() const;

  ///! expand CFGIntDir
  virtual std::string ExpandCFGIntDir(const std::string &str,
                                      const std::string &config) const;

  // void SetCurrentLocalGenerator(cmLocalGenerator *);

  /** Return true if the generated build tree may contain multiple builds.
   i.e. "Can I build Debug and Release in the same tree?" */
  virtual bool IsMultiConfig();

  virtual bool SetGeneratorToolset(std::string const &ts, cmMakefile *mf);

  void AppendFlag(std::string &flags, std::string const &flag);

protected:
  virtual void AddExtraIDETargets();

  virtual void Generate();

private:
  std::string GetGradleBuildCommand() const;

  static std::string GetOptionalDefinition(cmMakefile *mk,
                                           const std::string &name,
                                           const std::string &defaultValue);

  void SetGenerationRoot(cmLocalGenerator *root);

  void AddExtraTargets(cmLocalGenerator *root,
                       std::vector<cmLocalGenerator *> &gens);

  void OutputGradleProject(cmLocalGenerator *root,
                           std::vector<cmLocalGenerator *> &generators);

  bool CreateGradleObjects(cmLocalGenerator *root,
                           std::vector<cmLocalGenerator *> &generators);

  void WriteBuildGradle(std::ostream &fout, cmLocalGenerator *root,
                        std::vector<cmLocalGenerator *> &generators);

  void FillModelBlock(cmLocalGenerator *root, cmGradleBlock *modelBlock);

  void FillAndroidBlock(cmLocalGenerator *root, cmGradleBlock *androidBlock);

  void FillDefaultConfigBlock(cmLocalGenerator *root,
                              cmGradleBlock *defaultConfigBlock);

  void FillNDKBlock(cmLocalGenerator *root, cmGradleBlock *ndkBlock);

  enum class SourceFileType { UNKOWN, JAVA, CPP };

  SourceFileType GetSourceFileType(cmSourceFile *src) const;

  bool HaveNativeSourceFiles(cmMakefile *mk) const;

private:
  class Factory;

  std::vector<cmGradleObject *> Objects;
};

#endif // cmGlobalGradleGenerator_h
