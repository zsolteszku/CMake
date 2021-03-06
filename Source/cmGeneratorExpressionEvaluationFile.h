/*============================================================================
  CMake - Cross Platform Makefile Generator
  Copyright 2013 Stephen Kelly <steveire@gmail.com>

  Distributed under the OSI-approved BSD License (the "License");
  see accompanying file Copyright.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the License for more information.
============================================================================*/
#ifndef cmGeneratorExpressionEvaluationFile_h
#define cmGeneratorExpressionEvaluationFile_h

#include "cmGeneratorExpression.h"

#include <cmsys/auto_ptr.hxx>
#include <sys/types.h>

class cmLocalGenerator;

//----------------------------------------------------------------------------
class cmGeneratorExpressionEvaluationFile
{
public:
  cmGeneratorExpressionEvaluationFile(const std::string &input,
        cmsys::auto_ptr<cmCompiledGeneratorExpression> outputFileExpr,
        cmsys::auto_ptr<cmCompiledGeneratorExpression> condition,
        bool inputIsContent);

  void Generate(cmLocalGenerator* lg);

  std::vector<std::string> GetFiles() const { return this->Files; }

  void CreateOutputFile(cmLocalGenerator* lg, std::string const& config);

private:
  void Generate(cmLocalGenerator* lg, const std::string& config,
                const std::string& lang,
                cmCompiledGeneratorExpression* inputExpression,
                std::map<std::string, std::string> &outputFiles, mode_t perm);

private:
  const std::string Input;
  const cmsys::auto_ptr<cmCompiledGeneratorExpression> OutputFileExpr;
  const cmsys::auto_ptr<cmCompiledGeneratorExpression> Condition;
  std::vector<std::string> Files;
  const bool InputIsContent;
};

#endif
