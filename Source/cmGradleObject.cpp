/*============================================================================
 CMake - Cross Platform Makefile Generator
 Copyright 2000-2009 Kitware, Inc., Insight Software Consortium

 Distributed under the OSI-approved BSD License (the "License");
 see accompanying file Copyright.txt for details.

 This software is distributed WITHOUT ANY WARRANTY; without even the
 implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 See the License for more information.
 ============================================================================*/
#include "cmGradleObject.h"

cmGradlePlugin::cmGradlePlugin(const std::string &name) : PluginName(name) {}

void cmGradleObject::Indent(std::ostream &fout,
                            cmGradleCurrentState &state) const {
  for (size_t curr = 0; curr < state.IndentSize; ++curr) {
    fout << '\t';
  }
}

void cmGradlePlugin::Write(std::ostream &fout,
                           cmGradleCurrentState &state) const {
  Indent(fout, state);
  fout << "apply plugin: '" << PluginName << "'" << std::endl;
}

cmGradleBlock::cmGradleBlock(const std::string &name) : BlockName(name) {}

void cmGradleBlock::AppendChild(cmGradleExpression *value) {
  Childs.push_back(value);
}

cmGradleBlock::~cmGradleBlock() {
  for (const auto &child : Childs) {
    delete child;
  }
  Childs.clear();
}

void cmGradleBlock::Write(std::ostream &fout,
                          cmGradleCurrentState &state) const {
  Indent(fout, state);
  fout << BlockName << " {" << std::endl;
  state.IndentSize++;
  for (const auto &child : Childs) {
    child->Write(fout, state);
  }
  state.IndentSize--;
  Indent(fout, state);
  fout << "}" << std::endl;
}

cmGradleSetSetting::cmGradleSetSetting(const std::string &name,
                                       cmGradleValue *value, Equality equality)
    : SettingName(name), Value(value), UseEqualitySign(equality) {}

const char *cmGradleSetSetting::GetEquality() const {
  return (UseEqualitySign == Equality::USE ? " = " : " ");
}

void cmGradleSetSetting::Write(std::ostream &fout,
                               cmGradleCurrentState &state) const {
  Indent(fout, state);
  fout << SettingName << GetEquality();
  Value->Write(fout, state);
  fout << std::endl;
}

cmGradleSimpleValue::cmGradleSimpleValue(const std::string &value,
                                         Apostrope apostrophe)
    : SettingValue(value), UseApostrophes(apostrophe) {}

void cmGradleSimpleValue::Write(std::ostream &fout,
                                cmGradleCurrentState &state) const {
  fout << GetApostrophe() << SettingValue << GetApostrophe();
}

const char *cmGradleSimpleValue::GetApostrophe() const {
  return (UseApostrophes == Apostrope::SIMPLE ? "'" : "");
}
