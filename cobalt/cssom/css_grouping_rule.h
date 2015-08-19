/*
 * Copyright 2015 Google Inc. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CSSOM_CSS_GROUPING_RULE_H_
#define CSSOM_CSS_GROUPING_RULE_H_

#include <utility>
#include <vector>

#include "base/memory/ref_counted.h"
#include "cobalt/cssom/css_rule.h"

namespace cobalt {
namespace cssom {

class CSSStyleRule;
class CSSRuleList;
class StyleSheet;

// The CSSGroupingRule interface represents an at-rule that contains other rules
//  nested inside itself.
//   http://www.w3.org/TR/cssom/#cssgroupingrule
//   http://www.w3.org/TR/css3-conditional/#cssgroupingrule
class CSSGroupingRule : public CSSRule {
 public:
  CSSGroupingRule();
  explicit CSSGroupingRule(StyleSheet* parent_style_sheet);
  explicit CSSGroupingRule(const scoped_refptr<CSSRuleList>& css_rule_list);

  // Set the css rules for the style sheet.
  void set_css_rules(const scoped_refptr<CSSRuleList>& css_rule_list);

  // Returns a read-only, live object representing the CSS rules.
  scoped_refptr<CSSRuleList> css_rules();

  // Inserts a new rule to the css rule list of the group.
  unsigned int InsertRule(const scoped_refptr<CSSStyleRule>& css_rule,
                          unsigned int index);

  // Custom, not in any spec.
  //

  // From CSSRule.
  void AttachToStyleSheet(StyleSheet* style_sheet) OVERRIDE;
  StyleSheet* ParentStyleSheet() OVERRIDE { return parent_style_sheet_; }

  DEFINE_WRAPPABLE_TYPE(CSSGroupingRule);

 protected:
  virtual ~CSSGroupingRule() OVERRIDE;

 private:
  scoped_refptr<CSSRuleList> css_rule_list_;

  StyleSheet* parent_style_sheet_;

  DISALLOW_COPY_AND_ASSIGN(CSSGroupingRule);
};

}  // namespace cssom
}  // namespace cobalt

#endif  // CSSOM_CSS_GROUPING_RULE_H_
