// EXECUE+ Syntax Sheet - Full Definition in C++
// Language: EXECUE+
// Description: Full syntax sheet outlining the grammar, token rules, pattern recognition, and compiler interpretation

#include <string>
#include <vector>
#include <unordered_map>
#include <regex>

namespace EXECUE {

// ============================
// Syntax Token Categories
// ============================
enum class SyntaxTokenType {
    KEYWORD,
    IDENTIFIER,
    OPERATOR,
    LITERAL,
    SEPARATOR,
    DIRECTIVE,
    BLOCK,
    COMMENT,
    MACRO
};

// ============================
// Token Rule Definition
// ============================
struct SyntaxToken {
    SyntaxTokenType type;
    std::string pattern;
    bool caseSensitive;
};

// ============================
// Lexer Ruleset Definitions
// ============================
std::vector<SyntaxToken> getSyntaxRuleset() {
    return {
        { SyntaxTokenType::KEYWORD, R"(routine|opcode|loop|if|then|else|match|rule|start_cycle|flush|execute|bind_dominions|handle|error)"s, true },
        { SyntaxTokenType::IDENTIFIER, R"([a-zA-Z_][a-zA-Z0-9_]*)"s, true },
        { SyntaxTokenType::OPERATOR, R"(\+|\-|\*|\/|==|!=|<=|>=|<|>|=)"s, true },
        { SyntaxTokenType::LITERAL, R"(".*?"|\d+(\.\d+)?)"s, true },
        { SyntaxTokenType::SEPARATOR, R"(\{|\}|\(|\)|\[|\]|,|;)"s, true },
        { SyntaxTokenType::DIRECTIVE, R"(@[a-zA-Z_]+)"s, true },
        { SyntaxTokenType::BLOCK, R"(begin|end|loop|match)"s, true },
        { SyntaxTokenType::COMMENT, R"(\/\/.*?$|\/\*[\s\S]*?\*\/|#.*?$)"s, false },
        { SyntaxTokenType::MACRO, R"(\$[a-zA-Z_][a-zA-Z0-9_]*)"s, true }
    };
}

// ============================
// Expression Parsing Grammar
// ============================
struct SyntaxRule {
    std::string name;
    std::string pattern;
    bool greedy;
};

std::vector<SyntaxRule> getGrammarRules() {
    return {
        { "statement", R"((keyword)\s+(identifier)(\s*=\s*(expression))?)", true },
        { "loop", R"(loop\s+(identifier)?\s*\{[^\}]*\})", true },
        { "opcode", R"(opcode\s+(identifier)\s*\{[^\}]*\})", true },
        { "conditional", R"(if\s+[^\{]+\{[^\}]*\})", true },
        { "rule", R"(rule\s+(identifier)\s*\{[^\}]*\})", true },
        { "literal", R"(\"[^\"]*\"|\d+(\.\d+)?)", true },
        { "binding", R"(bind_dominions\(\))", false },
        { "cycle", R"(start_cycle\(\))", false },
        { "flush", R"(flush\(\))", false }
    };
}

// ============================
// Compiler Directive Interpreter
// ============================
struct DirectiveHandler {
    std::string name;
    std::function<void(std::string)> executor;
};

// Example Handler (Can Expand Later)
void handleDirective(const std::string& directive) {
    if (directive == "@inline") {
        // Apply inline execution optimization
    }
    else if (directive == "@fold") {
        // Fold redundant expressions
    }
    else if (directive == "@hotroute") {
        // Mark this code path as hot
    }
}

// ============================
// Reserved Keywords
// ============================
const std::vector<std::string> reservedKeywords = {
    "routine", "opcode", "loop", "if", "then", "else", "match", "rule",
    "start_cycle", "flush", "execute", "bind_dominions", "handle", "error"
};

// ============================
// Execution Entry Points
// ============================
const std::string mainRoutinePattern = R"(routine\s+main\s*\{[^\}]*\})";
const std::string opcodeDefinitionPattern = R"(opcode\s+[a-zA-Z_]+\s*\{[^\}]*\})";

// ============================
// Utility: Validate Identifier
// ============================
bool isValidIdentifier(const std::string& id) {
    return std::regex_match(id, std::regex(R"([a-zA-Z_][a-zA-Z0-9_]*)"));
}

// ============================
// Blueprint for Syntax Highlighting
// ============================
struct SyntaxHighlightStyle {
    SyntaxTokenType type;
    std::string colorHex;
    bool bold;
};

const std::vector<SyntaxHighlightStyle> highlightMap = {
    { SyntaxTokenType::KEYWORD, "#FF00AA", true },
    { SyntaxTokenType::IDENTIFIER, "#33CCFF", false },
    { SyntaxTokenType::LITERAL, "#FFDD55", false },
    { SyntaxTokenType::OPERATOR, "#AAAAAA", false },
    { SyntaxTokenType::SEPARATOR, "#888888", false },
    { SyntaxTokenType::DIRECTIVE, "#CC33FF", true },
    { SyntaxTokenType::COMMENT, "#666666", false },
    { SyntaxTokenType::MACRO, "#00FF99", true }
};

} // namespace EXECUE
