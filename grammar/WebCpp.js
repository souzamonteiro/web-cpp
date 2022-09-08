// This file was generated on Thu Sep 8, 2022 17:22 (UTC-03) by REx v5.55 which is Copyright (c) 1979-2022 by Gunther Rademacher <grd@gmx.net>
// REx command line: WebCpp.ebnf -javascript -tree -main -ll 3 -backtrack -ll 2

function WebCpp(string, parsingEventHandler)
{
  init(string, parsingEventHandler);

  var thisParser = this;

  this.ParseException = function(b, e, s, o, x)
  {
    var begin = b;
    var end = e;
    var state = s;
    var offending = o;
    var expected = x;

    this.getBegin = function() {return begin;};
    this.getEnd = function() {return end;};
    this.getState = function() {return state;};
    this.getExpected = function() {return expected;};
    this.getOffending = function() {return offending;};
    this.isAmbiguousInput = function() {return false;};

    this.getMessage = function()
    {
      return offending < 0
           ? "lexical analysis failed"
           : "syntax error";
    };
  };

  function init(source, parsingEventHandler)
  {
    eventHandler = parsingEventHandler;
    input = source;
    size = source.length;
    reset(0, 0, 0);
  }

  this.getInput = function()
  {
    return input;
  };

  this.getTokenOffset = function()
  {
    return b0;
  };

  this.getTokenEnd = function()
  {
    return e0;
  };

  function reset(l, b, e)
  {
            b0 = b; e0 = b;
    l1 = l; b1 = b; e1 = e;
    l2 = 0; b2 = 0; e2 = 0;
    end = e;
    ex = -1;
    memo = {};
    eventHandler.reset(input);
  }

  this.reset = function(l, b, e)
  {
    reset(l, b, e);
  };

  this.getOffendingToken = function(e)
  {
    var o = e.getOffending();
    return o >= 0 ? WebCpp.TOKEN[o] : null;
  };

  this.getExpectedTokenSet = function(e)
  {
    var expected;
    if (e.getExpected() < 0)
    {
      expected = WebCpp.getTokenSet(- e.getState());
    }
    else
    {
      expected = [WebCpp.TOKEN[e.getExpected()]];
    }
    return expected;
  };

  this.getErrorMessage = function(e)
  {
    var message = e.getMessage();
    var found = this.getOffendingToken(e);
    var tokenSet = this.getExpectedTokenSet(e);
    var size = e.getEnd() - e.getBegin();
    message += (found == null ? "" : ", found " + found)
            + "\nwhile expecting "
            + (tokenSet.length == 1 ? tokenSet[0] : ("[" + tokenSet.join(", ") + "]"))
            + "\n"
            + (size == 0 || found != null ? "" : "after successfully scanning " + size + " characters beginning ");
    var prefix = input.substring(0, e.getBegin());
    var lines = prefix.split("\n");
    var line = lines.length;
    var column = lines[line - 1].length + 1;
    return message
         + "at line " + line + ", column " + column + ":\n..."
         + input.substring(e.getBegin(), Math.min(input.length, e.getBegin() + 64))
         + "...";
  };

  this.parse_webcpp = function()
  {
    eventHandler.startNonterminal("webcpp", e0);
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
    }
    eventHandler.endNonterminal("webcpp", e0);
  };

  function parse_operation()
  {
    eventHandler.startNonterminal("operation", e0);
    parse_variableAssignment();
    eventHandler.endNonterminal("operation", e0);
  }

  function try_operation()
  {
    try_variableAssignment();
  }

  function parse_variableAssignment()
  {
    eventHandler.startNonterminal("variableAssignment", e0);
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
    eventHandler.endNonterminal("variableAssignment", e0);
  }

  function try_variableAssignment()
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

  function parse_logicalORExpression()
  {
    eventHandler.startNonterminal("logicalORExpression", e0);
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
    eventHandler.endNonterminal("logicalORExpression", e0);
  }

  function try_logicalORExpression()
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

  function parse_logicalANDExpression()
  {
    eventHandler.startNonterminal("logicalANDExpression", e0);
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
    eventHandler.endNonterminal("logicalANDExpression", e0);
  }

  function try_logicalANDExpression()
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

  function parse_bitwiseORExpression()
  {
    eventHandler.startNonterminal("bitwiseORExpression", e0);
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
    eventHandler.endNonterminal("bitwiseORExpression", e0);
  }

  function try_bitwiseORExpression()
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

  function parse_bitwiseXORExpression()
  {
    eventHandler.startNonterminal("bitwiseXORExpression", e0);
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
    eventHandler.endNonterminal("bitwiseXORExpression", e0);
  }

  function try_bitwiseXORExpression()
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

  function parse_bitwiseANDExpression()
  {
    eventHandler.startNonterminal("bitwiseANDExpression", e0);
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
    eventHandler.endNonterminal("bitwiseANDExpression", e0);
  }

  function try_bitwiseANDExpression()
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

  function parse_equalityExpression()
  {
    eventHandler.startNonterminal("equalityExpression", e0);
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
      }
      parse_relationalExpression();
    }
    eventHandler.endNonterminal("equalityExpression", e0);
  }

  function try_equalityExpression()
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
      }
      try_relationalExpression();
    }
  }

  function parse_relationalExpression()
  {
    eventHandler.startNonterminal("relationalExpression", e0);
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
      }
      parse_shiftExpression();
    }
    eventHandler.endNonterminal("relationalExpression", e0);
  }

  function try_relationalExpression()
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
      }
      try_shiftExpression();
    }
  }

  function parse_shiftExpression()
  {
    eventHandler.startNonterminal("shiftExpression", e0);
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
      }
      parse_additiveExpression();
    }
    eventHandler.endNonterminal("shiftExpression", e0);
  }

  function try_shiftExpression()
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
      }
      try_additiveExpression();
    }
  }

  function parse_additiveExpression()
  {
    eventHandler.startNonterminal("additiveExpression", e0);
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
      }
      parse_multiplicativeExpression();
    }
    eventHandler.endNonterminal("additiveExpression", e0);
  }

  function try_additiveExpression()
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
      }
      try_multiplicativeExpression();
    }
  }

  function parse_multiplicativeExpression()
  {
    eventHandler.startNonterminal("multiplicativeExpression", e0);
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
          var b0A = b0; var e0A = e0; var l1A = l1;
          var b1A = b1; var e1A = e1; var l2A = l2;
          var b2A = b2; var e2A = e2;
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
            }
            try_unaryExpression();
            lk = -1;
          }
          catch (p1A)
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
      }
      parse_unaryExpression();
    }
    eventHandler.endNonterminal("multiplicativeExpression", e0);
  }

  function try_multiplicativeExpression()
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
          var b0A = b0; var e0A = e0; var l1A = l1;
          var b1A = b1; var e1A = e1; var l2A = l2;
          var b2A = b2; var e2A = e2;
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
            }
            try_unaryExpression();
            memoize(0, e0A, -1);
            continue;
          }
          catch (p1A)
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
      }
      try_unaryExpression();
    }
  }

  function parse_unaryExpression()
  {
    eventHandler.startNonterminal("unaryExpression", e0);
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
        var b0A = b0; var e0A = e0; var l1A = l1;
        var b1A = b1; var e1A = e1; var l2A = l2;
        var b2A = b2; var e2A = e2;
        try
        {
          try_primary();
          lookahead1(7);            // '++'
          consumeT(25);             // '++'
          lk = -6;
        }
        catch (p6A)
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
          catch (p7A)
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
    }
    eventHandler.endNonterminal("unaryExpression", e0);
  }

  function try_unaryExpression()
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
        var b0A = b0; var e0A = e0; var l1A = l1;
        var b1A = b1; var e1A = e1; var l2A = l2;
        var b2A = b2; var e2A = e2;
        try
        {
          try_primary();
          lookahead1(7);            // '++'
          consumeT(25);             // '++'
          memoize(1, e0A, -6);
          lk = -9;
        }
        catch (p6A)
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
          catch (p7A)
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
    }
  }

  function parse_primary()
  {
    eventHandler.startNonterminal("primary", e0);
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
    }
    eventHandler.endNonterminal("primary", e0);
  }

  function try_primary()
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
    }
  }

  function parse_statement()
  {
    eventHandler.startNonterminal("statement", e0);
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
        var b0A = b0; var e0A = e0; var l1A = l1;
        var b1A = b1; var e1A = e1; var l2A = l2;
        var b2A = b2; var e2A = e2;
        try
        {
          try_functionDefinition();
          lk = -6;
        }
        catch (p6A)
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
    }
    eventHandler.endNonterminal("statement", e0);
  }

  function try_statement()
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
        var b0A = b0; var e0A = e0; var l1A = l1;
        var b1A = b1; var e1A = e1; var l2A = l2;
        var b2A = b2; var e2A = e2;
        try
        {
          try_functionDefinition();
          memoize(2, e0A, -6);
          lk = -18;
        }
        catch (p6A)
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
    }
  }

  function parse_preprocessorDirective()
  {
    eventHandler.startNonterminal("preprocessorDirective", e0);
    switch (l1)
    {
    case 15:                        // '#'
      lookahead2(37);               // 'define' | 'error' | 'if' | 'ifdef' | 'ifndef' | 'import' | 'include' | 'undef' |
                                    // 'using'
      break;
    default:
      lk = l1;
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
    }
    eventHandler.endNonterminal("preprocessorDirective", e0);
  }

  function try_preprocessorDirective()
  {
    switch (l1)
    {
    case 15:                        // '#'
      lookahead2(37);               // 'define' | 'error' | 'if' | 'ifdef' | 'ifndef' | 'import' | 'include' | 'undef' |
                                    // 'using'
      break;
    default:
      lk = l1;
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
    }
  }

  function parse_defineDirective()
  {
    eventHandler.startNonterminal("defineDirective", e0);
    switch (l1)
    {
    case 15:                        // '#'
      lookahead2(13);               // 'define'
      break;
    default:
      lk = l1;
    }
    lk = memoized(3, e0);
    if (lk == 0)
    {
      var b0A = b0; var e0A = e0; var l1A = l1;
      var b1A = b1; var e1A = e1; var l2A = l2;
      var b2A = b2; var e2A = e2;
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
      catch (p1A)
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
    }
    eventHandler.endNonterminal("defineDirective", e0);
  }

  function try_defineDirective()
  {
    switch (l1)
    {
    case 15:                        // '#'
      lookahead2(13);               // 'define'
      break;
    default:
      lk = l1;
    }
    lk = memoized(3, e0);
    if (lk == 0)
    {
      var b0A = b0; var e0A = e0; var l1A = l1;
      var b1A = b1; var e1A = e1; var l2A = l2;
      var b2A = b2; var e2A = e2;
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
      catch (p1A)
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
    }
  }

  function parse_ifStatementDirective()
  {
    eventHandler.startNonterminal("ifStatementDirective", e0);
    switch (l1)
    {
    case 15:                        // '#'
      lookahead2(36);               // 'if' | 'ifdef' | 'ifndef'
      break;
    default:
      lk = l1;
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
    }
    if (lk == 7567)                 // '#' 'else'
    {
      parse_elseDirective();
    }
    parse_endifDirective();
    eventHandler.endNonterminal("ifStatementDirective", e0);
  }

  function try_ifStatementDirective()
  {
    switch (l1)
    {
    case 15:                        // '#'
      lookahead2(36);               // 'if' | 'ifdef' | 'ifndef'
      break;
    default:
      lk = l1;
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
    }
    if (lk == 7567)                 // '#' 'else'
    {
      try_elseDirective();
    }
    try_endifDirective();
  }

  function parse_ifDirective()
  {
    eventHandler.startNonterminal("ifDirective", e0);
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
    eventHandler.endNonterminal("ifDirective", e0);
  }

  function try_ifDirective()
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

  function parse_ifdefDirective()
  {
    eventHandler.startNonterminal("ifdefDirective", e0);
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
    eventHandler.endNonterminal("ifdefDirective", e0);
  }

  function try_ifdefDirective()
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

  function parse_ifndefDirective()
  {
    eventHandler.startNonterminal("ifndefDirective", e0);
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
    eventHandler.endNonterminal("ifndefDirective", e0);
  }

  function try_ifndefDirective()
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

  function parse_elifDirective()
  {
    eventHandler.startNonterminal("elifDirective", e0);
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
    eventHandler.endNonterminal("elifDirective", e0);
  }

  function try_elifDirective()
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

  function parse_elseDirective()
  {
    eventHandler.startNonterminal("elseDirective", e0);
    consume(15);                    // '#'
    lookahead1(15);                 // 'else'
    consume(59);                    // 'else'
    lookahead1(3);                  // lineTerminator
    consume(12);                    // lineTerminator
    lookahead1(2);                  // unicode
    consume(8);                     // unicode
    lookahead1(3);                  // lineTerminator
    consume(12);                    // lineTerminator
    eventHandler.endNonterminal("elseDirective", e0);
  }

  function try_elseDirective()
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

  function parse_endifDirective()
  {
    eventHandler.startNonterminal("endifDirective", e0);
    lookahead1(4);                  // '#'
    consume(15);                    // '#'
    lookahead1(16);                 // 'endif'
    consume(61);                    // 'endif'
    eventHandler.endNonterminal("endifDirective", e0);
  }

  function try_endifDirective()
  {
    lookahead1(4);                  // '#'
    consumeT(15);                   // '#'
    lookahead1(16);                 // 'endif'
    consumeT(61);                   // 'endif'
  }

  function parse_errorDirective()
  {
    eventHandler.startNonterminal("errorDirective", e0);
    consume(15);                    // '#'
    lookahead1(17);                 // 'error'
    consume(62);                    // 'error'
    lookahead1(1);                  // string
    consume(7);                     // string
    eventHandler.endNonterminal("errorDirective", e0);
  }

  function try_errorDirective()
  {
    consumeT(15);                   // '#'
    lookahead1(17);                 // 'error'
    consumeT(62);                   // 'error'
    lookahead1(1);                  // string
    consumeT(7);                    // string
  }

  function parse_importDirective()
  {
    eventHandler.startNonterminal("importDirective", e0);
    switch (l1)
    {
    case 15:                        // '#'
      lookahead2(21);               // 'import'
      break;
    default:
      lk = l1;
    }
    lk = memoized(4, e0);
    if (lk == 0)
    {
      var b0A = b0; var e0A = e0; var l1A = l1;
      var b1A = b1; var e1A = e1; var l2A = l2;
      var b2A = b2; var e2A = e2;
      try
      {
        consumeT(15);               // '#'
        lookahead1(21);             // 'import'
        consumeT(68);               // 'import'
        lookahead1(1);              // string
        consumeT(7);                // string
        lk = -1;
      }
      catch (p1A)
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
    }
    eventHandler.endNonterminal("importDirective", e0);
  }

  function try_importDirective()
  {
    switch (l1)
    {
    case 15:                        // '#'
      lookahead2(21);               // 'import'
      break;
    default:
      lk = l1;
    }
    lk = memoized(4, e0);
    if (lk == 0)
    {
      var b0A = b0; var e0A = e0; var l1A = l1;
      var b1A = b1; var e1A = e1; var l2A = l2;
      var b2A = b2; var e2A = e2;
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
      catch (p1A)
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
    }
  }

  function parse_includeDirective()
  {
    eventHandler.startNonterminal("includeDirective", e0);
    switch (l1)
    {
    case 15:                        // '#'
      lookahead2(22);               // 'include'
      break;
    default:
      lk = l1;
    }
    lk = memoized(5, e0);
    if (lk == 0)
    {
      var b0A = b0; var e0A = e0; var l1A = l1;
      var b1A = b1; var e1A = e1; var l2A = l2;
      var b2A = b2; var e2A = e2;
      try
      {
        consumeT(15);               // '#'
        lookahead1(22);             // 'include'
        consumeT(69);               // 'include'
        lookahead1(1);              // string
        consumeT(7);                // string
        lk = -1;
      }
      catch (p1A)
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
    }
    eventHandler.endNonterminal("includeDirective", e0);
  }

  function try_includeDirective()
  {
    switch (l1)
    {
    case 15:                        // '#'
      lookahead2(22);               // 'include'
      break;
    default:
      lk = l1;
    }
    lk = memoized(5, e0);
    if (lk == 0)
    {
      var b0A = b0; var e0A = e0; var l1A = l1;
      var b1A = b1; var e1A = e1; var l2A = l2;
      var b2A = b2; var e2A = e2;
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
      catch (p1A)
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
    }
  }

  function parse_undefDirective()
  {
    eventHandler.startNonterminal("undefDirective", e0);
    consume(15);                    // '#'
    lookahead1(24);                 // 'undef'
    consume(81);                    // 'undef'
    lookahead1(0);                  // identifier
    consume(3);                     // identifier
    eventHandler.endNonterminal("undefDirective", e0);
  }

  function try_undefDirective()
  {
    consumeT(15);                   // '#'
    lookahead1(24);                 // 'undef'
    consumeT(81);                   // 'undef'
    lookahead1(0);                  // identifier
    consumeT(3);                    // identifier
  }

  function parse_usingDirective()
  {
    eventHandler.startNonterminal("usingDirective", e0);
    consume(15);                    // '#'
    lookahead1(25);                 // 'using'
    consume(82);                    // 'using'
    lookahead1(1);                  // string
    consume(7);                     // string
    eventHandler.endNonterminal("usingDirective", e0);
  }

  function try_usingDirective()
  {
    consumeT(15);                   // '#'
    lookahead1(25);                 // 'using'
    consumeT(82);                   // 'using'
    lookahead1(1);                  // string
    consumeT(7);                    // string
  }

  function parse_class()
  {
    eventHandler.startNonterminal("class", e0);
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
    eventHandler.endNonterminal("class", e0);
  }

  function try_class()
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

  function parse_accessDefinition()
  {
    eventHandler.startNonterminal("accessDefinition", e0);
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
    }
    eventHandler.endNonterminal("accessDefinition", e0);
  }

  function try_accessDefinition()
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
    }
  }

  function parse_namespace()
  {
    eventHandler.startNonterminal("namespace", e0);
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
    eventHandler.endNonterminal("namespace", e0);
  }

  function try_namespace()
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

  function parse_using()
  {
    eventHandler.startNonterminal("using", e0);
    switch (l1)
    {
    case 82:                        // 'using'
      lookahead2(28);               // identifier | 'namespace'
      break;
    default:
      lk = l1;
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
    }
    eventHandler.endNonterminal("using", e0);
  }

  function try_using()
  {
    switch (l1)
    {
    case 82:                        // 'using'
      lookahead2(28);               // identifier | 'namespace'
      break;
    default:
      lk = l1;
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
    }
  }

  function parse_functionDefinition()
  {
    eventHandler.startNonterminal("functionDefinition", e0);
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
    eventHandler.endNonterminal("functionDefinition", e0);
  }

  function try_functionDefinition()
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

  function parse_variableDefinition()
  {
    eventHandler.startNonterminal("variableDefinition", e0);
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
      }
      if (lk == 411)                // ',' identifier
      {
        lk = memoized(6, e0);
        if (lk == 0)
        {
          var b0A = b0; var e0A = e0; var l1A = l1;
          var b1A = b1; var e1A = e1; var l2A = l2;
          var b2A = b2; var e2A = e2;
          try
          {
            consumeT(27);           // ','
            lookahead1(0);          // identifier
            consumeT(3);            // identifier
            lk = -1;
          }
          catch (p1A)
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
    eventHandler.endNonterminal("variableDefinition", e0);
  }

  function try_variableDefinition()
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
      }
      if (lk == 411)                // ',' identifier
      {
        lk = memoized(6, e0);
        if (lk == 0)
        {
          var b0A = b0; var e0A = e0; var l1A = l1;
          var b1A = b1; var e1A = e1; var l2A = l2;
          var b2A = b2; var e2A = e2;
          try
          {
            consumeT(27);           // ','
            lookahead1(0);          // identifier
            consumeT(3);            // identifier
            memoize(6, e0A, -1);
            continue;
          }
          catch (p1A)
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

  function parse_template()
  {
    eventHandler.startNonterminal("template", e0);
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
    }
    lookahead1(0);                  // identifier
    consume(3);                     // identifier
    lookahead1(12);                 // '>'
    consume(43);                    // '>'
    eventHandler.endNonterminal("template", e0);
  }

  function try_template()
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
    }
    lookahead1(0);                  // identifier
    consumeT(3);                    // identifier
    lookahead1(12);                 // '>'
    consumeT(43);                   // '>'
  }

  function parse_if()
  {
    eventHandler.startNonterminal("if", e0);
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
    eventHandler.endNonterminal("if", e0);
  }

  function try_if()
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

  function parse_elseif()
  {
    eventHandler.startNonterminal("elseif", e0);
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
    eventHandler.endNonterminal("elseif", e0);
  }

  function try_elseif()
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

  function parse_else()
  {
    eventHandler.startNonterminal("else", e0);
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
    eventHandler.endNonterminal("else", e0);
  }

  function try_else()
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

  function parse_do()
  {
    eventHandler.startNonterminal("do", e0);
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
    eventHandler.endNonterminal("do", e0);
  }

  function try_do()
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

  function parse_while()
  {
    eventHandler.startNonterminal("while", e0);
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
    eventHandler.endNonterminal("while", e0);
  }

  function try_while()
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

  function parse_for()
  {
    eventHandler.startNonterminal("for", e0);
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
    }
    if (lk == 4644)                 // ';' ';'
    {
      lk = memoized(7, e0);
      if (lk == 0)
      {
        var b0A = b0; var e0A = e0; var l1A = l1;
        var b1A = b1; var e1A = e1; var l2A = l2;
        var b2A = b2; var e2A = e2;
        try
        {
          try_expression();
          lk = -1;
        }
        catch (p1A)
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
    }
    if (lk == 4644)                 // ';' ';'
    {
      lk = memoized(8, e0);
      if (lk == 0)
      {
        var b0A = b0; var e0A = e0; var l1A = l1;
        var b1A = b1; var e1A = e1; var l2A = l2;
        var b2A = b2; var e2A = e2;
        try
        {
          try_expression();
          lk = -1;
        }
        catch (p1A)
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
    eventHandler.endNonterminal("for", e0);
  }

  function try_for()
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
    }
    if (lk == 4644)                 // ';' ';'
    {
      lk = memoized(7, e0);
      if (lk == 0)
      {
        var b0A = b0; var e0A = e0; var l1A = l1;
        var b1A = b1; var e1A = e1; var l2A = l2;
        var b2A = b2; var e2A = e2;
        try
        {
          try_expression();
          memoize(7, e0A, -1);
        }
        catch (p1A)
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
    }
    if (lk == 4644)                 // ';' ';'
    {
      lk = memoized(8, e0);
      if (lk == 0)
      {
        var b0A = b0; var e0A = e0; var l1A = l1;
        var b1A = b1; var e1A = e1; var l2A = l2;
        var b2A = b2; var e2A = e2;
        try
        {
          try_expression();
          memoize(8, e0A, -1);
        }
        catch (p1A)
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

  function parse_try()
  {
    eventHandler.startNonterminal("try", e0);
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
    eventHandler.endNonterminal("try", e0);
  }

  function try_try()
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

  function parse_catch()
  {
    eventHandler.startNonterminal("catch", e0);
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
    eventHandler.endNonterminal("catch", e0);
  }

  function try_catch()
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

  function parse_break()
  {
    eventHandler.startNonterminal("break", e0);
    consume(50);                    // 'break'
    eventHandler.endNonterminal("break", e0);
  }

  function try_break()
  {
    consumeT(50);                   // 'break'
  }

  function parse_continue()
  {
    eventHandler.startNonterminal("continue", e0);
    consume(54);                    // 'continue'
    eventHandler.endNonterminal("continue", e0);
  }

  function try_continue()
  {
    consumeT(54);                   // 'continue'
  }

  function parse_return()
  {
    eventHandler.startNonterminal("return", e0);
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
        var b0A = b0; var e0A = e0; var l1A = l1;
        var b1A = b1; var e1A = e1; var l2A = l2;
        var b2A = b2; var e2A = e2;
        try
        {
          try_expression();
          lk = -1;
        }
        catch (p1A)
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
    eventHandler.endNonterminal("return", e0);
  }

  function try_return()
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
        var b0A = b0; var e0A = e0; var l1A = l1;
        var b1A = b1; var e1A = e1; var l2A = l2;
        var b2A = b2; var e2A = e2;
        try
        {
          try_expression();
          memoize(9, e0A, -1);
        }
        catch (p1A)
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

  function parse_throw()
  {
    eventHandler.startNonterminal("throw", e0);
    consume(78);                    // 'throw'
    parse_expression();
    eventHandler.endNonterminal("throw", e0);
  }

  function try_throw()
  {
    consumeT(78);                   // 'throw'
    try_expression();
  }

  function parse_type()
  {
    eventHandler.startNonterminal("type", e0);
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
    }
    eventHandler.endNonterminal("type", e0);
  }

  function try_type()
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
    }
  }

  function parse_expression()
  {
    eventHandler.startNonterminal("expression", e0);
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
    }
    eventHandler.endNonterminal("expression", e0);
  }

  function try_expression()
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
    }
  }

  function parse_arguments()
  {
    eventHandler.startNonterminal("arguments", e0);
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
    eventHandler.endNonterminal("arguments", e0);
  }

  function try_arguments()
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

  function parse_member()
  {
    eventHandler.startNonterminal("member", e0);
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
    }
    lk = memoized(10, e0);
    if (lk == 0)
    {
      var b0A = b0; var e0A = e0; var l1A = l1;
      var b1A = b1; var e1A = e1; var l2A = l2;
      var b2A = b2; var e2A = e2;
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
      catch (p1A)
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
                var b0B = b0; var e0B = e0; var l1B = l1;
                var b1B = b1; var e1B = e1; var l2B = l2;
                var b2B = b2; var e2B = e2;
                try
                {
                  consumeT(47);     // '['
                  try_arguments();
                  consumeT(48);     // ']'
                  memoize(11, e0B, -1);
                  continue;
                }
                catch (p1B)
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
        catch (p2A)
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
          catch (p3A)
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
            var b0B = b0; var e0B = e0; var l1B = l1;
            var b1B = b1; var e1B = e1; var l2B = l2;
            var b2B = b2; var e2B = e2;
            try
            {
              consumeT(47);         // '['
              try_arguments();
              consumeT(48);         // ']'
              lk = -1;
            }
            catch (p1B)
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
            var b0B = b0; var e0B = e0; var l1B = l1;
            var b1B = b1; var e1B = e1; var l2B = l2;
            var b2B = b2; var e2B = e2;
            try
            {
              consumeT(47);         // '['
              try_arguments();
              consumeT(48);         // ']'
              lk = -1;
            }
            catch (p1B)
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
    }
    eventHandler.endNonterminal("member", e0);
  }

  function try_member()
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
    }
    lk = memoized(10, e0);
    if (lk == 0)
    {
      var b0A = b0; var e0A = e0; var l1A = l1;
      var b1A = b1; var e1A = e1; var l2A = l2;
      var b2A = b2; var e2A = e2;
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
      catch (p1A)
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
                var b0B = b0; var e0B = e0; var l1B = l1;
                var b1B = b1; var e1B = e1; var l2B = l2;
                var b2B = b2; var e2B = e2;
                try
                {
                  consumeT(47);     // '['
                  try_arguments();
                  consumeT(48);     // ']'
                  memoize(11, e0B, -1);
                  continue;
                }
                catch (p1B)
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
        catch (p2A)
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
          catch (p3A)
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
            var b0B = b0; var e0B = e0; var l1B = l1;
            var b1B = b1; var e1B = e1; var l2B = l2;
            var b2B = b2; var e2B = e2;
            try
            {
              consumeT(47);         // '['
              try_arguments();
              consumeT(48);         // ']'
              memoize(11, e0B, -1);
              continue;
            }
            catch (p1B)
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
            var b0B = b0; var e0B = e0; var l1B = l1;
            var b1B = b1; var e1B = e1; var l2B = l2;
            var b2B = b2; var e2B = e2;
            try
            {
              consumeT(47);         // '['
              try_arguments();
              consumeT(48);         // ']'
              memoize(12, e0B, -1);
              continue;
            }
            catch (p1B)
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
    }
  }

  function parse_array()
  {
    eventHandler.startNonterminal("array", e0);
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
    eventHandler.endNonterminal("array", e0);
  }

  function try_array()
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

  function parse_matrix()
  {
    eventHandler.startNonterminal("matrix", e0);
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
    }
    if (lk == 4644)                 // ';' ';'
    {
      lk = memoized(13, e0);
      if (lk == 0)
      {
        var b0A = b0; var e0A = e0; var l1A = l1;
        var b1A = b1; var e1A = e1; var l2A = l2;
        var b2A = b2; var e2A = e2;
        try
        {
          try_row();
          lk = -1;
        }
        catch (p1A)
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
    eventHandler.endNonterminal("matrix", e0);
  }

  function try_matrix()
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
    }
    if (lk == 4644)                 // ';' ';'
    {
      lk = memoized(13, e0);
      if (lk == 0)
      {
        var b0A = b0; var e0A = e0; var l1A = l1;
        var b1A = b1; var e1A = e1; var l2A = l2;
        var b2A = b2; var e2A = e2;
        try
        {
          try_row();
          memoize(13, e0A, -1);
        }
        catch (p1A)
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

  function parse_element()
  {
    eventHandler.startNonterminal("element", e0);
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
    }
    if (lk == 4483                  // identifier ':'
     || lk == 4487)                 // string ':'
    {
      parse_key();
      lookahead1(9);                // ':'
      consume(35);                  // ':'
    }
    parse_expression();
    eventHandler.endNonterminal("element", e0);
  }

  function try_element()
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

  function parse_key()
  {
    eventHandler.startNonterminal("key", e0);
    switch (l1)
    {
    case 3:                         // identifier
      consume(3);                   // identifier
      break;
    default:
      consume(7);                   // string
    }
    eventHandler.endNonterminal("key", e0);
  }

  function try_key()
  {
    switch (l1)
    {
    case 3:                         // identifier
      consumeT(3);                  // identifier
      break;
    default:
      consumeT(7);                  // string
    }
  }

  function parse_row()
  {
    eventHandler.startNonterminal("row", e0);
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
    eventHandler.endNonterminal("row", e0);
  }

  function try_row()
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

  function parse_column()
  {
    eventHandler.startNonterminal("column", e0);
    parse_expression();
    eventHandler.endNonterminal("column", e0);
  }

  function try_column()
  {
    try_expression();
  }

  function parse_parenthesizedExpression()
  {
    eventHandler.startNonterminal("parenthesizedExpression", e0);
    consume(20);                    // '('
    parse_expression();
    lookahead1(6);                  // ')'
    consume(21);                    // ')'
    eventHandler.endNonterminal("parenthesizedExpression", e0);
  }

  function try_parenthesizedExpression()
  {
    consumeT(20);                   // '('
    try_expression();
    lookahead1(6);                  // ')'
    consumeT(21);                   // ')'
  }

  function parse_value()
  {
    eventHandler.startNonterminal("value", e0);
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
    }
    eventHandler.endNonterminal("value", e0);
  }

  function try_value()
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
    }
  }

  function consume(t)
  {
    if (l1 == t)
    {
      eventHandler.terminal(WebCpp.TOKEN[l1], b1, e1);
      b0 = b1; e0 = e1; l1 = l2; if (l1 != 0) {
      b1 = b2; e1 = e2; l2 = 0; }
    }
    else
    {
      error(b1, e1, 0, l1, t);
    }
  }

  function consumeT(t)
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

  function lookahead1(tokenSetId)
  {
    if (l1 == 0)
    {
      l1 = match(tokenSetId);
      b1 = begin;
      e1 = end;
    }
  }

  function lookahead2(tokenSetId)
  {
    if (l2 == 0)
    {
      l2 = match(tokenSetId);
      b2 = begin;
      e2 = end;
    }
    lk = (l2 << 7) | l1;
  }

  function error(b, e, s, l, t)
  {
    if (e >= ex)
    {
      bx = b;
      ex = e;
      sx = s;
      lx = l;
      tx = t;
    }
    throw new thisParser.ParseException(bx, ex, sx, lx, tx);
  }

  var lk, b0, e0;
  var l1, b1, e1;
  var l2, b2, e2;
  var bx, ex, sx, lx, tx;
  var eventHandler;
  var memo;

  function memoize(i, e, v)
  {
    memo[(e << 4) + i] = v;
  }

  function memoized(i, e)
  {
    var v = memo[(e << 4) + i];
    return typeof v != "undefined" ? v : 0;
  }

  var input;
  var size;

  var begin;
  var end;

  function match(tokenSetId)
  {
    begin = end;
    var current = end;
    var result = WebCpp.INITIAL[tokenSetId];
    var state = 0;

    for (var code = result & 511; code != 0; )
    {
      var charclass;
      var c0 = current < size ? input.charCodeAt(current) : 0;
      ++current;
      if (c0 < 0x80)
      {
        charclass = WebCpp.MAP0[c0];
      }
      else if (c0 < 0xd800)
      {
        var c1 = c0 >> 5;
        charclass = WebCpp.MAP1[(c0 & 31) + WebCpp.MAP1[(c1 & 31) + WebCpp.MAP1[c1 >> 5]]];
      }
      else
      {
        if (c0 < 0xdc00)
        {
          var c1 = current < size ? input.charCodeAt(current) : 0;
          if (c1 >= 0xdc00 && c1 < 0xe000)
          {
            ++current;
            c0 = ((c0 & 0x3ff) << 10) + (c1 & 0x3ff) + 0x10000;
          }
        }

        var lo = 0, hi = 1;
        for (var m = 1; ; m = (hi + lo) >> 1)
        {
          if (WebCpp.MAP2[m] > c0) hi = m - 1;
          else if (WebCpp.MAP2[2 + m] < c0) lo = m + 1;
          else {charclass = WebCpp.MAP2[4 + m]; break;}
          if (lo > hi) {charclass = 0; break;}
        }
      }

      state = code;
      var i0 = (charclass << 9) + code - 1;
      code = WebCpp.TRANSITION[(i0 & 7) + WebCpp.TRANSITION[i0 >> 3]];

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
      var c1 = end < size ? input.charCodeAt(end) : 0;
      if (c1 >= 0xdc00 && c1 < 0xe000) --end;
      return error(begin, end, state, -1, -1);
    }

    if (end > size) end = size;
    return (result & 127) - 1;
  }

}

WebCpp.XmlSerializer = function(log, indent)
{
  var input = null;
  var delayedTag = null;
  var hasChildElement = false;
  var depth = 0;

  this.reset = function(string)
  {
    log("<?xml version=\"1.0\" encoding=\"UTF-8\"?" + ">");
    input = string;
    delayedTag = null;
    hasChildElement = false;
    depth = 0;
  };

  this.startNonterminal = function(tag, begin)
  {
    if (delayedTag != null)
    {
      log("<");
      log(delayedTag);
      log(">");
    }
    delayedTag = tag;
    if (indent)
    {
      log("\n");
      for (var i = 0; i < depth; ++i)
      {
        log("  ");
      }
    }
    hasChildElement = false;
    ++depth;
  };

  this.endNonterminal = function(tag, end)
  {
    --depth;
    if (delayedTag != null)
    {
      delayedTag = null;
      log("<");
      log(tag);
      log("/>");
    }
    else
    {
      if (indent)
      {
        if (hasChildElement)
        {
          log("\n");
          for (var i = 0; i < depth; ++i)
          {
            log("  ");
          }
        }
      }
      log("</");
      log(tag);
      log(">");
    }
    hasChildElement = true;
  };

  this.terminal = function(tag, begin, end)
  {
    if (tag.charAt(0) == '\'') tag = "TOKEN";
    this.startNonterminal(tag, begin);
    characters(begin, end);
    this.endNonterminal(tag, end);
  };

  this.whitespace = function(begin, end)
  {
    characters(begin, end);
  };

  function characters(begin, end)
  {
    if (begin < end)
    {
      if (delayedTag != null)
      {
        log("<");
        log(delayedTag);
        log(">");
        delayedTag = null;
      }
      log(input.substring(begin, end)
               .replace(/&/g, "&amp;")
               .replace(/</g, "&lt;")
               .replace(/>/g, "&gt;"));
    }
  }
};

WebCpp.getTokenSet = function(tokenSetId)
{
  var set = [];
  var s = tokenSetId < 0 ? - tokenSetId : WebCpp.INITIAL[tokenSetId] & 511;
  for (var i = 0; i < 90; i += 32)
  {
    var j = i;
    var i0 = (i >> 5) * 368 + s - 1;
    var i1 = i0 >> 2;
    var f = WebCpp.EXPECTED[(i0 & 3) + WebCpp.EXPECTED[(i1 & 3) + WebCpp.EXPECTED[i1 >> 2]]];
    for ( ; f != 0; f >>>= 1, ++j)
    {
      if ((f & 1) != 0)
      {
        set.push(WebCpp.TOKEN[j]);
      }
    }
  }
  return set;
};

WebCpp.TopDownTreeBuilder = function()
{
  var input = null;
  var stack = null;

  this.reset = function(i)
  {
    input = i;
    stack = [];
  };

  this.startNonterminal = function(name, begin)
  {
    var nonterminal = new WebCpp.Nonterminal(name, begin, begin, []);
    if (stack.length > 0) addChild(nonterminal);
    stack.push(nonterminal);
  };

  this.endNonterminal = function(name, end)
  {
    stack[stack.length - 1].end = end;
    if (stack.length > 1) stack.pop();
  };

  this.terminal = function(name, begin, end)
  {
    addChild(new WebCpp.Terminal(name, begin, end));
  };

  this.whitespace = function(begin, end)
  {
  };

  function addChild(s)
  {
    var current = stack[stack.length - 1];
    current.children.push(s);
  }

  this.serialize = function(e)
  {
    e.reset(input);
    stack[0].send(e);
  };
};

WebCpp.Terminal = function(name, begin, end)
{
  this.begin = begin;
  this.end = end;

  this.send = function(e)
  {
    e.terminal(name, begin, end);
  };
};

WebCpp.Nonterminal = function(name, begin, end, children)
{
  this.begin = begin;
  this.end = end;

  this.send = function(e)
  {
    e.startNonterminal(name, begin);
    var pos = begin;
    children.forEach
    (
      function(c)
      {
        if (pos < c.begin) e.whitespace(pos, c.begin);
        c.send(e);
        pos = c.end;
      }
    );
    if (pos < end) e.whitespace(pos, end);
    e.endNonterminal(name, end);
  };
};

WebCpp.MAP0 =
[
  /*   0 */ 56, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 5, 6,
  /*  36 */ 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 20, 21, 22, 23, 24, 10,
  /*  64 */ 10, 7, 7, 7, 7, 25, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 26, 27, 28, 29, 7, 10,
  /*  97 */ 30, 31, 32, 33, 34, 35, 36, 37, 38, 7, 39, 40, 41, 42, 43, 44, 7, 45, 46, 47, 48, 49, 50, 7, 51, 7, 52, 53,
  /* 125 */ 54, 55, 10
];

WebCpp.MAP1 =
[
  /*   0 */ 54, 87, 87, 87, 87, 87, 87, 87, 85, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87,
  /*  27 */ 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87,
  /*  54 */ 119, 151, 182, 214, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  /*  75 */ 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 245, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  /*  96 */ 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  /* 117 */ 255, 255, 56, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  /* 151 */ 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 20, 21, 22,
  /* 180 */ 23, 24, 10, 7, 7, 7, 7, 25, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 26, 27, 28, 29,
  /* 213 */ 7, 10, 30, 31, 32, 33, 34, 35, 36, 37, 38, 7, 39, 40, 41, 42, 43, 44, 7, 45, 46, 47, 48, 49, 50, 7, 51, 7,
  /* 241 */ 52, 53, 54, 55, 10, 10, 10, 10, 10, 10, 10, 10, 2, 2, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
  /* 268 */ 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10
];

WebCpp.MAP2 =
[
  /* 0 */ 57344, 65536, 65533, 1114111, 10, 10
];

WebCpp.INITIAL =
[
  /*  0 */ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
  /* 29 */ 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 1067, 44, 45, 46, 1071, 48, 49, 1074, 1075, 1076,
  /* 53 */ 1077, 1078, 1079, 1080
];

WebCpp.TRANSITION =
[
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
  /* 3732 */ 4096, 0, 0, 0, 0, 0, 0, 0, 257, 0, 0, 73, 0, 0, 0, 0, 0, 0, 0, 192, 0, 0, 4608, 0, 8192, 0, 0, 0, 0, 2119,
  /* 3762 */ 0, 0, 0, 0, 2139, 0, 0, 0, 0, 8813, 8813, 8813, 8813, 0, 0, 8192, 8192, 8192, 8192, 8192, 8192, 8192, 8192,
  /* 3785 */ 6337, 2105, 2105, 2105, 2105, 2105, 2105, 2105, 183, 8813, 8813, 0, 0, 0, 0, 0, 0, 100, 106, 9838, 9838, 0,
  /* 3807 */ 0, 0, 0, 0, 0, 101, 107, 0, 0, 4608, 0, 0, 10752, 0, 0, 0, 0, 25600, 25600, 25600, 25600, 0, 0, 0, 0, 0, 0,
  /* 3834 */ 2106, 2106, 0, 10752, 10752, 10752, 10752, 10752, 10752, 10752, 10752, 0, 0, 4608, 0, 0, 0, 11264, 0, 0, 0,
  /* 3855 */ 0, 11264, 11264, 11264, 11264, 11264, 11264, 11264, 11264, 11887, 11887, 11776, 11776, 11776, 11776, 11776,
  /* 3871 */ 11776, 11887, 11887, 11887, 11887, 257, 0, 0, 0, 0, 0, 0, 0, 7168, 184, 184, 0, 0, 0, 0, 0, 0, 173, 0, 0,
  /* 3896 */ 0, 4608, 0, 0, 0, 0, 75, 12912, 12912, 106, 106, 106, 106, 106, 106, 12936, 12936, 12936, 12936, 73, 0,
  /* 3917 */ 13312, 0, 0, 0, 0, 0, 13312, 0, 252, 0, 0, 0, 0, 0, 192, 0, 0, 0, 0, 0, 0, 0, 182, 0, 303, 100, 0, 100, 0,
  /* 3946 */ 0, 0, 0, 9838, 9838, 9838, 9838, 100, 100, 0, 0, 0, 0, 0, 0, 184, 0, 0, 14336, 14336, 14336, 14336, 14336,
  /* 3969 */ 14336, 14336, 14336, 0, 0, 0, 0, 0, 14336, 0, 0, 0, 0, 0, 0, 2235, 2199, 76, 0, 0, 0, 0, 0, 0, 0, 11776,
  /* 3995 */ 14961, 14967, 107, 107, 107, 107, 107, 107, 14985, 14987, 14985, 14987, 73, 0, 0, 15360, 0, 0, 0, 0, 0,
  /* 4016 */ 15360, 0, 0, 182, 15542, 0, 0, 0, 0, 0, 0, 5734, 5734, 15542, 0, 15542, 0, 0, 0, 0, 0, 0, 10752, 10752, 0,
  /* 4041 */ 0, 16896, 0, 0, 0, 0, 0, 0, 244, 245, 249, 249, 0, 0, 0, 0, 0, 0, 249, 0, 17522, 17522, 120, 120, 120, 120,
  /* 4067 */ 120, 120, 17546, 17546, 17546, 17546, 0, 6337, 0, 0, 0, 0, 0, 0, 0, 0, 87, 6336, 0, 0, 0, 0, 0, 0, 0,
  /* 4092 */ 13312, 0, 0, 5734, 5734, 5734, 5734, 5734, 5734, 5734, 5734, 5734, 0, 5734, 2105, 2105, 2105, 2105, 2105,
  /* 4111 */ 2208, 2105, 2105, 5883, 5885, 2105, 2105, 2105, 0, 0, 192, 2105, 2105, 2105, 2105, 2105, 2105, 2105, 330,
  /* 4130 */ 302, 304, 5937, 5885, 5938, 2105, 2105, 2105, 2197, 2105, 2105, 2105, 2105, 2206, 2105, 2105, 2210, 5937,
  /* 4148 */ 5938, 2105, 2105, 2105, 2105, 2105, 2105, 57, 0, 0, 18432, 0, 0, 0, 0, 0, 0, 292, 0, 18432, 18432, 0, 0, 0,
  /* 4172 */ 0, 0, 0, 334, 0, 0, 0, 18944, 0, 0, 0, 0, 0, 0, 16896, 16896, 0, 0, 18944, 18944, 18944, 18944, 18944,
  /* 4195 */ 18944, 18944, 18944, 0, 0, 0, 19456, 0, 0, 0, 0, 171, 0, 0, 0, 0, 2105, 2105, 2105, 2105, 2265, 19571,
  /* 4217 */ 19571, 0, 0, 0, 0, 0, 0, 1536, 0, 0, 0, 20158, 0, 0, 0, 0, 0, 0, 24576, 24576, 21620, 21620, 0, 0, 0, 0, 0,
  /* 4244 */ 0, 2105, 2105, 2105, 15872, 17920, 20992, 22016, 23040, 0, 15872, 0, 0, 0, 297, 0, 2105, 0, 0, 0, 0, 350,
  /* 4266 */ 2399, 0, 15872, 17920, 15872, 0, 0, 0, 0, 0, 0, 44032, 44032, 22645, 22645, 0, 0, 0, 0, 0, 0, 2105, 2236,
  /* 4289 */ 0, 0, 0, 16384, 0, 0, 0, 0, 0, 0, 0, 186, 0, 0, 2105, 2105, 2105, 0, 0, 192, 186, 250, 2105, 2105, 2105,
  /* 4314 */ 2105, 2105, 2105, 57, 2105, 2372, 0, 24576, 24576, 24576, 24576, 24576, 24576, 24576, 24576, 163, 0, 0, 0,
  /* 4333 */ 0, 0, 0, 0, 13824, 25088, 25088, 25088, 25088, 25088, 25088, 25088, 25088, 2199, 2105, 2105, 2105, 2105,
  /* 4351 */ 2105, 2105, 2105, 357, 0, 0, 2225, 0, 0, 0, 0, 0, 77, 2108, 2108, 2105, 2189, 2105, 2235, 2105, 2105, 2225,
  /* 4373 */ 2105, 0, 4608, 0, 0, 0, 0, 0, 0, 2196, 2105, 6337, 2105, 2307, 2308, 2105, 2105, 2105, 2105, 57, 2105,
  /* 4394 */ 2105, 2105, 2105, 192, 2356, 2105, 2105, 2105, 2105, 2360, 2105, 0, 34816, 0, 0, 0, 2105, 0, 0, 0, 167,
  /* 4415 */ 167, 2373, 2374, 2105, 2105, 2105, 2105, 2105, 0, 0, 2105, 0, 2387, 2105, 2105, 2105, 2105, 2105, 2105,
  /* 4434 */ 2105, 2105, 0, 2105, 2406, 2105, 2105, 0, 360, 2409, 0, 0, 0, 331, 0, 0, 0, 0, 290, 0, 0, 293, 2106, 0,
  /* 4458 */ 4608, 0, 0, 0, 0, 0, 92, 0, 0, 0, 0, 2169, 2169, 2169, 2169, 2169, 2169, 2169, 2169, 2105, 2105, 2275,
  /* 4480 */ 2105, 2105, 2105, 2105, 2105, 2244, 2261, 0, 2105, 2280, 2105, 2105, 2105, 2105, 2105, 2105, 2197, 2105,
  /* 4498 */ 93, 0, 0, 0, 0, 0, 2107, 2107, 0, 4608, 0, 0, 0, 0, 0, 99, 2118, 2118, 0, 0, 2170, 2170, 2170, 2170, 2170,
  /* 4523 */ 2170, 2182, 2170, 2170, 2170, 2170, 2170, 2105, 2369, 2105, 2105, 2105, 2105, 2105, 2105, 2209, 2105, 2415,
  /* 4541 */ 0, 2105, 2105, 2105, 0, 2105, 2105, 2105, 0, 2105, 2105, 2105, 0, 2105, 38457, 2108, 0, 4608, 0, 0, 0, 0,
  /* 4563 */ 0, 100, 0, 0, 0, 0, 2171, 2171, 2171, 2171, 2171, 2171, 2171, 2171, 80, 81, 0, 0, 0, 0, 0, 0, 2114, 2114,
  /* 4587 */ 0, 95, 0, 96, 0, 81, 2109, 2109, 0, 4608, 0, 0, 0, 0, 0, 172, 0, 174, 0, 0, 2109, 2109, 2109, 2109, 2109,
  /* 4612 */ 2109, 2183, 2109, 2109, 2109, 2109, 73, 0, 0, 0, 164, 0, 0, 0, 0, 21620, 21620, 21620, 21620, 2205, 0, 0,
  /* 4634 */ 0, 0, 0, 0, 0, 14336, 2253, 2254, 2105, 2105, 2105, 2105, 2105, 0, 0, 2105, 362, 2272, 2105, 2105, 2105,
  /* 4655 */ 2105, 2105, 2105, 2105, 2271, 6337, 2306, 2105, 2105, 2105, 2105, 2105, 2105, 2236, 2252, 0, 0, 30720, 0,
  /* 4674 */ 0, 291, 0, 0, 0, 182, 182, 100, 0, 0, 0, 0, 44662, 44662, 44662, 44662, 0, 0, 0, 0, 0, 0, 2151, 2151, 0,
  /* 4699 */ 295, 296, 0, 0, 2347, 0, 301, 0, 0, 186, 0, 0, 2105, 2105, 3129, 2105, 2105, 31042, 2105, 2105, 2105, 2105,
  /* 4721 */ 2105, 2260, 2105, 0, 2105, 2105, 2375, 2105, 2105, 2105, 2105, 0, 2105, 2105, 0, 0, 43520, 2105, 0, 0, 250,
  /* 4742 */ 0, 0, 0, 183, 5734, 5734, 2105, 2105, 57, 2105, 2105, 2328, 2105, 2105, 73, 0, 0, 0, 0, 0, 0, 167, 186,
  /* 4765 */ 250, 3641, 2105, 2105, 2105, 2105, 2105, 2269, 2105, 2105, 2105, 2388, 2105, 2105, 2105, 2105, 2105, 43577,
  /* 4783 */ 2105, 28672, 0, 0, 0, 0, 2105, 0, 0, 0, 0, 0, 250, 0, 2105, 29753, 2105, 2105, 2105, 2105, 2105, 0, 2267,
  /* 4806 */ 2105, 366, 2105, 37945, 2105, 2105, 0, 2105, 2279, 0, 0, 0, 2105, 2263, 2105, 2105, 2105, 2322, 2105, 2105,
  /* 4826 */ 2105, 2105, 35840, 0, 2105, 0, 2105, 41472, 28217, 2416, 39993, 37376, 37433, 2105, 183, 5734, 0, 0, 0, 0,
  /* 4846 */ 0, 0, 16896, 0, 2110, 0, 4608, 0, 0, 0, 0, 0, 184, 0, 184, 0, 0, 0, 0, 0, 2172, 2172, 2172, 2172, 2172,
  /* 4871 */ 2172, 2172, 2172, 0, 33792, 169, 170, 0, 0, 0, 0, 2106, 0, 0, 0, 33973, 33973, 0, 0, 0, 0, 2105, 2105,
  /* 4894 */ 2105, 2105, 2105, 2105, 0, 30208, 0, 0, 0, 0, 0, 0, 2115, 2115, 31744, 0, 34304, 0, 0, 0, 0, 41984, 42496,
  /* 4917 */ 0, 2105, 0, 0, 0, 0, 0, 2105, 0, 0, 0, 0, 176, 2105, 0, 0, 0, 0, 0, 2105, 352, 2206, 0, 0, 0, 0, 0, 0, 0,
  /* 4946 */ 45568, 2105, 2243, 2105, 2105, 2105, 2105, 2105, 2105, 2267, 2105, 2105, 2255, 2105, 2105, 2259, 2243,
  /* 4963 */ 2105, 0, 0, 0, 239, 0, 0, 0, 0, 0, 0, 2116, 2116, 26681, 2105, 2105, 2105, 2105, 2105, 2105, 2105, 2304,
  /* 4985 */ 2111, 0, 4608, 0, 0, 0, 0, 0, 185, 0, 0, 0, 0, 82, 83, 84, 85, 86, 0, 0, 0, 2262, 2105, 2105, 2105, 2105,
  /* 5011 */ 2277, 2105, 2105, 2105, 0, 0, 2173, 2173, 2173, 2173, 2173, 2173, 2173, 2173, 2105, 2273, 2105, 2105, 2105,
  /* 5030 */ 2105, 2105, 2198, 2279, 2280, 0, 0, 234, 0, 0, 0, 0, 22528, 0, 0, 0, 0, 19571, 19571, 19571, 19571, 2105,
  /* 5052 */ 2105, 2313, 2105, 2105, 2105, 2105, 2105, 2317, 2105, 2105, 2319, 2320, 2321, 2105, 2105, 2105, 2105, 2105,
  /* 5070 */ 2324, 2105, 2105, 288, 0, 0, 289, 0, 0, 0, 0, 2108, 0, 0, 0, 0, 0, 2105, 2105, 2105, 2385, 2105, 2105,
  /* 5093 */ 2105, 2407, 0, 0, 2105, 0, 178, 0, 166, 180, 2105, 2105, 2389, 2105, 2105, 2105, 2105, 2105, 2330, 2105, 0,
  /* 5114 */ 0, 0, 2105, 26169, 2105, 2105, 2105, 2105, 2323, 2105, 2105, 2326, 2112, 0, 4608, 0, 0, 0, 0, 0, 186, 2105,
  /* 5136 */ 2105, 0, 0, 2174, 2174, 2174, 2174, 2174, 2174, 2174, 2174, 73, 0, 0, 0, 0, 165, 166, 0, 0, 0, 7680, 9216,
  /* 5159 */ 0, 12288, 13824, 2105, 2244, 2105, 2247, 2105, 2105, 2105, 2105, 2334, 2105, 2105, 2105, 0, 0, 2302, 2303,
  /* 5178 */ 2105, 0, 0, 192, 2105, 2105, 2105, 2105, 2105, 2105, 36921, 2105, 2105, 2332, 2105, 2105, 2105, 2105, 2267,
  /* 5197 */ 2105, 2105, 2105, 2105, 2105, 2105, 2371, 2105, 294, 0, 0, 0, 298, 2105, 0, 0, 0, 0, 183, 183, 0, 2105,
  /* 5219 */ 2105, 2363, 2105, 2105, 2105, 2105, 2105, 2335, 2105, 2105, 2368, 2105, 2105, 2105, 2105, 2105, 2105, 2105,
  /* 5237 */ 2329, 0, 0, 2105, 2105, 2105, 2105, 2386, 2105, 247, 0, 0, 0, 0, 0, 0, 2112, 2112, 2294, 0, 0, 0, 0, 0, 0,
  /* 5262 */ 0, 46080, 2113, 0, 4608, 0, 0, 0, 0, 0, 333, 0, 0, 0, 0, 2175, 2175, 2175, 2175, 2175, 2175, 2175, 2175, 0,
  /* 5286 */ 172, 174, 0, 0, 0, 2105, 2105, 2196, 2264, 2105, 6337, 2105, 2105, 2105, 2309, 2105, 2105, 2105, 2204,
  /* 5305 */ 2205, 2105, 2105, 2105, 2258, 2105, 2245, 2105, 0, 2105, 2312, 2105, 2105, 2105, 2105, 2105, 2105, 2270,
  /* 5323 */ 2105, 2105, 2105, 2393, 2105, 2105, 2105, 2105, 2105, 2366, 2105, 2105, 57, 0, 0, 0, 0, 0, 2105, 0, 0, 179,
  /* 5345 */ 0, 0, 2401, 2105, 2105, 2105, 2105, 39481, 2105, 0, 0, 0, 349, 0, 2105, 0, 0, 0, 238, 0, 0, 0, 0, 243, 0,
  /* 5370 */ 0, 2114, 0, 4608, 0, 0, 0, 0, 0, 2105, 300, 0, 0, 0, 2114, 2114, 2114, 2114, 2114, 2114, 2114, 2114, 2105,
  /* 5393 */ 2245, 29382, 2248, 2105, 2250, 2105, 2105, 2190, 2105, 2105, 2105, 2105, 2105, 2278, 2105, 2105, 2105,
  /* 5410 */ 2105, 2268, 2105, 2105, 2105, 2105, 2105, 40505, 2105, 2105, 2105, 2274, 2105, 2105, 2105, 2105, 2105,
  /* 5427 */ 2105, 2318, 2105, 6337, 2105, 2105, 2105, 2105, 2105, 2311, 2105, 2105, 2192, 145, 2105, 2196, 2105, 2198,
  /* 5445 */ 2105, 2105, 2314, 2105, 2105, 2105, 2105, 2105, 2365, 2105, 2105, 2105, 2105, 2105, 2196, 2105, 0, 0, 2105,
  /* 5464 */ 0, 0, 250, 0, 0, 2115, 0, 4608, 0, 0, 0, 0, 0, 2105, 2617, 2105, 0, 0, 2176, 2176, 2176, 2176, 2176, 2176,
  /* 5488 */ 2176, 2176, 2105, 2201, 2105, 2105, 2105, 2105, 2105, 2105, 2325, 2105, 0, 0, 240, 0, 0, 0, 0, 0, 2355,
  /* 5509 */ 2105, 2105, 2116, 0, 4608, 0, 0, 0, 0, 0, 20480, 24064, 192, 0, 0, 2177, 2177, 2177, 2177, 2177, 2177,
  /* 5530 */ 2177, 2177, 2105, 2188, 2105, 2105, 2105, 2105, 2105, 2105, 2367, 2105, 2105, 2105, 2202, 2105, 2207, 2105,
  /* 5548 */ 2105, 2105, 2276, 2105, 2105, 2105, 2105, 33849, 2105, 2105, 2105, 168, 0, 0, 0, 0, 0, 0, 0, 2119, 2119,
  /* 5569 */ 2237, 0, 0, 0, 0, 0, 0, 0, 2120, 2120, 2242, 2105, 2105, 2105, 2105, 2105, 2105, 2251, 2105, 2256, 2105,
  /* 5590 */ 2105, 2105, 2105, 2105, 0, 0, 0, 235, 0, 0, 6337, 2105, 2105, 2105, 2105, 2105, 2105, 33337, 192, 2105,
  /* 5610 */ 27193, 2105, 2105, 2105, 2105, 2105, 2391, 2105, 2105, 2105, 0, 32256, 0, 0, 332, 0, 0, 0, 0, 22645, 22645,
  /* 5631 */ 22645, 22645, 175, 0, 2105, 0, 0, 0, 0, 0, 45056, 0, 0, 0, 0, 175, 0, 0, 0, 2105, 2105, 57, 0, 0, 192,
  /* 5656 */ 2105, 2105, 2257, 2105, 2105, 2105, 2105, 0, 0, 234, 0, 0, 0, 5120, 192, 2105, 2105, 2357, 2105, 2105,
  /* 5676 */ 2105, 2105, 31232, 2105, 2105, 0, 0, 0, 2383, 27648, 0, 0, 0, 0, 2110, 0, 0, 0, 0, 0, 2105, 2105, 27705,
  /* 5699 */ 2105, 2105, 2105, 2315, 2105, 2105, 2105, 2105, 2249, 2105, 2105, 2105, 2117, 0, 4608, 0, 0, 0, 0, 0, 2111,
  /* 5720 */ 0, 0, 0, 94, 0, 0, 0, 0, 0, 2153, 2153, 0, 0, 2178, 2178, 2178, 2178, 2178, 2178, 2178, 2178, 36409, 2105,
  /* 5743 */ 2105, 2105, 2316, 2105, 2105, 2105, 2333, 2105, 2105, 2105, 2105, 2364, 2105, 2105, 2105, 2105, 2376, 2105,
  /* 5761 */ 2105, 2105, 0, 0, 0, 0, 236, 0, 2327, 2105, 2105, 2105, 2105, 2105, 2105, 2105, 41017, 2105, 2331, 2105,
  /* 5781 */ 2105, 2105, 2105, 2105, 2105, 2396, 57, 192, 2105, 2105, 2105, 2358, 2105, 2105, 2105, 2390, 2105, 2105,
  /* 5799 */ 2105, 2105, 2395, 2267, 2105, 2105, 2105, 2362, 2105, 2105, 2105, 2105, 2105, 2105, 2404, 0, 0, 0, 2105,
  /* 5818 */ 2105, 2105, 2105, 2105, 32825, 2105, 2105, 2402, 2105, 2105, 2105, 2105, 0, 0, 235, 0, 0, 0, 0, 2109, 0, 0,
  /* 5840 */ 0, 2105, 2105, 2412, 2413, 0, 2105, 2105, 0, 0, 35328, 0, 0, 2105, 0, 0, 302, 304, 304, 2118, 0, 4608, 0,
  /* 5863 */ 0, 0, 0, 0, 2112, 0, 0, 0, 88, 89, 0, 0, 2118, 0, 0, 0, 0, 23743, 0, 16384, 0, 0, 0, 2179, 2179, 2179,
  /* 5889 */ 2179, 2179, 2179, 2179, 2179, 2200, 2105, 2203, 2105, 2105, 2105, 2105, 2105, 2394, 2105, 2105, 2105, 2105,
  /* 5907 */ 2377, 2105, 2105, 0, 0, 0, 2200, 0, 0, 0, 0, 0, 2114, 0, 0, 0, 2266, 2105, 2105, 2105, 2105, 2105, 2105,
  /* 5930 */ 2105, 43065, 0, 0, 2105, 2105, 2304, 0, 0, 192, 2105, 2105, 2105, 2105, 2359, 2105, 2105, 73, 233, 0, 0, 0,
  /* 5952 */ 0, 2107, 0, 0, 0, 6337, 2105, 2105, 2105, 2105, 2310, 2105, 2105, 2191, 2105, 2194, 2195, 2105, 2105, 2189,
  /* 5972 */ 2105, 2105, 2109, 2105, 2105, 73, 0, 0, 0, 0, 237, 2267, 2105, 2105, 2105, 0, 0, 2105, 0, 336, 0, 0, 0,
  /* 5995 */ 2411, 2105, 2105, 2105, 0, 2105, 2105, 0, 248, 0, 0, 0, 0, 0, 10240, 0, 0, 2119, 0, 4608, 0, 0, 0, 0, 0,
  /* 6020 */ 2115, 0, 0, 0, 0, 0, 2180, 2180, 2180, 2180, 2180, 2180, 2180, 2180, 2330, 2105, 2105, 2105, 2105, 2105,
  /* 6040 */ 2105, 2105, 42553, 2105, 2361, 2105, 2105, 2105, 2105, 2105, 2105, 2105, 2403, 38969, 2105, 2105, 0, 0, 0,
  /* 6059 */ 0, 0, 0, 0, 6337, 2120, 0, 4608, 0, 0, 0, 0, 0, 2116, 0, 0, 0, 0, 0, 90, 0, 2120, 0, 0, 0, 0, 25088, 0, 0,
  /* 6088 */ 25088, 0, 0, 0, 0, 0, 77, 0, 0, 0, 0, 2181, 2181, 2181, 2181, 2181, 2181, 2181, 2181, 0, 0, 0, 44032, 0, 0,
  /* 6113 */ 0, 0, 2117, 0, 0, 0, 0, 0, 44032, 44032, 44032, 44032, 44032, 44032, 44032, 44032, 0, 45568, 45568, 45568,
  /* 6133 */ 45568, 45568, 45568, 45568, 0, 0, 0, 45568, 0, 0, 0, 241, 0, 0, 0, 0, 242, 0, 0, 0, 0, 0, 46080, 46080,
  /* 6157 */ 46080, 46080, 46080, 46080, 46080, 46080
];

WebCpp.EXPECTED =
[
  /*   0 */ 69, 103, 76, 80, 82, 168, 86, 90, 94, 116, 101, 107, 111, 116, 123, 200, 115, 117, 174, 121, 129, 133, 97,
  /*  23 */ 147, 137, 141, 145, 240, 151, 102, 155, 190, 102, 181, 162, 166, 172, 241, 207, 166, 178, 187, 197, 204,
  /*  44 */ 213, 217, 102, 221, 260, 225, 102, 229, 233, 193, 238, 102, 183, 245, 72, 249, 234, 253, 158, 239, 257, 209,
  /*  66 */ 125, 264, 268, 272, 278, 301, 302, 303, 537, 275, 450, 358, 282, 286, 290, 294, 374, 374, 298, 302, 381,
  /*  87 */ 308, 313, 317, 321, 510, 326, 330, 337, 343, 347, 374, 351, 350, 350, 355, 302, 302, 302, 302, 333, 532,
  /* 108 */ 362, 308, 370, 373, 374, 374, 339, 373, 374, 374, 374, 374, 375, 366, 374, 374, 374, 302, 302, 453, 574,
  /* 129 */ 374, 374, 532, 530, 374, 375, 374, 530, 379, 302, 302, 304, 387, 391, 395, 399, 402, 406, 302, 302, 454,
  /* 150 */ 540, 380, 302, 302, 443, 410, 414, 419, 302, 322, 546, 552, 559, 302, 302, 430, 440, 464, 302, 302, 524,
  /* 171 */ 302, 426, 425, 302, 302, 530, 362, 447, 302, 302, 436, 542, 302, 302, 479, 458, 379, 302, 557, 302, 423,
  /* 192 */ 508, 302, 468, 303, 514, 415, 463, 302, 302, 531, 364, 309, 555, 302, 468, 302, 558, 302, 302, 546, 563,
  /* 213 */ 475, 570, 302, 302, 435, 556, 433, 302, 470, 455, 459, 483, 497, 501, 302, 302, 471, 456, 565, 568, 505,
  /* 234 */ 302, 302, 302, 478, 518, 522, 302, 302, 302, 541, 380, 567, 528, 302, 302, 520, 302, 302, 302, 457, 566,
  /* 255 */ 302, 302, 477, 456, 565, 569, 487, 491, 494, 382, 548, 302, 574, 383, 533, 576, 580, 8, 128, 256, 4096,
  /* 276 */ 8192, 16384, 32768, 1048576, 2097152, 33554432, 2144288768, -2146304, 575713016, 577810168, 575713016,
  /* 287 */ 575713016, 575713020, 709930744, 712027896, 712031992, 712031992, 712031992, 2147483384, -264, 2147483384,
  /* 297 */ -264, 128, 256, 33554432, 536870912, 0, 0, 0, 0, 1, 0, 1536, 1536, 72, 24, 40, 40, 33554944, 536872448,
  /* 316 */ 16384, 131072, 524288, 8388608, 100663296, 1610612736, 0, 0, 0, 128, 8, 8, 8, 72, 8, 8, 24, 8, -2146435072,
  /* 335 */ 1048576, 134217728, 8, 40, 8, 8, 8, 40, 8, 8, 8, 100663808, 1610614272, 2048, -536869376, 8, 0, 8, 8, 0, 8,
  /* 356 */ 8, 128, 0, 0, 1050360, 575678200, 0, 512, 512, 512, 1536, 1536, 72, 8, 40, 0, 0, 2048, 8, 8, 8, 8, 0,
  /* 379 */ 536870912, 1073741824, 0, 0, 0, 512, 1024, 8192, 2097152, 671088640, 65552, 738197504, 0, 1082130432, 32768,
  /* 394 */ 32768, 163822, 196591, -2089517040, -2089517040, -2089451504, -2089517040, -2089517040, -2089451504,
  /* 403 */ -2089451504, -2088927216, -1686798320, -2089287690, -2089287690, -2089287689, -2089287689, 0, 4, 448, 1024,
  /* 414 */ 28672, 0, 0, 0, 262144, 262144, 7340032, 50331648, 0x80000000, 0, 7864320, 402653184, 0, 0, 0, 524288, 0,
  /* 431 */ 128, 16384, 0, 0, 4194304, 0, 0, 0, 8388608, 0, 262144, 1048576, 2097152, 0, 671088640, 738197504, 0,
  /* 448 */ 524288, 402653184, 0, 0, 134217728, 0, 0, 8, 16, 32, 256, 131072, 262144, 1048576, 2097152, 2097152,
  /* 464 */ 4194304, 33554432, 0x80000000, 0, 0, 8388608, 0, 0, 2, 4, 8, 4194304, 33554432, 0, 0, 4, 8, 16, 32, 256, 0,
  /* 485 */ 0, 16777216, 14, 393278, 2097152, 35651584, 29360128, 29360128, 37552067, 37552067, 54329283, 37552067,
  /* 497 */ 37552067, 54329283, 54329283, 54329283, 66912195, 66912195, 66912195, 66912195, 14, 62, 393216, 0, 4, 0, 0,
  /* 512 */ -536870912, 2048, 66, 128, 256, 3584, 4096, 57344, 262144, 524288, 1048576, 0, 0, 0, 8, 0x80000000, 12, 0,
  /* 530 */ 0, 0, 8, 0, 0, 0, 256, 64, 128, 1536, 2048, 8388608, 67108864, 134217728, 536870912, 1073741824, 512, 1024,
  /* 548 */ 2048, 4096, 8192, 0, 8192, 16384, 262144, 524288, 268435456, 0, 0, 0, 2097152, 0, 201326592, 8192, 16384,
  /* 565 */ 262144, 1048576, 256, 0, 65536, 0, 0, 0, 268435456, 32, 256, 256, 65536, 0, 1024, 8192, 256, 256, 1024
];

WebCpp.TOKEN =
[
  "(0)",
  "END",
  "eof",
  "identifier",
  "'null'",
  "'true'",
  "'false'",
  "string",
  "unicode",
  "complex",
  "real",
  "comment",
  "lineTerminator",
  "'!'",
  "'!='",
  "'#'",
  "'%'",
  "'%='",
  "'&'",
  "'&&'",
  "'('",
  "')'",
  "'*'",
  "'*='",
  "'+'",
  "'++'",
  "'+='",
  "','",
  "'-'",
  "'--'",
  "'-='",
  "'->'",
  "'.'",
  "'/'",
  "'/='",
  "':'",
  "';'",
  "'<'",
  "'<<'",
  "'<<='",
  "'<='",
  "'='",
  "'=='",
  "'>'",
  "'>='",
  "'>>'",
  "'>>='",
  "'['",
  "']'",
  "'^'",
  "'break'",
  "'catch'",
  "'char'",
  "'class'",
  "'continue'",
  "'define'",
  "'do'",
  "'double'",
  "'elif'",
  "'else'",
  "'else if'",
  "'endif'",
  "'error'",
  "'float'",
  "'for'",
  "'if'",
  "'ifdef'",
  "'ifndef'",
  "'import'",
  "'include'",
  "'int'",
  "'long'",
  "'namespace'",
  "'private'",
  "'protected'",
  "'public'",
  "'return'",
  "'template'",
  "'throw'",
  "'try'",
  "'typename'",
  "'undef'",
  "'using'",
  "'void'",
  "'while'",
  "'{'",
  "'|'",
  "'||'",
  "'}'",
  "'~'"
];

// main program for use with node.js, rhino, or jrunscript

function main(args)
{
  if (typeof process !== "undefined")   // assume node.js
  {
    var command = "node";
    var arguments = process.argv.slice(2);
    var log = function(string) {process.stdout.write(string);};
    var fs = require("fs");
    var readTextFile = fs.readFileSync;
  }
  else                                  // assume rhino or jrunscript
  {
    var arguments = function()
                    {
                      var strings = [];
                      for (var i = 0; i < args.length; ++i)
                      {
                        strings[i] = String(args[i]);
                      }
                      return strings;
                    }();

    if (typeof println == "undefined")  // assume rhino
    {
      var command = "java -jar js.jar";
      var log = function(string) {java.lang.System.out.write(java.lang.String(string).getBytes("utf-8"));};
      var readTextFile = readFile;
    }
    else                                // assume jrunscript
    {
      var command = "jrunscript";
      var log = function(string) {java.lang.System.out.print(string);};
      var readTextFile = function(filename, encoding)
                         {
                           var file = new java.io.File(filename);
                           var buffer = javaByteArray(file.length());
                           new java.io.FileInputStream(file).read(buffer);
                           return String(new java.lang.String(buffer, encoding));
                         };
    }
  }

  function read(input)
  {
    if (/^{.*}$/.test(input))
    {
      return input.substring(1, input.length - 1);
    }
    else
    {
      var content = readTextFile(input, "utf-8");
      return content.length > 0 && content.charCodeAt(0) == 0xFEFF
           ? content.substring(1)
           : content;
    }
  }

  if (arguments.length == 0)
  {
    log("Usage: " + command + " WebCpp.js [-i] INPUT...\n");
    log("\n");
    log("  parse INPUT, which is either a filename or literal text enclosed in curly braces\n");
    log("\n");
    log("  Option:\n");
    log("    -i     indented parse tree\n");
  }
  else
  {
    var indent = false;
    for (var i = 0; i < arguments.length; ++i)
    {
      if (arguments[i] === "-i")
      {
        indent = true;
        continue;
      }
      var input = read(String(arguments[i]));
      var s = new WebCpp.XmlSerializer(log, indent);
      var parser = new WebCpp(input, s);
      try
      {
        parser.parse_webcpp();
      }
      catch (pe)
      {
        if (! (pe instanceof parser.ParseException))
        {
          throw pe;
        }
        else
        {
          throw parser.getErrorMessage(pe);
        }
      }
    }
  }
}

main(arguments);

// End
