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

struct cmGradleCurrentState {
  size_t IndentSize;
  cmGradleCurrentState() : IndentSize(0) {}
};
/*
 * cmGradleObject
 */
class cmGradleObject {

public:
  enum class Type { PLUGIN, BLOCK, SETTING };
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

class cmGradleValue : public cmGradleObject {};

class cmGradleBlock : public cmGradleValue {
public:
  explicit cmGradleBlock(const std::string &name);
  virtual void Write(std::ostream &fout,
                     cmGradleCurrentState &state) const override;
  virtual Type GetType() const override { return Type::BLOCK; }
  virtual void AppendChild(cmGradleValue *value);

  virtual ~cmGradleBlock();

private:
  std::string BlockName;
  std::vector<cmGradleValue *> Childs;
};

class cmGradleSetting : public cmGradleValue {
public:
  explicit cmGradleSetting(const std::string &name);
  virtual Type GetType() const override { return Type::SETTING; }

protected:
  std::string SettingName;
};

class cmGradleSimpleSetting : public cmGradleSetting {
public:
  cmGradleSimpleSetting(const std::string &name, const std::string &value,
                        bool use_equality_sign = true);
  virtual void Write(std::ostream &fout,
                     cmGradleCurrentState &state) const override;

private:
  std::string SettingValue;
  bool UseEqualitySign;
};

#endif // cmGradleObject_h
