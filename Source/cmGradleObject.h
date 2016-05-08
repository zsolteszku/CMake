/*============================================================================
 CMake - Cross Platform Makefile Generator
 Copyright 2000-2009 Kitware, Inc., Insight Software Consortium

 Distributed under the OSI-approved BSD License (the "License");
 see accompanying file Copyright.txt for details.

 This software is distributed WITHOUT ANY WARRANTY; without even the
 implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 See the License for more information.
 ============================================================================*/
#ifndef cmGradleObject_h
#define cmGradleObject_h

#include "cmStandardIncludes.h"

#include <cmsys/auto_ptr.hxx>

struct cmGradleCurrentState {
  size_t IndentSize;
  cmGradleCurrentState() : IndentSize(0) {}
};
/*
 * cmGradleObject
 */
class cmGradleObject {

public:
  enum class Type { PLUGIN, BLOCK, SET_SETTING, VALUE, FUNCTION_CALL, COMMENT };
  void Indent(std::ostream &fout, cmGradleCurrentState &state) const;
  virtual void Write(std::ostream &fout, cmGradleCurrentState &state) const = 0;
  virtual Type GetType() const = 0;
  virtual ~cmGradleObject() {}
};

class cmGradlePlugin : public cmGradleObject {
public:
  explicit cmGradlePlugin(const std::string &name);
  virtual void Write(std::ostream &fout,
                     cmGradleCurrentState &state) const override;
  virtual Type GetType() const override { return Type::PLUGIN; }

private:
  std::string PluginName;
};

class cmGradleExpression : public cmGradleObject {};

class cmGradleBlock : public cmGradleExpression {
public:
  explicit cmGradleBlock(const std::string &name);
  virtual void Write(std::ostream &fout,
                     cmGradleCurrentState &state) const override;
  virtual Type GetType() const override { return Type::BLOCK; }
  virtual void AppendChild(cmGradleExpression *value);

  virtual ~cmGradleBlock();

private:
  std::string BlockName;
  std::vector<cmGradleExpression *> Childs;
};

class cmGradleValue : public cmGradleObject {
public:
  virtual Type GetType() const override { return Type::VALUE; }
};

class cmGradleSetSetting : public cmGradleExpression {
public:
  enum class Equality { USE, DO_NOT_USE };
  explicit cmGradleSetSetting(const std::string &name, cmGradleValue *value,
                              Equality equality = Equality::USE);
  virtual Type GetType() const override { return Type::SET_SETTING; }
  virtual void Write(std::ostream &fout,
                     cmGradleCurrentState &state) const override;

  const char *GetEquality() const;

protected:
  std::string SettingName;
  cmsys::auto_ptr<cmGradleValue> Value;
  Equality UseEqualitySign;
};

class cmGradleSimpleValue : public cmGradleValue {
public:
  enum class Apostrope { SIMPLE, NONE };
  cmGradleSimpleValue(const std::string &value,
                      Apostrope apostrophe = Apostrope::NONE);

  virtual void Write(std::ostream &fout,
                     cmGradleCurrentState &state) const override;

  const char *GetApostrophe() const;

private:
  std::string SettingValue;
  Apostrope UseApostrophes;
};

class cmGradleBoolValue : public cmGradleSimpleValue {
public:
  cmGradleBoolValue(bool value)
      : cmGradleSimpleValue(value ? "true" : "false") {}
};

class cmGradleListValue : public cmGradleValue {
public:
  cmGradleListValue(const std::vector<cmGradleValue *> &values =
                        std::vector<cmGradleValue *>());
  virtual void Write(std::ostream &fout,
                     cmGradleCurrentState &state) const override;

  void AppendArgument(cmGradleValue *argument);

  static cmGradleListValue *
  CreateFromSimples(const std::vector<std::string> &values,
                    cmGradleSimpleValue::Apostrope apostrophe =
                        cmGradleSimpleValue::Apostrope::NONE);

private:
  std::vector<cmGradleValue *> Values;
};

class cmGradleFunctionCall : public cmGradleExpression {
public:
  cmGradleFunctionCall(const std::string &func_name, cmGradleValue *arg = NULL);
  virtual Type GetType() const override { return Type::FUNCTION_CALL; }
  virtual void Write(std::ostream &fout,
                     cmGradleCurrentState &state) const override;

private:
  std::string FunctionName;
  cmsys::auto_ptr<cmGradleValue> Argument;
};

class cmGradleComment : public cmGradleExpression {
public:
  explicit cmGradleComment(const std::string &comment);
  virtual Type GetType() const override { return Type::COMMENT; }
  virtual void Write(std::ostream &fout,
                     cmGradleCurrentState &state) const override;

private:
  std::string Comment;
  bool IsMultiline;
};

#endif // cmGradleObject_h
