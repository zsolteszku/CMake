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

#include "cmSystemTools.h"

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

cmGradleListValue::cmGradleListValue(const std::vector<cmGradleValue *> &values)
    : Values(values) {}

void cmGradleListValue::AppendArgument(cmGradleValue *argument) {
  Values.push_back(argument);
}

void cmGradleListValue::Write(std::ostream &fout,
                              cmGradleCurrentState &state) const {
  fout << "[";
  bool isFirst = true;
  for (const auto &value : Values) {
    if (isFirst) {
      isFirst = false;
    } else {
      fout << ", ";
    }
    value->Write(fout, state);
  }
  fout << "]";
}

cmGradleListValue *cmGradleListValue::CreateFromSimples(
    const std::vector<std::string> &values,
    cmGradleSimpleValue::Apostrope apostrophe) {
  cmsys::auto_ptr<cmGradleListValue> listValues(new cmGradleListValue);
  for (const auto &value : values) {
    listValues->AppendArgument(new cmGradleSimpleValue(value, apostrophe));
  }
  return listValues.release();
}

cmGradleFunctionCall::cmGradleFunctionCall(const std::string &func_name,
                                           cmGradleValue *arg)
    : FunctionName(func_name), Argument(arg) {}

void cmGradleFunctionCall::Write(std::ostream &fout,
                                 cmGradleCurrentState &state) const {
  Indent(fout, state);
  fout << FunctionName << "(";
  if (Argument.get() != NULL)
    Argument->Write(fout, state);
  fout << ")" << std::endl;
}

cmGradleComment::cmGradleComment(const std::string &comment)
    : Comment(comment), IsMultiline(comment.find('\n') != std::string::npos) {}

void cmGradleComment::Write(std::ostream &fout,
                            cmGradleCurrentState &state) const {
  Indent(fout, state);
  if (IsMultiline) {
    fout << "/* ";
    std::vector<std::string> lines;
    cmSystemTools::Split(Comment.c_str(), lines);
    bool isFirst = true;
    for (const auto &line : lines) {
      if (isFirst) {
        isFirst = false;
        fout << line << std::endl;
      } else {
        Indent(fout, state);
        fout << " * " << line << std::endl;
      }
    }
    Indent(fout, state);
    fout << " */" << std::endl;
  } else {
    fout << "// " << Comment << std::endl;
  }
}