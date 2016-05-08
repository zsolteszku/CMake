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

void cmGradleBlock::AppendChild(cmGradleValue *value) {
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

cmGradleSetting::cmGradleSetting(const std::string &name) : SettingName(name) {}

cmGradleSimpleSetting::cmGradleSimpleSetting(const std::string &name,
                                             const std::string &value,
                                             bool use_equality_sign)
    : cmGradleSetting(name), SettingValue(value),
      UseEqualitySign(use_equality_sign) {}

void cmGradleSimpleSetting::Write(std::ostream &fout,
                                  cmGradleCurrentState &state) const {
  Indent(fout, state);
  fout << SettingName << (UseEqualitySign ? " = " : " ") << SettingValue
       << std::endl;
}
