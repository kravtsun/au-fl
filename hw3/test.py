#!/usr/bin/python2
# -*- coding: utf-8 -*-
import reCompiler

class Task():
    def __init__(self, name, expr, pass_tests=None, fail_tests=None):
        self.name = name
        self.expr = expr
        self.pass_tests = pass_tests
        self.fail_tests = fail_tests

    @staticmethod
    def replace_bad_labels(filename):
        import subprocess
        pairs = [(u"None", u"ε"),
                 ("\\[ab\\]","a,b"),
                 ("\\\\d", "[0-9]"),
                 ("\\[+---\\]", "+,-")]
        for first, second in pairs:
            sed_command = ["sed", "-s", "-i", u"s/" + first + "/" + second + "/", filename]
            subprocess.call(sed_command)

    def work_automata(self, name, fa, suffix):
        import os
        filename = name + "_" + suffix + ".gv"
        with open(filename, "w") as f:
            f.write(fa.toDotString())
        Task.replace_bad_labels(filename)
        png_filename = name + "_" + suffix + ".png"
        dot_command = " ".join(["dot", "-Tpng", filename, ">", png_filename])
        os.system(dot_command)
        rm_command = " ".join(["rm", filename])
        os.system(rm_command)

    def build_automatas(self):
        nfa = reCompiler.compileRE(self.expr, minimize=0).trimmed()
        dfa = nfa.determinized()
        dfa_min = dfa.minimized()
        automatas = [nfa, dfa, dfa_min]
        suffixes = ["nfa", "dfa", "dfa_min"]
        for p in zip(automatas, suffixes):
            self.work_automata(self.name, *p)
        return dfa

    def run(self):
        dfa = self.build_automatas()
        if self.pass_tests is not None:
            for pt in self.pass_tests:
                if (not dfa.accepts(pt)):
                    raise Exception(pt)
        if self.fail_tests is not None:
            for ft in self.fail_tests:
                if (dfa.accepts(ft)):
                    raise Exception(ft)

class RegExp:
    @staticmethod
    def enbrace(s):
        return "(" + s + ")"

    @staticmethod
    def num_regexp():
        return "(0|[1-9][0-9]*)"

    @staticmethod
    def maybe_regexp(s):
        return RegExp.enbrace(s) + "?"

    @staticmethod
    def or_regexp(s1, s2):
        return RegExp.enbrace(s1) + "|" + RegExp.enbrace(s2)

    @staticmethod
    def rational_regexp():
        point_regexp = "\\."
        sign_regexp = "(+|-)?"

        num_regexp = RegExp.num_regexp()
        # check on valid num_regexp.
        # Task("num", num_regexp, ["1", "0", "123"], ["a", "01", "001"]).run()

        maybe_regexp = RegExp.maybe_regexp
        or_regexp = RegExp.or_regexp
        exp_regexp = "e" + sign_regexp + num_regexp
        maybe_exp = maybe_regexp(exp_regexp)

        with_integer_part = RegExp.maybe_regexp("-") + num_regexp + maybe_regexp(point_regexp + num_regexp + "*") + maybe_exp
        without_integer_part = point_regexp + "[0-9][0-9]*" + maybe_exp
        res = or_regexp(with_integer_part, without_integer_part)
        return res

    @staticmethod
    def list_regexp():
        space_regexp = "\\s*"
        split_regexp = space_regexp + ";" + space_regexp
        num_regexp = RegExp.num_regexp()
        return "\\[" + space_regexp + RegExp.maybe_regexp(num_regexp + RegExp.maybe_regexp(RegExp.enbrace(split_regexp + num_regexp) + "*")) + space_regexp + "\\]"

def print_regexp(s):
    if s is None or len(s) == 0:
        return
    res = ""
    for i in range(len(s)):
        if s[i] != '\\' or (i + 1 < len(s) and s[i+1] == 's'):
            res += s[i]
    print res


if __name__ == '__main__':
    first = [("1a", "(a|b)*ab(a|b)*|(a|b)*a|b*"),
             ("1b", "(a|b)*(ab|ba)(a|b)*|a*|b*")]

    for p in first:
        Task(*p).run()

    second = [("2a", "(_|[a-zA-Z])(_|[a-zA-Z0-9])*"),
              ("2b", RegExp.rational_regexp()),
              ("2c", RegExp.list_regexp())
              ]
    pass_tests = [["a", "_a", "A", "a_b", "iDeNt", "_i_D_", "__agent007__", "_13", "e2_e4"],
                  ["0", "-0", "10", "0.0", "0.1e0", "0.10010e+1", "10e-123", ".1", ".5e+10"],
                  ["[]", "[1]", "[1;1;2;3;5;8]", "[ ]", "[ 4; 8; 15; 16 ; 23; 42]"]
                  ]
    fail_tests = [["", "1", "1st", "!myVar!"],
                  ["01", "+.2", "e, e.", ".", "", ".e", ".e-123"],
                  ["][", "[[]]", "[1;2;3", "[a]", "[1,2,3]", "[1;2;]", "[1; 23 4; 5]", "[;1]"]
                  ]

    for i, p in enumerate(second):
        print_regexp(p[1])
        Task(p[0], p[1], pass_tests[i], fail_tests[i]).run()
