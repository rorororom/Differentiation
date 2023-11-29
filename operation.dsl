SET_OPERATOR("add", ADD, "+",
    {
        return leftValue + rightValue;
    },
    {
        return _ADD(DIFL, DIFR);
    })
SET_OPERATOR("sub", SUB, "-",
    {
        return leftValue - rightValue;
    },
    {
        return _SUB(DIFL, DIFR);
    })
SET_OPERATOR("mul", MUL, "*",
    {
        return leftValue * rightValue;
    },
    {
        return _ADD(_MUL(DIFL, COPYR), _MUL(COPYL, DIFR));
    })
SET_OPERATOR("div", DIV, "/",
    {
        if (rightValue == 0)
        {
            printf("делить на ноль нельзя\n");
            return 0;
        }
        else return leftValue / rightValue;
    },
    {
        return _DIV(_SUB(_MUL(DIFL, COPYR), _MUL(COPYL, DIFR)), _POW(COPYR, NewNode(INT, 2, NULL, NULL)));
    })
SET_OPERATOR("pow", POW, "^",
    {
        for (int i = 0; i < rightValue - 1; i++)
        {
            leftValue *= leftValue;
        }
        return leftValue;
    },
    {
        if (nowNode->left->type == VAR)
            return _MUL(_MUL(COPYR, _POW(COPYL, _SUB(COPYR, NewNode(INT, 1, NULL, NULL)))), DIFL);
        else if ((nowNode->left->type == CONST || nowNode->left->type == INT) && nowNode->left->type == VAR)
            return _MUL(_POW(COPYL, COPYR), NewNode(OPERAT, LN, nowNode->left, NULL));
        else
            return NewNode(INT, 0, NULL, NULL);


    })
SET_OPERATOR("sin", SIN, "sin",
    {
        return sin(leftValue);
    },
    {
        return _MUL(_COS(COPYL), DIFL);
    })
SET_OPERATOR("cos", COS, "cos",
    {
        return cos(leftValue);
    },
    {
        return _MUL(_MUL(NewNode(INT, -1, NULL, NULL), _SIN(COPYL)), DIFL);
    })
SET_OPERATOR("tg", TAN, "tg",
    {
        return tan(leftValue);
    },
    {
        return _MUL(_DIV(NewNode(INT, 1, NULL, NULL), _POW(_COS(COPYL), NewNode(INT, 2, NULL, NULL))), DIFL);
    })
SET_OPERATOR("ctg", COT, "ctg",
    {
        return 1 / tan(leftValue);
    },
    {
        return _MUL(_DIV(DIFL, _POW(_SIN(COPYL), NewNode(INT, 2, NULL, NULL))), NewNode(INT, -1, NULL, NULL));
    })
SET_OPERATOR("ln", LN, "ln",
    {
        return log(leftValue);
    },
    {
        return _MUL(_DIV(NewNode(INT, 1, NULL, NULL), COPYL), DIFL);
    })
SET_OPERATOR("arcsin", ARCSIN, "arcsin",
    {
        return RAD_TO_DEG(asin(leftValue));
    },
    {
        return _DIV(DIFL, _POW(_SUB(NewNode(INT, 1, NULL, NULL), _POW(COPYL, NewNode(INT, 2, NULL, NULL))), NewNode(INT, 0.5, NULL, NULL)));
    })
SET_OPERATOR("arccos", ARCCOS, "arccos",
    {
        return RAD_TO_DEG(acos(leftValue));
    },
    {
        return _DIV(_MUL(NewNode(INT, -1, NULL, NULL), DIFL), _POW(_SUB(NewNode(INT, 1, NULL, NULL), _POW(COPYL, NewNode(INT, 2, NULL, NULL))), NewNode(INT, 0.5, NULL, NULL)));
    })
SET_OPERATOR("arctan", ARCTAN, "arctan",
    {
        return RAD_TO_DEG(atan(leftValue));
    },
    {
        return _DIV(DIFL, _ADD(NewNode(INT, 1, NULL, NULL), _POW(COPYL, NewNode(INT, 2, NULL, NULL))));
    })
SET_OPERATOR("arccot", ARCCOT, "arccot",
    {
        return RAD_TO_DEG(atan(1 / leftValue));
    },
    {
        return _DIV(_MUL(NewNode(INT, -1, NULL, NULL), DIFL), _ADD(NewNode(INT, 1, NULL, NULL), _POW(COPYL, NewNode(INT, 2, NULL, NULL))));
    })
