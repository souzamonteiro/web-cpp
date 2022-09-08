// This file was generated on Thu Sep 8, 2022 17:22 (UTC-03) by REx v5.55 which is Copyright (c) 1979-2022 by Gunther Rademacher <grd@gmx.net>
// REx command line: WebCpp.ebnf -tree -main -ll 3 -backtrack -ll 2 -cpp

#include <string.h>
#include <stdlib.h>
#ifdef _WIN32
  #include <fcntl.h>
  #include <io.h>
#endif
#include <vector>
#include <map>
#include <stdio.h>
#include <string>
#include <algorithm>

class WebCpp
{
public:
  class EventHandler;

  WebCpp(const wchar_t *string, EventHandler *t)
  {
    initialize(string, t);
  }

  virtual ~WebCpp()
  {
  }

  class EventHandler
  {
  public:
    virtual ~EventHandler() {}

    virtual void reset(const wchar_t *string) = 0;
    virtual void startNonterminal(const wchar_t *name, int begin) = 0;
    virtual void endNonterminal(const wchar_t *name, int end) = 0;
    virtual void terminal(const wchar_t *name, int begin, int end) = 0;
    virtual void whitespace(int begin, int end) = 0;
  };

  class XmlSerializer : public EventHandler
  {
  public:
    XmlSerializer(bool indent)
    : input(0)
    , delayedTag(0)
    , indent(indent)
    , hasChildElement(false)
    , depth(0)
    {
    }

    void reset(const wchar_t *input)
    {
      fputs("<?xml version=\"1.0\" encoding=\"UTF-8\"?>", stdout);

      this->input = input;
      delayedTag = 0;
      hasChildElement = false;
      depth = 0;
    }

    void startNonterminal(const wchar_t *tag, int b)
    {
      if (delayedTag != 0)
      {
        fputc('<', stdout);
        fputs(Utf8Encoder::encode(delayedTag).c_str(), stdout);
        fputc('>', stdout);
      }
      delayedTag = tag;
      if (indent)
      {
        fputc('\n', stdout);
        for (int i = 0; i < depth; ++i)
        {
          fputs("  ", stdout);
        }
      }
      hasChildElement = false;
      ++depth;
    }

    void endNonterminal(const wchar_t *tag, int e)
    {
      --depth;
      if (delayedTag != 0)
      {
        delayedTag = 0;
        fputc('<', stdout);
        fputs(Utf8Encoder::encode(tag).c_str(), stdout);
        fputs("/>", stdout);
      }
      else
      {
        if (indent)
        {
          if (hasChildElement)
          {
            fputc('\n', stdout);
            for (int i = 0; i < depth; ++i)
            {
              fputs("  ", stdout);
            }
          }
        }
        fputs("</", stdout);
        fputs(Utf8Encoder::encode(tag).c_str(), stdout);
        fputc('>', stdout);
      }
      hasChildElement = true;
    }

    void whitespace(int b, int e)
    {
      characters(b, e);
    }

    void characters(int b, int e)
    {
      if (b < e)
      {
        if (delayedTag != 0)
        {
          fputc('<', stdout);
          fputs(Utf8Encoder::encode(delayedTag).c_str(), stdout);
          fputc('>', stdout);
          delayedTag = 0;
        }
        std::string encoded = Utf8Encoder::encode(input + b, e - b);
        int size = encoded.size();
        for (int i = 0; i < size; ++i)
        {
          char c = encoded[i];
          switch (c)
          {
          case 0: break;
          case L'&': fputs("&amp;", stdout); break;
          case L'<': fputs("&lt;", stdout); break;
          case L'>': fputs("&gt;", stdout); break;
          default: fputc(c, stdout);
          }
        }
      }
    }

    void terminal(const wchar_t *tag, int b, int e)
    {
      if (tag[0] == L'\'') tag = L"TOKEN";
      startNonterminal(tag, b);
      characters(b, e);
      endNonterminal(tag, e);
    }

  private:
    const wchar_t *input;
    const wchar_t *delayedTag;
    bool indent;
    bool hasChildElement;
    int depth;
  };

  class Symbol
  {
  public:
    virtual ~Symbol() {}

    const wchar_t *name;
    int begin;
    int end;

    virtual void send(EventHandler *e) = 0;

  protected:
    Symbol(const wchar_t *name, int begin, int end)
    {
      this->name = name;
      this->begin = begin;
      this->end = end;
    }
  };

  class Terminal : public Symbol
  {
  public:
    Terminal(const wchar_t *name, int begin, int end)
    : Symbol(name, begin, end)
    {}

    void send(EventHandler *e)
    {
      e->terminal(name, begin, end);
    }
  };

  class Nonterminal : public Symbol
  {
  public:
    std::vector<Symbol *> *children;

    Nonterminal(const wchar_t *name, int begin, int end, std::vector<Symbol *> *children)
    : Symbol(name, begin, end)
    {
      this->children = children;
    }

    ~Nonterminal()
    {
      for (std::vector<Symbol *>::iterator child = children->begin(); child != children->end(); ++child)
        delete *child;
      delete children;
    }

    void send(EventHandler *e)
    {
      e->startNonterminal(name, begin);
      int pos = begin;
      for (std::vector<Symbol *>::iterator i = children->begin(); i != children->end(); ++i)
      {
        Symbol *c = *i;
        if (pos < c->begin) e->whitespace(pos, c->begin);
        c->send(e);
        pos = c->end;
      }
      if (pos < end) e->whitespace(pos, end);
      e->endNonterminal(name, end);
    }
  };

  class TopDownTreeBuilder : public EventHandler
  {
  public:
    TopDownTreeBuilder()
    {
      input = 0;
      stack.clear();
      top = -1;
    }

    void reset(const wchar_t *input)
    {
      this->input = input;
      top = -1;
    }

    void startNonterminal(const wchar_t *name, int begin)
    {
      Nonterminal *nonterminal = new Nonterminal(name, begin, begin, new std::vector<Symbol *>());
      if (top++ >= 0) addChild(nonterminal);
      if ((size_t) top >= stack.size())
        stack.resize(stack.size() == 0 ? 64 : stack.size() << 1);
      stack[top] = nonterminal;
    }

    void endNonterminal(const wchar_t *name, int end)
    {
      stack[top]->end = end;
      if (top > 0) --top;
    }

    void terminal(const wchar_t *name, int begin, int end)
    {
      addChild(new Terminal(name, begin, end));
    }

    void whitespace(int begin, int end)
    {
    }

    void serialize(EventHandler *e)
    {
      e->reset(input);
      stack[0]->send(e);
    }

  private:
    void addChild(Symbol *s)
    {
      Nonterminal *current = stack[top];
      current->children->push_back(s);
    }

    const wchar_t *input;
    std::vector<Nonterminal *> stack;
    int top;
  };

  static int main(int argc, char **argv)
  {
    int returnCode = 0;

    if (argc < 2)
    {
      fprintf(stderr, "Usage: %s [-i] INPUT...\n", argv[0]);
      fprintf(stderr, "\n");
      fprintf(stderr, "  parse INPUT, which is either a filename or literal text enclosed in curly braces\n");
      fprintf(stderr, "\n");
      fprintf(stderr, "  Option:\n");
      fprintf(stderr, "    -i     indented parse tree\n");
    }
    else
    {
#ifdef _WIN32
      _setmode(1, O_BINARY);
#endif

      bool indent = false;
      for (int i = 1; i < argc; ++i)
      {
        if (strcmp(argv[i], "-i") == 0)
        {
          indent = true;
          continue;
        }
        try
        {
          XmlSerializer s(indent);
          std::wstring input = read(argv[i]);
          WebCpp parser(input.c_str(), &s);
          try
          {
            parser.parse_webcpp();
          }
          catch (ParseException &pe)
          {
            fprintf(stderr, "\n");
            fprintf(stderr, "%s\n", Utf8Encoder::encode(parser.getErrorMessage(pe).c_str()).c_str());
            returnCode = 1;
            break;
          }
        }
        catch (FileNotFound &fnf)
        {
          fprintf(stderr, "error: file not found: %s\n", fnf.getFilename().c_str());
          returnCode = 1;
          break;
        }
        catch (MalformedInputException &mie)
        {
          fprintf(stderr, "error: UTF-8 decoding error in %s at offset %d\n",
            argv[i], static_cast<int>(mie.getOffset()));
          returnCode = 1;
          break;
        }
      }
    }
    return returnCode;
  }

  class ParseException
  {
  private:
    int begin, end, offending, expected, state;
    friend class WebCpp;

  protected:
    ParseException(int b, int e, int s, int o, int x)
    : begin(b), end(e), offending(o), expected(x), state(s)
    {
    }

  public:
    const wchar_t *getMessage() const
    {
      return offending < 0
           ? L"lexical analysis failed"
           : L"syntax error";
    }

    int getBegin() const {return begin;}
    int getEnd() const {return end;}
    int getState() const {return state;}
    int getOffending() const {return offending;}
    int getExpected() const {return expected;}
  };

  void initialize(const wchar_t *source, EventHandler *parsingEventHandler)
  {
    eventHandler = parsingEventHandler;
    input = source;
    reset(0, 0, 0);
  }

  const wchar_t *getInput() const
  {
    return input;
  }

  int getTokenOffset() const
  {
    return b0;
  }

  int getTokenEnd() const
  {
    return e0;
  }

  void reset(int l, int b, int e)
  {
            b0 = b; e0 = b;
    l1 = l; b1 = b; e1 = e;
    l2 = 0; b2 = 0; e2 = 0;
    end = e;
    ex = -1;
    memo.clear();
    eventHandler->reset(input);
  }

  void reset()
  {
    reset(0, 0, 0);
  }

  static const wchar_t *getOffendingToken(ParseException e)
  {
    return e.getOffending() < 0 ? 0 : TOKEN[e.getOffending()];
  }

  static void getExpectedTokenSet(const ParseException &e, const wchar_t **set, int size)
  {
    if (e.expected < 0)
    {
      getTokenSet(- e.state, set, size);
    }
    else if (size == 1)
    {
      set[0] = 0;
    }
    else if (size > 1)
    {
      set[0] = TOKEN[e.expected];
      set[1] = 0;
    }
  }

  static std::wstring to_wstring(int i)
  {
    const wchar_t *sign = i < 0 ? L"-" : L"";
    std::wstring a;
    do
    {
      a += L'0' + abs(i % 10);
      i /= 10;
    }
    while (i != 0);
    a += sign;
    std::reverse(a.begin(), a.end());
    return a;
  }

  std::wstring getErrorMessage(const ParseException &e)
  {
    std::wstring message(e.getMessage());
    const wchar_t *found = getOffendingToken(e);
    if (found != 0)
    {
      message += L", found ";
      message += found;
    }
    const wchar_t *expected[64];
    getExpectedTokenSet(e, expected, sizeof expected / sizeof *expected);
    message += L"\nwhile expecting ";
    const wchar_t *delimiter(expected[1] ? L"[" : L"");
    for (const wchar_t **x = expected; *x; ++x)
    {
      message += delimiter;
      message += *x;
      delimiter = L", ";
    }
    message += expected[1] ? L"]\n" : L"\n";
    int size = e.getEnd() - e.getBegin();
    if (size != 0 && found == 0)
    {
      message += L"after successfully scanning ";
      message += to_wstring(size);
      message += L" characters beginning ";
    }
    int line = 1;
    int column = 1;
    for (int i = 0; i < e.getBegin(); ++i)
    {
      if (input[i] == L'\n')
      {
        ++line;
        column = 1;
      }
      else
      {
        ++column;
      }
    }
    message += L"at line ";
    message += to_wstring(line);
    message += L", column ";
    message += to_wstring(column);
    message += L":\n...";
    const wchar_t *w = input + e.getBegin();
    for (int i = 0; i < 64 && *w; ++i)
    {
      message += *w++;
    }
    message += L"...";
    return message;
  }

  void parse_webcpp()
  {
    eventHandler->startNonterminal(L"webcpp", e0);
    lookahead1(47);                 // END | eof | identifier | null | true | false | string | complex | real |
                                    // comment | '!' | '#' | '(' | '*' | '++' | '--' | ';' | '[' | 'break' | 'char' |
                                    // 'class' | 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' |
                                    // 'long' | 'namespace' | 'private' | 'protected' | 'public' | 'return' |
                                    // 'template' | 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '~'
    switch (l1)
    {
    case 2:                         // eof
      consume(2);                   // eof
      break;
    default:
      for (;;)
      {
        lookahead1(43);             // END | identifier | null | true | false | string | complex | real | comment |
                                    // '!' | '#' | '(' | '*' | '++' | '--' | ';' | '[' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '~'
        if (l1 == 1)                // END
        {
          break;
        }
        parse_expression();
      }
      break;
    }
    eventHandler->endNonterminal(L"webcpp", e0);
  }

private:

  void parse_operation()
  {
    eventHandler->startNonterminal(L"operation", e0);
    parse_variableAssignment();
    eventHandler->endNonterminal(L"operation", e0);
  }

  void try_operation()
  {
    try_variableAssignment();
  }

  void parse_variableAssignment()
  {
    eventHandler->startNonterminal(L"variableAssignment", e0);
    parse_logicalORExpression();
    for (;;)
    {
      if (l1 != 41)                 // '='
      {
        break;
      }
      consume(41);                  // '='
      parse_logicalORExpression();
    }
    eventHandler->endNonterminal(L"variableAssignment", e0);
  }

  void try_variableAssignment()
  {
    try_logicalORExpression();
    for (;;)
    {
      if (l1 != 41)                 // '='
      {
        break;
      }
      consumeT(41);                 // '='
      try_logicalORExpression();
    }
  }

  void parse_logicalORExpression()
  {
    eventHandler->startNonterminal(L"logicalORExpression", e0);
    parse_logicalANDExpression();
    for (;;)
    {
      if (l1 != 87)                 // '||'
      {
        break;
      }
      consume(87);                  // '||'
      parse_logicalANDExpression();
    }
    eventHandler->endNonterminal(L"logicalORExpression", e0);
  }

  void try_logicalORExpression()
  {
    try_logicalANDExpression();
    for (;;)
    {
      if (l1 != 87)                 // '||'
      {
        break;
      }
      consumeT(87);                 // '||'
      try_logicalANDExpression();
    }
  }

  void parse_logicalANDExpression()
  {
    eventHandler->startNonterminal(L"logicalANDExpression", e0);
    parse_bitwiseORExpression();
    for (;;)
    {
      if (l1 != 19)                 // '&&'
      {
        break;
      }
      consume(19);                  // '&&'
      parse_bitwiseORExpression();
    }
    eventHandler->endNonterminal(L"logicalANDExpression", e0);
  }

  void try_logicalANDExpression()
  {
    try_bitwiseORExpression();
    for (;;)
    {
      if (l1 != 19)                 // '&&'
      {
        break;
      }
      consumeT(19);                 // '&&'
      try_bitwiseORExpression();
    }
  }

  void parse_bitwiseORExpression()
  {
    eventHandler->startNonterminal(L"bitwiseORExpression", e0);
    parse_bitwiseXORExpression();
    for (;;)
    {
      if (l1 != 86)                 // '|'
      {
        break;
      }
      consume(86);                  // '|'
      parse_bitwiseXORExpression();
    }
    eventHandler->endNonterminal(L"bitwiseORExpression", e0);
  }

  void try_bitwiseORExpression()
  {
    try_bitwiseXORExpression();
    for (;;)
    {
      if (l1 != 86)                 // '|'
      {
        break;
      }
      consumeT(86);                 // '|'
      try_bitwiseXORExpression();
    }
  }

  void parse_bitwiseXORExpression()
  {
    eventHandler->startNonterminal(L"bitwiseXORExpression", e0);
    parse_bitwiseANDExpression();
    for (;;)
    {
      if (l1 != 49)                 // '^'
      {
        break;
      }
      consume(49);                  // '^'
      parse_bitwiseANDExpression();
    }
    eventHandler->endNonterminal(L"bitwiseXORExpression", e0);
  }

  void try_bitwiseXORExpression()
  {
    try_bitwiseANDExpression();
    for (;;)
    {
      if (l1 != 49)                 // '^'
      {
        break;
      }
      consumeT(49);                 // '^'
      try_bitwiseANDExpression();
    }
  }

  void parse_bitwiseANDExpression()
  {
    eventHandler->startNonterminal(L"bitwiseANDExpression", e0);
    parse_equalityExpression();
    for (;;)
    {
      if (l1 != 18)                 // '&'
      {
        break;
      }
      consume(18);                  // '&'
      parse_equalityExpression();
    }
    eventHandler->endNonterminal(L"bitwiseANDExpression", e0);
  }

  void try_bitwiseANDExpression()
  {
    try_equalityExpression();
    for (;;)
    {
      if (l1 != 18)                 // '&'
      {
        break;
      }
      consumeT(18);                 // '&'
      try_equalityExpression();
    }
  }

  void parse_equalityExpression()
  {
    eventHandler->startNonterminal(L"equalityExpression", e0);
    parse_relationalExpression();
    for (;;)
    {
      if (l1 != 14                  // '!='
       && l1 != 42)                 // '=='
      {
        break;
      }
      switch (l1)
      {
      case 42:                      // '=='
        consume(42);                // '=='
        break;
      default:
        consume(14);                // '!='
        break;
      }
      parse_relationalExpression();
    }
    eventHandler->endNonterminal(L"equalityExpression", e0);
  }

  void try_equalityExpression()
  {
    try_relationalExpression();
    for (;;)
    {
      if (l1 != 14                  // '!='
       && l1 != 42)                 // '=='
      {
        break;
      }
      switch (l1)
      {
      case 42:                      // '=='
        consumeT(42);               // '=='
        break;
      default:
        consumeT(14);               // '!='
        break;
      }
      try_relationalExpression();
    }
  }

  void parse_relationalExpression()
  {
    eventHandler->startNonterminal(L"relationalExpression", e0);
    parse_shiftExpression();
    for (;;)
    {
      if (l1 != 37                  // '<'
       && l1 != 40                  // '<='
       && l1 != 43                  // '>'
       && l1 != 44)                 // '>='
      {
        break;
      }
      switch (l1)
      {
      case 37:                      // '<'
        consume(37);                // '<'
        break;
      case 43:                      // '>'
        consume(43);                // '>'
        break;
      case 40:                      // '<='
        consume(40);                // '<='
        break;
      default:
        consume(44);                // '>='
        break;
      }
      parse_shiftExpression();
    }
    eventHandler->endNonterminal(L"relationalExpression", e0);
  }

  void try_relationalExpression()
  {
    try_shiftExpression();
    for (;;)
    {
      if (l1 != 37                  // '<'
       && l1 != 40                  // '<='
       && l1 != 43                  // '>'
       && l1 != 44)                 // '>='
      {
        break;
      }
      switch (l1)
      {
      case 37:                      // '<'
        consumeT(37);               // '<'
        break;
      case 43:                      // '>'
        consumeT(43);               // '>'
        break;
      case 40:                      // '<='
        consumeT(40);               // '<='
        break;
      default:
        consumeT(44);               // '>='
        break;
      }
      try_shiftExpression();
    }
  }

  void parse_shiftExpression()
  {
    eventHandler->startNonterminal(L"shiftExpression", e0);
    parse_additiveExpression();
    for (;;)
    {
      if (l1 != 38                  // '<<'
       && l1 != 39                  // '<<='
       && l1 != 45                  // '>>'
       && l1 != 46)                 // '>>='
      {
        break;
      }
      switch (l1)
      {
      case 38:                      // '<<'
        consume(38);                // '<<'
        break;
      case 45:                      // '>>'
        consume(45);                // '>>'
        break;
      case 39:                      // '<<='
        consume(39);                // '<<='
        break;
      default:
        consume(46);                // '>>='
        break;
      }
      parse_additiveExpression();
    }
    eventHandler->endNonterminal(L"shiftExpression", e0);
  }

  void try_shiftExpression()
  {
    try_additiveExpression();
    for (;;)
    {
      if (l1 != 38                  // '<<'
       && l1 != 39                  // '<<='
       && l1 != 45                  // '>>'
       && l1 != 46)                 // '>>='
      {
        break;
      }
      switch (l1)
      {
      case 38:                      // '<<'
        consumeT(38);               // '<<'
        break;
      case 45:                      // '>>'
        consumeT(45);               // '>>'
        break;
      case 39:                      // '<<='
        consumeT(39);               // '<<='
        break;
      default:
        consumeT(46);               // '>>='
        break;
      }
      try_additiveExpression();
    }
  }

  void parse_additiveExpression()
  {
    eventHandler->startNonterminal(L"additiveExpression", e0);
    parse_multiplicativeExpression();
    for (;;)
    {
      if (l1 != 24                  // '+'
       && l1 != 26                  // '+='
       && l1 != 28                  // '-'
       && l1 != 30)                 // '-='
      {
        break;
      }
      switch (l1)
      {
      case 24:                      // '+'
        consume(24);                // '+'
        break;
      case 28:                      // '-'
        consume(28);                // '-'
        break;
      case 26:                      // '+='
        consume(26);                // '+='
        break;
      default:
        consume(30);                // '-='
        break;
      }
      parse_multiplicativeExpression();
    }
    eventHandler->endNonterminal(L"additiveExpression", e0);
  }

  void try_additiveExpression()
  {
    try_multiplicativeExpression();
    for (;;)
    {
      if (l1 != 24                  // '+'
       && l1 != 26                  // '+='
       && l1 != 28                  // '-'
       && l1 != 30)                 // '-='
      {
        break;
      }
      switch (l1)
      {
      case 24:                      // '+'
        consumeT(24);               // '+'
        break;
      case 28:                      // '-'
        consumeT(28);               // '-'
        break;
      case 26:                      // '+='
        consumeT(26);               // '+='
        break;
      default:
        consumeT(30);               // '-='
        break;
      }
      try_multiplicativeExpression();
    }
  }

  void parse_multiplicativeExpression()
  {
    eventHandler->startNonterminal(L"multiplicativeExpression", e0);
    parse_unaryExpression();
    for (;;)
    {
      lookahead1(53);               // END | identifier | null | true | false | string | complex | real | comment |
                                    // lineTerminator | '!' | '!=' | '#' | '%' | '%=' | '&' | '&&' | '(' | ')' | '*' |
                                    // '*=' | '+' | '++' | '+=' | ',' | '-' | '--' | '-=' | '/' | '/=' | ';' | '<' |
                                    // '<<' | '<<=' | '<=' | '=' | '==' | '>' | '>=' | '>>' | '>>=' | '[' | ']' | '^' |
                                    // 'break' | 'char' | 'class' | 'continue' | 'do' | 'double' | 'float' | 'for' |
                                    // 'if' | 'int' | 'long' | 'namespace' | 'private' | 'protected' | 'public' |
                                    // 'return' | 'template' | 'throw' | 'try' | 'using' | 'void' | 'while' | '{' |
                                    // '|' | '||' | '}' | '~'
      switch (l1)
      {
      case 22:                      // '*'
        lookahead2(39);             // identifier | null | true | false | string | complex | real | '!' | '(' | '*' |
                                    // '++' | '--' | '[' | '{' | '~'
        break;
      default:
        lk = l1;
        break;
      }
      if (lk == 406                 // '*' identifier
       || lk == 534                 // '*' null
       || lk == 662                 // '*' true
       || lk == 790                 // '*' false
       || lk == 918                 // '*' string
       || lk == 1174                // '*' complex
       || lk == 1302                // '*' real
       || lk == 2582                // '*' '('
       || lk == 6038                // '*' '['
       || lk == 10902)              // '*' '{'
      {
        lk = memoized(0, e0);
        if (lk == 0)
        {
          int b0A = b0; int e0A = e0; int l1A = l1;
          int b1A = b1; int e1A = e1; int l2A = l2;
          int b2A = b2; int e2A = e2;
          try
          {
            switch (l1)
            {
            case 22:                // '*'
              consumeT(22);         // '*'
              break;
            case 33:                // '/'
              consumeT(33);         // '/'
              break;
            case 16:                // '%'
              consumeT(16);         // '%'
              break;
            case 23:                // '*='
              consumeT(23);         // '*='
              break;
            case 34:                // '/='
              consumeT(34);         // '/='
              break;
            default:
              consumeT(17);         // '%='
              break;
            }
            try_unaryExpression();
            lk = -1;
          }
          catch (ParseException &)
          {
            lk = -2;
          }
          b0 = b0A; e0 = e0A; l1 = l1A; if (l1 == 0) {end = e0A;} else {
          b1 = b1A; e1 = e1A; l2 = l2A; if (l2 == 0) {end = e1A;} else {
          b2 = b2A; e2 = e2A; end = e2A; }}
          memoize(0, e0, lk);
        }
      }
      if (lk != -1
       && lk != 16                  // '%'
       && lk != 17                  // '%='
       && lk != 23                  // '*='
       && lk != 33                  // '/'
       && lk != 34                  // '/='
       && lk != 1686                // '*' '!'
       && lk != 2838                // '*' '*'
       && lk != 3222                // '*' '++'
       && lk != 3734                // '*' '--'
       && lk != 11414)              // '*' '~'
      {
        break;
      }
      switch (l1)
      {
      case 22:                      // '*'
        consume(22);                // '*'
        break;
      case 33:                      // '/'
        consume(33);                // '/'
        break;
      case 16:                      // '%'
        consume(16);                // '%'
        break;
      case 23:                      // '*='
        consume(23);                // '*='
        break;
      case 34:                      // '/='
        consume(34);                // '/='
        break;
      default:
        consume(17);                // '%='
        break;
      }
      parse_unaryExpression();
    }
    eventHandler->endNonterminal(L"multiplicativeExpression", e0);
  }

  void try_multiplicativeExpression()
  {
    try_unaryExpression();
    for (;;)
    {
      lookahead1(53);               // END | identifier | null | true | false | string | complex | real | comment |
                                    // lineTerminator | '!' | '!=' | '#' | '%' | '%=' | '&' | '&&' | '(' | ')' | '*' |
                                    // '*=' | '+' | '++' | '+=' | ',' | '-' | '--' | '-=' | '/' | '/=' | ';' | '<' |
                                    // '<<' | '<<=' | '<=' | '=' | '==' | '>' | '>=' | '>>' | '>>=' | '[' | ']' | '^' |
                                    // 'break' | 'char' | 'class' | 'continue' | 'do' | 'double' | 'float' | 'for' |
                                    // 'if' | 'int' | 'long' | 'namespace' | 'private' | 'protected' | 'public' |
                                    // 'return' | 'template' | 'throw' | 'try' | 'using' | 'void' | 'while' | '{' |
                                    // '|' | '||' | '}' | '~'
      switch (l1)
      {
      case 22:                      // '*'
        lookahead2(39);             // identifier | null | true | false | string | complex | real | '!' | '(' | '*' |
                                    // '++' | '--' | '[' | '{' | '~'
        break;
      default:
        lk = l1;
        break;
      }
      if (lk == 406                 // '*' identifier
       || lk == 534                 // '*' null
       || lk == 662                 // '*' true
       || lk == 790                 // '*' false
       || lk == 918                 // '*' string
       || lk == 1174                // '*' complex
       || lk == 1302                // '*' real
       || lk == 2582                // '*' '('
       || lk == 6038                // '*' '['
       || lk == 10902)              // '*' '{'
      {
        lk = memoized(0, e0);
        if (lk == 0)
        {
          int b0A = b0; int e0A = e0; int l1A = l1;
          int b1A = b1; int e1A = e1; int l2A = l2;
          int b2A = b2; int e2A = e2;
          try
          {
            switch (l1)
            {
            case 22:                // '*'
              consumeT(22);         // '*'
              break;
            case 33:                // '/'
              consumeT(33);         // '/'
              break;
            case 16:                // '%'
              consumeT(16);         // '%'
              break;
            case 23:                // '*='
              consumeT(23);         // '*='
              break;
            case 34:                // '/='
              consumeT(34);         // '/='
              break;
            default:
              consumeT(17);         // '%='
              break;
            }
            try_unaryExpression();
            memoize(0, e0A, -1);
            continue;
          }
          catch (ParseException &)
          {
            b0 = b0A; e0 = e0A; l1 = l1A; if (l1 == 0) {end = e0A;} else {
            b1 = b1A; e1 = e1A; l2 = l2A; if (l2 == 0) {end = e1A;} else {
            b2 = b2A; e2 = e2A; end = e2A; }}
            memoize(0, e0A, -2);
            break;
          }
        }
      }
      if (lk != -1
       && lk != 16                  // '%'
       && lk != 17                  // '%='
       && lk != 23                  // '*='
       && lk != 33                  // '/'
       && lk != 34                  // '/='
       && lk != 1686                // '*' '!'
       && lk != 2838                // '*' '*'
       && lk != 3222                // '*' '++'
       && lk != 3734                // '*' '--'
       && lk != 11414)              // '*' '~'
      {
        break;
      }
      switch (l1)
      {
      case 22:                      // '*'
        consumeT(22);               // '*'
        break;
      case 33:                      // '/'
        consumeT(33);               // '/'
        break;
      case 16:                      // '%'
        consumeT(16);               // '%'
        break;
      case 23:                      // '*='
        consumeT(23);               // '*='
        break;
      case 34:                      // '/='
        consumeT(34);               // '/='
        break;
      default:
        consumeT(17);               // '%='
        break;
      }
      try_unaryExpression();
    }
  }

  void parse_unaryExpression()
  {
    eventHandler->startNonterminal(L"unaryExpression", e0);
    lookahead1(39);                 // identifier | null | true | false | string | complex | real | '!' | '(' | '*' |
                                    // '++' | '--' | '[' | '{' | '~'
    switch (l1)
    {
    case 3:                         // identifier
      lookahead2(56);               // END | identifier | null | true | false | string | complex | real | comment |
                                    // lineTerminator | '!' | '!=' | '#' | '%' | '%=' | '&' | '&&' | '(' | ')' | '*' |
                                    // '*=' | '+' | '++' | '+=' | ',' | '-' | '--' | '-=' | '->' | '.' | '/' | '/=' |
                                    // ';' | '<' | '<<' | '<<=' | '<=' | '=' | '==' | '>' | '>=' | '>>' | '>>=' | '[' |
                                    // ']' | '^' | 'break' | 'char' | 'class' | 'continue' | 'do' | 'double' | 'float' |
                                    // 'for' | 'if' | 'int' | 'long' | 'namespace' | 'private' | 'protected' |
                                    // 'public' | 'return' | 'template' | 'throw' | 'try' | 'using' | 'void' | 'while' |
                                    // '{' | '|' | '||' | '}' | '~'
      break;
    case 47:                        // '['
      lookahead2(45);               // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | '*' | '++' | '--' | ';' | '[' | ']' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '~'
      break;
    case 20:                        // '('
    case 85:                        // '{'
      lookahead2(42);               // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | '*' | '++' | '--' | ';' | '[' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '~'
      break;
    case 4:                         // null
    case 5:                         // true
    case 6:                         // false
    case 7:                         // string
    case 9:                         // complex
    case 10:                        // real
      lookahead2(53);               // END | identifier | null | true | false | string | complex | real | comment |
                                    // lineTerminator | '!' | '!=' | '#' | '%' | '%=' | '&' | '&&' | '(' | ')' | '*' |
                                    // '*=' | '+' | '++' | '+=' | ',' | '-' | '--' | '-=' | '/' | '/=' | ';' | '<' |
                                    // '<<' | '<<=' | '<=' | '=' | '==' | '>' | '>=' | '>>' | '>>=' | '[' | ']' | '^' |
                                    // 'break' | 'char' | 'class' | 'continue' | 'do' | 'double' | 'float' | 'for' |
                                    // 'if' | 'int' | 'long' | 'namespace' | 'private' | 'protected' | 'public' |
                                    // 'return' | 'template' | 'throw' | 'try' | 'using' | 'void' | 'while' | '{' |
                                    // '|' | '||' | '}' | '~'
      break;
    default:
      lk = l1;
      break;
    }
    if (lk == 404                   // '(' identifier
     || lk == 431                   // '[' identifier
     || lk == 469                   // '{' identifier
     || lk == 532                   // '(' null
     || lk == 559                   // '[' null
     || lk == 597                   // '{' null
     || lk == 660                   // '(' true
     || lk == 687                   // '[' true
     || lk == 725                   // '{' true
     || lk == 788                   // '(' false
     || lk == 815                   // '[' false
     || lk == 853                   // '{' false
     || lk == 916                   // '(' string
     || lk == 943                   // '[' string
     || lk == 981                   // '{' string
     || lk == 1172                  // '(' complex
     || lk == 1199                  // '[' complex
     || lk == 1237                  // '{' complex
     || lk == 1300                  // '(' real
     || lk == 1327                  // '[' real
     || lk == 1365                  // '{' real
     || lk == 1428                  // '(' comment
     || lk == 1455                  // '[' comment
     || lk == 1493                  // '{' comment
     || lk == 1684                  // '(' '!'
     || lk == 1711                  // '[' '!'
     || lk == 1749                  // '{' '!'
     || lk == 1940                  // '(' '#'
     || lk == 1967                  // '[' '#'
     || lk == 2005                  // '{' '#'
     || lk == 2563                  // identifier '('
     || lk == 2580                  // '(' '('
     || lk == 2607                  // '[' '('
     || lk == 2645                  // '{' '('
     || lk == 2836                  // '(' '*'
     || lk == 2863                  // '[' '*'
     || lk == 2901                  // '{' '*'
     || lk == 3203                  // identifier '++'
     || lk == 3204                  // null '++'
     || lk == 3205                  // true '++'
     || lk == 3206                  // false '++'
     || lk == 3207                  // string '++'
     || lk == 3209                  // complex '++'
     || lk == 3210                  // real '++'
     || lk == 3220                  // '(' '++'
     || lk == 3247                  // '[' '++'
     || lk == 3285                  // '{' '++'
     || lk == 3715                  // identifier '--'
     || lk == 3716                  // null '--'
     || lk == 3717                  // true '--'
     || lk == 3718                  // false '--'
     || lk == 3719                  // string '--'
     || lk == 3721                  // complex '--'
     || lk == 3722                  // real '--'
     || lk == 3732                  // '(' '--'
     || lk == 3759                  // '[' '--'
     || lk == 3797                  // '{' '--'
     || lk == 3971                  // identifier '->'
     || lk == 4099                  // identifier '.'
     || lk == 4628                  // '(' ';'
     || lk == 4655                  // '[' ';'
     || lk == 4693                  // '{' ';'
     || lk == 6019                  // identifier '['
     || lk == 6036                  // '(' '['
     || lk == 6063                  // '[' '['
     || lk == 6101                  // '{' '['
     || lk == 6191                  // '[' ']'
     || lk == 6420                  // '(' 'break'
     || lk == 6447                  // '[' 'break'
     || lk == 6485                  // '{' 'break'
     || lk == 6676                  // '(' 'char'
     || lk == 6703                  // '[' 'char'
     || lk == 6741                  // '{' 'char'
     || lk == 6804                  // '(' 'class'
     || lk == 6831                  // '[' 'class'
     || lk == 6869                  // '{' 'class'
     || lk == 6932                  // '(' 'continue'
     || lk == 6959                  // '[' 'continue'
     || lk == 6997                  // '{' 'continue'
     || lk == 7188                  // '(' 'do'
     || lk == 7215                  // '[' 'do'
     || lk == 7253                  // '{' 'do'
     || lk == 7316                  // '(' 'double'
     || lk == 7343                  // '[' 'double'
     || lk == 7381                  // '{' 'double'
     || lk == 8084                  // '(' 'float'
     || lk == 8111                  // '[' 'float'
     || lk == 8149                  // '{' 'float'
     || lk == 8212                  // '(' 'for'
     || lk == 8239                  // '[' 'for'
     || lk == 8277                  // '{' 'for'
     || lk == 8340                  // '(' 'if'
     || lk == 8367                  // '[' 'if'
     || lk == 8405                  // '{' 'if'
     || lk == 8980                  // '(' 'int'
     || lk == 9007                  // '[' 'int'
     || lk == 9045                  // '{' 'int'
     || lk == 9108                  // '(' 'long'
     || lk == 9135                  // '[' 'long'
     || lk == 9173                  // '{' 'long'
     || lk == 9236                  // '(' 'namespace'
     || lk == 9263                  // '[' 'namespace'
     || lk == 9301                  // '{' 'namespace'
     || lk == 9364                  // '(' 'private'
     || lk == 9391                  // '[' 'private'
     || lk == 9429                  // '{' 'private'
     || lk == 9492                  // '(' 'protected'
     || lk == 9519                  // '[' 'protected'
     || lk == 9557                  // '{' 'protected'
     || lk == 9620                  // '(' 'public'
     || lk == 9647                  // '[' 'public'
     || lk == 9685                  // '{' 'public'
     || lk == 9748                  // '(' 'return'
     || lk == 9775                  // '[' 'return'
     || lk == 9813                  // '{' 'return'
     || lk == 9876                  // '(' 'template'
     || lk == 9903                  // '[' 'template'
     || lk == 9941                  // '{' 'template'
     || lk == 10004                 // '(' 'throw'
     || lk == 10031                 // '[' 'throw'
     || lk == 10069                 // '{' 'throw'
     || lk == 10132                 // '(' 'try'
     || lk == 10159                 // '[' 'try'
     || lk == 10197                 // '{' 'try'
     || lk == 10516                 // '(' 'using'
     || lk == 10543                 // '[' 'using'
     || lk == 10581                 // '{' 'using'
     || lk == 10644                 // '(' 'void'
     || lk == 10671                 // '[' 'void'
     || lk == 10709                 // '{' 'void'
     || lk == 10772                 // '(' 'while'
     || lk == 10799                 // '[' 'while'
     || lk == 10837                 // '{' 'while'
     || lk == 10900                 // '(' '{'
     || lk == 10927                 // '[' '{'
     || lk == 10965                 // '{' '{'
     || lk == 11412                 // '(' '~'
     || lk == 11439                 // '[' '~'
     || lk == 11477)                // '{' '~'
    {
      lk = memoized(1, e0);
      if (lk == 0)
      {
        int b0A = b0; int e0A = e0; int l1A = l1;
        int b1A = b1; int e1A = e1; int l2A = l2;
        int b2A = b2; int e2A = e2;
        try
        {
          try_primary();
          lookahead1(7);            // '++'
          consumeT(25);             // '++'
          lk = -6;
        }
        catch (ParseException &)
        {
          try
          {
            b0 = b0A; e0 = e0A; l1 = l1A; if (l1 == 0) {end = e0A;} else {
            b1 = b1A; e1 = e1A; l2 = l2A; if (l2 == 0) {end = e1A;} else {
            b2 = b2A; e2 = e2A; end = e2A; }}
            try_primary();
            lookahead1(8);          // '--'
            consumeT(29);           // '--'
            lk = -7;
          }
          catch (ParseException &)
          {
            lk = -8;
          }
        }
        b0 = b0A; e0 = e0A; l1 = l1A; if (l1 == 0) {end = e0A;} else {
        b1 = b1A; e1 = e1A; l2 = l2A; if (l2 == 0) {end = e1A;} else {
        b2 = b2A; e2 = e2A; end = e2A; }}
        memoize(1, e0, lk);
      }
    }
    switch (lk)
    {
    case 89:                        // '~'
      consume(89);                  // '~'
      parse_primary();
      break;
    case 13:                        // '!'
      consume(13);                  // '!'
      parse_primary();
      break;
    case 22:                        // '*'
      consume(22);                  // '*'
      parse_primary();
      break;
    case 25:                        // '++'
      consume(25);                  // '++'
      parse_primary();
      break;
    case 29:                        // '--'
      consume(29);                  // '--'
      parse_primary();
      break;
    case -6:
      parse_primary();
      lookahead1(7);                // '++'
      consume(25);                  // '++'
      break;
    case -7:
      parse_primary();
      lookahead1(8);                // '--'
      consume(29);                  // '--'
      break;
    default:
      parse_primary();
      break;
    }
    eventHandler->endNonterminal(L"unaryExpression", e0);
  }

  void try_unaryExpression()
  {
    lookahead1(39);                 // identifier | null | true | false | string | complex | real | '!' | '(' | '*' |
                                    // '++' | '--' | '[' | '{' | '~'
    switch (l1)
    {
    case 3:                         // identifier
      lookahead2(56);               // END | identifier | null | true | false | string | complex | real | comment |
                                    // lineTerminator | '!' | '!=' | '#' | '%' | '%=' | '&' | '&&' | '(' | ')' | '*' |
                                    // '*=' | '+' | '++' | '+=' | ',' | '-' | '--' | '-=' | '->' | '.' | '/' | '/=' |
                                    // ';' | '<' | '<<' | '<<=' | '<=' | '=' | '==' | '>' | '>=' | '>>' | '>>=' | '[' |
                                    // ']' | '^' | 'break' | 'char' | 'class' | 'continue' | 'do' | 'double' | 'float' |
                                    // 'for' | 'if' | 'int' | 'long' | 'namespace' | 'private' | 'protected' |
                                    // 'public' | 'return' | 'template' | 'throw' | 'try' | 'using' | 'void' | 'while' |
                                    // '{' | '|' | '||' | '}' | '~'
      break;
    case 47:                        // '['
      lookahead2(45);               // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | '*' | '++' | '--' | ';' | '[' | ']' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '~'
      break;
    case 20:                        // '('
    case 85:                        // '{'
      lookahead2(42);               // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | '*' | '++' | '--' | ';' | '[' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '~'
      break;
    case 4:                         // null
    case 5:                         // true
    case 6:                         // false
    case 7:                         // string
    case 9:                         // complex
    case 10:                        // real
      lookahead2(53);               // END | identifier | null | true | false | string | complex | real | comment |
                                    // lineTerminator | '!' | '!=' | '#' | '%' | '%=' | '&' | '&&' | '(' | ')' | '*' |
                                    // '*=' | '+' | '++' | '+=' | ',' | '-' | '--' | '-=' | '/' | '/=' | ';' | '<' |
                                    // '<<' | '<<=' | '<=' | '=' | '==' | '>' | '>=' | '>>' | '>>=' | '[' | ']' | '^' |
                                    // 'break' | 'char' | 'class' | 'continue' | 'do' | 'double' | 'float' | 'for' |
                                    // 'if' | 'int' | 'long' | 'namespace' | 'private' | 'protected' | 'public' |
                                    // 'return' | 'template' | 'throw' | 'try' | 'using' | 'void' | 'while' | '{' |
                                    // '|' | '||' | '}' | '~'
      break;
    default:
      lk = l1;
      break;
    }
    if (lk == 404                   // '(' identifier
     || lk == 431                   // '[' identifier
     || lk == 469                   // '{' identifier
     || lk == 532                   // '(' null
     || lk == 559                   // '[' null
     || lk == 597                   // '{' null
     || lk == 660                   // '(' true
     || lk == 687                   // '[' true
     || lk == 725                   // '{' true
     || lk == 788                   // '(' false
     || lk == 815                   // '[' false
     || lk == 853                   // '{' false
     || lk == 916                   // '(' string
     || lk == 943                   // '[' string
     || lk == 981                   // '{' string
     || lk == 1172                  // '(' complex
     || lk == 1199                  // '[' complex
     || lk == 1237                  // '{' complex
     || lk == 1300                  // '(' real
     || lk == 1327                  // '[' real
     || lk == 1365                  // '{' real
     || lk == 1428                  // '(' comment
     || lk == 1455                  // '[' comment
     || lk == 1493                  // '{' comment
     || lk == 1684                  // '(' '!'
     || lk == 1711                  // '[' '!'
     || lk == 1749                  // '{' '!'
     || lk == 1940                  // '(' '#'
     || lk == 1967                  // '[' '#'
     || lk == 2005                  // '{' '#'
     || lk == 2563                  // identifier '('
     || lk == 2580                  // '(' '('
     || lk == 2607                  // '[' '('
     || lk == 2645                  // '{' '('
     || lk == 2836                  // '(' '*'
     || lk == 2863                  // '[' '*'
     || lk == 2901                  // '{' '*'
     || lk == 3203                  // identifier '++'
     || lk == 3204                  // null '++'
     || lk == 3205                  // true '++'
     || lk == 3206                  // false '++'
     || lk == 3207                  // string '++'
     || lk == 3209                  // complex '++'
     || lk == 3210                  // real '++'
     || lk == 3220                  // '(' '++'
     || lk == 3247                  // '[' '++'
     || lk == 3285                  // '{' '++'
     || lk == 3715                  // identifier '--'
     || lk == 3716                  // null '--'
     || lk == 3717                  // true '--'
     || lk == 3718                  // false '--'
     || lk == 3719                  // string '--'
     || lk == 3721                  // complex '--'
     || lk == 3722                  // real '--'
     || lk == 3732                  // '(' '--'
     || lk == 3759                  // '[' '--'
     || lk == 3797                  // '{' '--'
     || lk == 3971                  // identifier '->'
     || lk == 4099                  // identifier '.'
     || lk == 4628                  // '(' ';'
     || lk == 4655                  // '[' ';'
     || lk == 4693                  // '{' ';'
     || lk == 6019                  // identifier '['
     || lk == 6036                  // '(' '['
     || lk == 6063                  // '[' '['
     || lk == 6101                  // '{' '['
     || lk == 6191                  // '[' ']'
     || lk == 6420                  // '(' 'break'
     || lk == 6447                  // '[' 'break'
     || lk == 6485                  // '{' 'break'
     || lk == 6676                  // '(' 'char'
     || lk == 6703                  // '[' 'char'
     || lk == 6741                  // '{' 'char'
     || lk == 6804                  // '(' 'class'
     || lk == 6831                  // '[' 'class'
     || lk == 6869                  // '{' 'class'
     || lk == 6932                  // '(' 'continue'
     || lk == 6959                  // '[' 'continue'
     || lk == 6997                  // '{' 'continue'
     || lk == 7188                  // '(' 'do'
     || lk == 7215                  // '[' 'do'
     || lk == 7253                  // '{' 'do'
     || lk == 7316                  // '(' 'double'
     || lk == 7343                  // '[' 'double'
     || lk == 7381                  // '{' 'double'
     || lk == 8084                  // '(' 'float'
     || lk == 8111                  // '[' 'float'
     || lk == 8149                  // '{' 'float'
     || lk == 8212                  // '(' 'for'
     || lk == 8239                  // '[' 'for'
     || lk == 8277                  // '{' 'for'
     || lk == 8340                  // '(' 'if'
     || lk == 8367                  // '[' 'if'
     || lk == 8405                  // '{' 'if'
     || lk == 8980                  // '(' 'int'
     || lk == 9007                  // '[' 'int'
     || lk == 9045                  // '{' 'int'
     || lk == 9108                  // '(' 'long'
     || lk == 9135                  // '[' 'long'
     || lk == 9173                  // '{' 'long'
     || lk == 9236                  // '(' 'namespace'
     || lk == 9263                  // '[' 'namespace'
     || lk == 9301                  // '{' 'namespace'
     || lk == 9364                  // '(' 'private'
     || lk == 9391                  // '[' 'private'
     || lk == 9429                  // '{' 'private'
     || lk == 9492                  // '(' 'protected'
     || lk == 9519                  // '[' 'protected'
     || lk == 9557                  // '{' 'protected'
     || lk == 9620                  // '(' 'public'
     || lk == 9647                  // '[' 'public'
     || lk == 9685                  // '{' 'public'
     || lk == 9748                  // '(' 'return'
     || lk == 9775                  // '[' 'return'
     || lk == 9813                  // '{' 'return'
     || lk == 9876                  // '(' 'template'
     || lk == 9903                  // '[' 'template'
     || lk == 9941                  // '{' 'template'
     || lk == 10004                 // '(' 'throw'
     || lk == 10031                 // '[' 'throw'
     || lk == 10069                 // '{' 'throw'
     || lk == 10132                 // '(' 'try'
     || lk == 10159                 // '[' 'try'
     || lk == 10197                 // '{' 'try'
     || lk == 10516                 // '(' 'using'
     || lk == 10543                 // '[' 'using'
     || lk == 10581                 // '{' 'using'
     || lk == 10644                 // '(' 'void'
     || lk == 10671                 // '[' 'void'
     || lk == 10709                 // '{' 'void'
     || lk == 10772                 // '(' 'while'
     || lk == 10799                 // '[' 'while'
     || lk == 10837                 // '{' 'while'
     || lk == 10900                 // '(' '{'
     || lk == 10927                 // '[' '{'
     || lk == 10965                 // '{' '{'
     || lk == 11412                 // '(' '~'
     || lk == 11439                 // '[' '~'
     || lk == 11477)                // '{' '~'
    {
      lk = memoized(1, e0);
      if (lk == 0)
      {
        int b0A = b0; int e0A = e0; int l1A = l1;
        int b1A = b1; int e1A = e1; int l2A = l2;
        int b2A = b2; int e2A = e2;
        try
        {
          try_primary();
          lookahead1(7);            // '++'
          consumeT(25);             // '++'
          memoize(1, e0A, -6);
          lk = -9;
        }
        catch (ParseException &)
        {
          try
          {
            b0 = b0A; e0 = e0A; l1 = l1A; if (l1 == 0) {end = e0A;} else {
            b1 = b1A; e1 = e1A; l2 = l2A; if (l2 == 0) {end = e1A;} else {
            b2 = b2A; e2 = e2A; end = e2A; }}
            try_primary();
            lookahead1(8);          // '--'
            consumeT(29);           // '--'
            memoize(1, e0A, -7);
            lk = -9;
          }
          catch (ParseException &)
          {
            lk = -8;
            b0 = b0A; e0 = e0A; l1 = l1A; if (l1 == 0) {end = e0A;} else {
            b1 = b1A; e1 = e1A; l2 = l2A; if (l2 == 0) {end = e1A;} else {
            b2 = b2A; e2 = e2A; end = e2A; }}
            memoize(1, e0A, -8);
          }
        }
      }
    }
    switch (lk)
    {
    case 89:                        // '~'
      consumeT(89);                 // '~'
      try_primary();
      break;
    case 13:                        // '!'
      consumeT(13);                 // '!'
      try_primary();
      break;
    case 22:                        // '*'
      consumeT(22);                 // '*'
      try_primary();
      break;
    case 25:                        // '++'
      consumeT(25);                 // '++'
      try_primary();
      break;
    case 29:                        // '--'
      consumeT(29);                 // '--'
      try_primary();
      break;
    case -6:
      try_primary();
      lookahead1(7);                // '++'
      consumeT(25);                 // '++'
      break;
    case -7:
      try_primary();
      lookahead1(8);                // '--'
      consumeT(29);                 // '--'
      break;
    case -9:
      break;
    default:
      try_primary();
      break;
    }
  }

  void parse_primary()
  {
    eventHandler->startNonterminal(L"primary", e0);
    lookahead1(38);                 // identifier | null | true | false | string | complex | real | '(' | '[' | '{'
    switch (l1)
    {
    case 3:                         // identifier
      parse_member();
      break;
    case 20:                        // '('
      parse_parenthesizedExpression();
      break;
    default:
      parse_value();
      break;
    }
    eventHandler->endNonterminal(L"primary", e0);
  }

  void try_primary()
  {
    lookahead1(38);                 // identifier | null | true | false | string | complex | real | '(' | '[' | '{'
    switch (l1)
    {
    case 3:                         // identifier
      try_member();
      break;
    case 20:                        // '('
      try_parenthesizedExpression();
      break;
    default:
      try_value();
      break;
    }
  }

  void parse_statement()
  {
    eventHandler->startNonterminal(L"statement", e0);
    switch (l1)
    {
    case 52:                        // 'char'
    case 57:                        // 'double'
    case 63:                        // 'float'
    case 70:                        // 'int'
    case 71:                        // 'long'
    case 83:                        // 'void'
      lookahead2(0);                // identifier
      break;
    default:
      lk = l1;
      break;
    }
    if (lk == 436                   // 'char' identifier
     || lk == 441                   // 'double' identifier
     || lk == 447                   // 'float' identifier
     || lk == 454                   // 'int' identifier
     || lk == 455                   // 'long' identifier
     || lk == 467)                  // 'void' identifier
    {
      lk = memoized(2, e0);
      if (lk == 0)
      {
        int b0A = b0; int e0A = e0; int l1A = l1;
        int b1A = b1; int e1A = e1; int l2A = l2;
        int b2A = b2; int e2A = e2;
        try
        {
          try_functionDefinition();
          lk = -6;
        }
        catch (ParseException &)
        {
          lk = -7;
        }
        b0 = b0A; e0 = e0A; l1 = l1A; if (l1 == 0) {end = e0A;} else {
        b1 = b1A; e1 = e1A; l2 = l2A; if (l2 == 0) {end = e1A;} else {
        b2 = b2A; e2 = e2A; end = e2A; }}
        memoize(2, e0, lk);
      }
    }
    switch (lk)
    {
    case 15:                        // '#'
      parse_preprocessorDirective();
      break;
    case 53:                        // 'class'
      parse_class();
      break;
    case 72:                        // 'namespace'
      parse_namespace();
      break;
    case 82:                        // 'using'
      parse_using();
      break;
    case -6:
      parse_functionDefinition();
      break;
    case -7:
      parse_variableDefinition();
      break;
    case 77:                        // 'template'
      parse_template();
      break;
    case 65:                        // 'if'
      parse_if();
      break;
    case 56:                        // 'do'
      parse_do();
      break;
    case 84:                        // 'while'
      parse_while();
      break;
    case 64:                        // 'for'
      parse_for();
      break;
    case 79:                        // 'try'
      parse_try();
      break;
    case 50:                        // 'break'
      parse_break();
      break;
    case 54:                        // 'continue'
      parse_continue();
      break;
    case 76:                        // 'return'
      parse_return();
      break;
    case 78:                        // 'throw'
      parse_throw();
      break;
    default:
      parse_accessDefinition();
      break;
    }
    eventHandler->endNonterminal(L"statement", e0);
  }

  void try_statement()
  {
    switch (l1)
    {
    case 52:                        // 'char'
    case 57:                        // 'double'
    case 63:                        // 'float'
    case 70:                        // 'int'
    case 71:                        // 'long'
    case 83:                        // 'void'
      lookahead2(0);                // identifier
      break;
    default:
      lk = l1;
      break;
    }
    if (lk == 436                   // 'char' identifier
     || lk == 441                   // 'double' identifier
     || lk == 447                   // 'float' identifier
     || lk == 454                   // 'int' identifier
     || lk == 455                   // 'long' identifier
     || lk == 467)                  // 'void' identifier
    {
      lk = memoized(2, e0);
      if (lk == 0)
      {
        int b0A = b0; int e0A = e0; int l1A = l1;
        int b1A = b1; int e1A = e1; int l2A = l2;
        int b2A = b2; int e2A = e2;
        try
        {
          try_functionDefinition();
          memoize(2, e0A, -6);
          lk = -18;
        }
        catch (ParseException &)
        {
          lk = -7;
          b0 = b0A; e0 = e0A; l1 = l1A; if (l1 == 0) {end = e0A;} else {
          b1 = b1A; e1 = e1A; l2 = l2A; if (l2 == 0) {end = e1A;} else {
          b2 = b2A; e2 = e2A; end = e2A; }}
          memoize(2, e0A, -7);
        }
      }
    }
    switch (lk)
    {
    case 15:                        // '#'
      try_preprocessorDirective();
      break;
    case 53:                        // 'class'
      try_class();
      break;
    case 72:                        // 'namespace'
      try_namespace();
      break;
    case 82:                        // 'using'
      try_using();
      break;
    case -6:
      try_functionDefinition();
      break;
    case -7:
      try_variableDefinition();
      break;
    case 77:                        // 'template'
      try_template();
      break;
    case 65:                        // 'if'
      try_if();
      break;
    case 56:                        // 'do'
      try_do();
      break;
    case 84:                        // 'while'
      try_while();
      break;
    case 64:                        // 'for'
      try_for();
      break;
    case 79:                        // 'try'
      try_try();
      break;
    case 50:                        // 'break'
      try_break();
      break;
    case 54:                        // 'continue'
      try_continue();
      break;
    case 76:                        // 'return'
      try_return();
      break;
    case 78:                        // 'throw'
      try_throw();
      break;
    case -18:
      break;
    default:
      try_accessDefinition();
      break;
    }
  }

  void parse_preprocessorDirective()
  {
    eventHandler->startNonterminal(L"preprocessorDirective", e0);
    switch (l1)
    {
    case 15:                        // '#'
      lookahead2(37);               // 'define' | 'error' | 'if' | 'ifdef' | 'ifndef' | 'import' | 'include' | 'undef' |
                                    // 'using'
      break;
    default:
      lk = l1;
      break;
    }
    switch (lk)
    {
    case 7055:                      // '#' 'define'
      parse_defineDirective();
      break;
    case 7951:                      // '#' 'error'
      parse_errorDirective();
      break;
    case 8719:                      // '#' 'import'
      parse_importDirective();
      break;
    case 8847:                      // '#' 'include'
      parse_includeDirective();
      break;
    case 10383:                     // '#' 'undef'
      parse_undefDirective();
      break;
    case 10511:                     // '#' 'using'
      parse_usingDirective();
      break;
    default:
      parse_ifStatementDirective();
      break;
    }
    eventHandler->endNonterminal(L"preprocessorDirective", e0);
  }

  void try_preprocessorDirective()
  {
    switch (l1)
    {
    case 15:                        // '#'
      lookahead2(37);               // 'define' | 'error' | 'if' | 'ifdef' | 'ifndef' | 'import' | 'include' | 'undef' |
                                    // 'using'
      break;
    default:
      lk = l1;
      break;
    }
    switch (lk)
    {
    case 7055:                      // '#' 'define'
      try_defineDirective();
      break;
    case 7951:                      // '#' 'error'
      try_errorDirective();
      break;
    case 8719:                      // '#' 'import'
      try_importDirective();
      break;
    case 8847:                      // '#' 'include'
      try_includeDirective();
      break;
    case 10383:                     // '#' 'undef'
      try_undefDirective();
      break;
    case 10511:                     // '#' 'using'
      try_usingDirective();
      break;
    default:
      try_ifStatementDirective();
      break;
    }
  }

  void parse_defineDirective()
  {
    eventHandler->startNonterminal(L"defineDirective", e0);
    switch (l1)
    {
    case 15:                        // '#'
      lookahead2(13);               // 'define'
      break;
    default:
      lk = l1;
      break;
    }
    lk = memoized(3, e0);
    if (lk == 0)
    {
      int b0A = b0; int e0A = e0; int l1A = l1;
      int b1A = b1; int e1A = e1; int l2A = l2;
      int b2A = b2; int e2A = e2;
      try
      {
        consumeT(15);               // '#'
        lookahead1(13);             // 'define'
        consumeT(55);               // 'define'
        lookahead1(0);              // identifier
        consumeT(3);                // identifier
        try_expression();
        lk = -1;
      }
      catch (ParseException &)
      {
        lk = -2;
      }
      b0 = b0A; e0 = e0A; l1 = l1A; if (l1 == 0) {end = e0A;} else {
      b1 = b1A; e1 = e1A; l2 = l2A; if (l2 == 0) {end = e1A;} else {
      b2 = b2A; e2 = e2A; end = e2A; }}
      memoize(3, e0, lk);
    }
    switch (lk)
    {
    case -1:
      consume(15);                  // '#'
      lookahead1(13);               // 'define'
      consume(55);                  // 'define'
      lookahead1(0);                // identifier
      consume(3);                   // identifier
      parse_expression();
      break;
    default:
      consume(15);                  // '#'
      lookahead1(13);               // 'define'
      consume(55);                  // 'define'
      lookahead1(0);                // identifier
      consume(3);                   // identifier
      lookahead1(5);                // '('
      consume(20);                  // '('
      lookahead1(44);               // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | ')' | '*' | '++' | '--' | ';' | '[' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '~'
      if (l1 != 21)                 // ')'
      {
        parse_arguments();
      }
      consume(21);                  // ')'
      parse_expression();
      break;
    }
    eventHandler->endNonterminal(L"defineDirective", e0);
  }

  void try_defineDirective()
  {
    switch (l1)
    {
    case 15:                        // '#'
      lookahead2(13);               // 'define'
      break;
    default:
      lk = l1;
      break;
    }
    lk = memoized(3, e0);
    if (lk == 0)
    {
      int b0A = b0; int e0A = e0; int l1A = l1;
      int b1A = b1; int e1A = e1; int l2A = l2;
      int b2A = b2; int e2A = e2;
      try
      {
        consumeT(15);               // '#'
        lookahead1(13);             // 'define'
        consumeT(55);               // 'define'
        lookahead1(0);              // identifier
        consumeT(3);                // identifier
        try_expression();
        memoize(3, e0A, -1);
        lk = -3;
      }
      catch (ParseException &)
      {
        lk = -2;
        b0 = b0A; e0 = e0A; l1 = l1A; if (l1 == 0) {end = e0A;} else {
        b1 = b1A; e1 = e1A; l2 = l2A; if (l2 == 0) {end = e1A;} else {
        b2 = b2A; e2 = e2A; end = e2A; }}
        memoize(3, e0A, -2);
      }
    }
    switch (lk)
    {
    case -1:
      consumeT(15);                 // '#'
      lookahead1(13);               // 'define'
      consumeT(55);                 // 'define'
      lookahead1(0);                // identifier
      consumeT(3);                  // identifier
      try_expression();
      break;
    case -3:
      break;
    default:
      consumeT(15);                 // '#'
      lookahead1(13);               // 'define'
      consumeT(55);                 // 'define'
      lookahead1(0);                // identifier
      consumeT(3);                  // identifier
      lookahead1(5);                // '('
      consumeT(20);                 // '('
      lookahead1(44);               // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | ')' | '*' | '++' | '--' | ';' | '[' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '~'
      if (l1 != 21)                 // ')'
      {
        try_arguments();
      }
      consumeT(21);                 // ')'
      try_expression();
      break;
    }
  }

  void parse_ifStatementDirective()
  {
    eventHandler->startNonterminal(L"ifStatementDirective", e0);
    switch (l1)
    {
    case 15:                        // '#'
      lookahead2(36);               // 'if' | 'ifdef' | 'ifndef'
      break;
    default:
      lk = l1;
      break;
    }
    switch (lk)
    {
    case 8335:                      // '#' 'if'
      parse_ifDirective();
      break;
    case 8463:                      // '#' 'ifdef'
      parse_ifdefDirective();
      break;
    default:
      parse_ifndefDirective();
      break;
    }
    for (;;)
    {
      lookahead1(4);                // '#'
      switch (l1)
      {
      case 15:                      // '#'
        lookahead2(35);             // 'elif' | 'else' | 'endif'
        break;
      default:
        lk = l1;
        break;
      }
      if (lk != 7439)               // '#' 'elif'
      {
        break;
      }
      parse_elifDirective();
    }
    switch (l1)
    {
    case 15:                        // '#'
      lookahead2(33);               // 'else' | 'endif'
      break;
    default:
      lk = l1;
      break;
    }
    if (lk == 7567)                 // '#' 'else'
    {
      parse_elseDirective();
    }
    parse_endifDirective();
    eventHandler->endNonterminal(L"ifStatementDirective", e0);
  }

  void try_ifStatementDirective()
  {
    switch (l1)
    {
    case 15:                        // '#'
      lookahead2(36);               // 'if' | 'ifdef' | 'ifndef'
      break;
    default:
      lk = l1;
      break;
    }
    switch (lk)
    {
    case 8335:                      // '#' 'if'
      try_ifDirective();
      break;
    case 8463:                      // '#' 'ifdef'
      try_ifdefDirective();
      break;
    default:
      try_ifndefDirective();
      break;
    }
    for (;;)
    {
      lookahead1(4);                // '#'
      switch (l1)
      {
      case 15:                      // '#'
        lookahead2(35);             // 'elif' | 'else' | 'endif'
        break;
      default:
        lk = l1;
        break;
      }
      if (lk != 7439)               // '#' 'elif'
      {
        break;
      }
      try_elifDirective();
    }
    switch (l1)
    {
    case 15:                        // '#'
      lookahead2(33);               // 'else' | 'endif'
      break;
    default:
      lk = l1;
      break;
    }
    if (lk == 7567)                 // '#' 'else'
    {
      try_elseDirective();
    }
    try_endifDirective();
  }

  void parse_ifDirective()
  {
    eventHandler->startNonterminal(L"ifDirective", e0);
    consume(15);                    // '#'
    lookahead1(18);                 // 'if'
    consume(65);                    // 'if'
    parse_expression();
    lookahead1(3);                  // lineTerminator
    consume(12);                    // lineTerminator
    lookahead1(2);                  // unicode
    consume(8);                     // unicode
    lookahead1(3);                  // lineTerminator
    consume(12);                    // lineTerminator
    eventHandler->endNonterminal(L"ifDirective", e0);
  }

  void try_ifDirective()
  {
    consumeT(15);                   // '#'
    lookahead1(18);                 // 'if'
    consumeT(65);                   // 'if'
    try_expression();
    lookahead1(3);                  // lineTerminator
    consumeT(12);                   // lineTerminator
    lookahead1(2);                  // unicode
    consumeT(8);                    // unicode
    lookahead1(3);                  // lineTerminator
    consumeT(12);                   // lineTerminator
  }

  void parse_ifdefDirective()
  {
    eventHandler->startNonterminal(L"ifdefDirective", e0);
    consume(15);                    // '#'
    lookahead1(19);                 // 'ifdef'
    consume(66);                    // 'ifdef'
    lookahead1(0);                  // identifier
    consume(3);                     // identifier
    lookahead1(3);                  // lineTerminator
    consume(12);                    // lineTerminator
    lookahead1(2);                  // unicode
    consume(8);                     // unicode
    lookahead1(3);                  // lineTerminator
    consume(12);                    // lineTerminator
    eventHandler->endNonterminal(L"ifdefDirective", e0);
  }

  void try_ifdefDirective()
  {
    consumeT(15);                   // '#'
    lookahead1(19);                 // 'ifdef'
    consumeT(66);                   // 'ifdef'
    lookahead1(0);                  // identifier
    consumeT(3);                    // identifier
    lookahead1(3);                  // lineTerminator
    consumeT(12);                   // lineTerminator
    lookahead1(2);                  // unicode
    consumeT(8);                    // unicode
    lookahead1(3);                  // lineTerminator
    consumeT(12);                   // lineTerminator
  }

  void parse_ifndefDirective()
  {
    eventHandler->startNonterminal(L"ifndefDirective", e0);
    consume(15);                    // '#'
    lookahead1(20);                 // 'ifndef'
    consume(67);                    // 'ifndef'
    lookahead1(0);                  // identifier
    consume(3);                     // identifier
    lookahead1(3);                  // lineTerminator
    consume(12);                    // lineTerminator
    lookahead1(2);                  // unicode
    consume(8);                     // unicode
    lookahead1(3);                  // lineTerminator
    consume(12);                    // lineTerminator
    eventHandler->endNonterminal(L"ifndefDirective", e0);
  }

  void try_ifndefDirective()
  {
    consumeT(15);                   // '#'
    lookahead1(20);                 // 'ifndef'
    consumeT(67);                   // 'ifndef'
    lookahead1(0);                  // identifier
    consumeT(3);                    // identifier
    lookahead1(3);                  // lineTerminator
    consumeT(12);                   // lineTerminator
    lookahead1(2);                  // unicode
    consumeT(8);                    // unicode
    lookahead1(3);                  // lineTerminator
    consumeT(12);                   // lineTerminator
  }

  void parse_elifDirective()
  {
    eventHandler->startNonterminal(L"elifDirective", e0);
    consume(15);                    // '#'
    lookahead1(14);                 // 'elif'
    consume(58);                    // 'elif'
    parse_expression();
    lookahead1(3);                  // lineTerminator
    consume(12);                    // lineTerminator
    lookahead1(2);                  // unicode
    consume(8);                     // unicode
    lookahead1(3);                  // lineTerminator
    consume(12);                    // lineTerminator
    eventHandler->endNonterminal(L"elifDirective", e0);
  }

  void try_elifDirective()
  {
    consumeT(15);                   // '#'
    lookahead1(14);                 // 'elif'
    consumeT(58);                   // 'elif'
    try_expression();
    lookahead1(3);                  // lineTerminator
    consumeT(12);                   // lineTerminator
    lookahead1(2);                  // unicode
    consumeT(8);                    // unicode
    lookahead1(3);                  // lineTerminator
    consumeT(12);                   // lineTerminator
  }

  void parse_elseDirective()
  {
    eventHandler->startNonterminal(L"elseDirective", e0);
    consume(15);                    // '#'
    lookahead1(15);                 // 'else'
    consume(59);                    // 'else'
    lookahead1(3);                  // lineTerminator
    consume(12);                    // lineTerminator
    lookahead1(2);                  // unicode
    consume(8);                     // unicode
    lookahead1(3);                  // lineTerminator
    consume(12);                    // lineTerminator
    eventHandler->endNonterminal(L"elseDirective", e0);
  }

  void try_elseDirective()
  {
    consumeT(15);                   // '#'
    lookahead1(15);                 // 'else'
    consumeT(59);                   // 'else'
    lookahead1(3);                  // lineTerminator
    consumeT(12);                   // lineTerminator
    lookahead1(2);                  // unicode
    consumeT(8);                    // unicode
    lookahead1(3);                  // lineTerminator
    consumeT(12);                   // lineTerminator
  }

  void parse_endifDirective()
  {
    eventHandler->startNonterminal(L"endifDirective", e0);
    lookahead1(4);                  // '#'
    consume(15);                    // '#'
    lookahead1(16);                 // 'endif'
    consume(61);                    // 'endif'
    eventHandler->endNonterminal(L"endifDirective", e0);
  }

  void try_endifDirective()
  {
    lookahead1(4);                  // '#'
    consumeT(15);                   // '#'
    lookahead1(16);                 // 'endif'
    consumeT(61);                   // 'endif'
  }

  void parse_errorDirective()
  {
    eventHandler->startNonterminal(L"errorDirective", e0);
    consume(15);                    // '#'
    lookahead1(17);                 // 'error'
    consume(62);                    // 'error'
    lookahead1(1);                  // string
    consume(7);                     // string
    eventHandler->endNonterminal(L"errorDirective", e0);
  }

  void try_errorDirective()
  {
    consumeT(15);                   // '#'
    lookahead1(17);                 // 'error'
    consumeT(62);                   // 'error'
    lookahead1(1);                  // string
    consumeT(7);                    // string
  }

  void parse_importDirective()
  {
    eventHandler->startNonterminal(L"importDirective", e0);
    switch (l1)
    {
    case 15:                        // '#'
      lookahead2(21);               // 'import'
      break;
    default:
      lk = l1;
      break;
    }
    lk = memoized(4, e0);
    if (lk == 0)
    {
      int b0A = b0; int e0A = e0; int l1A = l1;
      int b1A = b1; int e1A = e1; int l2A = l2;
      int b2A = b2; int e2A = e2;
      try
      {
        consumeT(15);               // '#'
        lookahead1(21);             // 'import'
        consumeT(68);               // 'import'
        lookahead1(1);              // string
        consumeT(7);                // string
        lk = -1;
      }
      catch (ParseException &)
      {
        lk = -2;
      }
      b0 = b0A; e0 = e0A; l1 = l1A; if (l1 == 0) {end = e0A;} else {
      b1 = b1A; e1 = e1A; l2 = l2A; if (l2 == 0) {end = e1A;} else {
      b2 = b2A; e2 = e2A; end = e2A; }}
      memoize(4, e0, lk);
    }
    switch (lk)
    {
    case -1:
      consume(15);                  // '#'
      lookahead1(21);               // 'import'
      consume(68);                  // 'import'
      lookahead1(1);                // string
      consume(7);                   // string
      break;
    default:
      consume(15);                  // '#'
      lookahead1(21);               // 'import'
      consume(68);                  // 'import'
      lookahead1(11);               // '<'
      consume(37);                  // '<'
      lookahead1(2);                // unicode
      consume(8);                   // unicode
      lookahead1(12);               // '>'
      consume(43);                  // '>'
      break;
    }
    eventHandler->endNonterminal(L"importDirective", e0);
  }

  void try_importDirective()
  {
    switch (l1)
    {
    case 15:                        // '#'
      lookahead2(21);               // 'import'
      break;
    default:
      lk = l1;
      break;
    }
    lk = memoized(4, e0);
    if (lk == 0)
    {
      int b0A = b0; int e0A = e0; int l1A = l1;
      int b1A = b1; int e1A = e1; int l2A = l2;
      int b2A = b2; int e2A = e2;
      try
      {
        consumeT(15);               // '#'
        lookahead1(21);             // 'import'
        consumeT(68);               // 'import'
        lookahead1(1);              // string
        consumeT(7);                // string
        memoize(4, e0A, -1);
        lk = -3;
      }
      catch (ParseException &)
      {
        lk = -2;
        b0 = b0A; e0 = e0A; l1 = l1A; if (l1 == 0) {end = e0A;} else {
        b1 = b1A; e1 = e1A; l2 = l2A; if (l2 == 0) {end = e1A;} else {
        b2 = b2A; e2 = e2A; end = e2A; }}
        memoize(4, e0A, -2);
      }
    }
    switch (lk)
    {
    case -1:
      consumeT(15);                 // '#'
      lookahead1(21);               // 'import'
      consumeT(68);                 // 'import'
      lookahead1(1);                // string
      consumeT(7);                  // string
      break;
    case -3:
      break;
    default:
      consumeT(15);                 // '#'
      lookahead1(21);               // 'import'
      consumeT(68);                 // 'import'
      lookahead1(11);               // '<'
      consumeT(37);                 // '<'
      lookahead1(2);                // unicode
      consumeT(8);                  // unicode
      lookahead1(12);               // '>'
      consumeT(43);                 // '>'
      break;
    }
  }

  void parse_includeDirective()
  {
    eventHandler->startNonterminal(L"includeDirective", e0);
    switch (l1)
    {
    case 15:                        // '#'
      lookahead2(22);               // 'include'
      break;
    default:
      lk = l1;
      break;
    }
    lk = memoized(5, e0);
    if (lk == 0)
    {
      int b0A = b0; int e0A = e0; int l1A = l1;
      int b1A = b1; int e1A = e1; int l2A = l2;
      int b2A = b2; int e2A = e2;
      try
      {
        consumeT(15);               // '#'
        lookahead1(22);             // 'include'
        consumeT(69);               // 'include'
        lookahead1(1);              // string
        consumeT(7);                // string
        lk = -1;
      }
      catch (ParseException &)
      {
        lk = -2;
      }
      b0 = b0A; e0 = e0A; l1 = l1A; if (l1 == 0) {end = e0A;} else {
      b1 = b1A; e1 = e1A; l2 = l2A; if (l2 == 0) {end = e1A;} else {
      b2 = b2A; e2 = e2A; end = e2A; }}
      memoize(5, e0, lk);
    }
    switch (lk)
    {
    case -1:
      consume(15);                  // '#'
      lookahead1(22);               // 'include'
      consume(69);                  // 'include'
      lookahead1(1);                // string
      consume(7);                   // string
      break;
    default:
      consume(15);                  // '#'
      lookahead1(22);               // 'include'
      consume(69);                  // 'include'
      lookahead1(11);               // '<'
      consume(37);                  // '<'
      lookahead1(2);                // unicode
      consume(8);                   // unicode
      lookahead1(12);               // '>'
      consume(43);                  // '>'
      break;
    }
    eventHandler->endNonterminal(L"includeDirective", e0);
  }

  void try_includeDirective()
  {
    switch (l1)
    {
    case 15:                        // '#'
      lookahead2(22);               // 'include'
      break;
    default:
      lk = l1;
      break;
    }
    lk = memoized(5, e0);
    if (lk == 0)
    {
      int b0A = b0; int e0A = e0; int l1A = l1;
      int b1A = b1; int e1A = e1; int l2A = l2;
      int b2A = b2; int e2A = e2;
      try
      {
        consumeT(15);               // '#'
        lookahead1(22);             // 'include'
        consumeT(69);               // 'include'
        lookahead1(1);              // string
        consumeT(7);                // string
        memoize(5, e0A, -1);
        lk = -3;
      }
      catch (ParseException &)
      {
        lk = -2;
        b0 = b0A; e0 = e0A; l1 = l1A; if (l1 == 0) {end = e0A;} else {
        b1 = b1A; e1 = e1A; l2 = l2A; if (l2 == 0) {end = e1A;} else {
        b2 = b2A; e2 = e2A; end = e2A; }}
        memoize(5, e0A, -2);
      }
    }
    switch (lk)
    {
    case -1:
      consumeT(15);                 // '#'
      lookahead1(22);               // 'include'
      consumeT(69);                 // 'include'
      lookahead1(1);                // string
      consumeT(7);                  // string
      break;
    case -3:
      break;
    default:
      consumeT(15);                 // '#'
      lookahead1(22);               // 'include'
      consumeT(69);                 // 'include'
      lookahead1(11);               // '<'
      consumeT(37);                 // '<'
      lookahead1(2);                // unicode
      consumeT(8);                  // unicode
      lookahead1(12);               // '>'
      consumeT(43);                 // '>'
      break;
    }
  }

  void parse_undefDirective()
  {
    eventHandler->startNonterminal(L"undefDirective", e0);
    consume(15);                    // '#'
    lookahead1(24);                 // 'undef'
    consume(81);                    // 'undef'
    lookahead1(0);                  // identifier
    consume(3);                     // identifier
    eventHandler->endNonterminal(L"undefDirective", e0);
  }

  void try_undefDirective()
  {
    consumeT(15);                   // '#'
    lookahead1(24);                 // 'undef'
    consumeT(81);                   // 'undef'
    lookahead1(0);                  // identifier
    consumeT(3);                    // identifier
  }

  void parse_usingDirective()
  {
    eventHandler->startNonterminal(L"usingDirective", e0);
    consume(15);                    // '#'
    lookahead1(25);                 // 'using'
    consume(82);                    // 'using'
    lookahead1(1);                  // string
    consume(7);                     // string
    eventHandler->endNonterminal(L"usingDirective", e0);
  }

  void try_usingDirective()
  {
    consumeT(15);                   // '#'
    lookahead1(25);                 // 'using'
    consumeT(82);                   // 'using'
    lookahead1(1);                  // string
    consumeT(7);                    // string
  }

  void parse_class()
  {
    eventHandler->startNonterminal(L"class", e0);
    consume(53);                    // 'class'
    lookahead1(0);                  // identifier
    consume(3);                     // identifier
    lookahead1(27);                 // '{'
    consume(85);                    // '{'
    for (;;)
    {
      lookahead1(46);               // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | '*' | '++' | '--' | ';' | '[' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '}' | '~'
      if (l1 == 88)                 // '}'
      {
        break;
      }
      parse_expression();
    }
    consume(88);                    // '}'
    eventHandler->endNonterminal(L"class", e0);
  }

  void try_class()
  {
    consumeT(53);                   // 'class'
    lookahead1(0);                  // identifier
    consumeT(3);                    // identifier
    lookahead1(27);                 // '{'
    consumeT(85);                   // '{'
    for (;;)
    {
      lookahead1(46);               // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | '*' | '++' | '--' | ';' | '[' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '}' | '~'
      if (l1 == 88)                 // '}'
      {
        break;
      }
      try_expression();
    }
    consumeT(88);                   // '}'
  }

  void parse_accessDefinition()
  {
    eventHandler->startNonterminal(L"accessDefinition", e0);
    switch (l1)
    {
    case 73:                        // 'private'
      consume(73);                  // 'private'
      lookahead1(9);                // ':'
      consume(35);                  // ':'
      break;
    case 74:                        // 'protected'
      consume(74);                  // 'protected'
      lookahead1(9);                // ':'
      consume(35);                  // ':'
      break;
    default:
      consume(75);                  // 'public'
      lookahead1(9);                // ':'
      consume(35);                  // ':'
      break;
    }
    eventHandler->endNonterminal(L"accessDefinition", e0);
  }

  void try_accessDefinition()
  {
    switch (l1)
    {
    case 73:                        // 'private'
      consumeT(73);                 // 'private'
      lookahead1(9);                // ':'
      consumeT(35);                 // ':'
      break;
    case 74:                        // 'protected'
      consumeT(74);                 // 'protected'
      lookahead1(9);                // ':'
      consumeT(35);                 // ':'
      break;
    default:
      consumeT(75);                 // 'public'
      lookahead1(9);                // ':'
      consumeT(35);                 // ':'
      break;
    }
  }

  void parse_namespace()
  {
    eventHandler->startNonterminal(L"namespace", e0);
    consume(72);                    // 'namespace'
    lookahead1(0);                  // identifier
    consume(3);                     // identifier
    lookahead1(27);                 // '{'
    consume(85);                    // '{'
    for (;;)
    {
      lookahead1(46);               // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | '*' | '++' | '--' | ';' | '[' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '}' | '~'
      if (l1 == 88)                 // '}'
      {
        break;
      }
      parse_expression();
    }
    consume(88);                    // '}'
    eventHandler->endNonterminal(L"namespace", e0);
  }

  void try_namespace()
  {
    consumeT(72);                   // 'namespace'
    lookahead1(0);                  // identifier
    consumeT(3);                    // identifier
    lookahead1(27);                 // '{'
    consumeT(85);                   // '{'
    for (;;)
    {
      lookahead1(46);               // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | '*' | '++' | '--' | ';' | '[' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '}' | '~'
      if (l1 == 88)                 // '}'
      {
        break;
      }
      try_expression();
    }
    consumeT(88);                   // '}'
  }

  void parse_using()
  {
    eventHandler->startNonterminal(L"using", e0);
    switch (l1)
    {
    case 82:                        // 'using'
      lookahead2(28);               // identifier | 'namespace'
      break;
    default:
      lk = l1;
      break;
    }
    switch (lk)
    {
    case 466:                       // 'using' identifier
      consume(82);                  // 'using'
      lookahead1(0);                // identifier
      consume(3);                   // identifier
      break;
    default:
      consume(82);                  // 'using'
      lookahead1(23);               // 'namespace'
      consume(72);                  // 'namespace'
      lookahead1(0);                // identifier
      consume(3);                   // identifier
      break;
    }
    eventHandler->endNonterminal(L"using", e0);
  }

  void try_using()
  {
    switch (l1)
    {
    case 82:                        // 'using'
      lookahead2(28);               // identifier | 'namespace'
      break;
    default:
      lk = l1;
      break;
    }
    switch (lk)
    {
    case 466:                       // 'using' identifier
      consumeT(82);                 // 'using'
      lookahead1(0);                // identifier
      consumeT(3);                  // identifier
      break;
    default:
      consumeT(82);                 // 'using'
      lookahead1(23);               // 'namespace'
      consumeT(72);                 // 'namespace'
      lookahead1(0);                // identifier
      consumeT(3);                  // identifier
      break;
    }
  }

  void parse_functionDefinition()
  {
    eventHandler->startNonterminal(L"functionDefinition", e0);
    parse_type();
    lookahead1(0);                  // identifier
    consume(3);                     // identifier
    lookahead1(5);                  // '('
    consume(20);                    // '('
    lookahead1(44);                 // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | ')' | '*' | '++' | '--' | ';' | '[' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '~'
    if (l1 != 21)                   // ')'
    {
      parse_arguments();
    }
    consume(21);                    // ')'
    lookahead1(27);                 // '{'
    consume(85);                    // '{'
    for (;;)
    {
      lookahead1(46);               // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | '*' | '++' | '--' | ';' | '[' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '}' | '~'
      if (l1 == 88)                 // '}'
      {
        break;
      }
      parse_expression();
    }
    consume(88);                    // '}'
    eventHandler->endNonterminal(L"functionDefinition", e0);
  }

  void try_functionDefinition()
  {
    try_type();
    lookahead1(0);                  // identifier
    consumeT(3);                    // identifier
    lookahead1(5);                  // '('
    consumeT(20);                   // '('
    lookahead1(44);                 // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | ')' | '*' | '++' | '--' | ';' | '[' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '~'
    if (l1 != 21)                   // ')'
    {
      try_arguments();
    }
    consumeT(21);                   // ')'
    lookahead1(27);                 // '{'
    consumeT(85);                   // '{'
    for (;;)
    {
      lookahead1(46);               // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | '*' | '++' | '--' | ';' | '[' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '}' | '~'
      if (l1 == 88)                 // '}'
      {
        break;
      }
      try_expression();
    }
    consumeT(88);                   // '}'
  }

  void parse_variableDefinition()
  {
    eventHandler->startNonterminal(L"variableDefinition", e0);
    parse_type();
    lookahead1(0);                  // identifier
    consume(3);                     // identifier
    for (;;)
    {
      lookahead1(50);               // END | identifier | null | true | false | string | complex | real | comment |
                                    // lineTerminator | '!' | '#' | '(' | ')' | '*' | '++' | ',' | '--' | ';' | '[' |
                                    // ']' | 'break' | 'char' | 'class' | 'continue' | 'do' | 'double' | 'float' |
                                    // 'for' | 'if' | 'int' | 'long' | 'namespace' | 'private' | 'protected' |
                                    // 'public' | 'return' | 'template' | 'throw' | 'try' | 'using' | 'void' | 'while' |
                                    // '{' | '}' | '~'
      switch (l1)
      {
      case 27:                      // ','
        lookahead2(42);             // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | '*' | '++' | '--' | ';' | '[' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '~'
        break;
      default:
        lk = l1;
        break;
      }
      if (lk == 411)                // ',' identifier
      {
        lk = memoized(6, e0);
        if (lk == 0)
        {
          int b0A = b0; int e0A = e0; int l1A = l1;
          int b1A = b1; int e1A = e1; int l2A = l2;
          int b2A = b2; int e2A = e2;
          try
          {
            consumeT(27);           // ','
            lookahead1(0);          // identifier
            consumeT(3);            // identifier
            lk = -1;
          }
          catch (ParseException &)
          {
            lk = -2;
          }
          b0 = b0A; e0 = e0A; l1 = l1A; if (l1 == 0) {end = e0A;} else {
          b1 = b1A; e1 = e1A; l2 = l2A; if (l2 == 0) {end = e1A;} else {
          b2 = b2A; e2 = e2A; end = e2A; }}
          memoize(6, e0, lk);
        }
      }
      if (lk != -1)
      {
        break;
      }
      consume(27);                  // ','
      lookahead1(0);                // identifier
      consume(3);                   // identifier
    }
    eventHandler->endNonterminal(L"variableDefinition", e0);
  }

  void try_variableDefinition()
  {
    try_type();
    lookahead1(0);                  // identifier
    consumeT(3);                    // identifier
    for (;;)
    {
      lookahead1(50);               // END | identifier | null | true | false | string | complex | real | comment |
                                    // lineTerminator | '!' | '#' | '(' | ')' | '*' | '++' | ',' | '--' | ';' | '[' |
                                    // ']' | 'break' | 'char' | 'class' | 'continue' | 'do' | 'double' | 'float' |
                                    // 'for' | 'if' | 'int' | 'long' | 'namespace' | 'private' | 'protected' |
                                    // 'public' | 'return' | 'template' | 'throw' | 'try' | 'using' | 'void' | 'while' |
                                    // '{' | '}' | '~'
      switch (l1)
      {
      case 27:                      // ','
        lookahead2(42);             // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | '*' | '++' | '--' | ';' | '[' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '~'
        break;
      default:
        lk = l1;
        break;
      }
      if (lk == 411)                // ',' identifier
      {
        lk = memoized(6, e0);
        if (lk == 0)
        {
          int b0A = b0; int e0A = e0; int l1A = l1;
          int b1A = b1; int e1A = e1; int l2A = l2;
          int b2A = b2; int e2A = e2;
          try
          {
            consumeT(27);           // ','
            lookahead1(0);          // identifier
            consumeT(3);            // identifier
            memoize(6, e0A, -1);
            continue;
          }
          catch (ParseException &)
          {
            b0 = b0A; e0 = e0A; l1 = l1A; if (l1 == 0) {end = e0A;} else {
            b1 = b1A; e1 = e1A; l2 = l2A; if (l2 == 0) {end = e1A;} else {
            b2 = b2A; e2 = e2A; end = e2A; }}
            memoize(6, e0A, -2);
            break;
          }
        }
      }
      if (lk != -1)
      {
        break;
      }
      consumeT(27);                 // ','
      lookahead1(0);                // identifier
      consumeT(3);                  // identifier
    }
  }

  void parse_template()
  {
    eventHandler->startNonterminal(L"template", e0);
    consume(77);                    // 'template'
    lookahead1(11);                 // '<'
    consume(37);                    // '<'
    lookahead1(32);                 // 'class' | 'typename'
    switch (l1)
    {
    case 53:                        // 'class'
      consume(53);                  // 'class'
      break;
    default:
      consume(80);                  // 'typename'
      break;
    }
    lookahead1(0);                  // identifier
    consume(3);                     // identifier
    lookahead1(12);                 // '>'
    consume(43);                    // '>'
    eventHandler->endNonterminal(L"template", e0);
  }

  void try_template()
  {
    consumeT(77);                   // 'template'
    lookahead1(11);                 // '<'
    consumeT(37);                   // '<'
    lookahead1(32);                 // 'class' | 'typename'
    switch (l1)
    {
    case 53:                        // 'class'
      consumeT(53);                 // 'class'
      break;
    default:
      consumeT(80);                 // 'typename'
      break;
    }
    lookahead1(0);                  // identifier
    consumeT(3);                    // identifier
    lookahead1(12);                 // '>'
    consumeT(43);                   // '>'
  }

  void parse_if()
  {
    eventHandler->startNonterminal(L"if", e0);
    consume(65);                    // 'if'
    lookahead1(5);                  // '('
    consume(20);                    // '('
    parse_expression();
    lookahead1(6);                  // ')'
    consume(21);                    // ')'
    lookahead1(27);                 // '{'
    consume(85);                    // '{'
    for (;;)
    {
      lookahead1(46);               // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | '*' | '++' | '--' | ';' | '[' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '}' | '~'
      if (l1 == 88)                 // '}'
      {
        break;
      }
      parse_expression();
    }
    consume(88);                    // '}'
    for (;;)
    {
      lookahead1(52);               // END | identifier | null | true | false | string | complex | real | comment |
                                    // lineTerminator | '!' | '#' | '(' | ')' | '*' | '++' | ',' | '--' | ';' | '[' |
                                    // ']' | 'break' | 'char' | 'class' | 'continue' | 'do' | 'double' | 'else' |
                                    // 'else if' | 'float' | 'for' | 'if' | 'int' | 'long' | 'namespace' | 'private' |
                                    // 'protected' | 'public' | 'return' | 'template' | 'throw' | 'try' | 'using' |
                                    // 'void' | 'while' | '{' | '}' | '~'
      if (l1 != 60)                 // 'else if'
      {
        break;
      }
      parse_elseif();
    }
    if (l1 == 59)                   // 'else'
    {
      parse_else();
    }
    eventHandler->endNonterminal(L"if", e0);
  }

  void try_if()
  {
    consumeT(65);                   // 'if'
    lookahead1(5);                  // '('
    consumeT(20);                   // '('
    try_expression();
    lookahead1(6);                  // ')'
    consumeT(21);                   // ')'
    lookahead1(27);                 // '{'
    consumeT(85);                   // '{'
    for (;;)
    {
      lookahead1(46);               // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | '*' | '++' | '--' | ';' | '[' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '}' | '~'
      if (l1 == 88)                 // '}'
      {
        break;
      }
      try_expression();
    }
    consumeT(88);                   // '}'
    for (;;)
    {
      lookahead1(52);               // END | identifier | null | true | false | string | complex | real | comment |
                                    // lineTerminator | '!' | '#' | '(' | ')' | '*' | '++' | ',' | '--' | ';' | '[' |
                                    // ']' | 'break' | 'char' | 'class' | 'continue' | 'do' | 'double' | 'else' |
                                    // 'else if' | 'float' | 'for' | 'if' | 'int' | 'long' | 'namespace' | 'private' |
                                    // 'protected' | 'public' | 'return' | 'template' | 'throw' | 'try' | 'using' |
                                    // 'void' | 'while' | '{' | '}' | '~'
      if (l1 != 60)                 // 'else if'
      {
        break;
      }
      try_elseif();
    }
    if (l1 == 59)                   // 'else'
    {
      try_else();
    }
  }

  void parse_elseif()
  {
    eventHandler->startNonterminal(L"elseif", e0);
    consume(60);                    // 'else if'
    lookahead1(5);                  // '('
    consume(20);                    // '('
    parse_expression();
    lookahead1(6);                  // ')'
    consume(21);                    // ')'
    lookahead1(27);                 // '{'
    consume(85);                    // '{'
    for (;;)
    {
      lookahead1(46);               // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | '*' | '++' | '--' | ';' | '[' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '}' | '~'
      if (l1 == 88)                 // '}'
      {
        break;
      }
      parse_expression();
    }
    consume(88);                    // '}'
    eventHandler->endNonterminal(L"elseif", e0);
  }

  void try_elseif()
  {
    consumeT(60);                   // 'else if'
    lookahead1(5);                  // '('
    consumeT(20);                   // '('
    try_expression();
    lookahead1(6);                  // ')'
    consumeT(21);                   // ')'
    lookahead1(27);                 // '{'
    consumeT(85);                   // '{'
    for (;;)
    {
      lookahead1(46);               // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | '*' | '++' | '--' | ';' | '[' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '}' | '~'
      if (l1 == 88)                 // '}'
      {
        break;
      }
      try_expression();
    }
    consumeT(88);                   // '}'
  }

  void parse_else()
  {
    eventHandler->startNonterminal(L"else", e0);
    consume(59);                    // 'else'
    lookahead1(27);                 // '{'
    consume(85);                    // '{'
    for (;;)
    {
      lookahead1(46);               // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | '*' | '++' | '--' | ';' | '[' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '}' | '~'
      if (l1 == 88)                 // '}'
      {
        break;
      }
      parse_expression();
    }
    consume(88);                    // '}'
    eventHandler->endNonterminal(L"else", e0);
  }

  void try_else()
  {
    consumeT(59);                   // 'else'
    lookahead1(27);                 // '{'
    consumeT(85);                   // '{'
    for (;;)
    {
      lookahead1(46);               // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | '*' | '++' | '--' | ';' | '[' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '}' | '~'
      if (l1 == 88)                 // '}'
      {
        break;
      }
      try_expression();
    }
    consumeT(88);                   // '}'
  }

  void parse_do()
  {
    eventHandler->startNonterminal(L"do", e0);
    consume(56);                    // 'do'
    lookahead1(27);                 // '{'
    consume(85);                    // '{'
    for (;;)
    {
      lookahead1(46);               // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | '*' | '++' | '--' | ';' | '[' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '}' | '~'
      if (l1 == 88)                 // '}'
      {
        break;
      }
      parse_expression();
    }
    consume(88);                    // '}'
    lookahead1(26);                 // 'while'
    consume(84);                    // 'while'
    lookahead1(5);                  // '('
    consume(20);                    // '('
    parse_expression();
    lookahead1(6);                  // ')'
    consume(21);                    // ')'
    eventHandler->endNonterminal(L"do", e0);
  }

  void try_do()
  {
    consumeT(56);                   // 'do'
    lookahead1(27);                 // '{'
    consumeT(85);                   // '{'
    for (;;)
    {
      lookahead1(46);               // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | '*' | '++' | '--' | ';' | '[' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '}' | '~'
      if (l1 == 88)                 // '}'
      {
        break;
      }
      try_expression();
    }
    consumeT(88);                   // '}'
    lookahead1(26);                 // 'while'
    consumeT(84);                   // 'while'
    lookahead1(5);                  // '('
    consumeT(20);                   // '('
    try_expression();
    lookahead1(6);                  // ')'
    consumeT(21);                   // ')'
  }

  void parse_while()
  {
    eventHandler->startNonterminal(L"while", e0);
    consume(84);                    // 'while'
    lookahead1(5);                  // '('
    consume(20);                    // '('
    parse_expression();
    lookahead1(6);                  // ')'
    consume(21);                    // ')'
    lookahead1(27);                 // '{'
    consume(85);                    // '{'
    for (;;)
    {
      lookahead1(46);               // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | '*' | '++' | '--' | ';' | '[' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '}' | '~'
      if (l1 == 88)                 // '}'
      {
        break;
      }
      parse_expression();
    }
    consume(88);                    // '}'
    eventHandler->endNonterminal(L"while", e0);
  }

  void try_while()
  {
    consumeT(84);                   // 'while'
    lookahead1(5);                  // '('
    consumeT(20);                   // '('
    try_expression();
    lookahead1(6);                  // ')'
    consumeT(21);                   // ')'
    lookahead1(27);                 // '{'
    consumeT(85);                   // '{'
    for (;;)
    {
      lookahead1(46);               // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | '*' | '++' | '--' | ';' | '[' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '}' | '~'
      if (l1 == 88)                 // '}'
      {
        break;
      }
      try_expression();
    }
    consumeT(88);                   // '}'
  }

  void parse_for()
  {
    eventHandler->startNonterminal(L"for", e0);
    consume(64);                    // 'for'
    lookahead1(5);                  // '('
    consume(20);                    // '('
    lookahead1(42);                 // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | '*' | '++' | '--' | ';' | '[' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '~'
    switch (l1)
    {
    case 36:                        // ';'
      lookahead2(42);               // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | '*' | '++' | '--' | ';' | '[' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '~'
      break;
    default:
      lk = l1;
      break;
    }
    if (lk == 4644)                 // ';' ';'
    {
      lk = memoized(7, e0);
      if (lk == 0)
      {
        int b0A = b0; int e0A = e0; int l1A = l1;
        int b1A = b1; int e1A = e1; int l2A = l2;
        int b2A = b2; int e2A = e2;
        try
        {
          try_expression();
          lk = -1;
        }
        catch (ParseException &)
        {
          lk = -2;
        }
        b0 = b0A; e0 = e0A; l1 = l1A; if (l1 == 0) {end = e0A;} else {
        b1 = b1A; e1 = e1A; l2 = l2A; if (l2 == 0) {end = e1A;} else {
        b2 = b2A; e2 = e2A; end = e2A; }}
        memoize(7, e0, lk);
      }
    }
    if (lk == -1
     || lk == 3                     // identifier
     || lk == 4                     // null
     || lk == 5                     // true
     || lk == 6                     // false
     || lk == 7                     // string
     || lk == 9                     // complex
     || lk == 10                    // real
     || lk == 11                    // comment
     || lk == 13                    // '!'
     || lk == 15                    // '#'
     || lk == 20                    // '('
     || lk == 22                    // '*'
     || lk == 25                    // '++'
     || lk == 29                    // '--'
     || lk == 47                    // '['
     || lk == 50                    // 'break'
     || lk == 52                    // 'char'
     || lk == 53                    // 'class'
     || lk == 54                    // 'continue'
     || lk == 56                    // 'do'
     || lk == 57                    // 'double'
     || lk == 63                    // 'float'
     || lk == 64                    // 'for'
     || lk == 65                    // 'if'
     || lk == 70                    // 'int'
     || lk == 71                    // 'long'
     || lk == 72                    // 'namespace'
     || lk == 73                    // 'private'
     || lk == 74                    // 'protected'
     || lk == 75                    // 'public'
     || lk == 76                    // 'return'
     || lk == 77                    // 'template'
     || lk == 78                    // 'throw'
     || lk == 79                    // 'try'
     || lk == 82                    // 'using'
     || lk == 83                    // 'void'
     || lk == 84                    // 'while'
     || lk == 85                    // '{'
     || lk == 89)                   // '~'
    {
      parse_expression();
    }
    lookahead1(10);                 // ';'
    consume(36);                    // ';'
    lookahead1(42);                 // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | '*' | '++' | '--' | ';' | '[' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '~'
    switch (l1)
    {
    case 36:                        // ';'
      lookahead2(44);               // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | ')' | '*' | '++' | '--' | ';' | '[' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '~'
      break;
    default:
      lk = l1;
      break;
    }
    if (lk == 4644)                 // ';' ';'
    {
      lk = memoized(8, e0);
      if (lk == 0)
      {
        int b0A = b0; int e0A = e0; int l1A = l1;
        int b1A = b1; int e1A = e1; int l2A = l2;
        int b2A = b2; int e2A = e2;
        try
        {
          try_expression();
          lk = -1;
        }
        catch (ParseException &)
        {
          lk = -2;
        }
        b0 = b0A; e0 = e0A; l1 = l1A; if (l1 == 0) {end = e0A;} else {
        b1 = b1A; e1 = e1A; l2 = l2A; if (l2 == 0) {end = e1A;} else {
        b2 = b2A; e2 = e2A; end = e2A; }}
        memoize(8, e0, lk);
      }
    }
    if (lk == -1
     || lk == 3                     // identifier
     || lk == 4                     // null
     || lk == 5                     // true
     || lk == 6                     // false
     || lk == 7                     // string
     || lk == 9                     // complex
     || lk == 10                    // real
     || lk == 11                    // comment
     || lk == 13                    // '!'
     || lk == 15                    // '#'
     || lk == 20                    // '('
     || lk == 22                    // '*'
     || lk == 25                    // '++'
     || lk == 29                    // '--'
     || lk == 47                    // '['
     || lk == 50                    // 'break'
     || lk == 52                    // 'char'
     || lk == 53                    // 'class'
     || lk == 54                    // 'continue'
     || lk == 56                    // 'do'
     || lk == 57                    // 'double'
     || lk == 63                    // 'float'
     || lk == 64                    // 'for'
     || lk == 65                    // 'if'
     || lk == 70                    // 'int'
     || lk == 71                    // 'long'
     || lk == 72                    // 'namespace'
     || lk == 73                    // 'private'
     || lk == 74                    // 'protected'
     || lk == 75                    // 'public'
     || lk == 76                    // 'return'
     || lk == 77                    // 'template'
     || lk == 78                    // 'throw'
     || lk == 79                    // 'try'
     || lk == 82                    // 'using'
     || lk == 83                    // 'void'
     || lk == 84                    // 'while'
     || lk == 85                    // '{'
     || lk == 89)                   // '~'
    {
      parse_expression();
    }
    lookahead1(10);                 // ';'
    consume(36);                    // ';'
    lookahead1(44);                 // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | ')' | '*' | '++' | '--' | ';' | '[' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '~'
    if (l1 != 21)                   // ')'
    {
      parse_expression();
    }
    lookahead1(6);                  // ')'
    consume(21);                    // ')'
    lookahead1(27);                 // '{'
    consume(85);                    // '{'
    for (;;)
    {
      lookahead1(46);               // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | '*' | '++' | '--' | ';' | '[' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '}' | '~'
      if (l1 == 88)                 // '}'
      {
        break;
      }
      parse_expression();
    }
    consume(88);                    // '}'
    eventHandler->endNonterminal(L"for", e0);
  }

  void try_for()
  {
    consumeT(64);                   // 'for'
    lookahead1(5);                  // '('
    consumeT(20);                   // '('
    lookahead1(42);                 // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | '*' | '++' | '--' | ';' | '[' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '~'
    switch (l1)
    {
    case 36:                        // ';'
      lookahead2(42);               // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | '*' | '++' | '--' | ';' | '[' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '~'
      break;
    default:
      lk = l1;
      break;
    }
    if (lk == 4644)                 // ';' ';'
    {
      lk = memoized(7, e0);
      if (lk == 0)
      {
        int b0A = b0; int e0A = e0; int l1A = l1;
        int b1A = b1; int e1A = e1; int l2A = l2;
        int b2A = b2; int e2A = e2;
        try
        {
          try_expression();
          memoize(7, e0A, -1);
        }
        catch (ParseException &)
        {
          b0 = b0A; e0 = e0A; l1 = l1A; if (l1 == 0) {end = e0A;} else {
          b1 = b1A; e1 = e1A; l2 = l2A; if (l2 == 0) {end = e1A;} else {
          b2 = b2A; e2 = e2A; end = e2A; }}
          memoize(7, e0A, -2);
        }
        lk = -2;
      }
    }
    if (lk == -1
     || lk == 3                     // identifier
     || lk == 4                     // null
     || lk == 5                     // true
     || lk == 6                     // false
     || lk == 7                     // string
     || lk == 9                     // complex
     || lk == 10                    // real
     || lk == 11                    // comment
     || lk == 13                    // '!'
     || lk == 15                    // '#'
     || lk == 20                    // '('
     || lk == 22                    // '*'
     || lk == 25                    // '++'
     || lk == 29                    // '--'
     || lk == 47                    // '['
     || lk == 50                    // 'break'
     || lk == 52                    // 'char'
     || lk == 53                    // 'class'
     || lk == 54                    // 'continue'
     || lk == 56                    // 'do'
     || lk == 57                    // 'double'
     || lk == 63                    // 'float'
     || lk == 64                    // 'for'
     || lk == 65                    // 'if'
     || lk == 70                    // 'int'
     || lk == 71                    // 'long'
     || lk == 72                    // 'namespace'
     || lk == 73                    // 'private'
     || lk == 74                    // 'protected'
     || lk == 75                    // 'public'
     || lk == 76                    // 'return'
     || lk == 77                    // 'template'
     || lk == 78                    // 'throw'
     || lk == 79                    // 'try'
     || lk == 82                    // 'using'
     || lk == 83                    // 'void'
     || lk == 84                    // 'while'
     || lk == 85                    // '{'
     || lk == 89)                   // '~'
    {
      try_expression();
    }
    lookahead1(10);                 // ';'
    consumeT(36);                   // ';'
    lookahead1(42);                 // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | '*' | '++' | '--' | ';' | '[' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '~'
    switch (l1)
    {
    case 36:                        // ';'
      lookahead2(44);               // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | ')' | '*' | '++' | '--' | ';' | '[' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '~'
      break;
    default:
      lk = l1;
      break;
    }
    if (lk == 4644)                 // ';' ';'
    {
      lk = memoized(8, e0);
      if (lk == 0)
      {
        int b0A = b0; int e0A = e0; int l1A = l1;
        int b1A = b1; int e1A = e1; int l2A = l2;
        int b2A = b2; int e2A = e2;
        try
        {
          try_expression();
          memoize(8, e0A, -1);
        }
        catch (ParseException &)
        {
          b0 = b0A; e0 = e0A; l1 = l1A; if (l1 == 0) {end = e0A;} else {
          b1 = b1A; e1 = e1A; l2 = l2A; if (l2 == 0) {end = e1A;} else {
          b2 = b2A; e2 = e2A; end = e2A; }}
          memoize(8, e0A, -2);
        }
        lk = -2;
      }
    }
    if (lk == -1
     || lk == 3                     // identifier
     || lk == 4                     // null
     || lk == 5                     // true
     || lk == 6                     // false
     || lk == 7                     // string
     || lk == 9                     // complex
     || lk == 10                    // real
     || lk == 11                    // comment
     || lk == 13                    // '!'
     || lk == 15                    // '#'
     || lk == 20                    // '('
     || lk == 22                    // '*'
     || lk == 25                    // '++'
     || lk == 29                    // '--'
     || lk == 47                    // '['
     || lk == 50                    // 'break'
     || lk == 52                    // 'char'
     || lk == 53                    // 'class'
     || lk == 54                    // 'continue'
     || lk == 56                    // 'do'
     || lk == 57                    // 'double'
     || lk == 63                    // 'float'
     || lk == 64                    // 'for'
     || lk == 65                    // 'if'
     || lk == 70                    // 'int'
     || lk == 71                    // 'long'
     || lk == 72                    // 'namespace'
     || lk == 73                    // 'private'
     || lk == 74                    // 'protected'
     || lk == 75                    // 'public'
     || lk == 76                    // 'return'
     || lk == 77                    // 'template'
     || lk == 78                    // 'throw'
     || lk == 79                    // 'try'
     || lk == 82                    // 'using'
     || lk == 83                    // 'void'
     || lk == 84                    // 'while'
     || lk == 85                    // '{'
     || lk == 89)                   // '~'
    {
      try_expression();
    }
    lookahead1(10);                 // ';'
    consumeT(36);                   // ';'
    lookahead1(44);                 // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | ')' | '*' | '++' | '--' | ';' | '[' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '~'
    if (l1 != 21)                   // ')'
    {
      try_expression();
    }
    lookahead1(6);                  // ')'
    consumeT(21);                   // ')'
    lookahead1(27);                 // '{'
    consumeT(85);                   // '{'
    for (;;)
    {
      lookahead1(46);               // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | '*' | '++' | '--' | ';' | '[' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '}' | '~'
      if (l1 == 88)                 // '}'
      {
        break;
      }
      try_expression();
    }
    consumeT(88);                   // '}'
  }

  void parse_try()
  {
    eventHandler->startNonterminal(L"try", e0);
    consume(79);                    // 'try'
    lookahead1(27);                 // '{'
    consume(85);                    // '{'
    for (;;)
    {
      lookahead1(46);               // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | '*' | '++' | '--' | ';' | '[' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '}' | '~'
      if (l1 == 88)                 // '}'
      {
        break;
      }
      parse_expression();
    }
    consume(88);                    // '}'
    lookahead1(51);                 // END | identifier | null | true | false | string | complex | real | comment |
                                    // lineTerminator | '!' | '#' | '(' | ')' | '*' | '++' | ',' | '--' | ';' | '[' |
                                    // ']' | 'break' | 'catch' | 'char' | 'class' | 'continue' | 'do' | 'double' |
                                    // 'float' | 'for' | 'if' | 'int' | 'long' | 'namespace' | 'private' | 'protected' |
                                    // 'public' | 'return' | 'template' | 'throw' | 'try' | 'using' | 'void' | 'while' |
                                    // '{' | '}' | '~'
    if (l1 == 51)                   // 'catch'
    {
      parse_catch();
    }
    eventHandler->endNonterminal(L"try", e0);
  }

  void try_try()
  {
    consumeT(79);                   // 'try'
    lookahead1(27);                 // '{'
    consumeT(85);                   // '{'
    for (;;)
    {
      lookahead1(46);               // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | '*' | '++' | '--' | ';' | '[' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '}' | '~'
      if (l1 == 88)                 // '}'
      {
        break;
      }
      try_expression();
    }
    consumeT(88);                   // '}'
    lookahead1(51);                 // END | identifier | null | true | false | string | complex | real | comment |
                                    // lineTerminator | '!' | '#' | '(' | ')' | '*' | '++' | ',' | '--' | ';' | '[' |
                                    // ']' | 'break' | 'catch' | 'char' | 'class' | 'continue' | 'do' | 'double' |
                                    // 'float' | 'for' | 'if' | 'int' | 'long' | 'namespace' | 'private' | 'protected' |
                                    // 'public' | 'return' | 'template' | 'throw' | 'try' | 'using' | 'void' | 'while' |
                                    // '{' | '}' | '~'
    if (l1 == 51)                   // 'catch'
    {
      try_catch();
    }
  }

  void parse_catch()
  {
    eventHandler->startNonterminal(L"catch", e0);
    consume(51);                    // 'catch'
    lookahead1(5);                  // '('
    consume(20);                    // '('
    parse_expression();
    lookahead1(6);                  // ')'
    consume(21);                    // ')'
    lookahead1(27);                 // '{'
    consume(85);                    // '{'
    for (;;)
    {
      lookahead1(46);               // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | '*' | '++' | '--' | ';' | '[' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '}' | '~'
      if (l1 == 88)                 // '}'
      {
        break;
      }
      parse_expression();
    }
    consume(88);                    // '}'
    eventHandler->endNonterminal(L"catch", e0);
  }

  void try_catch()
  {
    consumeT(51);                   // 'catch'
    lookahead1(5);                  // '('
    consumeT(20);                   // '('
    try_expression();
    lookahead1(6);                  // ')'
    consumeT(21);                   // ')'
    lookahead1(27);                 // '{'
    consumeT(85);                   // '{'
    for (;;)
    {
      lookahead1(46);               // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | '*' | '++' | '--' | ';' | '[' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '}' | '~'
      if (l1 == 88)                 // '}'
      {
        break;
      }
      try_expression();
    }
    consumeT(88);                   // '}'
  }

  void parse_break()
  {
    eventHandler->startNonterminal(L"break", e0);
    consume(50);                    // 'break'
    eventHandler->endNonterminal(L"break", e0);
  }

  void try_break()
  {
    consumeT(50);                   // 'break'
  }

  void parse_continue()
  {
    eventHandler->startNonterminal(L"continue", e0);
    consume(54);                    // 'continue'
    eventHandler->endNonterminal(L"continue", e0);
  }

  void try_continue()
  {
    consumeT(54);                   // 'continue'
  }

  void parse_return()
  {
    eventHandler->startNonterminal(L"return", e0);
    consume(76);                    // 'return'
    lookahead1(50);                 // END | identifier | null | true | false | string | complex | real | comment |
                                    // lineTerminator | '!' | '#' | '(' | ')' | '*' | '++' | ',' | '--' | ';' | '[' |
                                    // ']' | 'break' | 'char' | 'class' | 'continue' | 'do' | 'double' | 'float' |
                                    // 'for' | 'if' | 'int' | 'long' | 'namespace' | 'private' | 'protected' |
                                    // 'public' | 'return' | 'template' | 'throw' | 'try' | 'using' | 'void' | 'while' |
                                    // '{' | '}' | '~'
    switch (l1)
    {
    case 3:                         // identifier
      lookahead2(56);               // END | identifier | null | true | false | string | complex | real | comment |
                                    // lineTerminator | '!' | '!=' | '#' | '%' | '%=' | '&' | '&&' | '(' | ')' | '*' |
                                    // '*=' | '+' | '++' | '+=' | ',' | '-' | '--' | '-=' | '->' | '.' | '/' | '/=' |
                                    // ';' | '<' | '<<' | '<<=' | '<=' | '=' | '==' | '>' | '>=' | '>>' | '>>=' | '[' |
                                    // ']' | '^' | 'break' | 'char' | 'class' | 'continue' | 'do' | 'double' | 'float' |
                                    // 'for' | 'if' | 'int' | 'long' | 'namespace' | 'private' | 'protected' |
                                    // 'public' | 'return' | 'template' | 'throw' | 'try' | 'using' | 'void' | 'while' |
                                    // '{' | '|' | '||' | '}' | '~'
      break;
    case 15:                        // '#'
      lookahead2(37);               // 'define' | 'error' | 'if' | 'ifdef' | 'ifndef' | 'import' | 'include' | 'undef' |
                                    // 'using'
      break;
    case 47:                        // '['
      lookahead2(45);               // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | '*' | '++' | '--' | ';' | '[' | ']' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '~'
      break;
    case 77:                        // 'template'
      lookahead2(11);               // '<'
      break;
    case 82:                        // 'using'
      lookahead2(28);               // identifier | 'namespace'
      break;
    case 56:                        // 'do'
    case 79:                        // 'try'
      lookahead2(27);               // '{'
      break;
    case 20:                        // '('
    case 78:                        // 'throw'
    case 85:                        // '{'
      lookahead2(42);               // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | '*' | '++' | '--' | ';' | '[' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '~'
      break;
    case 64:                        // 'for'
    case 65:                        // 'if'
    case 84:                        // 'while'
      lookahead2(5);                // '('
      break;
    case 73:                        // 'private'
    case 74:                        // 'protected'
    case 75:                        // 'public'
      lookahead2(9);                // ':'
      break;
    case 11:                        // comment
    case 36:                        // ';'
    case 50:                        // 'break'
    case 54:                        // 'continue'
    case 76:                        // 'return'
      lookahead2(50);               // END | identifier | null | true | false | string | complex | real | comment |
                                    // lineTerminator | '!' | '#' | '(' | ')' | '*' | '++' | ',' | '--' | ';' | '[' |
                                    // ']' | 'break' | 'char' | 'class' | 'continue' | 'do' | 'double' | 'float' |
                                    // 'for' | 'if' | 'int' | 'long' | 'namespace' | 'private' | 'protected' |
                                    // 'public' | 'return' | 'template' | 'throw' | 'try' | 'using' | 'void' | 'while' |
                                    // '{' | '}' | '~'
      break;
    case 13:                        // '!'
    case 22:                        // '*'
    case 25:                        // '++'
    case 29:                        // '--'
    case 89:                        // '~'
      lookahead2(38);               // identifier | null | true | false | string | complex | real | '(' | '[' | '{'
      break;
    case 4:                         // null
    case 5:                         // true
    case 6:                         // false
    case 7:                         // string
    case 9:                         // complex
    case 10:                        // real
      lookahead2(53);               // END | identifier | null | true | false | string | complex | real | comment |
                                    // lineTerminator | '!' | '!=' | '#' | '%' | '%=' | '&' | '&&' | '(' | ')' | '*' |
                                    // '*=' | '+' | '++' | '+=' | ',' | '-' | '--' | '-=' | '/' | '/=' | ';' | '<' |
                                    // '<<' | '<<=' | '<=' | '=' | '==' | '>' | '>=' | '>>' | '>>=' | '[' | ']' | '^' |
                                    // 'break' | 'char' | 'class' | 'continue' | 'do' | 'double' | 'float' | 'for' |
                                    // 'if' | 'int' | 'long' | 'namespace' | 'private' | 'protected' | 'public' |
                                    // 'return' | 'template' | 'throw' | 'try' | 'using' | 'void' | 'while' | '{' |
                                    // '|' | '||' | '}' | '~'
      break;
    case 52:                        // 'char'
    case 53:                        // 'class'
    case 57:                        // 'double'
    case 63:                        // 'float'
    case 70:                        // 'int'
    case 71:                        // 'long'
    case 72:                        // 'namespace'
    case 83:                        // 'void'
      lookahead2(0);                // identifier
      break;
    default:
      lk = l1;
      break;
    }
    if (lk != 1                     // END
     && lk != 12                    // lineTerminator
     && lk != 21                    // ')'
     && lk != 27                    // ','
     && lk != 48                    // ']'
     && lk != 88                    // '}'
     && lk != 1539                  // identifier lineTerminator
     && lk != 1540                  // null lineTerminator
     && lk != 1541                  // true lineTerminator
     && lk != 1542                  // false lineTerminator
     && lk != 1543                  // string lineTerminator
     && lk != 1545                  // complex lineTerminator
     && lk != 1546                  // real lineTerminator
     && lk != 1547                  // comment lineTerminator
     && lk != 1572                  // ';' lineTerminator
     && lk != 1586                  // 'break' lineTerminator
     && lk != 1590                  // 'continue' lineTerminator
     && lk != 1612                  // 'return' lineTerminator
     && lk != 6147                  // identifier ']'
     && lk != 6148                  // null ']'
     && lk != 6149                  // true ']'
     && lk != 6150                  // false ']'
     && lk != 6151                  // string ']'
     && lk != 6153                  // complex ']'
     && lk != 6154                  // real ']'
     && lk != 6155                  // comment ']'
     && lk != 6180                  // ';' ']'
     && lk != 6194                  // 'break' ']'
     && lk != 6198                  // 'continue' ']'
     && lk != 6220)                 // 'return' ']'
    {
      lk = memoized(9, e0);
      if (lk == 0)
      {
        int b0A = b0; int e0A = e0; int l1A = l1;
        int b1A = b1; int e1A = e1; int l2A = l2;
        int b2A = b2; int e2A = e2;
        try
        {
          try_expression();
          lk = -1;
        }
        catch (ParseException &)
        {
          lk = -2;
        }
        b0 = b0A; e0 = e0A; l1 = l1A; if (l1 == 0) {end = e0A;} else {
        b1 = b1A; e1 = e1A; l2 = l2A; if (l2 == 0) {end = e1A;} else {
        b2 = b2A; e2 = e2A; end = e2A; }}
        memoize(9, e0, lk);
      }
    }
    if (lk != -2
     && lk != 1                     // END
     && lk != 12                    // lineTerminator
     && lk != 21                    // ')'
     && lk != 27                    // ','
     && lk != 48                    // ']'
     && lk != 88)                   // '}'
    {
      parse_expression();
    }
    eventHandler->endNonterminal(L"return", e0);
  }

  void try_return()
  {
    consumeT(76);                   // 'return'
    lookahead1(50);                 // END | identifier | null | true | false | string | complex | real | comment |
                                    // lineTerminator | '!' | '#' | '(' | ')' | '*' | '++' | ',' | '--' | ';' | '[' |
                                    // ']' | 'break' | 'char' | 'class' | 'continue' | 'do' | 'double' | 'float' |
                                    // 'for' | 'if' | 'int' | 'long' | 'namespace' | 'private' | 'protected' |
                                    // 'public' | 'return' | 'template' | 'throw' | 'try' | 'using' | 'void' | 'while' |
                                    // '{' | '}' | '~'
    switch (l1)
    {
    case 3:                         // identifier
      lookahead2(56);               // END | identifier | null | true | false | string | complex | real | comment |
                                    // lineTerminator | '!' | '!=' | '#' | '%' | '%=' | '&' | '&&' | '(' | ')' | '*' |
                                    // '*=' | '+' | '++' | '+=' | ',' | '-' | '--' | '-=' | '->' | '.' | '/' | '/=' |
                                    // ';' | '<' | '<<' | '<<=' | '<=' | '=' | '==' | '>' | '>=' | '>>' | '>>=' | '[' |
                                    // ']' | '^' | 'break' | 'char' | 'class' | 'continue' | 'do' | 'double' | 'float' |
                                    // 'for' | 'if' | 'int' | 'long' | 'namespace' | 'private' | 'protected' |
                                    // 'public' | 'return' | 'template' | 'throw' | 'try' | 'using' | 'void' | 'while' |
                                    // '{' | '|' | '||' | '}' | '~'
      break;
    case 15:                        // '#'
      lookahead2(37);               // 'define' | 'error' | 'if' | 'ifdef' | 'ifndef' | 'import' | 'include' | 'undef' |
                                    // 'using'
      break;
    case 47:                        // '['
      lookahead2(45);               // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | '*' | '++' | '--' | ';' | '[' | ']' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '~'
      break;
    case 77:                        // 'template'
      lookahead2(11);               // '<'
      break;
    case 82:                        // 'using'
      lookahead2(28);               // identifier | 'namespace'
      break;
    case 56:                        // 'do'
    case 79:                        // 'try'
      lookahead2(27);               // '{'
      break;
    case 20:                        // '('
    case 78:                        // 'throw'
    case 85:                        // '{'
      lookahead2(42);               // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | '*' | '++' | '--' | ';' | '[' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '~'
      break;
    case 64:                        // 'for'
    case 65:                        // 'if'
    case 84:                        // 'while'
      lookahead2(5);                // '('
      break;
    case 73:                        // 'private'
    case 74:                        // 'protected'
    case 75:                        // 'public'
      lookahead2(9);                // ':'
      break;
    case 11:                        // comment
    case 36:                        // ';'
    case 50:                        // 'break'
    case 54:                        // 'continue'
    case 76:                        // 'return'
      lookahead2(50);               // END | identifier | null | true | false | string | complex | real | comment |
                                    // lineTerminator | '!' | '#' | '(' | ')' | '*' | '++' | ',' | '--' | ';' | '[' |
                                    // ']' | 'break' | 'char' | 'class' | 'continue' | 'do' | 'double' | 'float' |
                                    // 'for' | 'if' | 'int' | 'long' | 'namespace' | 'private' | 'protected' |
                                    // 'public' | 'return' | 'template' | 'throw' | 'try' | 'using' | 'void' | 'while' |
                                    // '{' | '}' | '~'
      break;
    case 13:                        // '!'
    case 22:                        // '*'
    case 25:                        // '++'
    case 29:                        // '--'
    case 89:                        // '~'
      lookahead2(38);               // identifier | null | true | false | string | complex | real | '(' | '[' | '{'
      break;
    case 4:                         // null
    case 5:                         // true
    case 6:                         // false
    case 7:                         // string
    case 9:                         // complex
    case 10:                        // real
      lookahead2(53);               // END | identifier | null | true | false | string | complex | real | comment |
                                    // lineTerminator | '!' | '!=' | '#' | '%' | '%=' | '&' | '&&' | '(' | ')' | '*' |
                                    // '*=' | '+' | '++' | '+=' | ',' | '-' | '--' | '-=' | '/' | '/=' | ';' | '<' |
                                    // '<<' | '<<=' | '<=' | '=' | '==' | '>' | '>=' | '>>' | '>>=' | '[' | ']' | '^' |
                                    // 'break' | 'char' | 'class' | 'continue' | 'do' | 'double' | 'float' | 'for' |
                                    // 'if' | 'int' | 'long' | 'namespace' | 'private' | 'protected' | 'public' |
                                    // 'return' | 'template' | 'throw' | 'try' | 'using' | 'void' | 'while' | '{' |
                                    // '|' | '||' | '}' | '~'
      break;
    case 52:                        // 'char'
    case 53:                        // 'class'
    case 57:                        // 'double'
    case 63:                        // 'float'
    case 70:                        // 'int'
    case 71:                        // 'long'
    case 72:                        // 'namespace'
    case 83:                        // 'void'
      lookahead2(0);                // identifier
      break;
    default:
      lk = l1;
      break;
    }
    if (lk != 1                     // END
     && lk != 12                    // lineTerminator
     && lk != 21                    // ')'
     && lk != 27                    // ','
     && lk != 48                    // ']'
     && lk != 88                    // '}'
     && lk != 1539                  // identifier lineTerminator
     && lk != 1540                  // null lineTerminator
     && lk != 1541                  // true lineTerminator
     && lk != 1542                  // false lineTerminator
     && lk != 1543                  // string lineTerminator
     && lk != 1545                  // complex lineTerminator
     && lk != 1546                  // real lineTerminator
     && lk != 1547                  // comment lineTerminator
     && lk != 1572                  // ';' lineTerminator
     && lk != 1586                  // 'break' lineTerminator
     && lk != 1590                  // 'continue' lineTerminator
     && lk != 1612                  // 'return' lineTerminator
     && lk != 6147                  // identifier ']'
     && lk != 6148                  // null ']'
     && lk != 6149                  // true ']'
     && lk != 6150                  // false ']'
     && lk != 6151                  // string ']'
     && lk != 6153                  // complex ']'
     && lk != 6154                  // real ']'
     && lk != 6155                  // comment ']'
     && lk != 6180                  // ';' ']'
     && lk != 6194                  // 'break' ']'
     && lk != 6198                  // 'continue' ']'
     && lk != 6220)                 // 'return' ']'
    {
      lk = memoized(9, e0);
      if (lk == 0)
      {
        int b0A = b0; int e0A = e0; int l1A = l1;
        int b1A = b1; int e1A = e1; int l2A = l2;
        int b2A = b2; int e2A = e2;
        try
        {
          try_expression();
          memoize(9, e0A, -1);
        }
        catch (ParseException &)
        {
          b0 = b0A; e0 = e0A; l1 = l1A; if (l1 == 0) {end = e0A;} else {
          b1 = b1A; e1 = e1A; l2 = l2A; if (l2 == 0) {end = e1A;} else {
          b2 = b2A; e2 = e2A; end = e2A; }}
          memoize(9, e0A, -2);
        }
        lk = -2;
      }
    }
    if (lk != -2
     && lk != 1                     // END
     && lk != 12                    // lineTerminator
     && lk != 21                    // ')'
     && lk != 27                    // ','
     && lk != 48                    // ']'
     && lk != 88)                   // '}'
    {
      try_expression();
    }
  }

  void parse_throw()
  {
    eventHandler->startNonterminal(L"throw", e0);
    consume(78);                    // 'throw'
    parse_expression();
    eventHandler->endNonterminal(L"throw", e0);
  }

  void try_throw()
  {
    consumeT(78);                   // 'throw'
    try_expression();
  }

  void parse_type()
  {
    eventHandler->startNonterminal(L"type", e0);
    switch (l1)
    {
    case 83:                        // 'void'
      consume(83);                  // 'void'
      break;
    case 52:                        // 'char'
      consume(52);                  // 'char'
      break;
    case 70:                        // 'int'
      consume(70);                  // 'int'
      break;
    case 71:                        // 'long'
      consume(71);                  // 'long'
      break;
    case 63:                        // 'float'
      consume(63);                  // 'float'
      break;
    default:
      consume(57);                  // 'double'
      break;
    }
    eventHandler->endNonterminal(L"type", e0);
  }

  void try_type()
  {
    switch (l1)
    {
    case 83:                        // 'void'
      consumeT(83);                 // 'void'
      break;
    case 52:                        // 'char'
      consumeT(52);                 // 'char'
      break;
    case 70:                        // 'int'
      consumeT(70);                 // 'int'
      break;
    case 71:                        // 'long'
      consumeT(71);                 // 'long'
      break;
    case 63:                        // 'float'
      consumeT(63);                 // 'float'
      break;
    default:
      consumeT(57);                 // 'double'
      break;
    }
  }

  void parse_expression()
  {
    eventHandler->startNonterminal(L"expression", e0);
    lookahead1(42);                 // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | '*' | '++' | '--' | ';' | '[' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '~'
    switch (l1)
    {
    case 3:                         // identifier
    case 4:                         // null
    case 5:                         // true
    case 6:                         // false
    case 7:                         // string
    case 9:                         // complex
    case 10:                        // real
    case 13:                        // '!'
    case 20:                        // '('
    case 22:                        // '*'
    case 25:                        // '++'
    case 29:                        // '--'
    case 47:                        // '['
    case 85:                        // '{'
    case 89:                        // '~'
      parse_operation();
      break;
    case 11:                        // comment
      consume(11);                  // comment
      break;
    case 36:                        // ';'
      consume(36);                  // ';'
      break;
    default:
      parse_statement();
      break;
    }
    eventHandler->endNonterminal(L"expression", e0);
  }

  void try_expression()
  {
    lookahead1(42);                 // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | '*' | '++' | '--' | ';' | '[' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '~'
    switch (l1)
    {
    case 3:                         // identifier
    case 4:                         // null
    case 5:                         // true
    case 6:                         // false
    case 7:                         // string
    case 9:                         // complex
    case 10:                        // real
    case 13:                        // '!'
    case 20:                        // '('
    case 22:                        // '*'
    case 25:                        // '++'
    case 29:                        // '--'
    case 47:                        // '['
    case 85:                        // '{'
    case 89:                        // '~'
      try_operation();
      break;
    case 11:                        // comment
      consumeT(11);                 // comment
      break;
    case 36:                        // ';'
      consumeT(36);                 // ';'
      break;
    default:
      try_statement();
      break;
    }
  }

  void parse_arguments()
  {
    eventHandler->startNonterminal(L"arguments", e0);
    parse_expression();
    for (;;)
    {
      lookahead1(49);               // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | ')' | '*' | '++' | ',' | '--' | ';' | '[' | ']' | 'break' | 'char' |
                                    // 'class' | 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' |
                                    // 'long' | 'namespace' | 'private' | 'protected' | 'public' | 'return' |
                                    // 'template' | 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '~'
      if (l1 != 27)                 // ','
      {
        break;
      }
      consume(27);                  // ','
      parse_expression();
    }
    eventHandler->endNonterminal(L"arguments", e0);
  }

  void try_arguments()
  {
    try_expression();
    for (;;)
    {
      lookahead1(49);               // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | ')' | '*' | '++' | ',' | '--' | ';' | '[' | ']' | 'break' | 'char' |
                                    // 'class' | 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' |
                                    // 'long' | 'namespace' | 'private' | 'protected' | 'public' | 'return' |
                                    // 'template' | 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '~'
      if (l1 != 27)                 // ','
      {
        break;
      }
      consumeT(27);                 // ','
      try_expression();
    }
  }

  void parse_member()
  {
    eventHandler->startNonterminal(L"member", e0);
    switch (l1)
    {
    case 3:                         // identifier
      lookahead2(56);               // END | identifier | null | true | false | string | complex | real | comment |
                                    // lineTerminator | '!' | '!=' | '#' | '%' | '%=' | '&' | '&&' | '(' | ')' | '*' |
                                    // '*=' | '+' | '++' | '+=' | ',' | '-' | '--' | '-=' | '->' | '.' | '/' | '/=' |
                                    // ';' | '<' | '<<' | '<<=' | '<=' | '=' | '==' | '>' | '>=' | '>>' | '>>=' | '[' |
                                    // ']' | '^' | 'break' | 'char' | 'class' | 'continue' | 'do' | 'double' | 'float' |
                                    // 'for' | 'if' | 'int' | 'long' | 'namespace' | 'private' | 'protected' |
                                    // 'public' | 'return' | 'template' | 'throw' | 'try' | 'using' | 'void' | 'while' |
                                    // '{' | '|' | '||' | '}' | '~'
      break;
    default:
      lk = l1;
      break;
    }
    lk = memoized(10, e0);
    if (lk == 0)
    {
      int b0A = b0; int e0A = e0; int l1A = l1;
      int b1A = b1; int e1A = e1; int l2A = l2;
      int b2A = b2; int e2A = e2;
      try
      {
        consumeT(3);                // identifier
        for (;;)
        {
          lookahead1(30);           // '(' | '.'
          if (l1 != 32)             // '.'
          {
            break;
          }
          consumeT(32);             // '.'
          lookahead1(0);            // identifier
          consumeT(3);              // identifier
        }
        consumeT(20);               // '('
        for (;;)
        {
          lookahead1(44);           // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | ')' | '*' | '++' | '--' | ';' | '[' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '~'
          if (l1 == 21)             // ')'
          {
            break;
          }
          try_arguments();
        }
        consumeT(21);               // ')'
        lk = -1;
      }
      catch (ParseException &)
      {
        try
        {
          b0 = b0A; e0 = e0A; l1 = l1A; if (l1 == 0) {end = e0A;} else {
          b1 = b1A; e1 = e1A; l2 = l2A; if (l2 == 0) {end = e1A;} else {
          b2 = b2A; e2 = e2A; end = e2A; }}
          consumeT(3);              // identifier
          for (;;)
          {
            lookahead1(55);         // END | identifier | null | true | false | string | complex | real | comment |
                                    // lineTerminator | '!' | '!=' | '#' | '%' | '%=' | '&' | '&&' | '(' | ')' | '*' |
                                    // '*=' | '+' | '++' | '+=' | ',' | '-' | '--' | '-=' | '.' | '/' | '/=' | ';' |
                                    // '<' | '<<' | '<<=' | '<=' | '=' | '==' | '>' | '>=' | '>>' | '>>=' | '[' | ']' |
                                    // '^' | 'break' | 'char' | 'class' | 'continue' | 'do' | 'double' | 'float' |
                                    // 'for' | 'if' | 'int' | 'long' | 'namespace' | 'private' | 'protected' |
                                    // 'public' | 'return' | 'template' | 'throw' | 'try' | 'using' | 'void' | 'while' |
                                    // '{' | '|' | '||' | '}' | '~'
            if (l1 != 32)           // '.'
            {
              break;
            }
            consumeT(32);           // '.'
            lookahead1(0);          // identifier
            consumeT(3);            // identifier
          }
          for (;;)
          {
            lookahead1(53);         // END | identifier | null | true | false | string | complex | real | comment |
                                    // lineTerminator | '!' | '!=' | '#' | '%' | '%=' | '&' | '&&' | '(' | ')' | '*' |
                                    // '*=' | '+' | '++' | '+=' | ',' | '-' | '--' | '-=' | '/' | '/=' | ';' | '<' |
                                    // '<<' | '<<=' | '<=' | '=' | '==' | '>' | '>=' | '>>' | '>>=' | '[' | ']' | '^' |
                                    // 'break' | 'char' | 'class' | 'continue' | 'do' | 'double' | 'float' | 'for' |
                                    // 'if' | 'int' | 'long' | 'namespace' | 'private' | 'protected' | 'public' |
                                    // 'return' | 'template' | 'throw' | 'try' | 'using' | 'void' | 'while' | '{' |
                                    // '|' | '||' | '}' | '~'
            switch (l1)
            {
            case 47:                // '['
              lookahead2(45);       // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | '*' | '++' | '--' | ';' | '[' | ']' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '~'
              break;
            default:
              lk = l1;
              break;
            }
            if (lk == 431           // '[' identifier
             || lk == 559           // '[' null
             || lk == 687           // '[' true
             || lk == 815           // '[' false
             || lk == 943           // '[' string
             || lk == 1199          // '[' complex
             || lk == 1327          // '[' real
             || lk == 1455          // '[' comment
             || lk == 1711          // '[' '!'
             || lk == 1967          // '[' '#'
             || lk == 2607          // '[' '('
             || lk == 2863          // '[' '*'
             || lk == 3247          // '[' '++'
             || lk == 3759          // '[' '--'
             || lk == 4655          // '[' ';'
             || lk == 6063          // '[' '['
             || lk == 6447          // '[' 'break'
             || lk == 6703          // '[' 'char'
             || lk == 6831          // '[' 'class'
             || lk == 6959          // '[' 'continue'
             || lk == 7215          // '[' 'do'
             || lk == 7343          // '[' 'double'
             || lk == 8111          // '[' 'float'
             || lk == 8239          // '[' 'for'
             || lk == 8367          // '[' 'if'
             || lk == 9007          // '[' 'int'
             || lk == 9135          // '[' 'long'
             || lk == 9263          // '[' 'namespace'
             || lk == 9391          // '[' 'private'
             || lk == 9519          // '[' 'protected'
             || lk == 9647          // '[' 'public'
             || lk == 9775          // '[' 'return'
             || lk == 9903          // '[' 'template'
             || lk == 10031         // '[' 'throw'
             || lk == 10159         // '[' 'try'
             || lk == 10543         // '[' 'using'
             || lk == 10671         // '[' 'void'
             || lk == 10799         // '[' 'while'
             || lk == 10927         // '[' '{'
             || lk == 11439)        // '[' '~'
            {
              lk = memoized(11, e0);
              if (lk == 0)
              {
                int b0B = b0; int e0B = e0; int l1B = l1;
                int b1B = b1; int e1B = e1; int l2B = l2;
                int b2B = b2; int e2B = e2;
                try
                {
                  consumeT(47);     // '['
                  try_arguments();
                  consumeT(48);     // ']'
                  memoize(11, e0B, -1);
                  continue;
                }
                catch (ParseException &)
                {
                  b0 = b0B; e0 = e0B; l1 = l1B; if (l1 == 0) {end = e0B;} else {
                  b1 = b1B; e1 = e1B; l2 = l2B; if (l2 == 0) {end = e1B;} else {
                  b2 = b2B; e2 = e2B; end = e2B; }}
                  memoize(11, e0B, -2);
                  break;
                }
              }
            }
            if (lk != -1)
            {
              break;
            }
            consumeT(47);           // '['
            try_arguments();
            consumeT(48);           // ']'
          }
          lk = -2;
        }
        catch (ParseException &)
        {
          try
          {
            b0 = b0A; e0 = e0A; l1 = l1A; if (l1 == 0) {end = e0A;} else {
            b1 = b1A; e1 = e1A; l2 = l2A; if (l2 == 0) {end = e1A;} else {
            b2 = b2A; e2 = e2A; end = e2A; }}
            consumeT(3);            // identifier
            for (;;)
            {
              lookahead1(29);       // '(' | '->'
              if (l1 != 31)         // '->'
              {
                break;
              }
              consumeT(31);         // '->'
              lookahead1(0);        // identifier
              consumeT(3);          // identifier
            }
            consumeT(20);           // '('
            for (;;)
            {
              lookahead1(44);       // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | ')' | '*' | '++' | '--' | ';' | '[' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '~'
              if (l1 == 21)         // ')'
              {
                break;
              }
              try_arguments();
            }
            consumeT(21);           // ')'
            lk = -3;
          }
          catch (ParseException &)
          {
            lk = -4;
          }
        }
      }
      b0 = b0A; e0 = e0A; l1 = l1A; if (l1 == 0) {end = e0A;} else {
      b1 = b1A; e1 = e1A; l2 = l2A; if (l2 == 0) {end = e1A;} else {
      b2 = b2A; e2 = e2A; end = e2A; }}
      memoize(10, e0, lk);
    }
    switch (lk)
    {
    case -1:
      consume(3);                   // identifier
      for (;;)
      {
        lookahead1(30);             // '(' | '.'
        if (l1 != 32)               // '.'
        {
          break;
        }
        consume(32);                // '.'
        lookahead1(0);              // identifier
        consume(3);                 // identifier
      }
      consume(20);                  // '('
      for (;;)
      {
        lookahead1(44);             // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | ')' | '*' | '++' | '--' | ';' | '[' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '~'
        if (l1 == 21)               // ')'
        {
          break;
        }
        parse_arguments();
      }
      consume(21);                  // ')'
      break;
    case -2:
      consume(3);                   // identifier
      for (;;)
      {
        lookahead1(55);             // END | identifier | null | true | false | string | complex | real | comment |
                                    // lineTerminator | '!' | '!=' | '#' | '%' | '%=' | '&' | '&&' | '(' | ')' | '*' |
                                    // '*=' | '+' | '++' | '+=' | ',' | '-' | '--' | '-=' | '.' | '/' | '/=' | ';' |
                                    // '<' | '<<' | '<<=' | '<=' | '=' | '==' | '>' | '>=' | '>>' | '>>=' | '[' | ']' |
                                    // '^' | 'break' | 'char' | 'class' | 'continue' | 'do' | 'double' | 'float' |
                                    // 'for' | 'if' | 'int' | 'long' | 'namespace' | 'private' | 'protected' |
                                    // 'public' | 'return' | 'template' | 'throw' | 'try' | 'using' | 'void' | 'while' |
                                    // '{' | '|' | '||' | '}' | '~'
        if (l1 != 32)               // '.'
        {
          break;
        }
        consume(32);                // '.'
        lookahead1(0);              // identifier
        consume(3);                 // identifier
      }
      for (;;)
      {
        lookahead1(53);             // END | identifier | null | true | false | string | complex | real | comment |
                                    // lineTerminator | '!' | '!=' | '#' | '%' | '%=' | '&' | '&&' | '(' | ')' | '*' |
                                    // '*=' | '+' | '++' | '+=' | ',' | '-' | '--' | '-=' | '/' | '/=' | ';' | '<' |
                                    // '<<' | '<<=' | '<=' | '=' | '==' | '>' | '>=' | '>>' | '>>=' | '[' | ']' | '^' |
                                    // 'break' | 'char' | 'class' | 'continue' | 'do' | 'double' | 'float' | 'for' |
                                    // 'if' | 'int' | 'long' | 'namespace' | 'private' | 'protected' | 'public' |
                                    // 'return' | 'template' | 'throw' | 'try' | 'using' | 'void' | 'while' | '{' |
                                    // '|' | '||' | '}' | '~'
        switch (l1)
        {
        case 47:                    // '['
          lookahead2(45);           // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | '*' | '++' | '--' | ';' | '[' | ']' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '~'
          break;
        default:
          lk = l1;
          break;
        }
        if (lk == 431               // '[' identifier
         || lk == 559               // '[' null
         || lk == 687               // '[' true
         || lk == 815               // '[' false
         || lk == 943               // '[' string
         || lk == 1199              // '[' complex
         || lk == 1327              // '[' real
         || lk == 1455              // '[' comment
         || lk == 1711              // '[' '!'
         || lk == 1967              // '[' '#'
         || lk == 2607              // '[' '('
         || lk == 2863              // '[' '*'
         || lk == 3247              // '[' '++'
         || lk == 3759              // '[' '--'
         || lk == 4655              // '[' ';'
         || lk == 6063              // '[' '['
         || lk == 6447              // '[' 'break'
         || lk == 6703              // '[' 'char'
         || lk == 6831              // '[' 'class'
         || lk == 6959              // '[' 'continue'
         || lk == 7215              // '[' 'do'
         || lk == 7343              // '[' 'double'
         || lk == 8111              // '[' 'float'
         || lk == 8239              // '[' 'for'
         || lk == 8367              // '[' 'if'
         || lk == 9007              // '[' 'int'
         || lk == 9135              // '[' 'long'
         || lk == 9263              // '[' 'namespace'
         || lk == 9391              // '[' 'private'
         || lk == 9519              // '[' 'protected'
         || lk == 9647              // '[' 'public'
         || lk == 9775              // '[' 'return'
         || lk == 9903              // '[' 'template'
         || lk == 10031             // '[' 'throw'
         || lk == 10159             // '[' 'try'
         || lk == 10543             // '[' 'using'
         || lk == 10671             // '[' 'void'
         || lk == 10799             // '[' 'while'
         || lk == 10927             // '[' '{'
         || lk == 11439)            // '[' '~'
        {
          lk = memoized(11, e0);
          if (lk == 0)
          {
            int b0B = b0; int e0B = e0; int l1B = l1;
            int b1B = b1; int e1B = e1; int l2B = l2;
            int b2B = b2; int e2B = e2;
            try
            {
              consumeT(47);         // '['
              try_arguments();
              consumeT(48);         // ']'
              lk = -1;
            }
            catch (ParseException &)
            {
              lk = -2;
            }
            b0 = b0B; e0 = e0B; l1 = l1B; if (l1 == 0) {end = e0B;} else {
            b1 = b1B; e1 = e1B; l2 = l2B; if (l2 == 0) {end = e1B;} else {
            b2 = b2B; e2 = e2B; end = e2B; }}
            memoize(11, e0, lk);
          }
        }
        if (lk != -1)
        {
          break;
        }
        consume(47);                // '['
        parse_arguments();
        consume(48);                // ']'
      }
      break;
    case -3:
      consume(3);                   // identifier
      for (;;)
      {
        lookahead1(29);             // '(' | '->'
        if (l1 != 31)               // '->'
        {
          break;
        }
        consume(31);                // '->'
        lookahead1(0);              // identifier
        consume(3);                 // identifier
      }
      consume(20);                  // '('
      for (;;)
      {
        lookahead1(44);             // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | ')' | '*' | '++' | '--' | ';' | '[' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '~'
        if (l1 == 21)               // ')'
        {
          break;
        }
        parse_arguments();
      }
      consume(21);                  // ')'
      break;
    default:
      consume(3);                   // identifier
      for (;;)
      {
        lookahead1(54);             // END | identifier | null | true | false | string | complex | real | comment |
                                    // lineTerminator | '!' | '!=' | '#' | '%' | '%=' | '&' | '&&' | '(' | ')' | '*' |
                                    // '*=' | '+' | '++' | '+=' | ',' | '-' | '--' | '-=' | '->' | '/' | '/=' | ';' |
                                    // '<' | '<<' | '<<=' | '<=' | '=' | '==' | '>' | '>=' | '>>' | '>>=' | '[' | ']' |
                                    // '^' | 'break' | 'char' | 'class' | 'continue' | 'do' | 'double' | 'float' |
                                    // 'for' | 'if' | 'int' | 'long' | 'namespace' | 'private' | 'protected' |
                                    // 'public' | 'return' | 'template' | 'throw' | 'try' | 'using' | 'void' | 'while' |
                                    // '{' | '|' | '||' | '}' | '~'
        if (l1 != 31)               // '->'
        {
          break;
        }
        consume(31);                // '->'
        lookahead1(0);              // identifier
        consume(3);                 // identifier
      }
      for (;;)
      {
        lookahead1(53);             // END | identifier | null | true | false | string | complex | real | comment |
                                    // lineTerminator | '!' | '!=' | '#' | '%' | '%=' | '&' | '&&' | '(' | ')' | '*' |
                                    // '*=' | '+' | '++' | '+=' | ',' | '-' | '--' | '-=' | '/' | '/=' | ';' | '<' |
                                    // '<<' | '<<=' | '<=' | '=' | '==' | '>' | '>=' | '>>' | '>>=' | '[' | ']' | '^' |
                                    // 'break' | 'char' | 'class' | 'continue' | 'do' | 'double' | 'float' | 'for' |
                                    // 'if' | 'int' | 'long' | 'namespace' | 'private' | 'protected' | 'public' |
                                    // 'return' | 'template' | 'throw' | 'try' | 'using' | 'void' | 'while' | '{' |
                                    // '|' | '||' | '}' | '~'
        switch (l1)
        {
        case 47:                    // '['
          lookahead2(45);           // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | '*' | '++' | '--' | ';' | '[' | ']' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '~'
          break;
        default:
          lk = l1;
          break;
        }
        if (lk == 431               // '[' identifier
         || lk == 559               // '[' null
         || lk == 687               // '[' true
         || lk == 815               // '[' false
         || lk == 943               // '[' string
         || lk == 1199              // '[' complex
         || lk == 1327              // '[' real
         || lk == 1455              // '[' comment
         || lk == 1711              // '[' '!'
         || lk == 1967              // '[' '#'
         || lk == 2607              // '[' '('
         || lk == 2863              // '[' '*'
         || lk == 3247              // '[' '++'
         || lk == 3759              // '[' '--'
         || lk == 4655              // '[' ';'
         || lk == 6063              // '[' '['
         || lk == 6447              // '[' 'break'
         || lk == 6703              // '[' 'char'
         || lk == 6831              // '[' 'class'
         || lk == 6959              // '[' 'continue'
         || lk == 7215              // '[' 'do'
         || lk == 7343              // '[' 'double'
         || lk == 8111              // '[' 'float'
         || lk == 8239              // '[' 'for'
         || lk == 8367              // '[' 'if'
         || lk == 9007              // '[' 'int'
         || lk == 9135              // '[' 'long'
         || lk == 9263              // '[' 'namespace'
         || lk == 9391              // '[' 'private'
         || lk == 9519              // '[' 'protected'
         || lk == 9647              // '[' 'public'
         || lk == 9775              // '[' 'return'
         || lk == 9903              // '[' 'template'
         || lk == 10031             // '[' 'throw'
         || lk == 10159             // '[' 'try'
         || lk == 10543             // '[' 'using'
         || lk == 10671             // '[' 'void'
         || lk == 10799             // '[' 'while'
         || lk == 10927             // '[' '{'
         || lk == 11439)            // '[' '~'
        {
          lk = memoized(12, e0);
          if (lk == 0)
          {
            int b0B = b0; int e0B = e0; int l1B = l1;
            int b1B = b1; int e1B = e1; int l2B = l2;
            int b2B = b2; int e2B = e2;
            try
            {
              consumeT(47);         // '['
              try_arguments();
              consumeT(48);         // ']'
              lk = -1;
            }
            catch (ParseException &)
            {
              lk = -2;
            }
            b0 = b0B; e0 = e0B; l1 = l1B; if (l1 == 0) {end = e0B;} else {
            b1 = b1B; e1 = e1B; l2 = l2B; if (l2 == 0) {end = e1B;} else {
            b2 = b2B; e2 = e2B; end = e2B; }}
            memoize(12, e0, lk);
          }
        }
        if (lk != -1)
        {
          break;
        }
        consume(47);                // '['
        parse_arguments();
        consume(48);                // ']'
      }
      break;
    }
    eventHandler->endNonterminal(L"member", e0);
  }

  void try_member()
  {
    switch (l1)
    {
    case 3:                         // identifier
      lookahead2(56);               // END | identifier | null | true | false | string | complex | real | comment |
                                    // lineTerminator | '!' | '!=' | '#' | '%' | '%=' | '&' | '&&' | '(' | ')' | '*' |
                                    // '*=' | '+' | '++' | '+=' | ',' | '-' | '--' | '-=' | '->' | '.' | '/' | '/=' |
                                    // ';' | '<' | '<<' | '<<=' | '<=' | '=' | '==' | '>' | '>=' | '>>' | '>>=' | '[' |
                                    // ']' | '^' | 'break' | 'char' | 'class' | 'continue' | 'do' | 'double' | 'float' |
                                    // 'for' | 'if' | 'int' | 'long' | 'namespace' | 'private' | 'protected' |
                                    // 'public' | 'return' | 'template' | 'throw' | 'try' | 'using' | 'void' | 'while' |
                                    // '{' | '|' | '||' | '}' | '~'
      break;
    default:
      lk = l1;
      break;
    }
    lk = memoized(10, e0);
    if (lk == 0)
    {
      int b0A = b0; int e0A = e0; int l1A = l1;
      int b1A = b1; int e1A = e1; int l2A = l2;
      int b2A = b2; int e2A = e2;
      try
      {
        consumeT(3);                // identifier
        for (;;)
        {
          lookahead1(30);           // '(' | '.'
          if (l1 != 32)             // '.'
          {
            break;
          }
          consumeT(32);             // '.'
          lookahead1(0);            // identifier
          consumeT(3);              // identifier
        }
        consumeT(20);               // '('
        for (;;)
        {
          lookahead1(44);           // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | ')' | '*' | '++' | '--' | ';' | '[' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '~'
          if (l1 == 21)             // ')'
          {
            break;
          }
          try_arguments();
        }
        consumeT(21);               // ')'
        memoize(10, e0A, -1);
        lk = -5;
      }
      catch (ParseException &)
      {
        try
        {
          b0 = b0A; e0 = e0A; l1 = l1A; if (l1 == 0) {end = e0A;} else {
          b1 = b1A; e1 = e1A; l2 = l2A; if (l2 == 0) {end = e1A;} else {
          b2 = b2A; e2 = e2A; end = e2A; }}
          consumeT(3);              // identifier
          for (;;)
          {
            lookahead1(55);         // END | identifier | null | true | false | string | complex | real | comment |
                                    // lineTerminator | '!' | '!=' | '#' | '%' | '%=' | '&' | '&&' | '(' | ')' | '*' |
                                    // '*=' | '+' | '++' | '+=' | ',' | '-' | '--' | '-=' | '.' | '/' | '/=' | ';' |
                                    // '<' | '<<' | '<<=' | '<=' | '=' | '==' | '>' | '>=' | '>>' | '>>=' | '[' | ']' |
                                    // '^' | 'break' | 'char' | 'class' | 'continue' | 'do' | 'double' | 'float' |
                                    // 'for' | 'if' | 'int' | 'long' | 'namespace' | 'private' | 'protected' |
                                    // 'public' | 'return' | 'template' | 'throw' | 'try' | 'using' | 'void' | 'while' |
                                    // '{' | '|' | '||' | '}' | '~'
            if (l1 != 32)           // '.'
            {
              break;
            }
            consumeT(32);           // '.'
            lookahead1(0);          // identifier
            consumeT(3);            // identifier
          }
          for (;;)
          {
            lookahead1(53);         // END | identifier | null | true | false | string | complex | real | comment |
                                    // lineTerminator | '!' | '!=' | '#' | '%' | '%=' | '&' | '&&' | '(' | ')' | '*' |
                                    // '*=' | '+' | '++' | '+=' | ',' | '-' | '--' | '-=' | '/' | '/=' | ';' | '<' |
                                    // '<<' | '<<=' | '<=' | '=' | '==' | '>' | '>=' | '>>' | '>>=' | '[' | ']' | '^' |
                                    // 'break' | 'char' | 'class' | 'continue' | 'do' | 'double' | 'float' | 'for' |
                                    // 'if' | 'int' | 'long' | 'namespace' | 'private' | 'protected' | 'public' |
                                    // 'return' | 'template' | 'throw' | 'try' | 'using' | 'void' | 'while' | '{' |
                                    // '|' | '||' | '}' | '~'
            switch (l1)
            {
            case 47:                // '['
              lookahead2(45);       // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | '*' | '++' | '--' | ';' | '[' | ']' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '~'
              break;
            default:
              lk = l1;
              break;
            }
            if (lk == 431           // '[' identifier
             || lk == 559           // '[' null
             || lk == 687           // '[' true
             || lk == 815           // '[' false
             || lk == 943           // '[' string
             || lk == 1199          // '[' complex
             || lk == 1327          // '[' real
             || lk == 1455          // '[' comment
             || lk == 1711          // '[' '!'
             || lk == 1967          // '[' '#'
             || lk == 2607          // '[' '('
             || lk == 2863          // '[' '*'
             || lk == 3247          // '[' '++'
             || lk == 3759          // '[' '--'
             || lk == 4655          // '[' ';'
             || lk == 6063          // '[' '['
             || lk == 6447          // '[' 'break'
             || lk == 6703          // '[' 'char'
             || lk == 6831          // '[' 'class'
             || lk == 6959          // '[' 'continue'
             || lk == 7215          // '[' 'do'
             || lk == 7343          // '[' 'double'
             || lk == 8111          // '[' 'float'
             || lk == 8239          // '[' 'for'
             || lk == 8367          // '[' 'if'
             || lk == 9007          // '[' 'int'
             || lk == 9135          // '[' 'long'
             || lk == 9263          // '[' 'namespace'
             || lk == 9391          // '[' 'private'
             || lk == 9519          // '[' 'protected'
             || lk == 9647          // '[' 'public'
             || lk == 9775          // '[' 'return'
             || lk == 9903          // '[' 'template'
             || lk == 10031         // '[' 'throw'
             || lk == 10159         // '[' 'try'
             || lk == 10543         // '[' 'using'
             || lk == 10671         // '[' 'void'
             || lk == 10799         // '[' 'while'
             || lk == 10927         // '[' '{'
             || lk == 11439)        // '[' '~'
            {
              lk = memoized(11, e0);
              if (lk == 0)
              {
                int b0B = b0; int e0B = e0; int l1B = l1;
                int b1B = b1; int e1B = e1; int l2B = l2;
                int b2B = b2; int e2B = e2;
                try
                {
                  consumeT(47);     // '['
                  try_arguments();
                  consumeT(48);     // ']'
                  memoize(11, e0B, -1);
                  continue;
                }
                catch (ParseException &)
                {
                  b0 = b0B; e0 = e0B; l1 = l1B; if (l1 == 0) {end = e0B;} else {
                  b1 = b1B; e1 = e1B; l2 = l2B; if (l2 == 0) {end = e1B;} else {
                  b2 = b2B; e2 = e2B; end = e2B; }}
                  memoize(11, e0B, -2);
                  break;
                }
              }
            }
            if (lk != -1)
            {
              break;
            }
            consumeT(47);           // '['
            try_arguments();
            consumeT(48);           // ']'
          }
          memoize(10, e0A, -2);
          lk = -5;
        }
        catch (ParseException &)
        {
          try
          {
            b0 = b0A; e0 = e0A; l1 = l1A; if (l1 == 0) {end = e0A;} else {
            b1 = b1A; e1 = e1A; l2 = l2A; if (l2 == 0) {end = e1A;} else {
            b2 = b2A; e2 = e2A; end = e2A; }}
            consumeT(3);            // identifier
            for (;;)
            {
              lookahead1(29);       // '(' | '->'
              if (l1 != 31)         // '->'
              {
                break;
              }
              consumeT(31);         // '->'
              lookahead1(0);        // identifier
              consumeT(3);          // identifier
            }
            consumeT(20);           // '('
            for (;;)
            {
              lookahead1(44);       // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | ')' | '*' | '++' | '--' | ';' | '[' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '~'
              if (l1 == 21)         // ')'
              {
                break;
              }
              try_arguments();
            }
            consumeT(21);           // ')'
            memoize(10, e0A, -3);
            lk = -5;
          }
          catch (ParseException &)
          {
            lk = -4;
            b0 = b0A; e0 = e0A; l1 = l1A; if (l1 == 0) {end = e0A;} else {
            b1 = b1A; e1 = e1A; l2 = l2A; if (l2 == 0) {end = e1A;} else {
            b2 = b2A; e2 = e2A; end = e2A; }}
            memoize(10, e0A, -4);
          }
        }
      }
    }
    switch (lk)
    {
    case -1:
      consumeT(3);                  // identifier
      for (;;)
      {
        lookahead1(30);             // '(' | '.'
        if (l1 != 32)               // '.'
        {
          break;
        }
        consumeT(32);               // '.'
        lookahead1(0);              // identifier
        consumeT(3);                // identifier
      }
      consumeT(20);                 // '('
      for (;;)
      {
        lookahead1(44);             // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | ')' | '*' | '++' | '--' | ';' | '[' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '~'
        if (l1 == 21)               // ')'
        {
          break;
        }
        try_arguments();
      }
      consumeT(21);                 // ')'
      break;
    case -2:
      consumeT(3);                  // identifier
      for (;;)
      {
        lookahead1(55);             // END | identifier | null | true | false | string | complex | real | comment |
                                    // lineTerminator | '!' | '!=' | '#' | '%' | '%=' | '&' | '&&' | '(' | ')' | '*' |
                                    // '*=' | '+' | '++' | '+=' | ',' | '-' | '--' | '-=' | '.' | '/' | '/=' | ';' |
                                    // '<' | '<<' | '<<=' | '<=' | '=' | '==' | '>' | '>=' | '>>' | '>>=' | '[' | ']' |
                                    // '^' | 'break' | 'char' | 'class' | 'continue' | 'do' | 'double' | 'float' |
                                    // 'for' | 'if' | 'int' | 'long' | 'namespace' | 'private' | 'protected' |
                                    // 'public' | 'return' | 'template' | 'throw' | 'try' | 'using' | 'void' | 'while' |
                                    // '{' | '|' | '||' | '}' | '~'
        if (l1 != 32)               // '.'
        {
          break;
        }
        consumeT(32);               // '.'
        lookahead1(0);              // identifier
        consumeT(3);                // identifier
      }
      for (;;)
      {
        lookahead1(53);             // END | identifier | null | true | false | string | complex | real | comment |
                                    // lineTerminator | '!' | '!=' | '#' | '%' | '%=' | '&' | '&&' | '(' | ')' | '*' |
                                    // '*=' | '+' | '++' | '+=' | ',' | '-' | '--' | '-=' | '/' | '/=' | ';' | '<' |
                                    // '<<' | '<<=' | '<=' | '=' | '==' | '>' | '>=' | '>>' | '>>=' | '[' | ']' | '^' |
                                    // 'break' | 'char' | 'class' | 'continue' | 'do' | 'double' | 'float' | 'for' |
                                    // 'if' | 'int' | 'long' | 'namespace' | 'private' | 'protected' | 'public' |
                                    // 'return' | 'template' | 'throw' | 'try' | 'using' | 'void' | 'while' | '{' |
                                    // '|' | '||' | '}' | '~'
        switch (l1)
        {
        case 47:                    // '['
          lookahead2(45);           // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | '*' | '++' | '--' | ';' | '[' | ']' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '~'
          break;
        default:
          lk = l1;
          break;
        }
        if (lk == 431               // '[' identifier
         || lk == 559               // '[' null
         || lk == 687               // '[' true
         || lk == 815               // '[' false
         || lk == 943               // '[' string
         || lk == 1199              // '[' complex
         || lk == 1327              // '[' real
         || lk == 1455              // '[' comment
         || lk == 1711              // '[' '!'
         || lk == 1967              // '[' '#'
         || lk == 2607              // '[' '('
         || lk == 2863              // '[' '*'
         || lk == 3247              // '[' '++'
         || lk == 3759              // '[' '--'
         || lk == 4655              // '[' ';'
         || lk == 6063              // '[' '['
         || lk == 6447              // '[' 'break'
         || lk == 6703              // '[' 'char'
         || lk == 6831              // '[' 'class'
         || lk == 6959              // '[' 'continue'
         || lk == 7215              // '[' 'do'
         || lk == 7343              // '[' 'double'
         || lk == 8111              // '[' 'float'
         || lk == 8239              // '[' 'for'
         || lk == 8367              // '[' 'if'
         || lk == 9007              // '[' 'int'
         || lk == 9135              // '[' 'long'
         || lk == 9263              // '[' 'namespace'
         || lk == 9391              // '[' 'private'
         || lk == 9519              // '[' 'protected'
         || lk == 9647              // '[' 'public'
         || lk == 9775              // '[' 'return'
         || lk == 9903              // '[' 'template'
         || lk == 10031             // '[' 'throw'
         || lk == 10159             // '[' 'try'
         || lk == 10543             // '[' 'using'
         || lk == 10671             // '[' 'void'
         || lk == 10799             // '[' 'while'
         || lk == 10927             // '[' '{'
         || lk == 11439)            // '[' '~'
        {
          lk = memoized(11, e0);
          if (lk == 0)
          {
            int b0B = b0; int e0B = e0; int l1B = l1;
            int b1B = b1; int e1B = e1; int l2B = l2;
            int b2B = b2; int e2B = e2;
            try
            {
              consumeT(47);         // '['
              try_arguments();
              consumeT(48);         // ']'
              memoize(11, e0B, -1);
              continue;
            }
            catch (ParseException &)
            {
              b0 = b0B; e0 = e0B; l1 = l1B; if (l1 == 0) {end = e0B;} else {
              b1 = b1B; e1 = e1B; l2 = l2B; if (l2 == 0) {end = e1B;} else {
              b2 = b2B; e2 = e2B; end = e2B; }}
              memoize(11, e0B, -2);
              break;
            }
          }
        }
        if (lk != -1)
        {
          break;
        }
        consumeT(47);               // '['
        try_arguments();
        consumeT(48);               // ']'
      }
      break;
    case -3:
      consumeT(3);                  // identifier
      for (;;)
      {
        lookahead1(29);             // '(' | '->'
        if (l1 != 31)               // '->'
        {
          break;
        }
        consumeT(31);               // '->'
        lookahead1(0);              // identifier
        consumeT(3);                // identifier
      }
      consumeT(20);                 // '('
      for (;;)
      {
        lookahead1(44);             // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | ')' | '*' | '++' | '--' | ';' | '[' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '~'
        if (l1 == 21)               // ')'
        {
          break;
        }
        try_arguments();
      }
      consumeT(21);                 // ')'
      break;
    case -5:
      break;
    default:
      consumeT(3);                  // identifier
      for (;;)
      {
        lookahead1(54);             // END | identifier | null | true | false | string | complex | real | comment |
                                    // lineTerminator | '!' | '!=' | '#' | '%' | '%=' | '&' | '&&' | '(' | ')' | '*' |
                                    // '*=' | '+' | '++' | '+=' | ',' | '-' | '--' | '-=' | '->' | '/' | '/=' | ';' |
                                    // '<' | '<<' | '<<=' | '<=' | '=' | '==' | '>' | '>=' | '>>' | '>>=' | '[' | ']' |
                                    // '^' | 'break' | 'char' | 'class' | 'continue' | 'do' | 'double' | 'float' |
                                    // 'for' | 'if' | 'int' | 'long' | 'namespace' | 'private' | 'protected' |
                                    // 'public' | 'return' | 'template' | 'throw' | 'try' | 'using' | 'void' | 'while' |
                                    // '{' | '|' | '||' | '}' | '~'
        if (l1 != 31)               // '->'
        {
          break;
        }
        consumeT(31);               // '->'
        lookahead1(0);              // identifier
        consumeT(3);                // identifier
      }
      for (;;)
      {
        lookahead1(53);             // END | identifier | null | true | false | string | complex | real | comment |
                                    // lineTerminator | '!' | '!=' | '#' | '%' | '%=' | '&' | '&&' | '(' | ')' | '*' |
                                    // '*=' | '+' | '++' | '+=' | ',' | '-' | '--' | '-=' | '/' | '/=' | ';' | '<' |
                                    // '<<' | '<<=' | '<=' | '=' | '==' | '>' | '>=' | '>>' | '>>=' | '[' | ']' | '^' |
                                    // 'break' | 'char' | 'class' | 'continue' | 'do' | 'double' | 'float' | 'for' |
                                    // 'if' | 'int' | 'long' | 'namespace' | 'private' | 'protected' | 'public' |
                                    // 'return' | 'template' | 'throw' | 'try' | 'using' | 'void' | 'while' | '{' |
                                    // '|' | '||' | '}' | '~'
        switch (l1)
        {
        case 47:                    // '['
          lookahead2(45);           // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | '*' | '++' | '--' | ';' | '[' | ']' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '~'
          break;
        default:
          lk = l1;
          break;
        }
        if (lk == 431               // '[' identifier
         || lk == 559               // '[' null
         || lk == 687               // '[' true
         || lk == 815               // '[' false
         || lk == 943               // '[' string
         || lk == 1199              // '[' complex
         || lk == 1327              // '[' real
         || lk == 1455              // '[' comment
         || lk == 1711              // '[' '!'
         || lk == 1967              // '[' '#'
         || lk == 2607              // '[' '('
         || lk == 2863              // '[' '*'
         || lk == 3247              // '[' '++'
         || lk == 3759              // '[' '--'
         || lk == 4655              // '[' ';'
         || lk == 6063              // '[' '['
         || lk == 6447              // '[' 'break'
         || lk == 6703              // '[' 'char'
         || lk == 6831              // '[' 'class'
         || lk == 6959              // '[' 'continue'
         || lk == 7215              // '[' 'do'
         || lk == 7343              // '[' 'double'
         || lk == 8111              // '[' 'float'
         || lk == 8239              // '[' 'for'
         || lk == 8367              // '[' 'if'
         || lk == 9007              // '[' 'int'
         || lk == 9135              // '[' 'long'
         || lk == 9263              // '[' 'namespace'
         || lk == 9391              // '[' 'private'
         || lk == 9519              // '[' 'protected'
         || lk == 9647              // '[' 'public'
         || lk == 9775              // '[' 'return'
         || lk == 9903              // '[' 'template'
         || lk == 10031             // '[' 'throw'
         || lk == 10159             // '[' 'try'
         || lk == 10543             // '[' 'using'
         || lk == 10671             // '[' 'void'
         || lk == 10799             // '[' 'while'
         || lk == 10927             // '[' '{'
         || lk == 11439)            // '[' '~'
        {
          lk = memoized(12, e0);
          if (lk == 0)
          {
            int b0B = b0; int e0B = e0; int l1B = l1;
            int b1B = b1; int e1B = e1; int l2B = l2;
            int b2B = b2; int e2B = e2;
            try
            {
              consumeT(47);         // '['
              try_arguments();
              consumeT(48);         // ']'
              memoize(12, e0B, -1);
              continue;
            }
            catch (ParseException &)
            {
              b0 = b0B; e0 = e0B; l1 = l1B; if (l1 == 0) {end = e0B;} else {
              b1 = b1B; e1 = e1B; l2 = l2B; if (l2 == 0) {end = e1B;} else {
              b2 = b2B; e2 = e2B; end = e2B; }}
              memoize(12, e0B, -2);
              break;
            }
          }
        }
        if (lk != -1)
        {
          break;
        }
        consumeT(47);               // '['
        try_arguments();
        consumeT(48);               // ']'
      }
      break;
    }
  }

  void parse_array()
  {
    eventHandler->startNonterminal(L"array", e0);
    consume(85);                    // '{'
    parse_element();
    for (;;)
    {
      lookahead1(31);               // ',' | '}'
      if (l1 != 27)                 // ','
      {
        break;
      }
      consume(27);                  // ','
      parse_element();
    }
    consume(88);                    // '}'
    eventHandler->endNonterminal(L"array", e0);
  }

  void try_array()
  {
    consumeT(85);                   // '{'
    try_element();
    for (;;)
    {
      lookahead1(31);               // ',' | '}'
      if (l1 != 27)                 // ','
      {
        break;
      }
      consumeT(27);                 // ','
      try_element();
    }
    consumeT(88);                   // '}'
  }

  void parse_matrix()
  {
    eventHandler->startNonterminal(L"matrix", e0);
    consume(47);                    // '['
    lookahead1(45);                 // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | '*' | '++' | '--' | ';' | '[' | ']' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '~'
    switch (l1)
    {
    case 36:                        // ';'
      lookahead2(48);               // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | '*' | '++' | ',' | '--' | ';' | '[' | ']' | 'break' | 'char' |
                                    // 'class' | 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' |
                                    // 'long' | 'namespace' | 'private' | 'protected' | 'public' | 'return' |
                                    // 'template' | 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '~'
      break;
    default:
      lk = l1;
      break;
    }
    if (lk == 4644)                 // ';' ';'
    {
      lk = memoized(13, e0);
      if (lk == 0)
      {
        int b0A = b0; int e0A = e0; int l1A = l1;
        int b1A = b1; int e1A = e1; int l2A = l2;
        int b2A = b2; int e2A = e2;
        try
        {
          try_row();
          lk = -1;
        }
        catch (ParseException &)
        {
          lk = -2;
        }
        b0 = b0A; e0 = e0A; l1 = l1A; if (l1 == 0) {end = e0A;} else {
        b1 = b1A; e1 = e1A; l2 = l2A; if (l2 == 0) {end = e1A;} else {
        b2 = b2A; e2 = e2A; end = e2A; }}
        memoize(13, e0, lk);
      }
    }
    if (lk != -2
     && lk != 48                    // ']'
     && lk != 420                   // ';' identifier
     && lk != 548                   // ';' null
     && lk != 676                   // ';' true
     && lk != 804                   // ';' false
     && lk != 932                   // ';' string
     && lk != 1188                  // ';' complex
     && lk != 1316                  // ';' real
     && lk != 1444                  // ';' comment
     && lk != 1700                  // ';' '!'
     && lk != 1956                  // ';' '#'
     && lk != 2596                  // ';' '('
     && lk != 2852                  // ';' '*'
     && lk != 3236                  // ';' '++'
     && lk != 3748                  // ';' '--'
     && lk != 6052                  // ';' '['
     && lk != 6436                  // ';' 'break'
     && lk != 6692                  // ';' 'char'
     && lk != 6820                  // ';' 'class'
     && lk != 6948                  // ';' 'continue'
     && lk != 7204                  // ';' 'do'
     && lk != 7332                  // ';' 'double'
     && lk != 8100                  // ';' 'float'
     && lk != 8228                  // ';' 'for'
     && lk != 8356                  // ';' 'if'
     && lk != 8996                  // ';' 'int'
     && lk != 9124                  // ';' 'long'
     && lk != 9252                  // ';' 'namespace'
     && lk != 9380                  // ';' 'private'
     && lk != 9508                  // ';' 'protected'
     && lk != 9636                  // ';' 'public'
     && lk != 9764                  // ';' 'return'
     && lk != 9892                  // ';' 'template'
     && lk != 10020                 // ';' 'throw'
     && lk != 10148                 // ';' 'try'
     && lk != 10532                 // ';' 'using'
     && lk != 10660                 // ';' 'void'
     && lk != 10788                 // ';' 'while'
     && lk != 10916                 // ';' '{'
     && lk != 11428)                // ';' '~'
    {
      parse_row();
    }
    for (;;)
    {
      if (l1 != 36)                 // ';'
      {
        break;
      }
      consume(36);                  // ';'
      parse_row();
    }
    consume(48);                    // ']'
    eventHandler->endNonterminal(L"matrix", e0);
  }

  void try_matrix()
  {
    consumeT(47);                   // '['
    lookahead1(45);                 // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | '*' | '++' | '--' | ';' | '[' | ']' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '~'
    switch (l1)
    {
    case 36:                        // ';'
      lookahead2(48);               // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | '*' | '++' | ',' | '--' | ';' | '[' | ']' | 'break' | 'char' |
                                    // 'class' | 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' |
                                    // 'long' | 'namespace' | 'private' | 'protected' | 'public' | 'return' |
                                    // 'template' | 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '~'
      break;
    default:
      lk = l1;
      break;
    }
    if (lk == 4644)                 // ';' ';'
    {
      lk = memoized(13, e0);
      if (lk == 0)
      {
        int b0A = b0; int e0A = e0; int l1A = l1;
        int b1A = b1; int e1A = e1; int l2A = l2;
        int b2A = b2; int e2A = e2;
        try
        {
          try_row();
          memoize(13, e0A, -1);
        }
        catch (ParseException &)
        {
          b0 = b0A; e0 = e0A; l1 = l1A; if (l1 == 0) {end = e0A;} else {
          b1 = b1A; e1 = e1A; l2 = l2A; if (l2 == 0) {end = e1A;} else {
          b2 = b2A; e2 = e2A; end = e2A; }}
          memoize(13, e0A, -2);
        }
        lk = -2;
      }
    }
    if (lk != -2
     && lk != 48                    // ']'
     && lk != 420                   // ';' identifier
     && lk != 548                   // ';' null
     && lk != 676                   // ';' true
     && lk != 804                   // ';' false
     && lk != 932                   // ';' string
     && lk != 1188                  // ';' complex
     && lk != 1316                  // ';' real
     && lk != 1444                  // ';' comment
     && lk != 1700                  // ';' '!'
     && lk != 1956                  // ';' '#'
     && lk != 2596                  // ';' '('
     && lk != 2852                  // ';' '*'
     && lk != 3236                  // ';' '++'
     && lk != 3748                  // ';' '--'
     && lk != 6052                  // ';' '['
     && lk != 6436                  // ';' 'break'
     && lk != 6692                  // ';' 'char'
     && lk != 6820                  // ';' 'class'
     && lk != 6948                  // ';' 'continue'
     && lk != 7204                  // ';' 'do'
     && lk != 7332                  // ';' 'double'
     && lk != 8100                  // ';' 'float'
     && lk != 8228                  // ';' 'for'
     && lk != 8356                  // ';' 'if'
     && lk != 8996                  // ';' 'int'
     && lk != 9124                  // ';' 'long'
     && lk != 9252                  // ';' 'namespace'
     && lk != 9380                  // ';' 'private'
     && lk != 9508                  // ';' 'protected'
     && lk != 9636                  // ';' 'public'
     && lk != 9764                  // ';' 'return'
     && lk != 9892                  // ';' 'template'
     && lk != 10020                 // ';' 'throw'
     && lk != 10148                 // ';' 'try'
     && lk != 10532                 // ';' 'using'
     && lk != 10660                 // ';' 'void'
     && lk != 10788                 // ';' 'while'
     && lk != 10916                 // ';' '{'
     && lk != 11428)                // ';' '~'
    {
      try_row();
    }
    for (;;)
    {
      if (l1 != 36)                 // ';'
      {
        break;
      }
      consumeT(36);                 // ';'
      try_row();
    }
    consumeT(48);                   // ']'
  }

  void parse_element()
  {
    eventHandler->startNonterminal(L"element", e0);
    lookahead1(42);                 // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | '*' | '++' | '--' | ';' | '[' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '~'
    switch (l1)
    {
    case 3:                         // identifier
      lookahead2(41);               // '!=' | '%' | '%=' | '&' | '&&' | '(' | '*' | '*=' | '+' | '++' | '+=' | ',' |
                                    // '-' | '--' | '-=' | '->' | '.' | '/' | '/=' | ':' | '<' | '<<' | '<<=' | '<=' |
                                    // '=' | '==' | '>' | '>=' | '>>' | '>>=' | '[' | '^' | '|' | '||' | '}'
      break;
    case 7:                         // string
      lookahead2(40);               // '!=' | '%' | '%=' | '&' | '&&' | '*' | '*=' | '+' | '++' | '+=' | ',' | '-' |
                                    // '--' | '-=' | '/' | '/=' | ':' | '<' | '<<' | '<<=' | '<=' | '=' | '==' | '>' |
                                    // '>=' | '>>' | '>>=' | '^' | '|' | '||' | '}'
      break;
    default:
      lk = l1;
      break;
    }
    if (lk == 4483                  // identifier ':'
     || lk == 4487)                 // string ':'
    {
      parse_key();
      lookahead1(9);                // ':'
      consume(35);                  // ':'
    }
    parse_expression();
    eventHandler->endNonterminal(L"element", e0);
  }

  void try_element()
  {
    lookahead1(42);                 // identifier | null | true | false | string | complex | real | comment | '!' |
                                    // '#' | '(' | '*' | '++' | '--' | ';' | '[' | 'break' | 'char' | 'class' |
                                    // 'continue' | 'do' | 'double' | 'float' | 'for' | 'if' | 'int' | 'long' |
                                    // 'namespace' | 'private' | 'protected' | 'public' | 'return' | 'template' |
                                    // 'throw' | 'try' | 'using' | 'void' | 'while' | '{' | '~'
    switch (l1)
    {
    case 3:                         // identifier
      lookahead2(41);               // '!=' | '%' | '%=' | '&' | '&&' | '(' | '*' | '*=' | '+' | '++' | '+=' | ',' |
                                    // '-' | '--' | '-=' | '->' | '.' | '/' | '/=' | ':' | '<' | '<<' | '<<=' | '<=' |
                                    // '=' | '==' | '>' | '>=' | '>>' | '>>=' | '[' | '^' | '|' | '||' | '}'
      break;
    case 7:                         // string
      lookahead2(40);               // '!=' | '%' | '%=' | '&' | '&&' | '*' | '*=' | '+' | '++' | '+=' | ',' | '-' |
                                    // '--' | '-=' | '/' | '/=' | ':' | '<' | '<<' | '<<=' | '<=' | '=' | '==' | '>' |
                                    // '>=' | '>>' | '>>=' | '^' | '|' | '||' | '}'
      break;
    default:
      lk = l1;
      break;
    }
    if (lk == 4483                  // identifier ':'
     || lk == 4487)                 // string ':'
    {
      try_key();
      lookahead1(9);                // ':'
      consumeT(35);                 // ':'
    }
    try_expression();
  }

  void parse_key()
  {
    eventHandler->startNonterminal(L"key", e0);
    switch (l1)
    {
    case 3:                         // identifier
      consume(3);                   // identifier
      break;
    default:
      consume(7);                   // string
      break;
    }
    eventHandler->endNonterminal(L"key", e0);
  }

  void try_key()
  {
    switch (l1)
    {
    case 3:                         // identifier
      consumeT(3);                  // identifier
      break;
    default:
      consumeT(7);                  // string
      break;
    }
  }

  void parse_row()
  {
    eventHandler->startNonterminal(L"row", e0);
    parse_column();
    for (;;)
    {
      lookahead1(34);               // ',' | ';' | ']'
      if (l1 != 27)                 // ','
      {
        break;
      }
      consume(27);                  // ','
      parse_column();
    }
    eventHandler->endNonterminal(L"row", e0);
  }

  void try_row()
  {
    try_column();
    for (;;)
    {
      lookahead1(34);               // ',' | ';' | ']'
      if (l1 != 27)                 // ','
      {
        break;
      }
      consumeT(27);                 // ','
      try_column();
    }
  }

  void parse_column()
  {
    eventHandler->startNonterminal(L"column", e0);
    parse_expression();
    eventHandler->endNonterminal(L"column", e0);
  }

  void try_column()
  {
    try_expression();
  }

  void parse_parenthesizedExpression()
  {
    eventHandler->startNonterminal(L"parenthesizedExpression", e0);
    consume(20);                    // '('
    parse_expression();
    lookahead1(6);                  // ')'
    consume(21);                    // ')'
    eventHandler->endNonterminal(L"parenthesizedExpression", e0);
  }

  void try_parenthesizedExpression()
  {
    consumeT(20);                   // '('
    try_expression();
    lookahead1(6);                  // ')'
    consumeT(21);                   // ')'
  }

  void parse_value()
  {
    eventHandler->startNonterminal(L"value", e0);
    switch (l1)
    {
    case 10:                        // real
      consume(10);                  // real
      break;
    case 9:                         // complex
      consume(9);                   // complex
      break;
    case 7:                         // string
      consume(7);                   // string
      break;
    case 85:                        // '{'
      parse_array();
      break;
    case 47:                        // '['
      parse_matrix();
      break;
    case 4:                         // null
      consume(4);                   // null
      break;
    case 5:                         // true
      consume(5);                   // true
      break;
    default:
      consume(6);                   // false
      break;
    }
    eventHandler->endNonterminal(L"value", e0);
  }

  void try_value()
  {
    switch (l1)
    {
    case 10:                        // real
      consumeT(10);                 // real
      break;
    case 9:                         // complex
      consumeT(9);                  // complex
      break;
    case 7:                         // string
      consumeT(7);                  // string
      break;
    case 85:                        // '{'
      try_array();
      break;
    case 47:                        // '['
      try_matrix();
      break;
    case 4:                         // null
      consumeT(4);                  // null
      break;
    case 5:                         // true
      consumeT(5);                  // true
      break;
    default:
      consumeT(6);                  // false
      break;
    }
  }

  void consume(int t)
  {
    if (l1 == t)
    {
      eventHandler->terminal(TOKEN[l1], b1, e1);
      b0 = b1; e0 = e1; l1 = l2; if (l1 != 0) {
      b1 = b2; e1 = e2; l2 = 0; }
    }
    else
    {
      error(b1, e1, 0, l1, t);
    }
  }

  void consumeT(int t)
  {
    if (l1 == t)
    {
      b0 = b1; e0 = e1; l1 = l2; if (l1 != 0) {
      b1 = b2; e1 = e2; l2 = 0; }
    }
    else
    {
      error(b1, e1, 0, l1, t);
    }
  }

  void lookahead1(int tokenSetId)
  {
    if (l1 == 0)
    {
      l1 = match(tokenSetId);
      b1 = begin;
      e1 = end;
    }
  }

  void lookahead2(int tokenSetId)
  {
    if (l2 == 0)
    {
      l2 = match(tokenSetId);
      b2 = begin;
      e2 = end;
    }
    lk = (l2 << 7) | l1;
  }

  int error(int b, int e, int s, int l, int t)
  {
    if (e >= ex)
    {
      bx = b;
      ex = e;
      sx = s;
      lx = l;
      tx = t;
    }
    throw ParseException(bx, ex, sx, lx, tx);
  }

  int lk, b0, e0;
  int l1, b1, e1;
  int l2, b2, e2;
  int bx, ex, sx, lx, tx;
  EventHandler *eventHandler;
  std::map<int, int> memo;

  void memoize(int i, int e, int v)
  {
    memo[(e << 4) + i] = v;
  }

  int memoized(int i, int e)
  {
    std::map<int, int>::iterator v = memo.find((e << 4) + i);
    return v != memo.end() ? v->second : 0;
  }

  const wchar_t *input;
  int begin;
  int end;

  int match(int tokenSetId)
  {
    begin = end;
    int current = end;
    int result = INITIAL[tokenSetId];
    int state = 0;

    for (int code = result & 511; code != 0; )
    {
      int charclass;
      int c0 = input[current];
      ++current;
      if (c0 < 0x80)
      {
        charclass = MAP0[c0];
      }
      else if (c0 < 0xd800)
      {
        int c1 = c0 >> 5;
        charclass = MAP1[(c0 & 31) + MAP1[(c1 & 31) + MAP1[c1 >> 5]]];
      }
      else
      {
        if (c0 < 0xdc00)
        {
          int c1 = input[current];
          if (c1 >= 0xdc00 && c1 < 0xe000)
          {
            ++current;
            c0 = ((c0 & 0x3ff) << 10) + (c1 & 0x3ff) + 0x10000;
          }
        }
        int lo = 0, hi = 1;
        for (int m = 1; ; m = (hi + lo) >> 1)
        {
          if (MAP2[m] > c0) hi = m - 1;
          else if (MAP2[2 + m] < c0) lo = m + 1;
          else {charclass = MAP2[4 + m]; break;}
          if (lo > hi) {charclass = 0; break;}
        }
      }

      state = code;
      int i0 = (charclass << 9) + code - 1;
      code = TRANSITION[(i0 & 7) + TRANSITION[i0 >> 3]];
      if (code > 511)
      {
        result = code;
        code &= 511;
        end = current;
      }
    }

    result >>= 9;
    if (result == 0)
    {
      end = current - 1;
      int c1 = input[end];
      if (c1 >= 0xdc00 && c1 < 0xe000) --end;
      return error(begin, end, state, -1, -1);
    }

    if (input[begin] == 0) end = begin;
    return (result & 127) - 1;
  }

  class FileNotFound
  {
  public:
    FileNotFound(std::string name) : filename(name) {}
    const std::string &getFilename() const {return filename;}

  private:
    std::string filename;
  };

  class MalformedInputException
  {
  public:
    MalformedInputException(size_t offset) : offset(offset) {}
    size_t getOffset() const {return offset;}

  private:
    size_t offset;
  };

  class Utf8Encoder
  {
  public:
    static std::string encode(const wchar_t *unencoded)
    {
      return encode(unencoded, wcslen(unencoded));
    }

    static std::string encode(const wchar_t *unencoded, size_t size)
    {
      std::string encoded;
      encoded.reserve(size + 3);

      for (size_t i = 0; i < size; ++i)
      {
        if (encoded.size() + 4 >= encoded.capacity()) encoded.reserve(encoded.capacity() * 2);

        int w = unencoded[i];
        if (w < 0x80)
        {
          encoded += w;
        }
        else if (w < 0x800)
        {
          encoded += 0xc0 | (w >> 6);
          encoded += 0x80 | (w & 0x3f);
        }
        else if (w < 0xd800)
        {
          encoded += 0xe0 | ( w          >> 12);
          encoded += 0x80 | ((w & 0xfff) >>  6);
          encoded += 0x80 | ( w &  0x3f       );
        }
        else if (w < 0xe000)
        {
          if (++i >= size)
          {
            throw MalformedInputException(i - 1);
          }
          int w2 = unencoded[i];
          if (w2 < 0xdc00 || w2 > 0xdfff)
          {
            throw MalformedInputException(i - 1);
          }
          w = (((w  & 0x3ff) << 10) | (w2 & 0x3ff)) + 0x10000;
          encoded += 0xf0 | ( w            >> 18);
          encoded += 0x80 | ((w & 0x3ffff) >> 12);
          encoded += 0x80 | ((w &   0xfff) >>  6);
          encoded += 0x80 | ( w &    0x3f       );
        }
        else if (w < 0x10000)
        {
          encoded += 0xe0 | ( w          >> 12);
          encoded += 0x80 | ((w & 0xfff) >>  6);
          encoded += 0x80 | ( w &  0x3f       );
        }
        else if (w < 0x110000)
        {
          encoded += 0xf0 | ( w            >> 18);
          encoded += 0x80 | ((w & 0x3ffff) >> 12);
          encoded += 0x80 | ((w &   0xfff) >>  6);
          encoded += 0x80 | ( w &    0x3f       );
        }
        else
        {
          throw MalformedInputException(i);
        }
      }
      return encoded;
    }
  };

  class Utf8Decoder
  {
  public:
    static std::wstring decode(const char *string)
    {
      return decode(string, strlen(string));
    }

    static std::wstring decode(const char *string, size_t size)
    {
      std::wstring decoded;
      decoded.reserve(size + 1);

      for (size_t consumed = 0; consumed < size; )
      {
        if (decoded.size() + 2 >= decoded.capacity()) decoded.reserve(decoded.capacity() * 2);

        size_t bytes;
        int codepoint = decodeChar(string + consumed, &bytes);

        if (bytes == 0)
        {
          throw MalformedInputException(consumed);
        }

        consumed += bytes;

        if (codepoint < 0x10000)
        {
          decoded += codepoint;
        }
        else
        {
          codepoint -= 0x10000;
          decoded += 0x0d800 | (codepoint >> 10);
          decoded += 0x0dc00 | (codepoint & 0x3ff);
        }
      }

      return decoded;
    }

  private:
    static int decodeChar(const char *input, size_t *size)
    {
      int codepoint = input[0];
      if ((codepoint & 0x80) == 0)
      {
        *size = 1;
      }
      else if (   (codepoint & 0x60) == 0x40
               && (input[1]  & 0xc0) == 0x80)
      {
        codepoint = ((codepoint & 0x1f) << 6)
                  |  (input[1]  & 0x3f);
        *size = codepoint < 0x80 ? 0 : 2;
      }
      else if (   (codepoint & 0x70) == 0x60
               && (input[1]  & 0xc0) == 0x80
               && (input[2]  & 0xc0) == 0x80)
      {
        codepoint = ((codepoint &  0xf) << 12)
                  | ((input[1]  & 0x3f) <<  6)
                  |  (input[2]  & 0x3f);
        *size = codepoint < 0x800 ? 0 : 3;
      }
      else if (   (codepoint & 0x78) == 0x70
               && (input[1]  & 0xc0) == 0x80
               && (input[2]  & 0xc0) == 0x80
               && (input[3]  & 0xc0) == 0x80)
      {
        codepoint  = ((codepoint &  0x7) << 18)
                   | ((input[1]  & 0x3f) << 12)
                   | ((input[2]  & 0x3f) <<  6)
                   | ( input[3]  & 0x3f       );
        *size = codepoint < 0x10000 || codepoint > 0x10ffff ? 0 : 4;
      }
      else
      {
        *size = 0;
      }
      return codepoint;
    }
  };

  static std::wstring read(const char *input)
  {
    size_t l = strlen(input);
    if (l > 0 && input[0] == '{' && input[l - 1] == '}')
    {
      return Utf8Decoder::decode(input + 1, l - 2);
    }
    else
    {
      FILE *file = fopen(input, "rb");
      if (file == 0)
      {
        throw FileNotFound(std::string(input));
      }

      std::string content;
      content.reserve(4096);

      for (int c = getc(file); c != EOF; c = getc(file))
      {
        if (content.size() + 1 >= content.capacity()) content.reserve(content.capacity() * 2);
        content += c;
      }

      fclose(file);

      if (content.size() >= 3
       && (unsigned char) content[0] == 0xef
       && (unsigned char) content[1] == 0xbb
       && (unsigned char) content[2] == 0xbf)
      {
        content.erase(0, 3);
      }

      return Utf8Decoder::decode(content.c_str());
    }
  }

  static void getTokenSet(int tokenSetId, const wchar_t **set, int size)
  {
    int s = tokenSetId < 0 ? - tokenSetId : INITIAL[tokenSetId] & 511;
    for (int i = 0; i < 90; i += 32)
    {
      int j = i;
      for (unsigned int f = ec(i >> 5, s); f != 0; f >>= 1, ++j)
      {
        if ((f & 1) != 0)
        {
          if (size > 1)
          {
            set[0] = TOKEN[j];
            ++set;
            --size;
          }
        }
      }
    }
    if (size > 0)
    {
      set[0] = 0;
    }
  }

  static int ec(int t, int s)
  {
    int i0 = t * 368 + s - 1;
    int i1 = i0 >> 2;
    return EXPECTED[(i0 & 3) + EXPECTED[(i1 & 3) + EXPECTED[i1 >> 2]]];
  }

  static const int MAP0[];
  static const int MAP1[];
  static const int MAP2[];
  static const int INITIAL[];
  static const int TRANSITION[];
  static const int EXPECTED[];
  static const wchar_t *TOKEN[];
};

const int WebCpp::MAP0[] =
{
/*   0 */ 56, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 5, 6,
/*  36 */ 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 20, 21, 22, 23, 24, 10,
/*  64 */ 10, 7, 7, 7, 7, 25, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 26, 27, 28, 29, 7, 10, 30,
/*  98 */ 31, 32, 33, 34, 35, 36, 37, 38, 7, 39, 40, 41, 42, 43, 44, 7, 45, 46, 47, 48, 49, 50, 7, 51, 7, 52, 53, 54,
/* 126 */ 55, 10
};

const int WebCpp::MAP1[] =
{
/*   0 */ 54, 87, 87, 87, 87, 87, 87, 87, 85, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87,
/*  27 */ 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87,
/*  54 */ 119, 151, 182, 214, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
/*  76 */ 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 245, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
/*  98 */ 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 56,
/* 120 */ 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 5, 6, 7, 8,
/* 157 */ 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 20, 21, 22, 23, 24, 10, 7, 7,
/* 185 */ 7, 7, 25, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 26, 27, 28, 29, 7, 10, 30, 31, 32,
/* 218 */ 33, 34, 35, 36, 37, 38, 7, 39, 40, 41, 42, 43, 44, 7, 45, 46, 47, 48, 49, 50, 7, 51, 7, 52, 53, 54, 55, 10,
/* 246 */ 10, 10, 10, 10, 10, 10, 10, 2, 2, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
/* 274 */ 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10
};

const int WebCpp::MAP2[] =
{
/* 0 */ 57344, 65536, 65533, 1114111, 10, 10
};

const int WebCpp::INITIAL[] =
{
/*  0 */ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
/* 30 */ 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 1067, 44, 45, 46, 1071, 48, 49, 1074, 1075, 1076, 1077,
/* 54 */ 1078, 1079, 1080
};

const int WebCpp::TRANSITION[] =
{
/*    0 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/*   18 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/*   36 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/*   54 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 3648, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/*   72 */ 4075, 3659, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 3744, 4074, 4075,
/*   90 */ 4075, 4075, 4075, 4075, 4075, 4075, 3931, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/*  108 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/*  126 */ 4075, 4075, 3669, 4075, 4075, 4075, 4075, 4075, 3681, 4075, 4075, 3743, 4075, 4075, 4075, 4075, 4075, 4075,
/*  144 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 3744, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 3931, 4075,
/*  162 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/*  180 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 3648, 4075, 4075, 4075, 4075, 4075,
/*  198 */ 4075, 4075, 4075, 3659, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 3744,
/*  216 */ 4074, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 3931, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 3689, 4075,
/*  234 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/*  252 */ 4075, 4075, 4075, 4075, 3699, 4075, 4075, 4075, 3877, 3710, 3714, 4075, 4075, 3743, 4075, 4075, 4075, 4075,
/*  270 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 3744, 4074, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/*  288 */ 3931, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/*  306 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 3742, 4075, 4075, 4075,
/*  324 */ 3661, 3722, 3724, 4075, 4075, 3732, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 3741, 4075,
/*  342 */ 4075, 3744, 4074, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 3931, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/*  360 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/*  378 */ 4075, 4075, 4075, 4075, 4075, 4075, 3752, 4075, 4075, 4075, 4075, 3775, 3777, 4075, 4075, 3743, 4075, 4075,
/*  396 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 3744, 4074, 4075, 4075, 4075, 4075, 4075,
/*  414 */ 4075, 4075, 3931, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/*  432 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4373, 4075,
/*  450 */ 4075, 4920, 4239, 4890, 4428, 4428, 4428, 3743, 4075, 4787, 4239, 6055, 4075, 4428, 4429, 4889, 4428, 4428,
/*  468 */ 6054, 4075, 6055, 4301, 3785, 4428, 4428, 4428, 4428, 4075, 4919, 4240, 4121, 4428, 4428, 4429, 4075, 4787,
/*  486 */ 4890, 4428, 4428, 4918, 4429, 4419, 4728, 4544, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/*  504 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 3699, 4075, 4075, 4075, 4075, 3794, 3767, 4075, 4075, 3743,
/*  522 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 3744, 4074, 4075, 4075, 4075,
/*  540 */ 4075, 4075, 4075, 4075, 3931, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/*  558 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/*  576 */ 3699, 4075, 4075, 4075, 4075, 3804, 3945, 4075, 4075, 3743, 4075, 4075, 4075, 6004, 4075, 4075, 4075, 4075,
/*  594 */ 4075, 4075, 4075, 4075, 4075, 3744, 4074, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 3931, 4075, 4075, 4075,
/*  612 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/*  630 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 3699, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/*  648 */ 4075, 3743, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 3744, 4074, 4075,
/*  666 */ 4075, 4075, 4075, 4075, 4075, 4075, 3931, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/*  684 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/*  702 */ 4075, 4075, 3814, 4075, 4075, 4033, 4032, 3836, 3837, 4075, 4075, 3743, 4075, 4075, 4075, 4075, 4075, 4075,
/*  720 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 3744, 4074, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 3931, 4075,
/*  738 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/*  756 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 3845, 4075, 4075, 4075, 4075, 3848,
/*  774 */ 3856, 4075, 4075, 3743, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 3744,
/*  792 */ 4074, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 3931, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/*  810 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/*  828 */ 4075, 4075, 4075, 4075, 3699, 4075, 4075, 4075, 3987, 3864, 3868, 4075, 4075, 3743, 4075, 4075, 3956, 4075,
/*  846 */ 3744, 4075, 4075, 3930, 4075, 4075, 4075, 4075, 3955, 3733, 4074, 4075, 4075, 4075, 4075, 4075, 4075, 4856,
/*  864 */ 3876, 4075, 4075, 4075, 4075, 4853, 3885, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/*  882 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 3895, 4075, 4075, 4075,
/*  900 */ 3796, 3903, 3907, 4075, 4075, 3915, 4075, 4075, 4559, 3916, 4075, 4075, 4085, 4075, 4075, 4075, 4075, 4075,
/*  918 */ 4075, 3924, 4074, 4075, 4075, 4075, 4075, 4075, 4075, 3940, 3931, 4075, 4075, 4075, 4075, 4075, 3953, 4075,
/*  936 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/*  954 */ 4075, 4075, 4075, 4075, 4075, 4075, 3699, 4075, 4075, 4632, 3975, 3970, 3964, 4075, 4075, 3743, 4075, 4075,
/*  972 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 3744, 4074, 4075, 4075, 4075, 4075, 4075,
/*  990 */ 4075, 4075, 3931, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/* 1008 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 3699, 3986,
/* 1026 */ 4075, 4459, 3806, 3995, 3999, 4075, 4075, 4007, 4075, 4075, 4676, 4018, 4010, 4075, 3932, 4029, 4075, 4075,
/* 1044 */ 4075, 4075, 4075, 3924, 4074, 4075, 4075, 4075, 4075, 4075, 4075, 3940, 3931, 4075, 4075, 4075, 4075, 4075,
/* 1062 */ 3953, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/* 1080 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 3699, 4075, 4075, 4842, 4075, 4041, 4180, 4075, 4075, 3743,
/* 1098 */ 4075, 4075, 4988, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4053, 3744, 4074, 4075, 4075, 4075,
/* 1116 */ 4075, 4075, 4075, 4075, 3931, 4075, 4075, 4075, 4075, 4075, 4051, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/* 1134 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/* 1152 */ 3699, 4075, 4075, 4075, 4075, 4061, 4065, 4075, 4075, 3743, 4075, 4075, 4075, 4075, 6056, 4075, 4075, 4073,
/* 1170 */ 4075, 4075, 3741, 4075, 4075, 3744, 4074, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4084, 4075, 4075, 4075,
/* 1188 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/* 1206 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 3699, 4075, 4075, 4075, 4021, 4093, 4095, 4428,
/* 1224 */ 4428, 3743, 4075, 4787, 4742, 4839, 4075, 4428, 3786, 4103, 4428, 4428, 6054, 4075, 5210, 4114, 3785, 4428,
/* 1242 */ 4428, 4428, 4428, 4075, 4919, 4130, 4121, 4428, 4428, 4429, 4075, 5851, 4147, 4428, 4428, 4918, 4429, 4419,
/* 1260 */ 4728, 4544, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/* 1278 */ 4075, 4075, 3699, 4157, 4075, 4075, 4075, 4167, 4075, 4075, 4075, 3743, 4075, 4075, 4075, 4075, 4075, 4075,
/* 1296 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 3744, 4074, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 3931, 4075,
/* 1314 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/* 1332 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 3699, 4177, 4075, 4075, 4177, 4188,
/* 1350 */ 4190, 4075, 4075, 3743, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 3744,
/* 1368 */ 4074, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 3931, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/* 1386 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/* 1404 */ 4075, 4075, 4075, 4075, 3699, 4198, 4075, 4075, 4075, 4216, 5043, 4075, 4075, 3743, 4075, 4075, 4075, 4075,
/* 1422 */ 4226, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 3744, 4074, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/* 1440 */ 3931, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/* 1458 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 3699, 4075, 4075, 4075,
/* 1476 */ 4075, 4237, 4623, 4075, 4075, 3743, 4075, 4075, 4075, 5154, 4248, 4075, 4330, 4268, 4075, 4075, 4075, 4075,
/* 1494 */ 4075, 5514, 4074, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 3931, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/* 1512 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/* 1530 */ 4075, 4075, 4075, 4075, 4075, 4075, 3699, 5038, 4075, 4075, 4075, 4279, 5625, 4075, 4075, 3743, 4075, 4289,
/* 1548 */ 4075, 4075, 5876, 4075, 4075, 4290, 4075, 4075, 4075, 4075, 4075, 3744, 4074, 4075, 4075, 4075, 4075, 4075,
/* 1566 */ 4075, 4075, 3931, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/* 1584 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4373, 4075,
/* 1602 */ 4075, 4920, 4239, 4890, 4428, 4428, 4428, 3743, 4075, 4787, 5129, 6055, 4075, 4428, 4429, 4889, 4428, 4428,
/* 1620 */ 6054, 4075, 4789, 4301, 3785, 4428, 4428, 4428, 4428, 4075, 4919, 4298, 4121, 4428, 4428, 4429, 4075, 5461,
/* 1638 */ 4309, 4428, 4428, 4918, 4429, 4419, 4728, 4544, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/* 1656 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 3699, 4075, 4075, 4075, 4229, 4320, 4321, 4075, 4075, 3743,
/* 1674 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 3744, 4074, 4075, 4075, 4075,
/* 1692 */ 4075, 4075, 4075, 4075, 3931, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/* 1710 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/* 1728 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4329, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/* 1746 */ 4075, 4075, 3741, 4075, 4075, 3744, 4074, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 3931, 4075, 4075, 4075,
/* 1764 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/* 1782 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 3699, 4075, 4075, 4075, 6086, 6081, 4338, 4075,
/* 1800 */ 4075, 3743, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 3744, 4074, 4075,
/* 1818 */ 4075, 4075, 4075, 4075, 4075, 4075, 3931, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/* 1836 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/* 1854 */ 4075, 4075, 3699, 4075, 4075, 4075, 4075, 3826, 3820, 4075, 4075, 3743, 4075, 4075, 4075, 4075, 4075, 4075,
/* 1872 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 3744, 4074, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 3931, 4075,
/* 1890 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/* 1908 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4373, 4075, 4075, 4920, 4239, 4890,
/* 1926 */ 4428, 5969, 4346, 3743, 3887, 4355, 3978, 6055, 4075, 4366, 4717, 5288, 4428, 4428, 6054, 4075, 5245, 4301,
/* 1944 */ 4384, 4428, 5065, 4135, 4428, 4075, 4919, 4240, 4397, 4428, 4428, 4416, 4075, 4787, 4890, 4427, 4428, 4930,
/* 1962 */ 5807, 4437, 4728, 4544, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/* 1980 */ 4075, 4075, 4075, 4075, 4456, 4075, 4075, 4878, 3828, 4467, 4469, 4428, 4428, 3743, 4075, 4787, 4239, 6055,
/* 1998 */ 4075, 4428, 4429, 4889, 4428, 4477, 4754, 4075, 6055, 4301, 3785, 5699, 4428, 4488, 4428, 4075, 4919, 4240,
/* 2016 */ 5939, 4428, 4428, 4429, 4075, 4787, 4890, 4428, 4428, 4918, 4429, 4419, 4728, 4544, 4075, 4075, 4075, 4075,
/* 2034 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4505, 4075, 4075, 5949,
/* 2052 */ 4498, 4516, 4522, 4428, 4428, 3743, 4075, 4787, 4239, 6055, 4075, 4428, 4429, 4889, 4428, 4428, 6054, 6141,
/* 2070 */ 6055, 4301, 3785, 4428, 5492, 4428, 4428, 4075, 4919, 4240, 4121, 4428, 4530, 4429, 4075, 4787, 4890, 4428,
/* 2088 */ 5780, 4918, 6049, 4419, 4800, 4540, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/* 2106 */ 4075, 4075, 4075, 4075, 4075, 4075, 4556, 6089, 4075, 5077, 4358, 4567, 4569, 4428, 4428, 3743, 4075, 4787,
/* 2124 */ 4239, 6055, 4075, 4428, 4429, 4889, 4428, 4428, 5761, 5363, 5359, 4301, 3785, 4428, 4428, 4428, 4311, 4159,
/* 2142 */ 4919, 4240, 4121, 5924, 4428, 4429, 4075, 4787, 4890, 4428, 4428, 5353, 4429, 4419, 4728, 4548, 4075, 4075,
/* 2160 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4594, 3691,
/* 2178 */ 4577, 5833, 4587, 4605, 4610, 4428, 5301, 4618, 4075, 4787, 5129, 4631, 4075, 4428, 4640, 5003, 4428, 4651,
/* 2196 */ 6054, 4075, 4789, 4301, 4660, 4428, 4428, 4748, 4428, 4670, 4698, 4706, 4121, 4428, 4714, 4725, 4444, 4736,
/* 2214 */ 4764, 4775, 4428, 4783, 4797, 4824, 4808, 4832, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/* 2232 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4850, 4075, 4075, 5686, 4690, 4864, 4866, 4311, 4428, 3743,
/* 2250 */ 4874, 4787, 4886, 6055, 4075, 5551, 4429, 4889, 4428, 4428, 5945, 4075, 6055, 4301, 3785, 4428, 4428, 4428,
/* 2268 */ 4428, 4898, 4919, 4240, 4121, 4428, 4428, 4429, 4908, 4787, 4890, 4428, 4428, 4404, 4429, 4419, 5674, 4544,
/* 2286 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/* 2304 */ 4373, 4075, 4075, 4920, 4239, 4890, 4428, 4428, 4428, 3743, 4075, 4787, 4239, 6055, 4075, 4428, 4429, 4889,
/* 2322 */ 4428, 4428, 6054, 4075, 6055, 4301, 3785, 4428, 4428, 4388, 4428, 4075, 4919, 4240, 5181, 4428, 4428, 4429,
/* 2340 */ 4075, 4916, 4890, 6036, 4428, 4918, 4429, 4419, 4728, 4544, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/* 2358 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4373, 4075, 4075, 4920, 4239, 4890, 4428, 5398,
/* 2376 */ 4139, 3743, 4075, 4928, 4239, 4938, 4075, 4947, 4957, 4889, 4428, 4428, 6054, 4075, 6055, 4301, 3785, 4428,
/* 2394 */ 4428, 4428, 4428, 4075, 4919, 4240, 4121, 4428, 4388, 4429, 4075, 4787, 4890, 4428, 4976, 4918, 4429, 4419,
/* 2412 */ 4728, 4544, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/* 2430 */ 4075, 4075, 4985, 4075, 4996, 5715, 3673, 5015, 5017, 4428, 4428, 3743, 4075, 4787, 4239, 6055, 4075, 4428,
/* 2448 */ 4429, 4889, 4428, 5025, 5033, 4043, 5662, 4301, 3785, 5051, 5062, 4428, 4428, 5073, 4919, 4240, 4121, 4428,
/* 2466 */ 5197, 5756, 4075, 4787, 5085, 5103, 4428, 4918, 4347, 4419, 4728, 4544, 4075, 4075, 4075, 4075, 4075, 4075,
/* 2484 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4373, 4075, 4075, 4920, 4239, 4890,
/* 2502 */ 4428, 4428, 4428, 3743, 4075, 4787, 4239, 6055, 4075, 4428, 4429, 4889, 4428, 4428, 6054, 4075, 6055, 4301,
/* 2520 */ 3785, 4428, 4428, 4428, 4428, 4075, 4919, 4240, 4121, 4428, 4389, 4429, 4075, 4787, 5114, 4428, 4428, 4918,
/* 2538 */ 4429, 4419, 4728, 4544, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/* 2556 */ 4075, 4075, 4075, 4075, 5126, 4075, 4075, 5863, 5247, 5137, 5139, 5963, 4428, 5147, 4075, 5095, 3702, 6055,
/* 2574 */ 4075, 5162, 4480, 4889, 4652, 4428, 6054, 4075, 6055, 5174, 3785, 4428, 4428, 4311, 5189, 4075, 5205, 5472,
/* 2592 */ 4121, 5218, 5229, 5106, 4075, 4787, 5238, 5612, 4428, 4918, 4429, 4419, 4728, 4544, 4075, 4075, 4075, 4075,
/* 2610 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4373, 4075, 4075, 4920,
/* 2628 */ 4239, 4890, 4428, 4428, 4428, 3743, 4202, 4787, 4205, 6055, 4075, 4428, 4429, 4889, 5316, 5007, 6054, 6145,
/* 2646 */ 5255, 4301, 3785, 5054, 4428, 4428, 4428, 4075, 4919, 4240, 4121, 4428, 4428, 4429, 4075, 4787, 4890, 4428,
/* 2664 */ 4428, 4918, 4429, 4643, 4728, 4544, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/* 2682 */ 4075, 4075, 4075, 4075, 4075, 4075, 5264, 4075, 4076, 3762, 3651, 5275, 5277, 4490, 4428, 4756, 4597, 4408,
/* 2700 */ 5285, 6055, 4075, 5703, 4429, 4208, 4767, 4428, 4754, 4966, 4963, 4301, 5296, 5314, 4428, 4428, 4428, 4075,
/* 2718 */ 4255, 4240, 4121, 5534, 4428, 4122, 4075, 5987, 4890, 4428, 5324, 5335, 5347, 4419, 4728, 4544, 4075, 4075,
/* 2736 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 5371, 4075,
/* 2754 */ 4075, 5915, 4579, 5382, 5384, 5437, 4532, 3743, 4075, 4787, 4376, 6055, 4075, 5392, 5306, 4889, 5409, 5420,
/* 2772 */ 6054, 4075, 6055, 4301, 5430, 5445, 4428, 4428, 5221, 4448, 4919, 4240, 4121, 5327, 4428, 4429, 4075, 4787,
/* 2790 */ 4890, 4428, 4428, 4918, 4429, 5457, 4728, 4544, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/* 2808 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 5469, 4075, 4075, 6016, 4900, 5480, 5482, 4428, 5490, 3743,
/* 2826 */ 4075, 4787, 4239, 6055, 4075, 4428, 4429, 4889, 4428, 4428, 6054, 5500, 6001, 4301, 3785, 4428, 4428, 4428,
/* 2844 */ 5166, 4075, 4919, 4240, 4121, 5449, 4428, 4429, 4075, 4787, 4890, 4428, 5899, 4260, 4429, 4419, 4728, 4544,
/* 2862 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/* 2880 */ 5511, 4075, 4075, 6068, 4968, 5522, 5524, 5532, 5542, 3743, 5559, 4787, 4239, 5569, 4075, 5579, 5587, 4889,
/* 2898 */ 4389, 5401, 5975, 4075, 6055, 4301, 5600, 5422, 4428, 5230, 4428, 4075, 4919, 4240, 5608, 4428, 4428, 5903,
/* 2916 */ 5620, 4787, 4890, 5793, 4428, 4918, 4429, 4419, 4728, 4544, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/* 2934 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4373, 4075, 4075, 4920, 4239, 4890, 4428, 4428,
/* 2952 */ 4106, 3743, 4075, 5633, 5644, 6055, 4075, 4428, 5656, 4889, 5195, 4428, 5592, 4075, 5829, 4301, 3785, 4428,
/* 2970 */ 5118, 4428, 4428, 4075, 5374, 5503, 5670, 4428, 4312, 4429, 4169, 5682, 5694, 4428, 4428, 4918, 4429, 4419,
/* 2988 */ 4728, 4544, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/* 3006 */ 4075, 4075, 5711, 4075, 4075, 6111, 5723, 5731, 5733, 4428, 4428, 3743, 4075, 4787, 4239, 6055, 4075, 4428,
/* 3024 */ 4429, 4815, 4388, 5547, 4754, 4075, 6055, 4301, 3785, 5741, 4820, 5769, 5778, 4075, 4919, 4240, 5788, 5805,
/* 3042 */ 4428, 4429, 4075, 4787, 5815, 4428, 5797, 5847, 5823, 5091, 5841, 4544, 4075, 4075, 4075, 4075, 4075, 4075,
/* 3060 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 5859, 4075, 4075, 5871, 4508, 5884,
/* 3078 */ 5886, 4428, 5894, 3743, 4075, 5911, 4281, 6055, 4075, 4662, 4429, 4889, 5923, 4949, 6054, 4075, 6055, 5932,
/* 3096 */ 5957, 4977, 4428, 4428, 5746, 4075, 4919, 4240, 4121, 5751, 4428, 4429, 5267, 4787, 4890, 4428, 4428, 4918,
/* 3114 */ 4429, 5983, 5995, 4544, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/* 3132 */ 4075, 4075, 4075, 4075, 6012, 4075, 4075, 3757, 5561, 6024, 6026, 4428, 4428, 3743, 4075, 4787, 4239, 6055,
/* 3150 */ 4075, 4428, 4429, 4889, 4428, 4428, 6054, 4075, 6055, 4301, 3785, 4428, 4428, 4428, 6034, 4075, 4919, 4240,
/* 3168 */ 4121, 6044, 4428, 4429, 4075, 4787, 4890, 4428, 4428, 4918, 4429, 4419, 4728, 4544, 4075, 4075, 4075, 4075,
/* 3186 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 6064, 4075, 4075, 6076,
/* 3204 */ 5571, 6097, 6099, 4428, 4428, 3743, 4075, 4787, 4239, 6055, 4075, 4428, 4429, 4889, 4428, 4428, 6054, 4075,
/* 3222 */ 6055, 4301, 3785, 4428, 4428, 4428, 4428, 4075, 4919, 4240, 4121, 4428, 4428, 4149, 4075, 4787, 4890, 5412,
/* 3240 */ 4428, 4918, 4429, 4419, 4728, 4544, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/* 3258 */ 4075, 4075, 4075, 4075, 4075, 4075, 4373, 4075, 4075, 4920, 4239, 4890, 4428, 4428, 4428, 3743, 4075, 5339,
/* 3276 */ 4239, 6055, 4075, 4428, 4429, 4889, 4428, 4311, 6054, 4075, 6055, 5648, 3785, 5770, 4428, 4428, 4428, 4075,
/* 3294 */ 4919, 4240, 4121, 4428, 4428, 4429, 4075, 4787, 4890, 4428, 4428, 4918, 4429, 4419, 4728, 4544, 4075, 4075,
/* 3312 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 3699, 4075,
/* 3330 */ 4075, 6107, 4271, 6119, 6121, 4075, 4075, 3743, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/* 3348 */ 4075, 4075, 4075, 3744, 4074, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 3931, 4075, 4075, 4075, 4075, 4075,
/* 3366 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/* 3384 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 3699, 4075, 4075, 4075, 4075, 4688, 4682, 4075, 4075, 3743,
/* 3402 */ 4075, 4075, 4075, 4075, 5636, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 3744, 4074, 4075, 4075, 4075,
/* 3420 */ 4075, 4075, 4075, 4075, 3931, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/* 3438 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/* 3456 */ 3699, 4075, 4075, 4939, 4075, 6135, 6129, 4075, 4075, 3743, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/* 3474 */ 4075, 4075, 4075, 4075, 4075, 3744, 4074, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 3931, 4075, 4075, 4075,
/* 3492 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/* 3510 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 3699, 4075, 4075, 4075, 5256, 6153, 6155, 4075,
/* 3528 */ 4075, 3743, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 3744, 4074, 4075,
/* 3546 */ 4075, 4075, 4075, 4075, 4075, 4075, 3931, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/* 3564 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/* 3582 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4218, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/* 3600 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/* 3618 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075,
/* 3636 */ 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 4075, 0, 0, 4682, 0, 0, 0, 0, 0, 0, 2152,
/* 3658 */ 2152, 73, 4682, 0, 0, 0, 0, 0, 0, 73, 73, 0, 0, 4608, 6656, 0, 0, 0, 0, 97, 98, 2111, 2111, 0, 6656, 6656,
/* 3684 */ 6656, 6656, 6656, 6656, 6656, 0, 344, 0, 0, 0, 0, 0, 0, 78, 79, 0, 0, 4608, 0, 0, 0, 0, 0, 0, 2195, 2105,
/* 3710 */ 108, 108, 7168, 7168, 7168, 7168, 7168, 7168, 7276, 7276, 7276, 7276, 0, 0, 73, 73, 73, 73, 73, 73, 73, 73,
/* 3732 */ 4096, 0, 0, 0, 0, 0, 0, 0, 257, 0, 0, 73, 0, 0, 0, 0, 0, 0, 0, 192, 0, 0, 4608, 0, 8192, 0, 0, 0, 0, 2119, 0,
/* 3763 */ 0, 0, 0, 2139, 0, 0, 0, 0, 8813, 8813, 8813, 8813, 0, 0, 8192, 8192, 8192, 8192, 8192, 8192, 8192, 8192,
/* 3785 */ 6337, 2105, 2105, 2105, 2105, 2105, 2105, 2105, 183, 8813, 8813, 0, 0, 0, 0, 0, 0, 100, 106, 9838, 9838, 0,
/* 3807 */ 0, 0, 0, 0, 0, 101, 107, 0, 0, 4608, 0, 0, 10752, 0, 0, 0, 0, 25600, 25600, 25600, 25600, 0, 0, 0, 0, 0, 0,
/* 3834 */ 2106, 2106, 0, 10752, 10752, 10752, 10752, 10752, 10752, 10752, 10752, 0, 0, 4608, 0, 0, 0, 11264, 0, 0, 0,
/* 3855 */ 0, 11264, 11264, 11264, 11264, 11264, 11264, 11264, 11264, 11887, 11887, 11776, 11776, 11776, 11776, 11776,
/* 3871 */ 11776, 11887, 11887, 11887, 11887, 257, 0, 0, 0, 0, 0, 0, 0, 7168, 184, 184, 0, 0, 0, 0, 0, 0, 173, 0, 0, 0,
/* 3897 */ 4608, 0, 0, 0, 0, 75, 12912, 12912, 106, 106, 106, 106, 106, 106, 12936, 12936, 12936, 12936, 73, 0, 13312,
/* 3918 */ 0, 0, 0, 0, 0, 13312, 0, 252, 0, 0, 0, 0, 0, 192, 0, 0, 0, 0, 0, 0, 0, 182, 0, 303, 100, 0, 100, 0, 0, 0, 0,
/* 3949 */ 9838, 9838, 9838, 9838, 100, 100, 0, 0, 0, 0, 0, 0, 184, 0, 0, 14336, 14336, 14336, 14336, 14336, 14336,
/* 3970 */ 14336, 14336, 0, 0, 0, 0, 0, 14336, 0, 0, 0, 0, 0, 0, 2235, 2199, 76, 0, 0, 0, 0, 0, 0, 0, 11776, 14961,
/* 3996 */ 14967, 107, 107, 107, 107, 107, 107, 14985, 14987, 14985, 14987, 73, 0, 0, 15360, 0, 0, 0, 0, 0, 15360, 0, 0,
/* 4019 */ 182, 15542, 0, 0, 0, 0, 0, 0, 5734, 5734, 15542, 0, 15542, 0, 0, 0, 0, 0, 0, 10752, 10752, 0, 0, 16896, 0, 0,
/* 4045 */ 0, 0, 0, 0, 244, 245, 249, 249, 0, 0, 0, 0, 0, 0, 249, 0, 17522, 17522, 120, 120, 120, 120, 120, 120, 17546,
/* 4070 */ 17546, 17546, 17546, 0, 6337, 0, 0, 0, 0, 0, 0, 0, 0, 87, 6336, 0, 0, 0, 0, 0, 0, 0, 13312, 0, 0, 5734, 5734,
/* 4097 */ 5734, 5734, 5734, 5734, 5734, 5734, 5734, 0, 5734, 2105, 2105, 2105, 2105, 2105, 2208, 2105, 2105, 5883,
/* 4115 */ 5885, 2105, 2105, 2105, 0, 0, 192, 2105, 2105, 2105, 2105, 2105, 2105, 2105, 330, 302, 304, 5937, 5885, 5938,
/* 4135 */ 2105, 2105, 2105, 2197, 2105, 2105, 2105, 2105, 2206, 2105, 2105, 2210, 5937, 5938, 2105, 2105, 2105, 2105,
/* 4153 */ 2105, 2105, 57, 0, 0, 18432, 0, 0, 0, 0, 0, 0, 292, 0, 18432, 18432, 0, 0, 0, 0, 0, 0, 334, 0, 0, 0, 18944,
/* 4180 */ 0, 0, 0, 0, 0, 0, 16896, 16896, 0, 0, 18944, 18944, 18944, 18944, 18944, 18944, 18944, 18944, 0, 0, 0, 19456,
/* 4202 */ 0, 0, 0, 0, 171, 0, 0, 0, 0, 2105, 2105, 2105, 2105, 2265, 19571, 19571, 0, 0, 0, 0, 0, 0, 1536, 0, 0, 0,
/* 4228 */ 20158, 0, 0, 0, 0, 0, 0, 24576, 24576, 21620, 21620, 0, 0, 0, 0, 0, 0, 2105, 2105, 2105, 15872, 17920, 20992,
/* 4251 */ 22016, 23040, 0, 15872, 0, 0, 0, 297, 0, 2105, 0, 0, 0, 0, 350, 2399, 0, 15872, 17920, 15872, 0, 0, 0, 0, 0,
/* 4276 */ 0, 44032, 44032, 22645, 22645, 0, 0, 0, 0, 0, 0, 2105, 2236, 0, 0, 0, 16384, 0, 0, 0, 0, 0, 0, 0, 186, 0, 0,
/* 4303 */ 2105, 2105, 2105, 0, 0, 192, 186, 250, 2105, 2105, 2105, 2105, 2105, 2105, 57, 2105, 2372, 0, 24576, 24576,
/* 4323 */ 24576, 24576, 24576, 24576, 24576, 24576, 163, 0, 0, 0, 0, 0, 0, 0, 13824, 25088, 25088, 25088, 25088, 25088,
/* 4343 */ 25088, 25088, 25088, 2199, 2105, 2105, 2105, 2105, 2105, 2105, 2105, 357, 0, 0, 2225, 0, 0, 0, 0, 0, 77,
/* 4364 */ 2108, 2108, 2105, 2189, 2105, 2235, 2105, 2105, 2225, 2105, 0, 4608, 0, 0, 0, 0, 0, 0, 2196, 2105, 6337,
/* 4385 */ 2105, 2307, 2308, 2105, 2105, 2105, 2105, 57, 2105, 2105, 2105, 2105, 192, 2356, 2105, 2105, 2105, 2105,
/* 4403 */ 2360, 2105, 0, 34816, 0, 0, 0, 2105, 0, 0, 0, 167, 167, 2373, 2374, 2105, 2105, 2105, 2105, 2105, 0, 0, 2105,
/* 4426 */ 0, 2387, 2105, 2105, 2105, 2105, 2105, 2105, 2105, 2105, 0, 2105, 2406, 2105, 2105, 0, 360, 2409, 0, 0, 0,
/* 4447 */ 331, 0, 0, 0, 0, 290, 0, 0, 293, 2106, 0, 4608, 0, 0, 0, 0, 0, 92, 0, 0, 0, 0, 2169, 2169, 2169, 2169, 2169,
/* 4474 */ 2169, 2169, 2169, 2105, 2105, 2275, 2105, 2105, 2105, 2105, 2105, 2244, 2261, 0, 2105, 2280, 2105, 2105,
/* 4492 */ 2105, 2105, 2105, 2105, 2197, 2105, 93, 0, 0, 0, 0, 0, 2107, 2107, 0, 4608, 0, 0, 0, 0, 0, 99, 2118, 2118, 0,
/* 4517 */ 0, 2170, 2170, 2170, 2170, 2170, 2170, 2182, 2170, 2170, 2170, 2170, 2170, 2105, 2369, 2105, 2105, 2105,
/* 4535 */ 2105, 2105, 2105, 2209, 2105, 2415, 0, 2105, 2105, 2105, 0, 2105, 2105, 2105, 0, 2105, 2105, 2105, 0, 2105,
/* 4555 */ 38457, 2108, 0, 4608, 0, 0, 0, 0, 0, 100, 0, 0, 0, 0, 2171, 2171, 2171, 2171, 2171, 2171, 2171, 2171, 80, 81,
/* 4579 */ 0, 0, 0, 0, 0, 0, 2114, 2114, 0, 95, 0, 96, 0, 81, 2109, 2109, 0, 4608, 0, 0, 0, 0, 0, 172, 0, 174, 0, 0,
/* 4607 */ 2109, 2109, 2109, 2109, 2109, 2109, 2183, 2109, 2109, 2109, 2109, 73, 0, 0, 0, 164, 0, 0, 0, 0, 21620, 21620,
/* 4629 */ 21620, 21620, 2205, 0, 0, 0, 0, 0, 0, 0, 14336, 2253, 2254, 2105, 2105, 2105, 2105, 2105, 0, 0, 2105, 362,
/* 4651 */ 2272, 2105, 2105, 2105, 2105, 2105, 2105, 2105, 2271, 6337, 2306, 2105, 2105, 2105, 2105, 2105, 2105, 2236,
/* 4669 */ 2252, 0, 0, 30720, 0, 0, 291, 0, 0, 0, 182, 182, 100, 0, 0, 0, 0, 44662, 44662, 44662, 44662, 0, 0, 0, 0, 0,
/* 4695 */ 0, 2151, 2151, 0, 295, 296, 0, 0, 2347, 0, 301, 0, 0, 186, 0, 0, 2105, 2105, 3129, 2105, 2105, 31042, 2105,
/* 4718 */ 2105, 2105, 2105, 2105, 2260, 2105, 0, 2105, 2105, 2375, 2105, 2105, 2105, 2105, 0, 2105, 2105, 0, 0, 43520,
/* 4738 */ 2105, 0, 0, 250, 0, 0, 0, 183, 5734, 5734, 2105, 2105, 57, 2105, 2105, 2328, 2105, 2105, 73, 0, 0, 0, 0, 0,
/* 4762 */ 0, 167, 186, 250, 3641, 2105, 2105, 2105, 2105, 2105, 2269, 2105, 2105, 2105, 2388, 2105, 2105, 2105, 2105,
/* 4781 */ 2105, 43577, 2105, 28672, 0, 0, 0, 0, 2105, 0, 0, 0, 0, 0, 250, 0, 2105, 29753, 2105, 2105, 2105, 2105, 2105,
/* 4804 */ 0, 2267, 2105, 366, 2105, 37945, 2105, 2105, 0, 2105, 2279, 0, 0, 0, 2105, 2263, 2105, 2105, 2105, 2322,
/* 4824 */ 2105, 2105, 2105, 2105, 35840, 0, 2105, 0, 2105, 41472, 28217, 2416, 39993, 37376, 37433, 2105, 183, 5734, 0,
/* 4843 */ 0, 0, 0, 0, 0, 16896, 0, 2110, 0, 4608, 0, 0, 0, 0, 0, 184, 0, 184, 0, 0, 0, 0, 0, 2172, 2172, 2172, 2172,
/* 4870 */ 2172, 2172, 2172, 2172, 0, 33792, 169, 170, 0, 0, 0, 0, 2106, 0, 0, 0, 33973, 33973, 0, 0, 0, 0, 2105, 2105,
/* 4894 */ 2105, 2105, 2105, 2105, 0, 30208, 0, 0, 0, 0, 0, 0, 2115, 2115, 31744, 0, 34304, 0, 0, 0, 0, 41984, 42496, 0,
/* 4918 */ 2105, 0, 0, 0, 0, 0, 2105, 0, 0, 0, 0, 176, 2105, 0, 0, 0, 0, 0, 2105, 352, 2206, 0, 0, 0, 0, 0, 0, 0, 45568,
/* 4947 */ 2105, 2243, 2105, 2105, 2105, 2105, 2105, 2105, 2267, 2105, 2105, 2255, 2105, 2105, 2259, 2243, 2105, 0, 0,
/* 4966 */ 0, 239, 0, 0, 0, 0, 0, 0, 2116, 2116, 26681, 2105, 2105, 2105, 2105, 2105, 2105, 2105, 2304, 2111, 0, 4608,
/* 4988 */ 0, 0, 0, 0, 0, 185, 0, 0, 0, 0, 82, 83, 84, 85, 86, 0, 0, 0, 2262, 2105, 2105, 2105, 2105, 2277, 2105, 2105,
/* 5014 */ 2105, 0, 0, 2173, 2173, 2173, 2173, 2173, 2173, 2173, 2173, 2105, 2273, 2105, 2105, 2105, 2105, 2105, 2198,
/* 5033 */ 2279, 2280, 0, 0, 234, 0, 0, 0, 0, 22528, 0, 0, 0, 0, 19571, 19571, 19571, 19571, 2105, 2105, 2313, 2105,
/* 5055 */ 2105, 2105, 2105, 2105, 2317, 2105, 2105, 2319, 2320, 2321, 2105, 2105, 2105, 2105, 2105, 2324, 2105, 2105,
/* 5073 */ 288, 0, 0, 289, 0, 0, 0, 0, 2108, 0, 0, 0, 0, 0, 2105, 2105, 2105, 2385, 2105, 2105, 2105, 2407, 0, 0, 2105,
/* 5098 */ 0, 178, 0, 166, 180, 2105, 2105, 2389, 2105, 2105, 2105, 2105, 2105, 2330, 2105, 0, 0, 0, 2105, 26169, 2105,
/* 5119 */ 2105, 2105, 2105, 2323, 2105, 2105, 2326, 2112, 0, 4608, 0, 0, 0, 0, 0, 186, 2105, 2105, 0, 0, 2174, 2174,
/* 5141 */ 2174, 2174, 2174, 2174, 2174, 2174, 73, 0, 0, 0, 0, 165, 166, 0, 0, 0, 7680, 9216, 0, 12288, 13824, 2105,
/* 5163 */ 2244, 2105, 2247, 2105, 2105, 2105, 2105, 2334, 2105, 2105, 2105, 0, 0, 2302, 2303, 2105, 0, 0, 192, 2105,
/* 5183 */ 2105, 2105, 2105, 2105, 2105, 36921, 2105, 2105, 2332, 2105, 2105, 2105, 2105, 2267, 2105, 2105, 2105, 2105,
/* 5201 */ 2105, 2105, 2371, 2105, 294, 0, 0, 0, 298, 2105, 0, 0, 0, 0, 183, 183, 0, 2105, 2105, 2363, 2105, 2105, 2105,
/* 5224 */ 2105, 2105, 2335, 2105, 2105, 2368, 2105, 2105, 2105, 2105, 2105, 2105, 2105, 2329, 0, 0, 2105, 2105, 2105,
/* 5243 */ 2105, 2386, 2105, 247, 0, 0, 0, 0, 0, 0, 2112, 2112, 2294, 0, 0, 0, 0, 0, 0, 0, 46080, 2113, 0, 4608, 0, 0,
/* 5269 */ 0, 0, 0, 333, 0, 0, 0, 0, 2175, 2175, 2175, 2175, 2175, 2175, 2175, 2175, 0, 172, 174, 0, 0, 0, 2105, 2105,
/* 5293 */ 2196, 2264, 2105, 6337, 2105, 2105, 2105, 2309, 2105, 2105, 2105, 2204, 2205, 2105, 2105, 2105, 2258, 2105,
/* 5311 */ 2245, 2105, 0, 2105, 2312, 2105, 2105, 2105, 2105, 2105, 2105, 2270, 2105, 2105, 2105, 2393, 2105, 2105,
/* 5329 */ 2105, 2105, 2105, 2366, 2105, 2105, 57, 0, 0, 0, 0, 0, 2105, 0, 0, 179, 0, 0, 2401, 2105, 2105, 2105, 2105,
/* 5352 */ 39481, 2105, 0, 0, 0, 349, 0, 2105, 0, 0, 0, 238, 0, 0, 0, 0, 243, 0, 0, 2114, 0, 4608, 0, 0, 0, 0, 0, 2105,
/* 5380 */ 300, 0, 0, 0, 2114, 2114, 2114, 2114, 2114, 2114, 2114, 2114, 2105, 2245, 29382, 2248, 2105, 2250, 2105,
/* 5399 */ 2105, 2190, 2105, 2105, 2105, 2105, 2105, 2278, 2105, 2105, 2105, 2105, 2268, 2105, 2105, 2105, 2105, 2105,
/* 5417 */ 40505, 2105, 2105, 2105, 2274, 2105, 2105, 2105, 2105, 2105, 2105, 2318, 2105, 6337, 2105, 2105, 2105, 2105,
/* 5435 */ 2105, 2311, 2105, 2105, 2192, 145, 2105, 2196, 2105, 2198, 2105, 2105, 2314, 2105, 2105, 2105, 2105, 2105,
/* 5453 */ 2365, 2105, 2105, 2105, 2105, 2105, 2196, 2105, 0, 0, 2105, 0, 0, 250, 0, 0, 2115, 0, 4608, 0, 0, 0, 0, 0,
/* 5477 */ 2105, 2617, 2105, 0, 0, 2176, 2176, 2176, 2176, 2176, 2176, 2176, 2176, 2105, 2201, 2105, 2105, 2105, 2105,
/* 5496 */ 2105, 2105, 2325, 2105, 0, 0, 240, 0, 0, 0, 0, 0, 2355, 2105, 2105, 2116, 0, 4608, 0, 0, 0, 0, 0, 20480,
/* 5520 */ 24064, 192, 0, 0, 2177, 2177, 2177, 2177, 2177, 2177, 2177, 2177, 2105, 2188, 2105, 2105, 2105, 2105, 2105,
/* 5539 */ 2105, 2367, 2105, 2105, 2105, 2202, 2105, 2207, 2105, 2105, 2105, 2276, 2105, 2105, 2105, 2105, 33849, 2105,
/* 5557 */ 2105, 2105, 168, 0, 0, 0, 0, 0, 0, 0, 2119, 2119, 2237, 0, 0, 0, 0, 0, 0, 0, 2120, 2120, 2242, 2105, 2105,
/* 5582 */ 2105, 2105, 2105, 2105, 2251, 2105, 2256, 2105, 2105, 2105, 2105, 2105, 0, 0, 0, 235, 0, 0, 6337, 2105, 2105,
/* 5603 */ 2105, 2105, 2105, 2105, 33337, 192, 2105, 27193, 2105, 2105, 2105, 2105, 2105, 2391, 2105, 2105, 2105, 0,
/* 5621 */ 32256, 0, 0, 332, 0, 0, 0, 0, 22645, 22645, 22645, 22645, 175, 0, 2105, 0, 0, 0, 0, 0, 45056, 0, 0, 0, 0,
/* 5646 */ 175, 0, 0, 0, 2105, 2105, 57, 0, 0, 192, 2105, 2105, 2257, 2105, 2105, 2105, 2105, 0, 0, 234, 0, 0, 0, 5120,
/* 5670 */ 192, 2105, 2105, 2357, 2105, 2105, 2105, 2105, 31232, 2105, 2105, 0, 0, 0, 2383, 27648, 0, 0, 0, 0, 2110, 0,
/* 5692 */ 0, 0, 0, 0, 2105, 2105, 27705, 2105, 2105, 2105, 2315, 2105, 2105, 2105, 2105, 2249, 2105, 2105, 2105, 2117,
/* 5712 */ 0, 4608, 0, 0, 0, 0, 0, 2111, 0, 0, 0, 94, 0, 0, 0, 0, 0, 2153, 2153, 0, 0, 2178, 2178, 2178, 2178, 2178,
/* 5738 */ 2178, 2178, 2178, 36409, 2105, 2105, 2105, 2316, 2105, 2105, 2105, 2333, 2105, 2105, 2105, 2105, 2364, 2105,
/* 5756 */ 2105, 2105, 2105, 2376, 2105, 2105, 2105, 0, 0, 0, 0, 236, 0, 2327, 2105, 2105, 2105, 2105, 2105, 2105, 2105,
/* 5777 */ 41017, 2105, 2331, 2105, 2105, 2105, 2105, 2105, 2105, 2396, 57, 192, 2105, 2105, 2105, 2358, 2105, 2105,
/* 5795 */ 2105, 2390, 2105, 2105, 2105, 2105, 2395, 2267, 2105, 2105, 2105, 2362, 2105, 2105, 2105, 2105, 2105, 2105,
/* 5813 */ 2404, 0, 0, 0, 2105, 2105, 2105, 2105, 2105, 32825, 2105, 2105, 2402, 2105, 2105, 2105, 2105, 0, 0, 235, 0,
/* 5834 */ 0, 0, 0, 2109, 0, 0, 0, 2105, 2105, 2412, 2413, 0, 2105, 2105, 0, 0, 35328, 0, 0, 2105, 0, 0, 302, 304, 304,
/* 5859 */ 2118, 0, 4608, 0, 0, 0, 0, 0, 2112, 0, 0, 0, 88, 89, 0, 0, 2118, 0, 0, 0, 0, 23743, 0, 16384, 0, 0, 0, 2179,
/* 5887 */ 2179, 2179, 2179, 2179, 2179, 2179, 2179, 2200, 2105, 2203, 2105, 2105, 2105, 2105, 2105, 2394, 2105, 2105,
/* 5905 */ 2105, 2105, 2377, 2105, 2105, 0, 0, 0, 2200, 0, 0, 0, 0, 0, 2114, 0, 0, 0, 2266, 2105, 2105, 2105, 2105,
/* 5928 */ 2105, 2105, 2105, 43065, 0, 0, 2105, 2105, 2304, 0, 0, 192, 2105, 2105, 2105, 2105, 2359, 2105, 2105, 73,
/* 5948 */ 233, 0, 0, 0, 0, 2107, 0, 0, 0, 6337, 2105, 2105, 2105, 2105, 2310, 2105, 2105, 2191, 2105, 2194, 2195, 2105,
/* 5970 */ 2105, 2189, 2105, 2105, 2109, 2105, 2105, 73, 0, 0, 0, 0, 237, 2267, 2105, 2105, 2105, 0, 0, 2105, 0, 336, 0,
/* 5993 */ 0, 0, 2411, 2105, 2105, 2105, 0, 2105, 2105, 0, 248, 0, 0, 0, 0, 0, 10240, 0, 0, 2119, 0, 4608, 0, 0, 0, 0,
/* 6019 */ 0, 2115, 0, 0, 0, 0, 0, 2180, 2180, 2180, 2180, 2180, 2180, 2180, 2180, 2330, 2105, 2105, 2105, 2105, 2105,
/* 6040 */ 2105, 2105, 42553, 2105, 2361, 2105, 2105, 2105, 2105, 2105, 2105, 2105, 2403, 38969, 2105, 2105, 0, 0, 0, 0,
/* 6060 */ 0, 0, 0, 6337, 2120, 0, 4608, 0, 0, 0, 0, 0, 2116, 0, 0, 0, 0, 0, 90, 0, 2120, 0, 0, 0, 0, 25088, 0, 0,
/* 6088 */ 25088, 0, 0, 0, 0, 0, 77, 0, 0, 0, 0, 2181, 2181, 2181, 2181, 2181, 2181, 2181, 2181, 0, 0, 0, 44032, 0, 0,
/* 6113 */ 0, 0, 2117, 0, 0, 0, 0, 0, 44032, 44032, 44032, 44032, 44032, 44032, 44032, 44032, 0, 45568, 45568, 45568,
/* 6133 */ 45568, 45568, 45568, 45568, 0, 0, 0, 45568, 0, 0, 0, 241, 0, 0, 0, 0, 242, 0, 0, 0, 0, 0, 46080, 46080,
/* 6157 */ 46080, 46080, 46080, 46080, 46080, 46080
};

const int WebCpp::EXPECTED[] =
{
/*   0 */ 69, 103, 76, 80, 82, 168, 86, 90, 94, 116, 101, 107, 111, 116, 123, 200, 115, 117, 174, 121, 129, 133, 97,
/*  23 */ 147, 137, 141, 145, 240, 151, 102, 155, 190, 102, 181, 162, 166, 172, 241, 207, 166, 178, 187, 197, 204, 213,
/*  45 */ 217, 102, 221, 260, 225, 102, 229, 233, 193, 238, 102, 183, 245, 72, 249, 234, 253, 158, 239, 257, 209, 125,
/*  67 */ 264, 268, 272, 278, 301, 302, 303, 537, 275, 450, 358, 282, 286, 290, 294, 374, 374, 298, 302, 381, 308, 313,
/*  89 */ 317, 321, 510, 326, 330, 337, 343, 347, 374, 351, 350, 350, 355, 302, 302, 302, 302, 333, 532, 362, 308, 370,
/* 111 */ 373, 374, 374, 339, 373, 374, 374, 374, 374, 375, 366, 374, 374, 374, 302, 302, 453, 574, 374, 374, 532, 530,
/* 133 */ 374, 375, 374, 530, 379, 302, 302, 304, 387, 391, 395, 399, 402, 406, 302, 302, 454, 540, 380, 302, 302, 443,
/* 155 */ 410, 414, 419, 302, 322, 546, 552, 559, 302, 302, 430, 440, 464, 302, 302, 524, 302, 426, 425, 302, 302, 530,
/* 177 */ 362, 447, 302, 302, 436, 542, 302, 302, 479, 458, 379, 302, 557, 302, 423, 508, 302, 468, 303, 514, 415, 463,
/* 199 */ 302, 302, 531, 364, 309, 555, 302, 468, 302, 558, 302, 302, 546, 563, 475, 570, 302, 302, 435, 556, 433, 302,
/* 221 */ 470, 455, 459, 483, 497, 501, 302, 302, 471, 456, 565, 568, 505, 302, 302, 302, 478, 518, 522, 302, 302, 302,
/* 243 */ 541, 380, 567, 528, 302, 302, 520, 302, 302, 302, 457, 566, 302, 302, 477, 456, 565, 569, 487, 491, 494, 382,
/* 265 */ 548, 302, 574, 383, 533, 576, 580, 8, 128, 256, 4096, 8192, 16384, 32768, 1048576, 2097152, 33554432,
/* 282 */ 2144288768, -2146304, 575713016, 577810168, 575713016, 575713016, 575713020, 709930744, 712027896, 712031992,
/* 292 */ 712031992, 712031992, 2147483384, -264, 2147483384, -264, 128, 256, 33554432, 536870912, 0, 0, 0, 0, 1, 0,
/* 308 */ 1536, 1536, 72, 24, 40, 40, 33554944, 536872448, 16384, 131072, 524288, 8388608, 100663296, 1610612736, 0, 0,
/* 324 */ 0, 128, 8, 8, 8, 72, 8, 8, 24, 8, -2146435072, 1048576, 134217728, 8, 40, 8, 8, 8, 40, 8, 8, 8, 100663808,
/* 347 */ 1610614272, 2048, -536869376, 8, 0, 8, 8, 0, 8, 8, 128, 0, 0, 1050360, 575678200, 0, 512, 512, 512, 1536,
/* 367 */ 1536, 72, 8, 40, 0, 0, 2048, 8, 8, 8, 8, 0, 536870912, 1073741824, 0, 0, 0, 512, 1024, 8192, 2097152,
/* 388 */ 671088640, 65552, 738197504, 0, 1082130432, 32768, 32768, 163822, 196591, -2089517040, -2089517040,
/* 399 */ -2089451504, -2089517040, -2089517040, -2089451504, -2089451504, -2088927216, -1686798320, -2089287690,
/* 407 */ -2089287690, -2089287689, -2089287689, 0, 4, 448, 1024, 28672, 0, 0, 0, 262144, 262144, 7340032, 50331648,
/* 422 */ (int) 0x80000000, 0, 7864320, 402653184, 0, 0, 0, 524288, 0, 128, 16384, 0, 0, 4194304, 0, 0, 0, 8388608, 0,
/* 441 */ 262144, 1048576, 2097152, 0, 671088640, 738197504, 0, 524288, 402653184, 0, 0, 134217728, 0, 0, 8, 16, 32,
/* 458 */ 256, 131072, 262144, 1048576, 2097152, 2097152, 4194304, 33554432, (int) 0x80000000, 0, 0, 8388608, 0, 0, 2,
/* 473 */ 4, 8, 4194304, 33554432, 0, 0, 4, 8, 16, 32, 256, 0, 0, 16777216, 14, 393278, 2097152, 35651584, 29360128,
/* 492 */ 29360128, 37552067, 37552067, 54329283, 37552067, 37552067, 54329283, 54329283, 54329283, 66912195, 66912195,
/* 503 */ 66912195, 66912195, 14, 62, 393216, 0, 4, 0, 0, -536870912, 2048, 66, 128, 256, 3584, 4096, 57344, 262144,
/* 521 */ 524288, 1048576, 0, 0, 0, 8, (int) 0x80000000, 12, 0, 0, 0, 8, 0, 0, 0, 256, 64, 128, 1536, 2048, 8388608,
/* 542 */ 67108864, 134217728, 536870912, 1073741824, 512, 1024, 2048, 4096, 8192, 0, 8192, 16384, 262144, 524288,
/* 556 */ 268435456, 0, 0, 0, 2097152, 0, 201326592, 8192, 16384, 262144, 1048576, 256, 0, 65536, 0, 0, 0, 268435456,
/* 574 */ 32, 256, 256, 65536, 0, 1024, 8192, 256, 256, 1024
};

const wchar_t *WebCpp::TOKEN[] =
{
  L"(0)",
  L"END",
  L"eof",
  L"identifier",
  L"'null'",
  L"'true'",
  L"'false'",
  L"string",
  L"unicode",
  L"complex",
  L"real",
  L"comment",
  L"lineTerminator",
  L"'!'",
  L"'!='",
  L"'#'",
  L"'%'",
  L"'%='",
  L"'&'",
  L"'&&'",
  L"'('",
  L"')'",
  L"'*'",
  L"'*='",
  L"'+'",
  L"'++'",
  L"'+='",
  L"','",
  L"'-'",
  L"'--'",
  L"'-='",
  L"'->'",
  L"'.'",
  L"'/'",
  L"'/='",
  L"':'",
  L"';'",
  L"'<'",
  L"'<<'",
  L"'<<='",
  L"'<='",
  L"'='",
  L"'=='",
  L"'>'",
  L"'>='",
  L"'>>'",
  L"'>>='",
  L"'['",
  L"']'",
  L"'^'",
  L"'break'",
  L"'catch'",
  L"'char'",
  L"'class'",
  L"'continue'",
  L"'define'",
  L"'do'",
  L"'double'",
  L"'elif'",
  L"'else'",
  L"'else if'",
  L"'endif'",
  L"'error'",
  L"'float'",
  L"'for'",
  L"'if'",
  L"'ifdef'",
  L"'ifndef'",
  L"'import'",
  L"'include'",
  L"'int'",
  L"'long'",
  L"'namespace'",
  L"'private'",
  L"'protected'",
  L"'public'",
  L"'return'",
  L"'template'",
  L"'throw'",
  L"'try'",
  L"'typename'",
  L"'undef'",
  L"'using'",
  L"'void'",
  L"'while'",
  L"'{'",
  L"'|'",
  L"'||'",
  L"'}'",
  L"'~'"
};

int main(int argc, char **argv)
{
  return WebCpp::main(argc, argv);
}

// End
