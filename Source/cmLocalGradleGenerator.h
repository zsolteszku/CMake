/*============================================================================
 CMake - Cross Platform Makefile Generator
 Copyright 2000-2009 Kitware, Inc., Insight Software Consortium

 Distributed under the OSI-approved BSD License (the "License");
 see accompanying file Copyright.txt for details.

 This software is distributed WITHOUT ANY WARRANTY; without even the
 implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 See the License for more information.
 ============================================================================*/
#ifndef cmLocalGradleGenerator_h
#define cmLocalGradleGenerator_h

#include "cmLocalGenerator.h"

/*
 * cmLocalGradleGenerator
 */
class cmLocalGradleGenerator : public cmLocalGenerator {

public:
  cmLocalGradleGenerator(cmGlobalGenerator *gg, cmMakefile *mf);

  virtual ~cmLocalGradleGenerator() {}

  virtual std::string GetTargetDirectory(cmGeneratorTarget const *target) const;

  virtual void AppendFlagEscape(std::string &flags, const std::string &rawFlag);

  virtual void Generate();
  virtual void GenerateInstallRules();
  virtual void
  ComputeObjectFilenames(std::map<cmSourceFile const *, std::string> &mapping,
                         cmGeneratorTarget const *gt = 0);
};

#endif // cmLocalGradleGenerator_h
