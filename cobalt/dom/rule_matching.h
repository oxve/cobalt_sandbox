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

#ifndef DOM_RULE_MATCHING_H_
#define DOM_RULE_MATCHING_H_

#include "base/memory/ref_counted.h"
#include "cobalt/cssom/cascade_priority.h"
#include "cobalt/cssom/css_style_declaration_data.h"
#include "cobalt/cssom/css_style_rule.h"
#include "cobalt/cssom/css_style_sheet.h"
#include "cobalt/cssom/selector_tree.h"
#include "cobalt/cssom/style_sheet_list.h"

namespace cobalt {
namespace dom {

class Element;
class HTMLElement;

// Returns whether a rule matches an element.
bool MatchRuleAndElement(cssom::CSSStyleRule* rule, Element* element);

// Evaluates the @media rules for the given width and height of the viewport.
void EvaluateStyleSheetMediaRules(
    const scoped_refptr<cssom::CSSStyleDeclarationData>& root_computed_style,
    const scoped_refptr<cssom::CSSStyleSheet>& user_agent_style_sheet,
    const scoped_refptr<cssom::StyleSheetList>& author_style_sheets);

// Updates the rule indexes used in UpdateMatchingRulesFromStyleSheet().
void UpdateStyleSheetRuleIndexes(
    const scoped_refptr<cssom::CSSStyleSheet>& user_agent_style_sheet,
    const scoped_refptr<cssom::StyleSheetList>& author_style_sheets);

// Scans one style sheet for rules that match the given HTMLElement and appends
// the matching rules to the element's |matching_rules_| field.
// UpdateStyleSheetRuleIndexes() must be called before this function.
void UpdateMatchingRulesFromStyleSheet(
    const scoped_refptr<cssom::CSSStyleSheet>& style_sheet,
    HTMLElement* element, cssom::Origin origin);

void UpdateMatchingRulesUsingSelectorTree(HTMLElement* dom_root,
                                          cssom::SelectorTree* selector_tree);

}  // namespace dom
}  // namespace cobalt

#endif  // DOM_RULE_MATCHING_H_
