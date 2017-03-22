#include "op_token.h"

std::map<std::string, OpType> OpToken::strop = {
    {"+",  Plus   },
    {"-",  Minus  },
    {"*",  Mult   },
    {"/",  Divide },
    {"%",  Percent},
    {"==", Eq     },
    {"!=", Neg    },
    {">",  Gt     },
    {">=", Ge     },
    {"<",  Lt     },
    {"<=", Le     },
    {"&&", And    },
    {"||", Or     }
};

std::map<OpType, std::string> OpToken::opstr = {
    {Plus   , "+"   },
    {Minus  , "-"   },
    {Mult   , "*"   },
    {Divide , "/"   },
    {Percent, "%"   },
    {Eq     , "=="  },
    {Neg    , "!="  },
    {Gt     , ">"   },
    {Ge     , ">="  },
    {Lt     , "<"   },
    {Le     , "<="  },
    {And    , "&&"  },
    {Or     , "||"  }
};

std::map<OpType, std::string> OpToken::opname = {
    {Plus   , "Plus"   },
    {Minus  , "Minus"  },
    {Mult   , "Mult"   },
    {Divide , "Divide" },
    {Percent, "Percent"},
    {Eq     , "Eq"     },
    {Neg    , "Neg"    },
    {Gt     , "Gt"     },
    {Ge     , "Ge"     },
    {Lt     , "Lt"     },
    {Le     , "Le"     },
    {And    , "And"    },
    {Or     , "Or"     }
};

